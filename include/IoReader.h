#ifndef IOREADER_H
#define IOREADER_H

#include <string>
#include <vector>
#include "Epoll.h"
//��װ�ײ�IO
class IoReader
{
    public:
        IoReader();
        virtual ~IoReader();
        IoReader(int fd);
        //��ȡhttp����ı�ͷ����ȡһ�У���ʹ�á� ���ָ��ɶ���ַ����󷵻�
        void getLineSplitedByBlank(std::vector<std::string>& buf);
    protected:

    private:
};
extern Epoll ep;

#endif // IOREADER_H
