--[[
��Ҷ���
--]]

Player = {}
Player.__index = Player

--[[
�������½�һ��Ҷ���
������ID --ΨһID strAccount --�˺� strName --����
����ֵ�� ��Ҷ���
--]]
function Player:new(ID, strAccount, strName)
        local self = {}        
        setmetatable(self, Player)
        self.ID = ID --ΨһID
        self.strAccount = strAccount --�˺�
        self.strName = strName --����
        self.iSessionID = Q_INVALID_ID --session id
        
        return self
end

--[[
��������ȡ���ID
������
����ֵ�� ID
--]]
function Player:getID()
    return self.ID
end

--[[
��������ȡ����˺�
������
����ֵ�� �˺�
--]]
function Player:getAccount()
    return self.strAccount
end

--[[
��������ȡ�������
������
����ֵ�� ����
--]]
function Player:getName()
    return self.strName
end

--[[
������������Ҷ�Ӧ��session id
������iSessionID --session id
����ֵ:
--]]
function Player:setSessionID(iSessionID)
    self.iSessionID = iSessionID
end

--[[
�����������ҷ�����Ϣ
������
����ֵ:
--]]
function Player:sendMessage(tMessage)
    if not tMessage then
        return
    end
    
    if IsTableEmpty(tMessage) then
        return
    end
    
    if Q_INVALID_ID == self.iSessionID then
        return
    end
    
    local strMsg = cjson.encode(tMessage)
    g_objSessionManager:sendToByID(self.iSessionID, strMsg, string.len(strMsg))
end


--�¼�ע��
local function OnPlayerEnterGame(objPlayer)
    --add your code
end
RegGameEvent(GameEvent_LogIned, "OnPlayerEnterGame", OnPlayerEnterGame)

local function OnPlayerLogOut()
    local objPlayer = getCurPlayer()
    if not objPlayer then
        return
    end
    
    --add your code
    
    World:getPlayerMgr():setOnLineStatus(objPlayer:getID(), false)    
    objPlayer:setSessionID(Q_INVALID_ID)
    
    DBLog(DBLogType_LogOut, objPlayer:getID())    
    Debug(string.format("player logout,id %s, name %s", objPlayer:getID(), objPlayer:getName()))
end
RegGameEvent(GameEvent_Close, "OnPlayerLogOut", OnPlayerLogOut)
