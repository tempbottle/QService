--[[
Э����,Э���ַ�
--]]

--Э����
System_DoString = 0                                          --ִ������
System_DoStringRtn = System_DoString + 1                     --ִ�������
System_RegSV = System_DoStringRtn + 1                        --��������ע��
System_RegSVRtn = System_RegSV + 1                           --��������ע�᷵��

CS_Ping = System_RegSVRtn + 1                                --ping
CS_LogIn = CS_Ping + 1                                       --�ͻ��������½
SA_LogIn = CS_LogIn + 1                                      --��Ϸ���������˺ŷ�����������֤����
AS_LogIn = SA_LogIn + 1                                      --�˺ŷ�����������֤����
SC_LogIn = AS_LogIn + 1                                      --��ͻ��˷�����֤��Ϣ
DB_LoadPlayer = SC_LogIn + 1                                 --������������Ϣ
SC_AccountInfo = DB_LoadPlayer + 1                           --�˺Ž�ɫ��Ϣ
CS_CreatPlayer = SC_AccountInfo + 1                          --������ɫ
DB_CreatPlayer = CS_CreatPlayer + 1                          --���ݿⴴ����ɫ
SC_CreatPlayer = DB_CreatPlayer + 1                          --������ɫ����
CS_SelectPlayer = SC_CreatPlayer + 1                         --ѡ���ɫ������Ϸ
SC_SelectPlayer = CS_SelectPlayer + 1                        --ѡ���ɫ������Ϸ����
DB_Log          = SC_SelectPlayer + 1                        --���ݿ���־
DB_SavePlayer   = DB_Log + 1                                 --���������Ϣ
SC_KickPlayer   = DB_SavePlayer + 1                          --�޳����

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

ProtocolStr_Log_Type    = "logtype"                          --��־����
ProtocolStr_Log_Param0  = "p0"                               --��־����0
ProtocolStr_Log_Param1  = "p1"                               --��־����1
ProtocolStr_Log_Param2  = "p2"                               --��־����2
ProtocolStr_Log_Param3  = "p3"                               --��־����3
ProtocolStr_Log_Param4  = "p4"                               --��־����4
ProtocolStr_Log_Memo    = "memo"                             --��־description
