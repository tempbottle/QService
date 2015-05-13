--[[
主lua
--]]

--路径添加
local luaDir = string.format("%s%s%s", 
    Q_GetModulPath(), "Lua", Q_GetPathSeparator())
package.path = string.format("%s;%s?.lua", 
    package.path, luaDir)

require("Game/InitModule")

local tNowDay = os.date("*t", time)

--session管理
if not g_objSessionMgr then
    g_objSessionMgr = nil
end
--二进制解析
if not g_objBinary then
    g_objBinary = nil
end
--非服务器连接，根据状态过滤操作码的函数
if not g_ProFilterFun then
    g_ProFilterFun = nil
end
--标记是否启动完成
if not g_Started then
    g_Started = false
end

--[[
描述：服务启动成功后调用
参数：
返回值： 无
--]]
function Lua_OnStartUp(objSessionMgr, objBinary)
    g_objSessionMgr = objSessionMgr
    g_objBinary = objBinary
    math.randomseed(tonumber(tostring(os.time()):reverse():sub(1,6)))
    
    onGameEvent(GameEvent.Start)--这里一般读取配置文件
    onGameEvent(GameEvent.Started)--这里读取配置文件完成后做初始化操作
end

--[[
描述：服务关闭时调用
参数：
返回值：无
--]]
function Lua_OnShutDown()
    onGameEvent(GameEvent.ShutDown)
    
    --确认退出，，，没这个不会退出
    g_objSessionMgr:confirmStop()
end

--[[
描述：新连接事件
参数：
返回值：无
--]]
function Lua_OnConnected(objSession)
    onGameEvent(GameEvent.OnConnected, objSession)
end

--[[
描述：可读事件
参数：
返回值：无
--]]
function Lua_OnSockRead()
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
    
    --检查操作码与状态是否匹配       
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
描述：连接断开时调用
参数：
返回值：无
--]]
function Lua_OnClose(objSession)
    onGameEvent(GameEvent.Close, objSession)
end

--[[
描述：HTTP事件
参数：
返回值：无
--]]
function Lua_OnHttpRead(objHttpBuffer)
    onGameEvent(GameEvent.HttpRead, objHttpBuffer)
end

--[[
描述：服务器间连接成功时调用
参数：
返回值：无
--]]
function Lua_OnLinkedOther(objSession)
    onGameEvent(GameEvent.LinkedOther, objSession)
end

--[[
描述：定时器调用
参数：
返回值：无
--]]
function Lua_OnTimer()    
    --每帧处理
    onGameEvent(GameEvent.FPS, uiClick)
    
    --1秒
    if 0 == ((g_objSessionMgr:getCount() * g_objSessionMgr:getTimer()) % 1000) then 
        onDelayEvent()
    end
end
