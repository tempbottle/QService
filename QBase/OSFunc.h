
#ifndef Q_HOST_H_
#define Q_HOST_H_

#include "InitSock.h"

/*��ȡ����������*/
std::string Q_HostName(void);
/*ִ�нű�*/
int Q_ExecCmd(const char *pszCmd, const unsigned int uimSec, 
    std::list<std::string> *lstRst);
/*uint64_t ת�����ֽ���*/
uint64_t ntohl64(uint64_t host);

#endif//Q_HOST_H_
