--[[
����ע��
--]]

local RegFuncs = {}

--������Ϣ
RegFuncs.NetEvent = {}
--��Ϸ�¼�
RegFuncs.GameEvent = {}

--[[
��������Ϸ�¼��ص�
������iEvent --�¼����
����ֵ����
--]]
function RegFuncs:OnGameEvent(iEvent, ...)
	if not RegFuncs.GameEvent[iEvent] then
		return
	end
	
	for _, Val in pairs(RegFuncs.GameEvent[iEvent]) do
		local strFunc = Val[1]
		local Func = Val[2]
		
		if Func then
			TimerReStart()
			CallFunc(Func, table.unpack{...})
			Debug("function "..strFunc.." elapsed time:"..tostring(TimerElapsed()).. " ms")
		end
	end
end

--[[
��������Ϸ�¼�ע��
������
����ֵ����
--]]
function RegFuncs:RegGameEvent(iEvent, strFunc, Func)
	if "function" ~= type(Func) then
		return
	end
	
	if not strFunc then
		return
	end
	
	if not RegFuncs.GameEvent[iEvent] then
		RegFuncs.GameEvent[iEvent] = {}
	end
	
	table.insert(RegFuncs.GameEvent[iEvent], {strFunc, Func})
end

--[[
����������ɶ��¼��ص�
������
����ֵ����
--]]
function RegFuncs:OnNetEvent(objSessionManager, iOpCode, tbMessage)
	local Func = RegFuncs.NetEvent[iOpCode]
	if Func then
		TimerReStart()
		CallFunc(Func, objSessionManager, tbMessage)
		Debug("protocol "..iOpCode.." elapsed time:"..tostring(TimerElapsed()).. " ms")		
	else
	    Debug("unknown protocol " .. iOpCode ..", close this link.")
		objSessionManager:closeCurClint()
	end
end

--[[
����������ɶ��¼�ע��
������
����ֵ����
--]]
function RegFuncs:RegNetEvent(iOpCode, Func)
	if "function" ~= type(Func) then
		return
	end
	
	if RegFuncs.NetEvent[iOpCode] then
		Debug("protocol "..iOpCode.." already register.")
		return
	end
	
	RegFuncs.NetEvent[iOpCode] = Func
	Debug("register protocol "..iOpCode)
end

return RegFuncs
