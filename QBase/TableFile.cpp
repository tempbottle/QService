
#include "TableFile.h"

CTableFile::CTableFile(void)
{
    m_itNowRow = m_lstAllValue.begin();
    m_bEof = true;
    m_iRemoveCount = Q_INIT_NUMBER;
}

void CTableFile::setFile(const char *pszFile)
{
    assert(NULL != pszFile);
    m_strFile = pszFile;
}

const char *CTableFile::getFile(void) const
{
    return m_strFile.c_str();
}

void CTableFile::setSplitFlag(const char *pszSplitFlag)
{
    assert(NULL != pszSplitFlag);
    m_strSplitFlag = pszSplitFlag;
}

bool CTableFile::checkNote(const std::string &strMsg) const
{
    if ((std::string("//") == strMsg.substr(0, strlen("//")))
        || (std::string("#") == strMsg.substr(0, strlen("#"))))
    {
        return true;
    }

    return false;
}

int CTableFile::Parse(void)
{
    std::fstream inStream(m_strFile.c_str(), std::ios::in);
    if (!inStream.good())
    {
        Q_Printf("open file %s error.", m_strFile.c_str());
        inStream.close();

        return Q_RTN_FAILE;
    }
    
    bool bHaveHead = false;
    std::string strTmp;
    char pBuffer[Q_ONEK * 2] = {0};    

    m_iRemoveCount = Q_INIT_NUMBER;

    //��ȡ��ͷ
    while(inStream.good())
    {
        inStream.getline(pBuffer, (std::streamsize)(sizeof(pBuffer) - 1));
        strTmp = pBuffer;
        strTmp = Q_Trim(strTmp);
        if (strTmp.empty())
        {
            continue;
        }

        if (checkNote(strTmp))
        {
            continue;
        }

        if (!getHead(strTmp, m_mapTableHead))
        {
            inStream.close();

            return Q_RTN_FAILE;
        }

        bHaveHead = true;
        break;
    }

    if (!bHaveHead)
    {
        inStream.close();

        return Q_RTN_FAILE;
    }    
    
    //��ȡֵ
    while(inStream.good())
    {
        Q_Zero(pBuffer, sizeof(pBuffer));
        inStream.getline(pBuffer, (std::streamsize)(sizeof(pBuffer) - 1));
        strTmp = pBuffer;

        if (strTmp.empty())
        {
            continue;
        }

        if (checkNote(strTmp))
        {
            continue;
        }

        getValue(strTmp, m_mapTableHead);      
    }

    inStream.close();

    m_itNowRow = m_lstAllValue.begin();
    checkEof();

    return Q_RTN_OK;
}

void CTableFile::getValue(const std::string &strValue, std::map<int, std::string> &mapTableHead)
{
    bool bAllEmpty = true;
    int iIndex = Q_INIT_NUMBER;
    std::list<std::string> lstValue;
    std::list<std::string>::iterator itValue;
    std::map<std::string, std::string> mapVal;

    Q_Split(strValue, m_strSplitFlag.c_str(), lstValue);

    for (itValue = lstValue.begin(); (lstValue.end() != itValue) && ((size_t)iIndex < mapTableHead.size()); 
        itValue++)
    {
        if(!(Q_Trim(*itValue).empty()))
        {
            bAllEmpty = false;
        }
    }

    if (bAllEmpty)
    {
        return;
    }

    size_t iCount = Q_INIT_NUMBER;
    for (itValue = lstValue.begin(); (lstValue.end() != itValue) && ((size_t)iIndex < mapTableHead.size()); 
        itValue++)
    {
        if (iCount >= (lstValue.size() - (size_t)m_iRemoveCount))
        {
            break;
        }

        (void)mapVal.insert(std::make_pair((mapTableHead.find(iIndex)->second), *itValue));
        iIndex++;
        iCount++;
    }

    m_lstAllValue.push_back(mapVal);
}

