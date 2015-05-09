
#ifndef Q_DBPOOL_H_
#define Q_DBPOOL_H_

#include "QMySQL.h"
#include "Sqlite.h"
#include "Cond.h"

/*���ݿ����ӳ�*/
class CDBPool
{
public:
    CDBPool(void);
    ~CDBPool(void);

    void setUrl(CDBUrl &objUrl);
    /*���ݿ����ӳ���С������*/
    void setMinSize(unsigned short usMinPoolSize);
    /*���ݿ����ӳ����������*/
    void setMaxSize(unsigned short usMaxPoolSize);
    void setType(DBType emType);
    /*����ʱ��(��)�� ��ʱ������δʹ�õĶ�������*/
    void setMaxFreeTime(unsigned int usS);

    int Start(void);
    void Stop(void);

    CDBLink *mallocLinker(void);
    void freeLinker(CDBLink *pLinker);

private:
    int getLinker(CDBLink **ppLinker);
    CDBLink *getFromMin(void);
    CDBLink *getFromMax(void);

private:
    long m_lRunFlag;
    CDBUrl m_objUrl;
    unsigned short m_usMinPoolSize;
    unsigned short m_usMaxPoolSize;
    unsigned short m_usMaxLinkerNum;
    DBType m_emType;
    unsigned int m_uiFreeTime;
    std::list<CDBLink *> m_lstMinLinker;
    std::list<struct stBDMaxPoolInfo *> m_lstMaxLinker;
    CQMutex m_objMinMutex;
    CQMutex m_objMaxMutex;
    CQMutex m_objMutex_Exit;
    CCond m_objCond_Exit;
    CQMutex m_objMutex_Start;
    CCond m_objCond_Start;
};

#endif//Q_DBPOOL_H_
