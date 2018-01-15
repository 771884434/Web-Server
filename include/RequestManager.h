#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include <string>

class Request;
//�ȴ�HTTP���󣬲���������
//����Request ���Ǿ��巽���Ļ��࣬�����������GET��POST�ȵȡ�
class RequestManager
{
    public:
        RequestManager();
        RequestManager(int connfd);
        virtual ~RequestManager();
        void run();
    private:
        Request* getRequestHandle();    //���ݲ�ͬ���������ͣ���ȡ��ͬ��������
        int fileDescriptor;             //�������ļ�������
        Request* request;               //��ȡ��������
};

#endif // REQUESTMANAGER_H
