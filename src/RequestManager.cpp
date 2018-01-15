#include "RequestManager.h"
#include "Request.h"
#include "RequestCreater.h"
#include "IoReader.h"

namespace {
//�����ͻ��˵���������
class Parser {
public:
    Parser(int connfd) {
        parseRequestHeaders(connfd);
    }

    const std::string getMethodName() {
        return method;
    }

    const std::string getUri() {
        return uri;
    }
private:
    //������������ݣ��õ���IOReader��
    void parseRequestHeaders(int fd) {
        IoReader reader(fd);
        std::vector<std::string> header;
        reader.getLineSplitedByBlank(header);
        //��Ϊ��
        method=header[0];
        uri=header[1];
        version=header[2];
    }

    std::string method;
    std::string uri;
    std::string version;    //Э��汾
};
}


RequestManager::RequestManager() {
    //ctor
}

RequestManager::RequestManager(int connfd):fileDescriptor(connfd),request(0){

}

RequestManager::~RequestManager() {
    //dtor
}

void RequestManager::run(){
    if(getRequestHandle())
        request->execute();
}

Request* RequestManager::getRequestHandle(){
    Parser parser(fileDescriptor);
    //ʹ�ù����ࣨRequestCreater�������첻ͬ�ķ���ʵ��
    return request=RequestCreater::getRequestHandler(fileDescriptor,parser.getMethodName(),parser.getUri());
}
