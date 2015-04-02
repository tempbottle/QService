--[[
����ע��
--]]

if not RegFuncs then
    RegFuncs = {}
    
    --������Ϣ
    RegFuncs.NetEvent = {}
    --proto��Ϣ�ַ���
    RegFuncs.ProtoStr = {}
    --��Ϸ�¼�
    RegFuncs.GameEvent = {}
    --�ӳ��¼�
    RegFuncs.DelayEvent = {}
end

--proto��Ϣ�ַ���
function setProtoStr(iProtocol, strProto)
    if strProto and (0 ~= string.len(strProto)) then
        RegFuncs.ProtoStr[iProtocol] = strProto
    end
end
function getProtoStr(iProtocol)
    local strProto = RegFuncs.ProtoStr[iProtocol]
    assert(strProto, string.format("get protobuf type by protocol %d error.", iProtocol))
    
    return strProto
end

--[[
�������ӳ��¼��ص�,ִ�к��ɾ��
������
����ֵ����
--]]
function onDelayEvent()   
    local iNow = os.time()
    local tDel = {}
    
    for key, val in pairs(RegFuncs.DelayEvent) do
        if math.abs(iNow - val.RegTime) >= val.Time then
            table.insert(tDel, key)
            
            if val.Func then
                callFunc(val.Func, table.unpack(val.Param))
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
function regDelayEvent(iTime, Func, ...)
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
    
    RegFuncs.DelayEvent[getID()] = tInfo
end

--[[
��������Ϸ�¼��ص�
������iEvent --�¼����
����ֵ����
--]]
function onGameEvent(iEvent, ...)
    if not RegFuncs.GameEvent[iEvent] then
        return
    end
    
    for _, val in pairs(RegFuncs.GameEvent[iEvent]) do     
        if val then
            callFunc(val, table.unpack{...})
        end
    end
end

--[[
��������Ϸ�¼�ע��
������
����ֵ����
--]]
function regGameEvent(iEvent, Func)
    if "function" ~= type(Func) then
        return
    end
    
    if not RegFuncs.GameEvent[iEvent] then
        RegFuncs.GameEvent[iEvent] = {}
    end
    
    table.insert(RegFuncs.GameEvent[iEvent], Func)
end

--[[
����������ɶ��¼��ص�
������
����ֵ����
--]]
function onNetEvent(iProtocol, ...)
    local Func = RegFuncs.NetEvent[iProtocol]
    if Func then
        callFunc(Func, table.unpack{...})      
    else
        g_objSessionMgr:closeCurLink()
    end
end

--[[
����������ɶ��¼�ע��
������
����ֵ����
--]]
function regNetEvent(iProtocol, Func)
    if "function" ~= type(Func) then
        return
    end
    
    RegFuncs.NetEvent[iProtocol] = Func
    Debug("register protocol "..iProtocol)
end
