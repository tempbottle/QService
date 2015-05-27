--[[
Lua�ڴ�ʹ�����
--]]

local m_iLogMemoTime = 60

local function onLogLuaMemory()
    local iTotalMem = collectgarbage("count")
    local strMemUsage = string.format("lua memory usage:%d,session size:%d", 
        iTotalMem, g_objSessionMgr:getSessionSize())
        
    Debug(strMemUsage)
    Q_LOG(LOGLV_INFO, strMemUsage)
    
    regDelayEvent(m_iLogMemoTime, onLogLuaMemory)
end

local function logLuaMemory()
    regDelayEvent(m_iLogMemoTime, onLogLuaMemory)
end
regGameEvent(GameEvent.Started, logLuaMemory)