bool CTableFile::getHead(const std::string &strHead, std::map<int, std::string> &mapTableHead)
{
    std::list<std::string> lstTableHead;

    Q_Split(strHead, m_strSplitFlag.c_str(), lstTableHead);
    //��¼����Ŀ�ֵ��
    for (std::list<std::string>::reverse_iterator itTmp = lstTableHead.rbegin(); 
        lstTableHead.rend() != itTmp; itTmp++)
    {
        if (!(itTmp->empty()))
        {
            break;
        }

        m_iRemoveCount++;
    }
    if (!checkHead(lstTableHead))//����Ƿ��п�ֵ
    {
        Q_Printf("%s", "Invalid table head.");

        return false;
    }

    if (!parseHead(lstTableHead, mapTableHead))//����
    {
        Q_Printf("%s", "parse table head error.");

        return false;
    }

    return true;
}

bool CTableFile::parseHead(std::list<std::string> &lstTableHead, std::map<int, std::string> &mapTableHead) const
{
    int iIndex = Q_INIT_NUMBER;
    std::string strTmp;
    std::list<std::string>::iterator itHead;    
    std::map<int, std::string>::iterator itMapHead;

    size_t iCount = Q_INIT_NUMBER;
    for (itHead = lstTableHead.begin(); lstTableHead.end() != itHead; itHead++)
    {
        if (iCount >= (lstTableHead.size() - (size_t)m_iRemoveCount))
        {
            break;
        }

        strTmp = Q_Trim(*itHead);

        //�Ƿ����ظ���
        for (itMapHead = mapTableHead.begin(); mapTableHead.end() != itMapHead; itMapHead++)
        {
            if (itMapHead->second == strTmp)
            {
                Q_Printf("repeat name %s", strTmp.c_str());

                return false;
            }
        }

        (void)mapTableHead.insert(std::make_pair(iIndex, strTmp));
        iIndex++;
        iCount++;
    }

    return true;
}

bool CTableFile::checkHead(std::list<std::string> &lstTableHead) const
{
    if (lstTableHead.empty())
    {
        return false;
    }

    size_t iCount = Q_INIT_NUMBER;
    std::list<std::string>::iterator itHead;
    for (itHead = lstTableHead.begin(); lstTableHead.end() != itHead; itHead++)
    {
        if (iCount >= (lstTableHead.size() - (size_t)m_iRemoveCount))
        {
            break;
        }

        if (itHead->empty())
        {
            return false;
        }

        iCount++;
    }

    return true;
}

void CTableFile::checkEof(void)
{    
    if (m_lstAllValue.end() == m_itNowRow)
    {
        m_bEof = true;
    }
    else
    {
        m_bEof = false;
    }
}

bool CTableFile::eof(void) const
{
    return m_bEof;
}

void CTableFile::nextRow(void)
{
    m_itNowRow++;

    checkEof();
}

void CTableFile::reSet(void)
{
    m_itNowRow = m_lstAllValue.begin();

    checkEof();
}

const char *CTableFile::getStringValue(const char *pszName, const char *pszDefault)
{
    if (m_lstAllValue.end() == m_itNowRow)
    {
        return pszDefault;
    }

    std::map<std::string, std::string>::iterator itVal;

    itVal = m_itNowRow->find(std::string(pszName));
    if (m_itNowRow->end() == itVal)
    {
        Q_Printf("in file %s line %s not find...........", m_strFile.c_str(), pszName);
        return pszDefault;
    }

    return itVal->second.c_str();
}

int CTableFile::getIntValue(const char *pszName, const int iDefault)
{
    const char *pszVal = getStringValue(pszName);
    if (0 == strlen(pszVal))
    {
        return iDefault;
    }

    return atoi(pszVal);
}

double CTableFile::getFloatValue(const char *pszName, const double dDefault)
{
    const char *pszVal = getStringValue(pszName);
    if (0 == strlen(pszVal))
    {
        return dDefault;
    }

    return atof(pszVal);
}
