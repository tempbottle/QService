--[[
��lua
--]]

--·�����
local luaDir = string.format("%s%s%s", 
    Q_GetModulPath(), "Lua", Q_GetPathSeparator())
package.path = string.format("%s;%s?.lua", 
    package.path, luaDir)

require("Game/InitModule")

local tNowDay = os.date("*t", time)

--session����
if not g_objSessionMgr then
    g_objSessionMgr = nil
end
--�����ƽ���
if not g_objBinary then
    g_objBinary = nil
end
--�Ƿ��������ӣ�����״̬���˲�����ĺ���
if not g_ProFilterFun then
    g_ProFilterFun = nil
end
--����Ƿ��������
if not g_Started then
    g_Started = false
end

--[[
���������������ɹ������
������
����ֵ�� ��
--]]
function Lua_OnStartUp(objSessionMgr, objBinary)
    g_objSessionMgr = objSessionMgr
    g_objBinary = objBinary
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
������tcp�ɶ��¼�
������
����ֵ����
--]]
function Lua_OnTcpRead()
    EchoSV()
    --[[
    local tbMessage = {}
    if 0 ~= iLens then
        --if CarrierType.Json == MsgCarrier then
            tbMessage = cjson.decode(strMsg)
        --elseif CarrierType.Protobuf == MsgCarrier then
            local strProro = getProtoStr(iProtocol)
            tbMessage = protobuf.decode(strProro, strMsg, iLens)
            assert(tbMessage, protobuf.lasterror()) 
        --else
            Debug("unknown message carrier.")
            return
        --end
    end
    
    local objCurSession = g_objSessionMgr:getCurSession()
    
    Debug(string.format("session type %d", objCurSession:getType()))
    Debug("recv messge:")
    table.print(tbMessage)
    
    --����������״̬�Ƿ�ƥ��       
    if SessionType.TcpClient ~= objCurSession:getType() then
        if not g_Started then
            Debug("service not start completed.")
            return
        end
        
        if g_ProFilterFun then
            local iStatus = objCurSession:getStatus()
            if not g_ProFilterFun(iProtocol, iStatus) then
                Q_LOG(LOGLV_WARN, string.format("session status %d, protocol %d was ignored.", 
                    iStatus, iProtocol))
                return
            end
        end
    end
    
    onNetEvent(iProtocol, tbMessage)
    --]]
end

--[[
������websock�ɶ��¼�
������
����ֵ����
--]]
function Lua_OnWebSockRead()
    Debug(string.sub(g_objBinary:getString(), 1, g_objBinary:getLens()))
    local strMsg = "this is websock server"
    g_objSessionMgr:sendToCur(strMsg, string.len(strMsg))
end

--[[
������HTTP�¼�
������
����ֵ����
--]]
function Lua_OnHttpRead(objHttpBuffer)
    onGameEvent(GameEvent.HttpRead, objHttpBuffer)
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
function Lua_OnLinkedOther(objSession)
    onGameEvent(GameEvent.LinkedOther, objSession)
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
    
    --3��
    if 0 == (uiElapseTime % (uiOneSecond * 3)) then
        onGameEvent(GameEvent.ThreeSecond)
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
        local iTotalMem = collectgarbage("count")
        local strMemUsage = string.format("lua memory usage:%d,session size:%d", 
            iTotalMem, g_objSessionMgr:getSessionSize())
        Debug(strMemUsage)
        Q_LOG(LOGLV_INFO, strMemUsage)
    end
    
    --3����
    if 0 == (uiElapseTime % (uiOneSecond * 60 * 3)) then
        onGameEvent(GameEvent.ThreeMinute)
    end
    
    --5����
    if 0 == (uiElapseTime % (uiOneSecond * 60 * 5)) then
        onGameEvent(GameEvent.FiveMinute)
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
