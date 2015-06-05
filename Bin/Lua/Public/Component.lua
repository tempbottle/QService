--[[
�������  
--]]

Component = {}
--����������Object    �̳�
setmetatable(Component, Object)
Component.__index = Component

--strName �������
--varSerialize ���л���ʶ �ַ����Ҳ�Ϊ�������л�Ϊprotobuf��table���л�Ϊstruct ����Ϊjson
function Component:new(strName, varSerialize)
    assert(strName and (0 ~= string.len(strName)))
    
    local self = Object:new(varSerialize)
    setmetatable(self, Component)
    
    self.Name = strName
    
    return self
end

--�������
function Component:getName()
    return self.Name
end
