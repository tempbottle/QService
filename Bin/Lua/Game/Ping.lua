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
    local objCurSession = g_objSessionManager:getCurSession()
    local iStatus = objCurSession:getStatus()
    if (SessionStatus.Playing ~= iStatus) 
        and (SessionStatus.GM ~= iStatus) then
        return
    end
    
    objCurSession:setPing(g_objSessionManager:getCount())
end
regNetEvent(Protocol.CS_Ping, CSPing)

--[[
������ping���
������
����ֵ����
--]]
local function CheckPing()
    g_objSessionManager:checkPing(PingTimeOut)
end
regGameEvent(GameEvent.FiveMinute, CheckPing)
