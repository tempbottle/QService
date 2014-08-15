--[[
��lua
--]]

--·�����
local luaDir = Q_GetModulPath() .. "Lua" .. Q_GetPathSeparator()
package.path = package.path .. ";" .. luaDir .. "?.lua"

require("InitModule")

local tNowDay = os.date("*t", time)

--[[
���������������ɹ������
������
����ֵ�� ��
--]]
function Lua_OnStartUp(objSessionManager)
    RegFuncs:OnGameEvent(Macros.GameEvent_StartUp)
end

--[[
����������ر�ʱ����
������
����ֵ����
--]]
function Lua_OnShutDown(objSessionManager)
    RegFuncs:OnGameEvent(Macros.GameEvent_ShutDown)    
end

--[[
���������ӿɶ��¼�
������
����ֵ����
--]]
function Lua_OnRead(objSessionManager, pszMessage, uiLens)
    Debug(string.sub(pszMessage, 1, uiLens))
    local tbMessage = cjson.decode(string.sub(pszMessage, 1, uiLens))
    local iOpCode = tbMessage[Macros.Protocol_Request]
    if not iOpCode then
        Debug("can't find protocol, close this link.")
        objSessionManager:closeCurLink()

        return
    end
    
    local objCurSession = objSessionManager:getCurSession()
    if not objCurSession then
        Debug("current session is nil.")
        objSessionManager:closeCurLink()

        return
    end

    local bServerLinker = objCurSession:isServerLinker()
    if not bServerLinker then
        local iStatus = objCurSession:getStatus()

        --���Ϊ��½������˵�½���������Ķ�������
        if (Macros.LinkStatus_LogIned ~= iStatus) 
            and (OpCodes.CS_LogIn ~= iOpCode) then
                Debug("not logined.")
                return
        end
        
        --����Ѿ���½�ɹ����ٷ��͵�½�����򲻴���
        if (Macros.LinkStatus_LogIned == iStatus) 
            and (OpCodes.CS_LogIn == iOpCode) then
                Debug("already logined.")
                return
        end
    end
    
    Debug("protocol is " .. iOpCode)    
    RegFuncs:OnNetEvent(objSessionManager, iOpCode, tbMessage)    
end

--[[
��������ʱ������
������
����ֵ����
--]]
function Lua_OnTimer(objSessionManager)
    local uiCount = objSessionManager:getCount()
    local uiClick = objSessionManager:getTimer()
    local uiElapseTime = uiClick * uiCount
    local uiOneSecond = 1000
    
    --ÿ֡����
    RegFuncs:OnGameEvent(Macros.GameEvent_FPS, uiClick)
    
    --1��
    if 0 == (uiElapseTime % uiOneSecond) then
        RegFuncs:OnDelayEvent()
        RegFuncs:OnGameEvent(Macros.GameEvent_1Second)        
        
        --������
        local tDay = os.date("*t", time)
        if (tDay.year ~= tNowDay.year) 
            or (tDay.month ~= tNowDay.month) 
            or (tDay.day ~= tNowDay.day) then
                --�����¼�
                tNowDay = tDay
                Debug("day changed.")
                
                RegFuncs:OnGameEvent(Macros.GameEvent_DayChange)
        end
    end
    
    --5��
    if 0 == (uiElapseTime % (uiOneSecond * 5)) then
        RegFuncs:OnGameEvent(Macros.GameEvent_5Second)
    end
    
    --10��
    if 0 == (uiElapseTime % (uiOneSecond * 10)) then
        RegFuncs:OnGameEvent(Macros.GameEvent_10Second)
    end
    
    --1����
    if 0 == (uiElapseTime % (uiOneSecond * 60)) then
        RegFuncs:OnGameEvent(Macros.GameEvent_1Minute)
    end
    
    --5����
    if 0 == (uiElapseTime % (uiOneSecond * 60 * 5)) then
        RegFuncs:OnGameEvent(Macros.GameEvent_5Minute)
    end
    
    --10����
    if 0 == (uiElapseTime % (uiOneSecond * 60 * 10)) then
        RegFuncs:OnGameEvent(Macros.GameEvent_10Minute)
    end    
end

--[[
���������ӶϿ�ʱ����
������
����ֵ����
--]]
function Lua_OnClose(objSessionManager)
    local bServerLinker = objSessionManager:getCurSession():isServerLinker()
    if not bServerLinker then
        RegFuncs:OnGameEvent(Macros.GameEvent_LogOut)
    end
end

--[[
�����������������ӳɹ�ʱ����
������
����ֵ����
--]]
function Lua_OnLinkedServer(objSessionManager, objSession)
    Debug("Lua_OnLinkedServer")
end
