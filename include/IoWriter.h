#ifndef IOWRITER_H
#define IOWRITER_H

#include <string>
#include "Epoll.h"
//�������IoReader��Ӧ���Ƿ�װ�ײ�IOд�����ģ�ʵ���Ͼ�����ͻ��˷������ݡ����������ͻ�����Ҫ���ʵ�uri������ͻὫ��Ӧ���ļ����ͻ�ȥ������������������ļ������Ǿ��ܿ�����ҳ��
class IoWriter
{
    public:
        IoWriter();
        virtual ~IoWriter();
        IoWriter(int fd);
        void writeString(const std::string& str);   //��Ҫ�����ڷ�����Ӧ��ͷ
        void writeFile(const std::string& filename,int filesSize);  //�ѿͻ�����Ҫ���ļ�����
    protected:

    private:
        int fileDescriptor;

};
extern Epoll ep;
#endif // IOWRITER_H
