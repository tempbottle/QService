--[[
�궨��
--]]

--����������
SVType_Game = 0
SVType_Account = 1
SVType_Payment = 2
SVType_Fight = 3

--����û���
MaxClintNum = 2000

--�ӳٹر�ʱ��
DelayCloseTime = 2
--��½��ʱʱ��
LoginTimeOut = 10

--��־����
LOGLV_NOLOG  = 0
LOGLV_ERROR  = 300
LOGLV_WARN   = 400
LOGLV_INFO   = 500
LOGLV_DEBUG  = 600

--����״̬
LinkStatus_Invalid = -1
LinkStatus_LogIning = 0
LinkStatus_LogIned = 1

--��Ϸ�¼�
GameEvent_StartUp = 0
GameEvent_ShutDown = GameEvent_StartUp + 1

GameEvent_FPS = GameEvent_ShutDown + 1
GameEvent_DayChange = GameEvent_FPS + 1
GameEvent_1Second = GameEvent_DayChange + 1
GameEvent_5Second = GameEvent_1Second + 1
GameEvent_10Second = GameEvent_5Second + 1
GameEvent_1Minute = GameEvent_10Second + 1
GameEvent_5Minute = GameEvent_1Minute + 1
GameEvent_10Minute = GameEvent_5Minute + 1

GameEvent_LogOut = GameEvent_10Minute + 1

--Э���ַ�
Protocol_Request = "rq" --������
Protocol_Account = "acc" --�˺�
Protocol_RTN = "rtn" --������
Protocol_PSW = "psw" --����
Protocol_ClientID = "cid" --�ͻ���session id
Protocol_CheckID = "ckid" --��֤id
Protocol_DebugStr = "dstr" --�����ַ�
Protocol_DebugStrRtn = "drtnstr" --�����ַ�ִ�з���
Protocol_ServerID = "svid" --������ID
