#ifndef IOREADER_H
#define IOREADER_H

#include <string>
#include <vector>
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

#endif // IOREADER_H
