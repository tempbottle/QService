--[[
ping
--]]

--ping ��ʱʱ��(��)
local m_iPingTimeOut = 5 * 60

--[[
������ping
������
����ֵ����
--]]
local function CSPing(tbMessage)
    local objCurSession = g_objSessionMgr:getCurSession()    
    objCurSession:setPing(g_objSessionMgr:getCount())
    --g_objSessionMgr:sendToCur(Protocol.SC_Ping, "", 0)
end
regNetEvent(Protocol.CS_Ping, CSPing)

--[[
������ping����
������
����ֵ����
--]]
local function SCPing(tbMessage)
    local objCurSession = g_objSessionMgr:getCurSession()    
    objCurSession:setPing(g_objSessionMgr:getCount())
end
regNetEvent(Protocol.SC_Ping, SCPing)

--[[
������ping���
������
����ֵ����
--]]
local function pingCheck(strIDCard, iSessionID)
    local objSession = g_objSessionMgr:getSessionByID(iSessionID)
    if not objSession then
        return
    end
    
    if strIDCard ~= objSession:getIDCard() then
        return
    end
    
    if ((g_objSessionMgr:getCount() - objSession:getPing()) * 
        g_objSessionMgr:getTimer()) >= m_iPingTimeOut then
        
        Debug(string.format("ID Card %s, session id %d timeout.", strIDCard, iSessionID))
        closeLink(iSessionID)
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
������ping���ע��
������
����ֵ����
--]]
local function pingOnLinkedOther(objSession)
    local strIDCard = getID()
    objSession:setIDCard(strIDCard)
    
    regDelayEvent(m_iPingTimeOut, pingCheck, strIDCard, objSession:getSessionID())
    --����ע�����ping��
end
regGameEvent(GameEvent.LinkedOther, pingOnLinkedOther)
