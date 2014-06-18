/*****************************************************************************
 * Copyright (c) 2011-2012. Qifu Luo All Rights Reserved.200309129@163.com 
 * svn:http://asuraserver.googlecode.com/svn/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#include "TableFile.h"

CTableFile::CTableFile(void)
{
    m_itNowRow = m_lstAllValue.begin();
    m_bEof = true;
}

void CTableFile::setFile(const char *pszFile)
{
    assert(NULL != pszFile);
    m_strFile = pszFile;
}

const char *CTableFile::getFile(void)
{
    return m_strFile.c_str();
}

void CTableFile::setSplitFlag(const char *pszSplitFlag)
{
    assert(NULL != pszSplitFlag);
    m_strSplitFlag = pszSplitFlag;
}

bool CTableFile::checkNote(std::string &strMsg)
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

    //��ȡ��ͷ
    while(inStream.good())
    {
        inStream.getline(pBuffer, sizeof(pBuffer) - 1);
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
        inStream.getline(pBuffer, sizeof(pBuffer) - 1);
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

void CTableFile::getValue(std::string &strValue, std::map<int, std::string> &mapTableHead)
{
    int iIndex = Q_INIT_NUMBER;
    std::list<std::string> lstValue;
    std::list<std::string>::iterator itValue;
    std::map<std::string, std::string> mapVal;

    Q_Split(strValue, m_strSplitFlag, lstValue);

    for (itValue = lstValue.begin(); (lstValue.end() != itValue) && ((size_t)iIndex < mapTableHead.size()); 
        itValue++)
    {
        mapVal.insert(std::make_pair((mapTableHead.find(iIndex)->second), Q_Trim(*itValue)));
        iIndex++;
    }

    m_lstAllValue.push_back(mapVal);
}

bool CTableFile::getHead(std::string &strHead, std::map<int, std::string> &mapTableHead)
{
    std::list<std::string> lstTableHead;

    Q_Split(strHead, m_strSplitFlag, lstTableHead);
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

bool CTableFile::parseHead(std::list<std::string> &lstTableHead, std::map<int, std::string> &mapTableHead)
{
    int iIndex = Q_INIT_NUMBER;
    std::string strTmp;
    std::list<std::string>::iterator itHead;    
    std::map<int, std::string>::iterator itMapHead;

    for (itHead = lstTableHead.begin(); lstTableHead.end() != itHead; itHead++)
    {
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

        mapTableHead.insert(std::make_pair(iIndex, strTmp));
        iIndex++;
    }

    return true;
}

bool CTableFile::checkHead(std::list<std::string> &lstTableHead)
{
    if (lstTableHead.empty())
    {
        return false;
    }

    std::list<std::string>::iterator itHead;
    for (itHead = lstTableHead.begin(); lstTableHead.end() != itHead; itHead++)
    {
        if (itHead->empty())
        {
            return false;
        }
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

bool CTableFile::eof(void)
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