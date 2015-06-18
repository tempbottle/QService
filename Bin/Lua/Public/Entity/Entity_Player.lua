--[[
���  
--]]

Player = {}
--�̳�Entity
setmetatable(Player, Entity)
Player.__index = Player

--varSerialize ���л���ʶ �ַ����Ҳ�Ϊ�������л�Ϊprotobuf��table���л�Ϊstruct ����Ϊjson
function Player:new(varSerialize)
    local self = Entity:new(varSerialize)
    setmetatable(self, Player)

    --�������
    local objAttr = CompAttr:new(varSerialize)
    --��������
    objAttr:setAttr(Player_SVID, 0)
    objAttr:setAttr(Player_Account, "")
    objAttr:setAttr(Player_Name, "")

    self:addComponent(objAttr)
    
    return self
end
