--[[
ִ������
--]]

--[[
������ִ���ַ�������
������
����ֵ����
--]]
local function Sys_DoStr(tbMessage)
    local Func = load(tbMessage[ProtocolStr_DebugStr])
    local tRtn = {}
    tRtn[Protocol.ProtocolStr_Request] = System_DoStringRtn
    if Func then
        local bRtn, rtnMsg = CallFunc(Func)
        if bRtn then
            tRtn[ProtocolStr_Rtn] = ErrorCode.OK
        else
            tRtn[ProtocolStr_Rtn] = ErrorCode.FAILE
        end
        
        tRtn[ProtocolStr_DebugStrRtn] = rtnMsg
    else
        tRtn[ProtocolStr_Rtn] = ErrorCode.FAILE
    end
    
    local strMsg = cjson.encode(tRtn)
    g_objSessionManager:sendToCur(strMsg, string.len(strMsg))
end
RegNetEvent(Protocol.System_DoString, Sys_DoStr)
