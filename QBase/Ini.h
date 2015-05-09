
#ifndef Q_INI_H_
#define Q_INI_H_

#include "QString.h"

/*
INI�ļ�����
ע����#��//��ͷ
*/
class CIniFile
{
public:
    CIniFile(void);
    explicit CIniFile(const char *pszFile);
    ~CIniFile(void);

    /*����INI�ļ�*/
    void setFile(const char *pszFile);

    /*��ȡֵ*/
    const char *getStringValue(const char *pszNode, const char *pszKey, const char *pszDefault = "");
    int getIntValue(const char *pszNode, const char *pszKey, int iDefault = Q_INIT_NUMBER);
    double getFloatValue(const char *pszNode, const char *pszKey, double dDefault = 0.0);
    
    /*����ֵ*/
    void setStringValue(const char *pszNode, const char *pszKey, 
        const char *pszValue);
    void setIntValue(const char *pszNode, const char *pszKey, 
        int iValue);
    void setFloatValue(const char *pszNode, const char *pszKey, 
        double dValue);

    /*ɾ��*/
    void delKey(const char *pszNode, const char *pszKey);
    void delNode(const char *pszNode);

    /*����*/
    void Save(void);
    void saveTo(const char *pszFile);

private:
    void readInfo(void);
    bool isNote(const std::string &str) const;
    void removeNote(std::string &str) const;
    bool isNode(const std::string &str) const;    
    std::string getNode(const std::string &str) const;
    bool isKey(const std::string &str) const;
    std::string getKey(const std::string &str) const;
    std::string getVal(const std::string &str) const;

private:
    struct stIniKey
    {
        std::string strKey;
        std::string strValue;
    };

    struct stIniNode
    {
        std::string strNode;
        std::list<stIniKey> lstKey;
    };

    std::string m_strFile;
    std::list<stIniNode> m_lstNodeInfo;
};

#endif//Q_INI_H_
