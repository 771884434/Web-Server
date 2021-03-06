/*
 * ResponseState.cpp
 *
 *  Created on: 2015年1月16日
 *      Author: augustus
 */
#include "ResponseState.h"
#include "TinyError.h"
#include "IoWriter.h"

ResponseState::ResponseState(int fd, std::string fName) : fileDescriptor(fd), fileName(fName)
{

}

void ResponseState::preRespond()
{
	if (stat(const_cast<char*>(fileName.c_str()), &sbuf) < 0)   //通过文件名filename获取文件信息，并保存在buf所指的结构体stat中
		throw TinyError("404", "Not found", "Tiny couldn't find this file");

	if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode))  //不是常规文件或用户无法进行文件读写
		throw TinyError("403", "Forbidden", buildForbiddenMsg());
}

void ResponseState::respond()
{
	doRespond();
}

void ResponseState::respondError(const std::string errNum, const std::string shortMsg, const std::string longMsg)
{
	IoWriter(getFileDescriptor()).writeString(buildRespondErrorHeaders(errNum, shortMsg)
			+ buildRespondErrorBody(errNum, shortMsg, longMsg));
}

const std::string ResponseState::buildRespondErrorHeaders(const std::string errNum, const std::string shortMsg)
{
	return "HTTP/1.0 "
			+ errNum
			+ " "
			+ shortMsg
			+ "\r\n"
			+ std::string("Content-type: text/html\r\n");
}

const std::string ResponseState::buildRespondErrorBody(const std::string errNum, const std::string shortMsg, const std::string longMsg)
{
	std::string bodyContent = "<html><title>Tiny Error</title>"
				+ std::string("<body bgcolor= ffffff >\r\n")
		        + errNum
				+ ": "
				+ shortMsg
				+ "\r\n"
				+ "<p>"
				+ longMsg
				+ ": "
				+ fileName
				+ "<hr><em>The Tiny Web server</em>\r\n";

	return "Content-length: "
			+ sizeTypeToStr(bodyContent.length())
			+ "\r\n\r\n"
			+ bodyContent;
}

const std::string ResponseState::sizeTypeToStr(std::string::size_type sizeType)
{
	    std::stringstream ss;
	    std::string s;
	    ss << sizeType;
	    ss >> s;
	    return s;
}

const std::string ResponseState::buildRespondHeaders()
{
    return "HTTP/1.0 200 OK\r\n" + std::string("Server: HLC Web Server\r\n");
	//return "HTTP/1.0 401 Authorization Required\r\n" + std::string("Server: Tiny Web Server\r\n");
}

int ResponseState::getFileDescriptor()
{
	return fileDescriptor;
}

const std::string ResponseState::getFileName()
{
	return fileName;
}
struct stat& ResponseState::getStat()
{
	return sbuf;
}
