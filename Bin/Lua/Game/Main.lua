--[[
��lua
--]]

--·�����
local luaDir = Q_GetModulPath() .. "Lua" .. Q_GetPathSeparator()
package.path = package.path .. ";" .. luaDir .. "Public"..Q_GetPathSeparator().."?.lua"
package.path = package.path .. ";" .. luaDir .. "Game"..Q_GetPathSeparator().."?.lua"

require("InitModule")

local tNowDay = os.date("*t", time)

if not g_objSessionManager then
    g_objSessionManager = nil
end

--[[
���������������ɹ������
������
����ֵ�� ��
--]]
function Lua_OnStartUp(objSessionManager)
    g_objSessionManager = objSessionManager
    OnGameEvent(GameEvent_StartUp)
end

--[[
����������ر�ʱ����
������
����ֵ����
--]]
function Lua_OnShutDown()
    OnGameEvent(GameEvent_ShutDown)    
end

--[[
���������ӿɶ��¼�
������
����ֵ����
--]]
function Lua_OnRead(pszMessage, uiLens)
    Debug(string.sub(pszMessage, 1, uiLens))
    local tbMessage = cjson.decode(string.sub(pszMessage, 1, uiLens))
    local iOpCode = tbMessage[Protocol_Request]
    if not iOpCode then
        Debug("can't find protocol, close this link.")
        g_objSessionManager:closeCurLink()

        return
    end
    
    local objCurSession = g_objSessionManager:getCurSession()
    if not objCurSession then
        Debug("current session is nil.")
        g_objSessionManager:closeCurLink()

        return
    end

    local bServerLinker = objCurSession:isServerLinker()
    if not bServerLinker then
        local iStatus = objCurSession:getStatus()

        --���Ϊ��½������˵�½���������Ķ�������
        if (LinkStatus_LogIned ~= iStatus) 
            and (CS_LogIn ~= iOpCode) then
                Debug("not logined.")
                return
        end
        
        --����Ѿ���½�ɹ����ٷ��͵�½�����򲻴���
        if (LinkStatus_LogIned == iStatus) 
            and (CS_LogIn == iOpCode) then
                Debug("already logined.")
                return
        end
    end
    
    Debug("protocol is " .. iOpCode)    
    OnNetEvent(iOpCode, tbMessage)    
end

--[[
��������ʱ������
������
����ֵ����
--]]
function Lua_OnTimer()
    local uiCount = g_objSessionManager:getCount()
    local uiClick = g_objSessionManager:getTimer()
    local uiElapseTime = uiClick * uiCount
    local uiOneSecond = 1000
    
    --ÿ֡����
    OnGameEvent(GameEvent_FPS, uiClick)
    
    --1��
    if 0 == (uiElapseTime % uiOneSecond) then
        OnDelayEvent()
        OnGameEvent(GameEvent_1Second)        
        
        --������
        local tDay = os.date("*t", time)
        if (tDay.year ~= tNowDay.year) 
            or (tDay.month ~= tNowDay.month) 
            or (tDay.day ~= tNowDay.day) then
                --�����¼�
                tNowDay = tDay
                Debug("day changed.")
                
                OnGameEvent(GameEvent_DayChange)
        end
    end
    
    --5��
    if 0 == (uiElapseTime % (uiOneSecond * 5)) then
        OnGameEvent(GameEvent_5Second)
    end
    
    --10��
    if 0 == (uiElapseTime % (uiOneSecond * 10)) then
        OnGameEvent(GameEvent_10Second)
    end
    
    --1����
    if 0 == (uiElapseTime % (uiOneSecond * 60)) then
        OnGameEvent(GameEvent_1Minute)
    end
    
    --5����
    if 0 == (uiElapseTime % (uiOneSecond * 60 * 5)) then
        OnGameEvent(GameEvent_5Minute)
        collectgarbage("collect")
    end
    
    --10����
    if 0 == (uiElapseTime % (uiOneSecond * 60 * 10)) then
        OnGameEvent(GameEvent_10Minute)
    end    
end

--[[
���������ӶϿ�ʱ����
������
����ֵ����
--]]
function Lua_OnClose()
    OnGameEvent(GameEvent_LogOut)
end

--[[
�����������������ӳɹ�ʱ����
������
����ֵ����
--]]
function Lua_OnLinkedServer(objSession)
    local tRegSV = {}
    tRegSV[Protocol_Request] = Server_RegServer
    tRegSV[Protocol_ServerID] = getServerID()
    
    local strMsg = cjson.encode(tRegSV)
    g_objSessionManager:sendToByID(objSession:getSessionID(), strMsg, string.len(strMsg))
end
