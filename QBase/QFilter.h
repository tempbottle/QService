
#ifndef Q_FILTER_H_
#define Q_FILTER_H_

#include "Macros.h"

/*
�ַ�������
*/
class CFilter
{
public:
    CFilter(void);
    ~CFilter(void);

    /*������д�*/
    bool addFilterWord(const char *pszWord, const unsigned int uiLens); 
    /*����Ƿ�������д�*/
    bool checkHave(const char *pszWord, const unsigned int uiLens);
    /*������дʲ��滻*/    
    const char *Filter(const char *pWord, const unsigned int uiLens);
    /*����*/
    void Clear(void);

private:
    struct FilterMap
    {
        std::tr1::unordered_map<char, FilterMap*> mapFilterMap;
    };

    FilterMap *m_pmapFilter;

private:
    void freeNode(FilterMap *pFilterMap);
    void checkAndModify(char *pWord, const unsigned int &uiLens, const char cReplace = '*');
};

#endif//Q_FILTER_H_
