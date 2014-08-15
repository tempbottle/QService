--[[
�궨��
--]]

local Macros = {}

--����������
Macros.SVType_Game = 0
Macros.SVType_Account = 1
Macros.SVType_Payment = 2
Macros.SVType_Fight = 3

--����û���
Macros.MaxClintNum = 2000

--�ӳٹر�ʱ��
Macros.DelayCloseTime = 2

--��־����
Macros.LOGLV_NOLOG  = 0
Macros.LOGLV_ERROR  = 300
Macros.LOGLV_WARN   = 400
Macros.LOGLV_INFO   = 500
Macros.LOGLV_DEBUG  = 600

--����״̬
Macros.LinkStatus_Invalid = -1
Macros.LinkStatus_LogIned = 0

--��Ϸ�¼�
Macros.GameEvent_StartUp = 0
Macros.GameEvent_ShutDown = Macros.GameEvent_StartUp + 1

Macros.GameEvent_FPS = Macros.GameEvent_ShutDown + 1
Macros.GameEvent_DayChange = Macros.GameEvent_FPS + 1
Macros.GameEvent_1Second = Macros.GameEvent_DayChange + 1
Macros.GameEvent_5Second = Macros.GameEvent_1Second + 1
Macros.GameEvent_10Second = Macros.GameEvent_5Second + 1
Macros.GameEvent_1Minute = Macros.GameEvent_10Second + 1
Macros.GameEvent_5Minute = Macros.GameEvent_1Minute + 1
Macros.GameEvent_10Minute = Macros.GameEvent_5Minute + 1

Macros.GameEvent_LogOut = Macros.GameEvent_10Minute + 1

--Э���ַ�
Macros.Protocol_Request = "rq" --������
Macros.Protocol_Account = "acc" --�˺�
Macros.Protocol_RTN = "rtn" --������
Macros.Protocol_PSW = "psw" --����
Macros.Protocol_ClientID = "cid" --�ͻ���session id
Macros.Protocol_CheckID = "ckid" --��֤id

return Macros
