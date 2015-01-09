--[[
�궨��
--]]

--��ЧID
Q_INVALID_ID = -1

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
SessionStatus = creatEnumTable(SessionStatus, -1)

--����������
SVType = {
    "Account",--�˺�
    "Payment",--֧��
    "Game",   --��Ϸ
    "Fight",  --ս��
    "DataBase",--���ݿ�
}
SVType = creatEnumTable(SVType, 1)

--��Ϸ�¼�
GameEvent = {
    "Start",--����
    "Started",--�������
    "ShutDown",--�ر�
    "FPS",--ÿ֡
    "DayChange",--���
    "WeekChange",--�ܱ�
    "MonthChange",--�±�
    "OneSecond",--1��
    "FiveSecond",--5��
    "TenSecond",--10��
    "OneMinute",--1����
    "FiveMinute",--5����
    "TenMinute",--10����
    "OneHour",--1Сʱ    
    "Connected",--�����ӣ����ӳɹ�
    "Close",--���ӹر�
}
GameEvent = creatEnumTable(GameEvent, 1)

--�ı���־����
LOGLV_NOLOG  = 0
LOGLV_ERROR  = 300
LOGLV_WARN   = 400
LOGLV_INFO   = 500
LOGLV_DEBUG  = 600
