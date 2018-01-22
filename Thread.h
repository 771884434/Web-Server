#ifndef THREAD_H_INCLUDED
#define THREAD_H_INCLUDED

#include <vector>
#include <string>
#include <pthread.h>

#include "Task.h"
using std::vector;

class CTask;

//�̳߳���
class CThreadPool
{
private:
    static vector<CTask*> m_TaskList;       //�����б�
    static bool shutdown;                   //�߳��˳���־
    int m_iThreadNum;                       //�̳߳����������߳���
    pthread_t *pthread_id;                  //����߳�id

    static pthread_mutex_t m_pthreadMutex;  //�߳�ͬ����
    static pthread_cond_t m_pthreadCond;    //�߳�ͬ������������

protected:
    static void* ThreadFunc(void * threadData); //���̵߳Ļص�����
    static int MoveToIdle(pthread_t tid) {}   //�߳�ִ�н��������뵽�����߳���
    static int MoveToBusy(pthread_t tid) {} //���̷߳���æµ������ȥ
    int Create();                          //�����̳߳��е��߳�

public:
    CThreadPool(int threadNum=10);          //���캯��
    int AddTask(CTask *task);               //��������ӵ����������
    int StopAll();                          //ʹ�̳߳��е��߳�ȫ���˳�
    int getTaskSize();                      //��ȡ��ǰ�����е�������
};



#endif // THREAD_H_INCLUDED
