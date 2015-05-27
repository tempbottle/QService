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
local iProLens = 2

--session����
if not g_objSessionMgr then
    g_objSessionMgr = nil
end
--�����ƽ���
if not g_objBinary then
    g_objBinary = nil
end
--�����
if not g_objEncrypt then
    g_objEncrypt = nil
end
--�ʼ�
if not g_objMail then
    g_objMail = Mail:new()
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
function Lua_OnStartUp(objSessionMgr, objBinary, objEncrypt)
    g_objSessionMgr = objSessionMgr
    g_objBinary = objBinary
    g_objEncrypt = objEncrypt
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
    shutDown()
end

--[[
�������������¼�
������
����ֵ����
--]]
function Lua_OnConnected(objSession)
    onGameEvent(GameEvent.OnConnected, objSession)
end

--[[
�������ɶ��¼�
������
����ֵ����
--]]
function Lua_OnSockRead()    
    --buffer�ܳ���
    local iBufferLens = g_objBinary:getLens()
    if iBufferLens < iProLens then
        return
    end
    
    --ǰ2�ֽڲ�����    
    local iProtocol = g_objBinary:getUint16()
    Debug("protocol %d", iProtocol)
    
    --ȡ��Ϣ
    local tMsg = {}
    if iBufferLens > iProLens then
        local iMsgLens = iBufferLens - iProLens
        local strMsg = g_objBinary:getByte(iMsgLens)
        
        tMsg = parseProBuf(iProtocol, strMsg, iMsgLens)
    end

    table.print(tMsg)
    
    --����������״̬�Ƿ�ƥ��
    --[[local objCurSession = g_objSessionMgr:getCurSession()
    if SessionType.Tcp == objCurSession:getType() then
        if not g_Started then
            Debug("service not start completed.")
            return
        end
        
        if g_ProFilterFun then
            local iStatus = objCurSession:getStatus()
            if not g_ProFilterFun(iProtocol, iStatus) then
                Log(LOGLV_WARN, "session status %d, protocol %d was ignored.", 
                    iStatus, iProtocol)
                return
            end
        end
    end--]]
    
    onNetEvent(iProtocol, tMsg)
end

--[[
����������
������
����ֵ����
--]]
function Lua_OnDebug()
    onGameEvent(GameEvent.Debug, g_objBinary:getByte(g_objBinary:getLens()))
end

--[[
���������ӶϿ�ʱ����
������
����ֵ����
--]]
function Lua_OnClose(objSession)
    onGameEvent(GameEvent.Close, objSession)
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
    --ÿ֡����
    onGameEvent(GameEvent.FPS, uiClick)
    
    --1��
    if 0 == ((g_objSessionMgr:getCount() * g_objSessionMgr:getTimer()) % 1000) then 
        onDelayEvent()
    end
end
