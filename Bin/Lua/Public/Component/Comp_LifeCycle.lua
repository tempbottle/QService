--[[
�����������   
--]]

CompLifeCycle = {}
--�̳�Component
setmetatable(CompLifeCycle, Component)
CompLifeCycle.__index = CompLifeCycle

--varSerialize ���л���ʶ �ַ����Ҳ�Ϊ�������л�Ϊprotobuf��table���л�Ϊstruct ����Ϊjson
function CompLifeCycle:new(varSerialize)
    local self = Component:new(CompNam_LifeCycle, varSerialize) 
    setmetatable(self, CompLifeCycle)
    
    self.Life = 0--��ʱ��
    self.TVBase = nil--ʱ����
    
    return self
end

--�ӿ�ʵ��
function CompLifeCycle:getInfo()
    local iLife  = self.Life
    if self.TVBase then
        iLife = self.Life - (getDelayEventTick() - self.TVBase:getRegTick())
    end

    local tInfo = {}
    tInfo.uiLife = iLife

    return tInfo
end

--�ӿ�ʵ��
function CompLifeCycle:setInfo(tMsg)
    self.Life = tMsg.uiLife
end

--��ʼ�������ڼ�ʱ uiLife ʹ��self.Life��ֵ
function CompLifeCycle:Register(uiLife, Func, ...)
    if uiLife then
        self.TVBase = regDelayEvent(uiLife, Func, table.unpack({...}))
    else
        self.TVBase = regDelayEvent(self.Life, Func, table.unpack({...}))
    end
end
