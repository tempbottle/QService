
#include "QBase.h"

/*ϵͳ��־*/
CSysLoger g_SysLoger;

/*��־ȫ�ֱ���*/
CTxtLoger *g_pTxtLoger = NULL;
Q_SOCK g_TxtLogerFD = Q_INVALID_SOCK;

/*���ݿ���־*/
class CDBLoger *g_pDBLoger = NULL;
Q_SOCK g_DBLogerFD = Q_INVALID_SOCK;

/*�����˳��ź���*/
CQMutex g_objExitMutex;
CCond g_objExitCond;
/*��������·��*/
char g_acModulPath[Q_FILEPATH_LENS] = {0};
/*������ID*/
int g_iServerID = 0;
/*����������*/
std::string g_strServerName;
