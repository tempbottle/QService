--[[
��Ҷ���
--]]

Player = {}
Player.__index = Player

--�����б�
g_PlayerAttr = {}

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
        self.bSave = false
        
        return self
end

--[[
���������ñ����־
������
����ֵ�� ��
--]]
function Player:setSave(bSave)
    self.bSave = bSave
end

--[[
��������ȡ�����־
������
����ֵ�� bool
--]]
function Player:getSave()
    return self.bSave
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
��������ȡ��Ҷ�Ӧ��session id
������iSessionID --session id
����ֵ:
--]]
function Player:getSessionID(iSessionID)
    return self.iSessionID
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

--[[
�����������ҷ�����Ϣ
������
����ֵ:
--]]
function Player:sendStrMessage(strMsg)
    if Q_INVALID_ID == self.iSessionID then
        return
    end
    
    g_objSessionManager:sendToByID(self.iSessionID, strMsg, string.len(strMsg))
end

--[[
����������ָ�����Ե���Ϣ
������tAttr --Ҫ��ȡ������ nil��Ϊ{}����������
����ֵ:table{Attr=val,...}
--]]
function Player:getAttr(tAttr)
    local tInfo = {}
    
    tInfo.test1 = 12
    tInfo.test2 = true
    tInfo.test3 = "string"
    tInfo.test4 = {"1", 2, "3", true}
    
    return tInfo
end
