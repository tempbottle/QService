local iProtocol = Protocol.CS_Ping --������
local tReqMsg = {}--��Ϣ
--���������Ϣ

createMsg(iProtocol, tReqMsg)
g_objToolDlg:sendBMainMsg()
