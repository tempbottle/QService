
#ifndef Q_HTTP_PARSER_H_
#define Q_HTTP_PARSER_H_

#include "Macros.h"

class CHttpParser
{
public:
    CHttpParser(void);
    ~CHttpParser(void);

    bool setHttpRequest(struct evhttp_request *req);

    /* ��ȡ */
    const char *getQuery(void) const;
    const char *getPostMsg(void) const;

    /* ����������� */
    void setReplyContent(const char *pszMsg);
    /* ���� */
    void Reply(const int iCode, const char *pszReason);

private:
    struct evbuffer *m_pEventBuf;
    struct evhttp_request *m_Req;
    std::string m_strPostMsg;
    std::string m_strQuery;
};

#endif//Q_HTTP_PARSER_H_
