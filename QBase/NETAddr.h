
#ifndef Q_NETADDR_H_
#define Q_NETADDR_H_

#include "InitSock.h"

/*
�����ַ
*/
class CNETAddr
{
public:
    CNETAddr(void);
    explicit CNETAddr(bool ipv6);
    ~CNETAddr(void);

    /*�������*/
    void Clear(void);

    /*����sockaddr_in*/
    int setAddr(const char *pszHostName, unsigned short usPort, bool bIpv6 = false);
    /*����sockaddr_in*/
    int	setAddr(const struct sockaddr *pAddr);
    /*����socket�����ȡԶ�˵�ַ��Ϣ*/
    int setRemoteAddr(const Q_SOCK &fd);
    /*����socket�����ȡ���ص�ַ��Ϣ*/
    int setLocalAddr(const Q_SOCK &fd);
    /*����sockaddr���*/
    const sockaddr *getAddr(void) const;
    /*��ȡ��ַ�ڴ泤��*/
    size_t getAddrSize(void) const;
    /*��ȡIP*/
    std::string getIp(void) const;
    /*��ȡ�˿�*/
    unsigned short getPort(void);
    /*�Ƿ�Ϊipv4*/
    bool is_ipv4() const;
    /*�Ƿ�Ϊipv6*/
    bool is_ipv6() const;
    /*ipת����*/
    unsigned long IpToNumber(const char *pszIp) const;

private:
    enum NetAdrrType
    {
        IPV4,
        IPV6
    };

    NetAdrrType m_Addr_Type;
    sockaddr_in	m_ipv4;
    sockaddr_in6 m_ipv6;
    CSockInit m_sockInit;
};

#endif//Q_NETADDR_H_
