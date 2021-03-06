#include "Request.h"

Request::Request()
{
    //ctor
}

Request::~Request()
{
    //dtor
}


void Request::init(int fd,std::string uri){
    this->fileDescriptor=fd;
    this->uri=uri;
}

void Request::execute(){
    doExecute();
}

int Request::getFileDescriptor() const{
    return fileDescriptor;
}

const std::string& Request::getUri() const{
    return uri;
}

