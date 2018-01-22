#include "IoReader.h"
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <iostream>
//#include <glog/logging.h>
using namespace std;

namespace{
const int MAX_LENGTH=8192;
//������ֵ
struct rio_t{
    int rio_fd; /* descriptor for this internal buf */
    int rio_cnt; /* unread bytes in internal buf */
    char *rio_bufptr; /* next unread byte in internal buf */
    char rio_buf[MAX_LENGTH]; /* internal buffer */
};

void unix_error(char* msg){
    fprintf(stderr,"%s: %s\n",msg,strerror(errno));
    exit(1);
}

void rio_readinitb(rio_t *rp,int fd){
    rp->rio_fd=fd;
    rp->rio_cnt=0;
    rp->rio_bufptr=rp->rio_buf;
}

void Rio_readinitb(rio_t *rp,int fd){
    rio_readinitb(rp,fd);
}
//���׽����ж�ȡһ���ַ�
static ssize_t rio_read(rio_t *rp,char *usrbuf,size_t n){
    int cnt;
    //����һ������֮�󣬾Ͳ����ˣ�ֱ��rp->rio_buf�е����ݴ�����֮���ٶ���һ��
    while(rp->rio_cnt<=0){      //��rio_bufΪ�գ��������
        rp->rio_cnt=read(rp->rio_fd,rp->rio_buf,sizeof(rp->rio_buf));
        if(rp->rio_cnt<0){
            if(errno!=EINTR)    //interrupted by sig handler return
                return -1;
        }else if(rp->rio_cnt==0)    //EOF
            return 0;
        else
            rp->rio_bufptr=rp->rio_buf;     //����bufָ��
    }

    /* Copy min(n, rp->rio_cnt) bytes from internal buf to user buf */
    cnt=n;
    if(rp->rio_cnt<n)
        cnt=rp->rio_cnt;
    memcpy(usrbuf,rp->rio_bufptr,cnt);
    rp->rio_bufptr+=cnt;
    rp->rio_cnt-=cnt;
    return cnt;
}
//
ssize_t rio_readlineb(rio_t *rp,void *usrbuf,size_t maxlen){
    int n,rc;
    char c,*bufp=reinterpret_cast<char*>(usrbuf);

    for(n=1;n<maxlen;n++){
        //��ÿ�ζ�ȡ��һ���ַ�����usrbuf��
        if((rc=rio_read(rp,&c,1))==1){
            *bufp++=c;
            if(c=='\n')
                break;
        }else if(rc==0){
            if(n==1)
                return 0;
            else
                break;
        }
    }
}

ssize_t Rio_readlineb(rio_t *rp,void *usrbuf,size_t maxlen){
    ssize_t rc;
    if((rc=rio_readlineb(rp,usrbuf,maxlen))<0)
        unix_error("Rio_readlineb error");
    return rc;
}

rio_t rio;

}


IoReader::IoReader()
{
    //ctor
}

IoReader::IoReader(int fd){
    Rio_readinitb(&rio,fd);
}

IoReader::~IoReader()
{
    //dtor
}
//���Ѿ��ָ��õ��ַ�����������
void IoReader::getLineSplitedByBlank(std::vector<std::string>&buf){
    char innerBuf[MAX_LENGTH],method[MAX_LENGTH],uri[MAX_LENGTH],version[MAX_LENGTH];
    Rio_readlineb(&rio,innerBuf,MAX_LENGTH);
//    cout<<innerBuf<<endl;
//    getchar();
//    LOG(INFO)<<"tid="<<pthread_self()<<" get message:"<<innerBuf<<endl;
    sscanf(innerBuf,"%s %s %s",method,uri,version);
    buf.push_back(method);
    buf.push_back(uri);
    buf.push_back(version);
}
