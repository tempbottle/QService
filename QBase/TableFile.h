
#ifndef Q_TABLEFILE_H_
#define Q_TABLEFILE_H_

#include "QString.h"
/*
��ȡ��ʽΪ��
���� ���� סַ
С�� 20   ��Ȫ
....
���ı��ļ�
ע��Ϊ#����//
*/
class CTableFile
{
public:
    CTableFile(void);
    ~CTableFile(void){};

    //�ļ���
    void setFile(const char *pszFile);
    const char *getFile(void) const;
    //��ֱ�־
    void setSplitFlag(const char *pszSplitFlag);

    //����
    int Parse(void);
    //�Ƿ���δ����
    bool eof(void) const;
    //��һ��
    void nextRow(void);
    //���ص���һ��
    void reSet(void);

    /*��ȡֵ*/
    const char *getStringValue(const char *pszName, const char *pszDefault = "");
    int getIntValue(const char *pszName, const int iDefault = 0);
    double getFloatValue(const char *pszName, const double dDefault = 0.0);

    /*����ͷ*/
    const std::map<int, std::string> *getHead(void) const
    {
        return &m_mapTableHead;
    };

private:
    void getValue(const std::string &strValue, std::map<int, std::string> &mapTableHead);
    bool getHead(const std::string &strHead, std::map<int, std::string> &mapTableHead);
    bool checkHead(std::list<std::string> &lstTableHead) const;
    bool parseHead(std::list<std::string> &lstTableHead, std::map<int, std::string> &mapTableHead) const;
    void checkEof(void);
    bool checkNote(const std::string &strMsg) const;

private:
    bool m_bEof;
    int m_iRemoveCount;
    std::string m_strFile;
    std::string m_strSplitFlag;
    std::map<int, std::string> m_mapTableHead;
    std::list<std::map<std::string, std::string> >::iterator m_itNowRow;
    std::list<std::map<std::string, std::string> > m_lstAllValue;
};

#endif//Q_TABLEFILE_H_
