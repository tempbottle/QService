--[[
�������   
--]]

CompAttr = {}
--����������Component    �̳�
setmetatable(CompAttr, Component)
CompAttr.__index = CompAttr

--varSerialize ���л���ʶ �ַ����Ҳ�Ϊ�������л�Ϊprotobuf��table���л�Ϊstruct ����Ϊjson
function CompAttr:new(varSerialize)
    local self = Component:new(CompNam_Attr, varSerialize) 
    setmetatable(self, CompAttr)
    
    self.Attr = {}
    
    return self
end

--�ӿ�ʵ��
function CompAttr:getInfo()
    return self.Attr
end

--�ӿ�ʵ��
function CompAttr:setInfo(tMsg)
    self.Attr = tMsg
end

--��������
function CompAttr:setAttr(varKey, varVal)
    self.Attr[varKey] = varVal
end

--���Ի�ȡ
function CompAttr:getAttr(varKey)   
    return assert(self.Attr[varKey])
end

--��������
function CompAttr:addAttr(varKey, iVal)
    assert("number" == type(iVal))
    assert(self.Attr[varKey])
    
    self.Attr[varKey] = self.Attr[varKey] + iVal
end
