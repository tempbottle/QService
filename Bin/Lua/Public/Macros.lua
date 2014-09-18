--[[
�궨��
--]]

--��ЧID
Q_INVALID_ID = -1

--�������ÿ�η�������
SavePlayer_PreNum  = 100

--���ݿ���־����
DBLogType = {
    "LogIn",
    "LogOut",
    "CreatePlayer",
}
DBLogType = CreatEnumTable(DBLogType)

--session״̬ ��c++��enum SessionStatus��Ӧ
SessionStatus = {
    "Unknown",  --δ֪
    "Logining",  --��½��
    "Logined",  --�Ѿ���½
    "Loading",  --���������Ϣ��
    "Loaded",  --���������Ϣ���
    "Createing",  --���������
    "Created",  --����������
    "Playing",  --��Ϸ��
}
SessionStatus = CreatEnumTable(SessionStatus, -1)

--����������
SVType = {
    "Account",
    "Payment",
    "Game",
    "Fight",
    "DataBase",
}
SVType = CreatEnumTable(SVType)

--��Ϸ�¼�
GameEvent = {
    "StartUp",
    "ShutDown",

    "FPS",
    "DayChange",

    "OneSecond",
    "FiveSecond",
    "TenSecond",
    "OneMinute",
    "FiveMinute",
    "TenMinute",
    "OneHour",

    "Close",

    "LogIned",
}
GameEvent = CreatEnumTable(GameEvent)

--��־����
LOGLV_NOLOG  = 0
LOGLV_ERROR  = 300
LOGLV_WARN   = 400
LOGLV_INFO   = 500
LOGLV_DEBUG  = 600
