--[[
ping
--]]

--ping ��ʱʱ��(��)
local m_iPingTimeOut = 5 * 60
local m_iSendPintTime = 2 * 60

--[[
������ping
������
����ֵ����
--]]
local function CSPing(tbMessage)
    local objCurSession = getCurSession()    
    objCurSession:setPing(g_objSessionMgr:getCount())
    
    sendToCur(Protocol.SC_Ping)
end
regNetEvent(Protocol.CS_Ping, CSPing)

--[[
������ping����
������
����ֵ����
--]]
local function SCPing(tbMessage)
    local objCurSession = getCurSession()    
    objCurSession:setPing(g_objSessionMgr:getCount())
end
regNetEvent(Protocol.SC_Ping, SCPing)

--[[
������ping���
������
����ֵ����
--]]
local function pingCheck(strIDCard, iSessionID)
    local objSession = getSessionByID(iSessionID)
    if not objSession then
        return
    end
    
    if strIDCard ~= objSession:getIDCard() then
        return
    end
    
    local iPassTime = (g_objSessionMgr:getCount() - objSession:getPing()) * g_objSessionMgr:getTimer()
    iPassTime = math.floor(iPassTime / 1000)
    if iPassTime >= m_iPingTimeOut then
        Debug("ID Card %s, session id %d timeout.", strIDCard, iSessionID)
        closeID(iSessionID)
        
        return
    end
    
    regDelayEvent(m_iPingTimeOut, pingCheck, strIDCard, iSessionID)
end

--[[
������ping���ע��
������
����ֵ����
--]]
local function pingOnConnected(objSession)
    local strIDCard = getID()
    objSession:setIDCard(strIDCard)
    
    regDelayEvent(m_iPingTimeOut, pingCheck, strIDCard, objSession:getSessionID())
end
regGameEvent(GameEvent.OnConnected, pingOnConnected)

--[[
������ping����
������
����ֵ����
--]]
local function pingSend(strIDCard, iSessionID)
    local objSession = getSessionByID(iSessionID)
    if not objSession then
        return
    end
    
    if strIDCard ~= objSession:getIDCard() then
        return
    end
    
    sendToID(iSessionID, Protocol.CS_Ping)
    
    regDelayEvent(m_iSendPintTime, pingSend, strIDCard, iSessionID)
end

--[[
������ping���ע��
������
����ֵ����
--]]
local function pingOnLinkedOther(objSession)
    local strIDCard = getID()
    objSession:setIDCard(strIDCard)
    
    regDelayEvent(m_iPingTimeOut, pingCheck, strIDCard, objSession:getSessionID())
    regDelayEvent(m_iSendPintTime, pingSend, strIDCard, objSession:getSessionID())
end
regGameEvent(GameEvent.LinkedOther, pingOnLinkedOther)
