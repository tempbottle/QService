--[[
��lua
--]]

--·�����
local luaDir = Q_GetModulPath() .. "Lua" .. Q_GetPathSeparator()
package.path = package.path .. ";" .. luaDir .. "?.lua"

cjson = require("cjson")
protobuf = require("Public/protobuf")
parser = require("Public/parser")
require("Game/InitModule")

local tNowDay = os.date("*t", time)

--session����
if not g_objSessionMgr then
    g_objSessionMgr = nil
end

--�Ƿ��������ӣ�����״̬���˲�����ĺ���
if not g_ProtocolFilterFun then
    g_ProtocolFilterFun = nil
end

--����Ƿ��������
if not g_StartCompleted then
    g_StartCompleted = false
end

--[[
���������������ɹ������
������
����ֵ�� ��
--]]
function Lua_OnStartUp(objSessionMgr)
    g_objSessionMgr = objSessionMgr
    math.randomseed(tonumber(tostring(os.time()):reverse():sub(1,6)))
    
    onGameEvent(GameEvent.Start)--����һ���ȡ�����ļ�
    onGameEvent(GameEvent.Started)--�����ȡ�����ļ���ɺ�����ʼ������
end

--[[
����������ر�ʱ����
������
����ֵ����
--]]
function Lua_OnShutDown()
    onGameEvent(GameEvent.ShutDown)
    --ȷ���˳�������û��������˳�
    g_objSessionMgr:confirmStop()
end

--[[
���������ӿɶ��¼�
������
����ֵ����
--]]
function Lua_OnRead(iProtocol, strMsg, iLens)
    local tbMessage = {}
    if 0 ~= iLens then
        if CarrierType.Json == MsgCarrier then
            tbMessage = cjson.decode(strMsg)
        elseif CarrierType.Protobuf == MsgCarrier then
            local strProro = getProtoStr(iProtocol)
            tbMessage = protobuf.decode(strProro, strMsg, iLens)
            assert(tbMessage, protobuf.lasterror()) 
        else
            Debug("unknown message carrier.")
            return
        end
    end
    
    local objCurSession = g_objSessionMgr:getCurSession()
    
    Debug(string.format("session type %d", objCurSession:getType()))
    Debug("recv messge:")
    table.print(tbMessage)
    
    --����������״̬�Ƿ�ƥ��       
    if SessionType.SVLinker ~= objCurSession:getType() then
        if not g_StartCompleted then
            Debug("service not start completed.")
            return
        end
        
        if g_ProtocolFilterFun then
            local iStatus = objCurSession:getStatus()
            if not g_ProtocolFilterFun(iProtocol, iStatus) then
                Q_LOG(LOGLV_WARN, string.format("session status %d, protocol %d was ignored.", 
                    iStatus, iProtocol))
                return
            end
        end
    end
    
    onNetEvent(iProtocol, tbMessage)
end

--[[
������HTTP�¼�
������
����ֵ����
--]]
function Lua_OnHttpRead(objHttpBuffer)
    if not g_StartCompleted then
        Debug("service not start completed.")
        return
    end
    
    onGameEvent(GameEvent.HttpRead, objHttpBuffer)
end

--[[
��������ʱ������
������
����ֵ����
--]]
function Lua_OnTimer()
    local uiCount = g_objSessionMgr:getCount()
    local uiClick = g_objSessionMgr:getTimer()
    local uiElapseTime = uiClick * uiCount
    local uiOneSecond = 1000
    
    --ÿ֡����
    onGameEvent(GameEvent.FPS, uiClick)
    
    --1��
    if 0 == (uiElapseTime % uiOneSecond) then
        onDelayEvent()
        onGameEvent(GameEvent.OneSecond)
        
        --������
        local tDay = os.date("*t", time)
        if (tDay.year ~= tNowDay.year) 
            or (tDay.month ~= tNowDay.month) 
            or (tDay.day ~= tNowDay.day) then
                --�����¼�
                tNowDay = tDay
                Debug("day changed.")
                onGameEvent(GameEvent.DayChange)
                
                local iNewWeek = tonumber(os.date("%U", os.time(tDay)))
                local iOldWeek = tonumber(os.date("%U", os.time(tNowDay)))
                if iNewWeek ~= iOldWeek then
                    onGameEvent(GameEvent.WeekChange)
                end
                if tDay.month ~= tNowDay.month then
                    onGameEvent(GameEvent.MonthChange)
                end
                
                tNowDay = tDay
        end
    end
    
    --5��
    if 0 == (uiElapseTime % (uiOneSecond * 5)) then
        onGameEvent(GameEvent.FiveSecond)
    end
    
    --10��
    if 0 == (uiElapseTime % (uiOneSecond * 10)) then
        onGameEvent(GameEvent.TenSecond)
    end
    
    --1����
    if 0 == (uiElapseTime % (uiOneSecond * 60)) then
        onGameEvent(GameEvent.OneMinute)
    end
    
    --5����
    if 0 == (uiElapseTime % (uiOneSecond * 60 * 5)) then
        onGameEvent(GameEvent.FiveMinute)
        collectgarbage("collect")
    end
    
    --10����
    if 0 == (uiElapseTime % (uiOneSecond * 60 * 10)) then
        onGameEvent(GameEvent.TenMinute)
    end
	
    --1Сʱ
    if 0 == (uiElapseTime % (uiOneSecond * 60 * 60)) then
        onGameEvent(GameEvent.OneHour)
    end
end

--[[
���������ӶϿ�ʱ����
������
����ֵ����
--]]
function Lua_OnClose()
    onGameEvent(GameEvent.Close)
end

--[[
�����������������ӳɹ�ʱ����
������
����ֵ����
--]]
function Lua_OnLinkedServer(objSession)
    requireRegSV(objSession)
end
