--[[
ping
--]]

local PingTimeOut = 5 * 60 * 1000

--[[
������ping
������
����ֵ����
--]]
local function CSPing(tbMessage)
    local objCurSession = g_objSessionMgr:getCurSession()
    local iStatus = objCurSession:getStatus()
    if (SessionStatus.Playing ~= iStatus) 
        and (SessionStatus.GM ~= iStatus) then
        return
    end
    
    objCurSession:setPing(g_objSessionMgr:getCount())
    g_objSessionMgr:sendToCur(Protocol.SC_Ping, "", 0)
end
regNetEvent(Protocol.CS_Ping, CSPing)

--[[
������ping���
������
����ֵ����
--]]
local function CheckPing()
    g_objSessionMgr:checkPing(PingTimeOut)
end
regGameEvent(GameEvent.FiveMinute, CheckPing)
