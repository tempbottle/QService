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

#ifndef Q_FILE_H_
#define Q_FILE_H_

#include "QString.h"

/*��ȡ�ļ���С*/
int Q_FileSize(const char *pszFileName, unsigned long &ulSize);
/*�ж��ļ��Ƿ����*/
int Q_FileExist(const char *pszFileName);
/*ɾ���ļ�*/
int Q_FileDel(const char *pszFileName);
/*�ļ�����*/
int Q_FileReName(const char *pszOldNam, const char *pszNewNam);

/*�����ļ�·��*/
int Q_DirName(const char *pszPath, std::string &strPath);
/*�ж��ļ�·���Ƿ����*/
int Q_DirExits(const char *pszPath);
/*��ȡ��Ŀ¼,windows Ϊ�̷� linux Ϊ/�µ���Ŀ¼*/
int Q_GetDrivce(std::list<std::string> &lstDrivce);
/*�����ļ���*/
int Q_MakeDir(const char *pszPath);
/*������·�������в����ڵ��ļ���*/
int Q_MakeAllDir(const char *pszPath);
/*��ȡ��������·��*/
int Q_GetProPath(std::string &strPath);
/*�ı䵱ǰ����Ŀ¼*/
int Q_ChDir(const char *pszPath);
/*ɾ����Ŀ¼*/
int Q_DelDir(const char *pszPath);
/*ɾ��Ŀ¼*/
void Q_DelAllDir(const char *pszPath);

/*��ȡ��Ŀ¼*/
int Q_GetSubDirName(const char *pszParentPathName, std::list<std::string> &lstDirName);
/*��ȡĿ¼�������ļ�*/
int Q_GetAllFileName(const char *pszParentPathName, std::list<std::string> &lstFileName);

/*�������Ƿ��Ѿ�����*/
int Q_RunOne(void);
/*�ļ�����*/
int Q_FileCopy(const char *pszOldPath, const char *pszNewPath);

#endif//Q_FILE_H_
