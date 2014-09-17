--[[
��Ҷ���
--]]

Player = {}
Player.__index = Player

PlayerAttr_ID = "id"
PlayerAttr_Account = "account"
PlayerAttr_Name = "name"

--[[
�������½�һ��Ҷ���
������ID --ΨһID strAccount --�˺� strName --����
����ֵ�� ��Ҷ���
--]]
function Player:new(ID, strAccount, strName)
        local self = {}
        setmetatable(self, Player)
        self.ID = ID --ΨһID
        self.Account = strAccount --�˺�
        self.Name = strName --����
        
        self.SessionID = Q_INVALID_ID --session id
        self.Save = false
        
        return self
end

--[[
���������ñ����־
������
����ֵ�� ��
--]]
function Player:setSave(bSave)
    self.Save = bSave
end

--[[
��������ȡ�����־
������
����ֵ�� bool
--]]
function Player:getSave()
    return self.Save
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
    return self.Account
end

--[[
��������ȡ�������
������
����ֵ�� ����
--]]
function Player:getName()
    return self.Name
end

--[[
������������Ҷ�Ӧ��session id
������iSessionID --session id
����ֵ:
--]]
function Player:setSessionID(iSessionID)
    self.SessionID = iSessionID
end

--[[
��������ȡ��Ҷ�Ӧ��session id
������iSessionID --session id
����ֵ:
--]]
function Player:getSessionID()
    return self.SessionID
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
    
    if Q_INVALID_ID == self.SessionID then
        return
    end
    
    local strMsg = cjson.encode(tMessage)
    g_objSessionManager:sendToByID(self.SessionID, strMsg, string.len(strMsg))
end

--[[
�����������ҷ�����Ϣ
������
����ֵ:
--]]
function Player:sendStrMessage(strMsg)
    if Q_INVALID_ID == self.SessionID then
        return
    end
    
    g_objSessionManager:sendToByID(self.SessionID, strMsg, string.len(strMsg))
end

function Player:setAttr(tAttr)
    if not tAttr then
        return
    end
end

--[[
����������ָ�����Ե���Ϣ
������tAttr --Ҫ��ȡ������ nil��Ϊ{}����������
����ֵ:table{Attr=val,...}
--]]
function Player:getAttr(tAttr)
    local tTmpAttr = tAttr
    local tRtnAttr = {}
    
    if (not tAttr) or (IsTableEmpty(tAttr)) then
        Debug("get all attr")
    else
        Debug("get some attr")
    end
    
    tRtnAttr[PlayerAttr_ID] = self.ID
    tRtnAttr[PlayerAttr_Account] = self.Account
    tRtnAttr[PlayerAttr_Name] = self.Name
    tRtnAttr.test = {1,2,3}
    
    return tRtnAttr
end
