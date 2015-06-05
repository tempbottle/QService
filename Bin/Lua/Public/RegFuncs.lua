--[[
����ע��
--]]

local m_iOneDay = 24 * 60 * 60

if not RegFuncs then
    RegFuncs = {}
    
    --������Ϣ
    RegFuncs.NetEvent = {}
    --��Ϣ����
    RegFuncs.MsgDesp = {}
    --��Ϸ�¼�
    RegFuncs.GameEvent = {}
    --�ӳ��¼�
    RegFuncs.DelayEvent = WheelMgr:new()
end

--��Ϣ����
function setMsgDesp(iProtocol, varDesp)
    RegFuncs.MsgDesp[iProtocol] = varDesp
end
function getMsgDesp(iProtocol)
    local varDesp = RegFuncs.MsgDesp[iProtocol]
    assert(varDesp, string.format("get message description by protocol %d error.", iProtocol))
    
    return varDesp
end

--[[
�������ӳ��¼�ִ��
������
����ֵ����
--]]
function onDelayEvent()   
    RegFuncs.DelayEvent:onTime()
end

--[[
�������ӳ��¼�ע��
������iTime --�Ӻ�ʱ�䣨�룩
����ֵ��TvecBase
--]]
function regDelayEvent(iTime, Func, ...)
    return RegFuncs.DelayEvent:addTimer(Func, iTime, table.unpack({...}))
end
function regDelayEventByBase(objTvecBase)
    return RegFuncs.DelayEvent:Add(objTvecBase:getTime(), objTvecBase)
end
--strTime ��ʽ(24Сʱ��)��12:36:28
function regDelayEventAtTime(strTime, Func, ...)
    local strHour, strMin, strSec = string.match(strTime, "(%d+):(%d+):(%d+)")
    local iTime = (tonumber(strHour) * 60 * 60) + (tonumber(strMin) * 60) + tonumber(strSec)
    local tNow = os.date("*t", time)
    local iNowTime = (tonumber(tNow.hour) * 60 * 60) + (tonumber(tNow.min) * 60) + tonumber(tNow.sec)
    
    local iDelayTime = 0
    
    if iTime >= iNowTime then
        iDelayTime = iTime - iNowTime
    else
        iDelayTime = m_iOneDay - (iNowTime - iTime)
    end
    
    return regDelayEvent(iDelayTime, Func, table.unpack({...}))
end

--[[
�������ӳ��¼��Ƴ�
������objTvecBase
����ֵ����
--]]
function removDelayEvent(objTvecBase)
    RegFuncs.DelayEvent:Remove(objTvecBase)
end

--[[
�������ӳ��¼��ܹ��߹�����ʱ��
��������
����ֵ����
--]]
function getDelayEventTick()
    return RegFuncs.DelayEvent:getTick()
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
    Debug("register protocol %d", iProtocol)
end
