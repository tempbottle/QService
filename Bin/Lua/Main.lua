--[[
��lua
--]]

--·�����
local luaDir = Q_GetModulPath() .. "Lua" .. Q_GetPathSeparator()
package.path = package.path .. ";" .. luaDir .. "?.lua"

require("InitModule")
local RegFunc = require("RegFunc")

--[[
���������������ɹ������
������
����ֵ�� ��
--]]
function Lua_OnStartUp(objSessionManager)
	print("Lua_OnStartUp")
end

--[[
����������ر�ʱ����
������
����ֵ����
--]]
function Lua_OnShutDown(objSessionManager)
	print("Lua_OnShutDown")
end

--[[
���������ӿɶ��¼�
������
����ֵ����
--]]
function Lua_OnRead(objSessionManager, pszMessage, uiLens)
	local iOpCode = 1
	local Func = RegFunc.Reg_Func[iOpCode]
	if Func then
		Func(objSessionManager, pszMessage, uiLens)
	else
	    print("unknown opcode " .. iOpCode .." close this link.")
		objSessionManager:closeCurClint()
	end
end

--[[
��������ʱ������
������
����ֵ����
--]]
function Lua_OnTimer(objSessionManager)
	print("Lua_OnTimer count "..objSessionManager:getCount().."  timer "..objSessionManager:getTimer())
	local strMsg = "this is timer.."
	objSessionManager:sendToAll(strMsg, string.len(strMsg))
end

--[[
���������ӶϿ�ʱ����
������
����ֵ����
--]]
function Lua_OnClose(objSessionManager)
	print("Lua_OnClose")
end

--[[
�����������������ӳɹ�ʱ����
������
����ֵ����
--]]
function Lua_OnLinkedServer(objSessionManager, objSession)
	print("Lua_OnLinkedServer")
end
