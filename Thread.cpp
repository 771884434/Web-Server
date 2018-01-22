#include "Thread.h"
#include <iostream>
#include <glog/logging.h>

using std::cout;
using std::endl;
using std::vector;

vector<CTask*> CThreadPool::m_TaskList;
bool CThreadPool::shutdown=false;

pthread_mutex_t CThreadPool::m_pthreadMutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t CThreadPool::m_pthreadCond=PTHREAD_COND_INITIALIZER;

//���캯��
CThreadPool::CThreadPool(int threadNum)
{
    this->m_iThreadNum=threadNum;
    cout<<"I will create "<<threadNum<<" threads"<<endl;
    Create();
}

void * CThreadPool::ThreadFunc(void* threadData)
{
    pthread_t tid=pthread_self();
    while(1) {
        LOG(INFO)<<"tid "<<tid<<" is waiting the mutex";
        pthread_mutex_lock(&m_pthreadMutex);
        LOG(INFO)<<"tid "<<tid<<" get the mutex";
        while(m_TaskList.size()==0 && !shutdown) {
            pthread_cond_wait(&m_pthreadCond,&m_pthreadMutex);  //�ȴ����������ı仯
        }
        if(shutdown) {
            pthread_mutex_unlock(&m_pthreadMutex);
            cout<<"thread "<<tid<<" will exit"<<endl;
            pthread_exit(NULL);
        }

        cout<<"tid "<<tid<<" run"<<endl;
        vector<CTask*>::iterator iter=m_TaskList.begin();

        //ȡ��һ������
        CTask* task=*iter;
        if(iter!=m_TaskList.end()) {
            task=*iter;
            m_TaskList.erase(iter);
        }
        LOG(INFO)<<"tid "<<tid<<" release the mutex"<<endl;
        //task->Run();
        pthread_mutex_unlock(&m_pthreadMutex);

        //ִ������
        LOG(INFO)<<"tid "<<tid<<" run"<<endl;
        task->Run();
        cout<<"tid:"<<tid<<" idle"<<endl;
    }
    return (void*)0;
}

int CThreadPool::AddTask(CTask *task){
    pthread_mutex_lock(&m_pthreadMutex);
    this->m_TaskList.push_back(task);
    pthread_cond_signal(&m_pthreadCond);
    pthread_mutex_unlock(&m_pthreadMutex);
    //���ٻ���һ���ȴ����������߳�
    //pthread_cond_signal(&m_pthreadCond);
}

int CThreadPool::Create(){
    pthread_id=(pthread_t*)malloc(sizeof(pthread_t)*m_iThreadNum);
    for(int i=0;i<m_iThreadNum;i++){
        pthread_create(&pthread_id[i],NULL,ThreadFunc,NULL);
    }
    return 0;
}

int CThreadPool::StopAll(){
    if(shutdown){
        return -1;
    }
    cout<<"Now I will end all threads!"<<endl;
    shutdown=true;
    //���ѵȴ��������������߳�
    pthread_cond_broadcast(&m_pthreadCond);
    for(int i=0;i<m_iThreadNum;i++){
        pthread_join(pthread_id[i],NULL);
    }
    free(pthread_id);
    pthread_id=NULL;
    pthread_mutex_destroy(&m_pthreadMutex);
    pthread_cond_destroy(&m_pthreadCond);

    return 0;
}

int CThreadPool::getTaskSize(){
    return m_TaskList.size();
}


