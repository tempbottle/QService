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
        self.SavePlayerCheckID = {}--��֤ID
        
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

--[[
����: �Ƴ�������֤��
����: 
����ֵ: ��
--]]
function PlayerManager:removSaveCheckID(strCheckID)
    for key, val in pairs(self.SavePlayerCheckID) do
        if val == strCheckID then
            table.remove(self.SavePlayerCheckID, key)
            return
        end
    end
end

--[[
����: ������֤���Ƿ�Ϊ��
����: 
����ֵ: ��
--]]
function PlayerManager:saveCheckIDEmpty(strCheckID)
    return IsTableEmpty(self.SavePlayerCheckID)
end

--[[
����: ������֤�����
����: 
����ֵ: ��
--]]
function PlayerManager:saveCheckIDEmpty(strCheckID)
    self.SavePlayerCheckID = {}
end

--[[
����: �㲥
����: 
����ֵ: ��
--]]
function PlayerManager:broadCast(tbMessage)
    local objPlayer = nil
    
    local strMsg = cjson.encode(tbMessage)
    
    for key, _ in pairs(self.OnLine) do
        objPlayer = self.getPlayerByID(key)
        if objPlayer then
            objPlayer:sendStrMessage(strMsg)
        end
    end
end

--[[
����: ����
����: strPlayerID nill:������� tAttr --nil��Ϊ{} Ϊ��������
����ֵ: ��
--]]
function PlayerManager:Save(strPlayerID, tAttr)
    local iDBSessionID = RandOneSV(SVType.DataBase)    
    if Q_INVALID_ID == iDBSessionID then
        Debug("save player error,not linked to any database server")
        Q_LOG(LOGLV_ERROR, "save player error,not linked to any database server") 
        
        return
    end
    
    --�Ƿ�Ϊ������������
    local bSaveAll = false
    if (not tAttr) or (IsTableEmpty(tAttr)) then
        bSaveAll = true
    end
    
    local tSaveMsg = {}
    local strCheckID = nil
    local strMsg = nil
    
    tSaveMsg[ProtocolStr_Request] = Protocol.DB_SavePlayer
    tSaveMsg[ProtocolStr_Info] = {}
    
    --���浥��
    if strPlayerID then
        if 0 == string.len(strPlayerID) then
            return
        end
        
        local objPlayer = self.getPlayerByID(strPlayerID)
        if not objPlayer then
            return
        end
        
        local Info = objPlayer:getAttr(tAttr)
        if (not Info) or (IsTableEmpty(Info)) then
            return
        end
        
        strCheckID = GetID()
        tSaveMsg[ProtocolStr_CheckID] = strCheckID
        tSaveMsg[ProtocolStr_Info][strPlayerID] = Info
        
        table.insert(self.SavePlayerCheckID, strCheckID)
        
        strMsg = cjson.encode(tSaveMsg)        
        g_objSessionManager:sendToByID(iDBSessionID, strMsg, string.len(strMsg))
        
        if not self:checkOnLineStatus(strPlayerID) then
            if bSaveAll then
                objPlayer:setSave(false)
            end
        end
        
        return
    end
    
    --������
    local iCount = 0
    for key, val in pairs(self.Player) do
        if val:getSave() then
            local Info = val:getAttr(tAttr)
            if (Info) and (not IsTableEmpty(Info)) then
                iCount = iCount + 1                
               
                tSaveMsg[ProtocolStr_Info][key] = Info
                if not self:checkOnLineStatus(key) then
                    if bSaveAll then
                        self.Player[key]:setSave(false)
                    end
                end
                
                --ÿSavePlayer_PreNum���������α���
                if iCount >= SavePlayer_PreNum then
                    strCheckID = GetID()
                    tSaveMsg[ProtocolStr_CheckID] = strCheckID
                    table.insert(self.SavePlayerCheckID, strCheckID)
        
                    strMsg = cjson.encode(tSaveMsg)
                    g_objSessionManager:sendToByID(iDBSessionID, strMsg, string.len(strMsg))
                        
                    iCount = 0
                    tSaveMsg[ProtocolStr_Info] = {}
                end
            else
                Debug("get player"..key.." attribute error.")
            end
        else
            Debug("player "..key.." not need save")
        end
    end
    
    if iCount > 0 then
        strCheckID = GetID()
        tSaveMsg[ProtocolStr_CheckID] = strCheckID
        table.insert(self.SavePlayerCheckID, strCheckID)
        
        strMsg = cjson.encode(tSaveMsg)
        g_objSessionManager:sendToByID(iDBSessionID, strMsg, string.len(strMsg))
    end
end
