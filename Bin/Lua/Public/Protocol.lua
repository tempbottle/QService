--[[
Э����,Э���ַ�
--]]

--Э����
Protocol = {
    "System_DoString", --ִ������
    "System_DoStringRtn",--ִ�������
    "System_RegSV",--��������ע��
    "System_RegSVRtn",--��������ע�᷵��

    "CS_Ping",--ping
    "CS_LogIn",--�ͻ��������½
    "SA_LogIn",--��Ϸ���������˺ŷ�����������֤����
    "AS_LogIn",--�˺ŷ�����������֤����
    "SC_LogIn",--��ͻ��˷�����֤��Ϣ
    "DB_LoadPlayer",--������������Ϣ
    "SC_AccountInfo",--�˺Ž�ɫ��Ϣ
    "CS_CreatPlayer",--������ɫ
    "DB_CreatPlayer",--���ݿⴴ����ɫ
    "SC_CreatPlayer",--������ɫ����
    "CS_SelectPlayer",--ѡ���ɫ������Ϸ
    "SC_SelectPlayer",--ѡ���ɫ������Ϸ����
    "DB_Log",--���ݿ���־
    "DB_SavePlayer",--���������Ϣ
    "SC_KickPlayer",--�޳����
}
Protocol = CreatEnumTable(Protocol)

--Э���ַ�
ProtocolStr_Request     = "rq"                               --������
ProtocolStr_Rtn         = "rtn"                              --������
ProtocolStr_DebugStr    = "dstr"                             --�����ַ�
ProtocolStr_DebugStrRtn = "drtnstr"                          --�����ַ�ִ�з���
ProtocolStr_ServerID    = "svid"                             --������ID
ProtocolStr_Flag        = "flag"                             --��־
ProtocolStr_Account     = "acc"                              --�˺�
ProtocolStr_PSW         = "psw"                              --����
ProtocolStr_ID          = "id"                               --ID
ProtocolStr_ClientID    = "cid"                              --����ID
ProtocolStr_CheckID     = "ckid"                             --У��ID
ProtocolStr_Number      = "num"                              --����
ProtocolStr_Info        = "info"                             --��ϸ��Ϣ
ProtocolStr_Name        = "nam"                              --����
ProtocolStr_Attr        = "attr"                             --����
