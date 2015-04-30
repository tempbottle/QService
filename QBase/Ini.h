/*****************************************************************************
 * Copyright (c) 2011-2012. Qifu Luo All Rights Reserved.200309129@163.com 
 * svn:http://asuraserver.googlecode.com/svn/
 * github:https://github.com/QService/QService
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this std::list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this std::list of conditions and the following disclaimer in the
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
