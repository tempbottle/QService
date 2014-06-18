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
    const char *getFile(void);
    //��ֱ�־
    void setSplitFlag(const char *pszSplitFlag);

    //����
    int Parse(void);
    //�Ƿ���δ����
    bool eof(void);
    //��һ��
    void nextRow(void);
    //���ص���һ��
    void reSet(void);

    /*��ȡֵ*/
    const char *getStringValue(const char *pszName, const char *pszDefault = "");
    int getIntValue(const char *pszName, const int iDefault = 0);
    double getFloatValue(const char *pszName, const double dDefault = 0.0);

    /*����ͷ*/
    const std::map<int, std::string> *getHead(void)
    {
        return &m_mapTableHead;
    };

private:
    void getValue(std::string &strValue, std::map<int, std::string> &mapTableHead);
    bool getHead(std::string &strHead, std::map<int, std::string> &mapTableHead);
    bool checkHead(std::list<std::string> &lstTableHead);
    bool parseHead(std::list<std::string> &lstTableHead, std::map<int, std::string> &mapTableHead);
    void checkEof(void);
    bool checkNote(std::string &strMsg);

private:
    bool m_bEof;
    std::string m_strFile;
    std::string m_strSplitFlag;
    std::map<int, std::string> m_mapTableHead;
    std::list<std::map<std::string, std::string> >::iterator m_itNowRow;
    std::list<std::map<std::string, std::string> > m_lstAllValue;
};

#endif//Q_TABLEFILE_H_
