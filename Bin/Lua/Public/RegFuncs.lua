--[[
����ע��
--]]

if not RegFuncs then
    RegFuncs = {}
    
    --������Ϣ
    RegFuncs.NetEvent = {}
    --��Ϸ�¼�
    RegFuncs.GameEvent = {}
    --�ӳ��¼�
    RegFuncs.DelayEvent = {}
end

--[[
�������ӳ��¼��ص�,ִ�к��ɾ��
������
����ֵ����
--]]
function OnDelayEvent()   
    local iNow = os.time()
    local tDel = {}
    
    for key, val in pairs(RegFuncs.DelayEvent) do
        if  math.abs(iNow - val.RegTime) >= val.Time then
            table.insert(tDel, key)
            
            local Func = val.Func
            if Func then
                CallFunc(Func, table.unpack(val.Param))
            end
        end
    end
    
    for _, val in pairs(tDel) do
        RegFuncs.DelayEvent[val] = nil
    end
end

--[[
�������ӳ��¼�ע��
������iTime --�Ӻ�ʱ�䣨�룩
����ֵ����
--]]
function RegDelayEvent(iTime, Func, ...)
    if "function" ~= type(Func) then
        return
    end
    
    if 0 >= iTime then
        return
    end
    
    local tInfo = {}
    
    tInfo.RegTime = os.time()
    tInfo.Time = iTime
    tInfo.Func = Func
    tInfo.Param = {...}
    
    RegFuncs.DelayEvent[GetID()] = tInfo
end

--[[
��������Ϸ�¼��ص�
������iEvent --�¼����
����ֵ����
--]]
function OnGameEvent(iEvent, ...)
    if not RegFuncs.GameEvent[iEvent] then
        return
    end
    
    for _, val in pairs(RegFuncs.GameEvent[iEvent]) do
        local strFunc = val[1]
        local Func = val[2]
        
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
function RegGameEvent(iEvent, strFunc, Func)
    if "function" ~= type(Func) then
        return
    end
    
    if not strFunc then
        return
    end
    
    if not RegFuncs.GameEvent[iEvent] then
        RegFuncs.GameEvent[iEvent] = {}
    end
    
    for _, val in pairs(RegFuncs.GameEvent[iEvent]) do
        if val[1] == strFunc then
            return
        end
    end
    
    table.insert(RegFuncs.GameEvent[iEvent], {strFunc, Func})
end

--[[
����������ɶ��¼��ص�
������
����ֵ����
--]]
function OnNetEvent(iOpCode, tbMessage)
    local Func = RegFuncs.NetEvent[iOpCode]
    if Func then
        TimerReStart()
        CallFunc(Func, tbMessage)
        Debug("protocol "..iOpCode.." elapsed time:"..tostring(TimerElapsed()).. " ms")        
    else
        Debug("unknown protocol " .. iOpCode ..", close this link.")
        g_objSessionManager:closeCurLink()
    end
end

--[[
����������ɶ��¼�ע��
������
����ֵ����
--]]
function RegNetEvent(iOpCode, Func)
    if "function" ~= type(Func) then
        return
    end
    
    RegFuncs.NetEvent[iOpCode] = Func
    Debug("register protocol "..iOpCode)
end
