#ifndef REQUEST_H
#define REQUEST_H

#include <string>

//Request��һ�������࣬ÿһ�����඼��Ҫʵ��doExecute��������ʵ����
class Request
{
    public:
        Request();
        virtual ~Request();
        void init(int fd,std::string uri);
        void execute();
    protected:
        int getFileDescriptor() const;
        const std::string& getUri() const;
    private:
        virtual void doExecute()=0;     //�麯��

        int fileDescriptor;
        std::string uri;
};

#endif // REQUEST_H
