
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
