--[[
��lua
--]]

--·�����
local luaDir = Q_GetModulPath() .. "Lua" .. Q_GetPathSeparator()
package.path = package.path .. ";" .. luaDir .. "?.lua"

require("Game/InitModule")

local tNowDay = os.date("*t", time)

if not g_objSessionManager then
    g_objSessionManager = nil
end

--���������
if not g_ProtocolFilterFun then
    g_ProtocolFilterFun = nil
end

--��������ע��
if not g_CheckSVRegFun then
    g_CheckSVRegFun = nil
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
    local iProtocol = tbMessage[ProtocolStr_Request]
    if not iProtocol then
        Debug("can't find protocol, close this link.")
        g_objSessionManager:closeCurLink()

        return
    end
    
    local objCurSession = g_objSessionManager:getCurSession()   
    if not objCurSession:isServerLinker() then
        if g_ProtocolFilterFun then
            local iStatus = objCurSession:getStatus()
            if not g_ProtocolFilterFun(iProtocol, iStatus) then
                Q_LOG(LOGLV_WARN, string.format("session status %d, protocol %d was ignored.", iStatus, iProtocol))
                return
            end
        end
    end
    
    if g_CheckSVRegFun and (System_RegSV ~= iProtocol) then
        if not g_CheckSVRegFun(objCurSession:getSessionID()) then
            Debug(string.format("client id %d not register.", objCurSession:getSessionID()))
            g_objSessionManager:closeCurLink()
            return
        end
    end
    
    Debug("protocol is " .. iProtocol)    
    OnNetEvent(iProtocol, tbMessage)    
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
	
    --1Сʱ
    if 0 == (uiElapseTime % (uiOneSecond * 60 * 60)) then
        OnGameEvent(GameEvent_1Hour)
    end
end

--[[
���������ӶϿ�ʱ����
������
����ֵ����
--]]
function Lua_OnClose()
    OnGameEvent(GameEvent_Close)
end

--[[
������������ע��
������
����ֵ����
--]]
local function RequireRegSV(objSession)
    local iClientID = objSession:getSessionID()
    local tRegSV = {}
    local strCheckID = GetID()
    
    objSession:setCheckID(strCheckID)
    
    tRegSV[ProtocolStr_Request] = System_RegSV
    tRegSV[ProtocolStr_ServerID] = getServerID()
    tRegSV[ProtocolStr_CheckID] = strCheckID
    tRegSV[ProtocolStr_ClientID] = iClientID
    
    
    local strMsg = cjson.encode(tRegSV)
    g_objSessionManager:sendToByID(iClientID, strMsg, string.len(strMsg))
end

--[[
�����������������ӳɹ�ʱ����
������
����ֵ����
--]]
function Lua_OnLinkedServer(objSession)
    RequireRegSV(objSession)
end
