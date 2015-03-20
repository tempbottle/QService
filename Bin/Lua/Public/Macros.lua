--[[
�궨��
--]]

--��ЧID
Q_INVALID_ID = -1

--session״̬ ��c++��enum SessionStatus��Ӧ
SessionStatus = {
    "Closed",
    "Linked",
    "ShakeHands",--�Ѿ�����,websockʹ��
    "Logining",  --��½��
    "Logined",  --�Ѿ���½
    "Loading",  --���������Ϣ��
    "Loaded",  --���������Ϣ���
    "Createing",  --���������
    "Created",  --����������
    "Playing",  --��Ϸ��
    "GM",--GM��¼
}
SessionStatus = creatEnumTable(SessionStatus, -1)

--��Ϣ��������
CarrierType = {
    "Json",
    "Protobuf",
}
CarrierType = creatEnumTable(CarrierType, 1)

--����ʹ�õ���Ϣ��������
MsgCarrier = CarrierType.Protobuf

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
    "Close",--���ӹر�
    "HttpRead",--http
}
GameEvent = creatEnumTable(GameEvent, 1)

SessionType = {
    "TcpClient",
    "SVLinker",
    "WebSock",
}
SessionType = creatEnumTable(SessionType)

--�ı���־����
LOGLV_NOLOG  = 0
LOGLV_ERROR  = 300
LOGLV_WARN   = 400
LOGLV_INFO   = 500
LOGLV_DEBUG  = 600
