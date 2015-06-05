--[[
���ú���
--]]

--[[
������session��ȡ
������
����ֵ����
--]]
function getCurSession()
    return g_objSessionMgr:getCurSession()
end

function getSessionByID(iSessionID)
    return g_objSessionMgr:getSessionByID(iSessionID)
end

--[[
����������
������
����ֵ����
--]]
function sendToCur(iProtocol, tMsg)
    local objCur = getCurSession()
    if not objCur then
        return
    end
    
    createMsg(iProtocol, tMsg)
    g_objSessionMgr:sendBToCur()
end

function sendToID(iSessionID, iProtocol, tMsg)
    if Q_INVALID_SOCK == iSessionID then
        return
    end
    
    createMsg(iProtocol, tMsg)
    g_objSessionMgr:sendBToByID(iSessionID)
end

function sendToName(strName, iProtocol, tMsg)
    local iSessionID = g_objSessionMgr:getLinkOtherID(strName)
    if Q_INVALID_SOCK == iSessionID then
        return
    end
    
    sendToID(iSessionID, iProtocol, tMsg)
end

function sendToType(usType, iProtocol, tMsg)
    local tID = g_objSessionMgr:getLinkOtherByType(usType)
    if table.empty(tID) then
        return
    end
    
    local iRand = 1
    if #tID > 1 then
        iRand = math.random(1, #tID)
    end
    local iSessionID = tID[iRand]
    
    sendToID(iSessionID, iProtocol, tMsg)
end

function broadcastToType(usType, iProtocol, tMsg)
    local tID = g_objSessionMgr:getLinkOtherByType(usType)
    if table.empty(tID) then
        return
    end
    
    createMsg(iProtocol, tMsg)
    
    for _, val in pairs(tID) do
        g_objSessionMgr:sendBToByID(val)
    end
end

--[[
���������ӹر�
������
����ֵ�� ��
--]]
local function realClose(iSessionID, strIDCard)
    local objSession = getSessionByID(iSessionID)
    if objSession:getIDCard() ~= strIDCard then
        return
    end
    
    g_objSessionMgr:closeLinkByID(iSessionID)
end

function closeCur()
    local objSession = getCurSession()
    if not objSession then
        return
    end
    
    local iSessionID = objSession:getSessionID()
    local strIDCard = objSession:getIDCard()
    
    sendToCur(Protocol.SC_Close)
    
    regDelayEvent(5, realClose, iSessionID, strIDCard)
end

function closeID(iSessionID)
    local objSession = getSessionByID(iSessionID)
    if not objSession then
        return
    end
    
    local iSessionID = objSession:getSessionID()
    local strIDCard = objSession:getIDCard()
    
    sendToID(iSessionID, Protocol.SC_Close)
    
    regDelayEvent(5, realClose, iSessionID, strIDCard)
end

--[[
�������˳�������
������
����ֵ�� ��
--]]
function shutDown()
    g_objSessionMgr:confirmStop()
end

--[[
���������tcp����
������
����ֵ�� ��
--]]
function addLinker(strIp, usPort, strName, usType)
    assert(strIp and usPort and strName and usType)
    
    g_objSessionMgr:addLinkOther(strIp, usPort, strName, usType)
end
