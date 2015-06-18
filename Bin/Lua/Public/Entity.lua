--[[
ʵ�����  
--]]

Entity = {}
--�̳�Object
setmetatable(Entity, Object)
Entity.__index = Entity

--varSerialize ���л���ʶ �ַ����Ҳ�Ϊ�������л�Ϊprotobuf��table���л�Ϊstruct ����Ϊjson
function Entity:new(varSerialize)
    local self = Object:new(varSerialize)
    setmetatable(self, Entity)
    
    self.Component = {}--���
    
    return self
end

--�ӿ�ʵ��
function Entity:getInfo()
    local tInfo = {}
    for key, val in pairs(self.Component) do
        tInfo[key] = val:toString()
    end
    
    return tInfo
end

--�ӿ�ʵ��
function Entity:setInfo(tMsg)
    for key, val in pairs(tMsg) do
        self:getComponent(key):fromString(val)
    end
end

--������
function Entity:addComponent(objComp)
    local strName = objComp:getName()
    assert(0 ~= string.len(strName))
    
    self.Component[strName] = objComp
end

--��ȡ���
function Entity:getComponent(strName)
    return assert(self.Component[strName])
end

--�Ƴ����
function Entity:removeComponent(strName)
    self.Component[strName] = nil 
end
