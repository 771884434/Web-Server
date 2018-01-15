#ifndef GETREQUEST_H
#define GETREQUEST_H

#include "Request.h"
#include <string>
//����GET�������󣬽����������URI�������䴫��Response��Ķ����һ������
class GetRequest:public Request
{
    public:
        GetRequest();
        virtual ~GetRequest();


    protected:

    private:
        virtual void doExecute();
        bool parseUri(std::string& filename,std::string& cgiargs);
        bool parseStaticContentUri(std::string& filename);
        //����̬�����uri�Ͳ�����
        bool parseDynamicContentUri(std::string& filename,std::string& cgiargs);
        void assignCigArgs(std::string& cgiargs);
        void doAssignCigArgs(std::string::size_type pos,std::string& cgiargs);
};

#endif // GETREQUEST_H
