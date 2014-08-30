--[[
��½����
--]]

--[[
��������Ϸ�����������½
������
����ֵ�� ��
--]]
local function SALogIn(tbMessage)    
    --��������    
    local tSALogIn = {}
    tSALogIn[ProtocolStr_Request] = AS_LogIn
    tSALogIn[ProtocolStr_Rtn] = 0
    tSALogIn[ProtocolStr_ClientID] = tbMessage[ProtocolStr_ClientID]
    tSALogIn[ProtocolStr_CheckID] = tbMessage[ProtocolStr_CheckID]
    
    local strMsg = cjson.encode(tSALogIn)
    Debug("SALogIn send message "..strMsg.." to game server")
    
    g_objSessionManager:sendToCur(strMsg, string.len(strMsg))
end
RegNetEvent(SA_LogIn, SALogIn)
