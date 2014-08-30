--[[
��Ҷ������
--]]

PlayerManager = {}
PlayerManager.__index = PlayerManager

--[[
�������½�һ��ҹ������
������
����ֵ�� ��ҹ������
--]]
function PlayerManager:new()
        local self = {}        
        setmetatable(self, PlayerManager)
        self.Player = {} --�洢���
        self.MapAccountID = {} --�洢����˺š�ID��Ӧ��Ϣ
        self.MapNameID = {} --�洢������ơ�ID��Ӧ��Ϣ
        self.OnLine = {} --�����������
        
        return self
end

--[[
����: ���һ���
����:
����ֵ:
--]]
function PlayerManager:addPlayer(objPlayer)
    local ID = objPlayer:getID()
    local strAccount = objPlayer:getAccount()
    local strName = objPlayer:getName()
    
    --�洢���
    self.Player[ID] = objPlayer
    
    --�������ID��Ӧ
    self.MapNameID[strName] = ID
    
    --����˺�ID��Ӧ
    if not self.MapAccountID[strAccount] then
        self.MapAccountID[strAccount] = {}
    end
    
    --�Ƿ��Ѿ�����
    for _, val in pairs(self.MapAccountID[strAccount]) do
        if val == ID then
            return
        end
    end
    
    table.insert(self.MapAccountID[strAccount], ID)
end

--[[
����: ����ID��ȡ���
����: 
����ֵ: �����Ϣ
--]]
function PlayerManager:getPlayerByID(ID)
    return self.Player[ID]
end

--[[
����: �������ƻ�ȡ���
����: 
����ֵ: �����Ϣ
--]]
function PlayerManager:getPlayerByName(strName)
    local ID = self.MapNameID[strName]
    if not ID then
        return nil
    end
    
    return self:getPlayerByID(ID)
end

--[[
����: �����˺Ż�ȡ���
����: 
����ֵ: �����Ϣ(table)
--]]
function PlayerManager:getPlayerByAccount(strAccount)
    if not self.MapAccountID[strAccount] then
        return nil
    end
    
    local tRtn = {}
    for _, val in pairs(self.MapAccountID[strAccount]) do
        local objPlayer = self:getPlayerByID(val)
        if objPlayer then
            table.insert(tRtn, objPlayer)
        end
    end
    
    if IsTableEmpty(tRtn) then
        return nil
    end
    
    return tRtn
end

--[[
����: �����������״̬
����: 
����ֵ: 
--]]
function PlayerManager:setOnLineStatus(ID, bOnLine)
    if bOnLine then
        self.OnLine[ID] = true
    else
        self.OnLine[ID] = nil
    end
end

--[[
����: ����������״̬
����: 
����ֵ: bool
--]]
function PlayerManager:checkOnLineStatus(ID)
    if self.OnLine[ID] then
        return true
    else
        return false
    end
end

--[[
����: ��ȡ�������ID
����: 
����ֵ: table
--]]
function PlayerManager:getOnLinePlayerID()
    local tRtn = {}
    
    for key,_ in pairs(self.OnLine) do
        table.insert(tRtn, key)
    end
    
    return tRtn
end

--[[
����: ��ȡ���������
����: 
����ֵ: int
--]]
function PlayerManager:getOnLineNum()
    return TableLens(self.OnLine)
end

--[[
����: ��ȡ�˺��½�ɫ����
����: 
����ֵ: int
--]]
function PlayerManager:getRoleNum(strAccount)
    if not self.MapAccountID[strAccount] then
        return 0
    end
    
    return TableLens(self.MapAccountID[strAccount])
end
