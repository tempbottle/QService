--[[
�����˺ż������
--]]

--������һص�
if not LoadPlayerCB then
    LoadPlayerCB = {}
end

--[[
�����������ݿⷵ�ص������Ϣ���ص���ҹ�����
������
����ֵ����
--]]
local function InitPlayerFromDB(tbMessage)
    local strAccount = tbMessage[ProtocolStr_Account]
    if not strAccount then
        return
    end
    
    local tInfo = tbMessage[ProtocolStr_Info]
    if not tInfo then
        return
    end
    
    for _, val in pairs(tInfo) do
        local ID = val[PlayerAttr_ID]
        local strName = val[PlayerAttr_Name]
        Debug(string.format("player id %s account %s name %s", ID, strAccount, strName))
        if ID then
            local objPlayer = Player:new(ID, strAccount, strName)
            --��ʼ������
            objPlayer:setAttr(val)
            World:getPlayerMgr():addPlayer(objPlayer)
        end
    end
end

--[[
������������������Ϣ
������
����ֵ�� bool
--]]
function RequestLoadPlayer(strAccount, iClientID, strCheckID)
    local iDBSessionID = RandOneSV(SVType_DataBase)
    if Q_INVALID_ID == iDBSessionID then
        Debug("RequestLoadPlayer not linked to any database server.")
        return false
    end
    
    local tLoadPlayer = {}
    
    tLoadPlayer[ProtocolStr_Request] = DB_LoadPlayer
    tLoadPlayer[ProtocolStr_Account] = strAccount
    tLoadPlayer[ProtocolStr_ClientID] = iClientID
    tLoadPlayer[ProtocolStr_CheckID] = strCheckID
    
    local strMsg = cjson.encode(tLoadPlayer)    
    g_objSessionManager:sendToByID(iDBSessionID, strMsg, string.len(strMsg))
    
    return true
end

--[[
��������Ҽ������ִ��
������
����ֵ����
--]]
function OnPlayerLoaded(tbMessage)
    InitPlayerFromDB(tbMessage)
    
    local strCheckID = tbMessage[ProtocolStr_CheckID]
    if not strCheckID then
        return
    end
    
    local tInfo = LoadPlayerCB[strCheckID]
    if not tInfo then
        return
    end
    
    local Func = tInfo.Func
    if Func then
        CallFunc(Func, tbMessage, table.unpack(tInfo.Param))
    end
    
    LoadPlayerCB[strCheckID] = nil
end

--[[
��������Ҽ�������¼�ע��
������
����ֵ����
--]]
function RegPlayerLoaded(strCheckID, Func, ...)
    if "function" ~= type(Func) then
        return
    end
    
    if not strCheckID then
        return
    end
    
    local tInfo = {}

    tInfo.Func = Func
    tInfo.Param = {...}
    
    LoadPlayerCB[strCheckID] = tInfo
end
