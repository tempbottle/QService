--[[
��½����
--]]

--����û���
local MaxClientNum = 2000
--һ���˺�������ɫ����
local MaxRoleNum = 5
--���Ƴ���
local MaxNameLens = 32

--[[
����������״̬���˲�����
������
����ֵ�� bool
--]]
local function FilterProtocol(iProtocol, iStatus)
    if SessionStatus.Unknown == iStatus then
        if Protocol.CS_LogIn ~= iProtocol then
            return false
        end
    elseif SessionStatus.Logining == iStatus then
        return false
    elseif SessionStatus.Logined == iStatus then
        if (Protocol.CS_CreatPlayer ~= iProtocol) and (Protocol.CS_SelectPlayer ~= iProtocol) then
            return false
        end
    elseif SessionStatus.Loading == iStatus then
        return false
    elseif SessionStatus.Loaded == iStatus then
        if (Protocol.CS_CreatPlayer ~= iProtocol) and (Protocol.CS_SelectPlayer ~= iProtocol) then
            return false
        end
    elseif SessionStatus.Createing == iStatus then
        return false
    elseif SessionStatus.Created == iStatus then
        if (Protocol.CS_CreatPlayer ~= iProtocol) and (Protocol.CS_SelectPlayer ~= iProtocol) then
            return false
        end
    else
        if Protocol.CS_LogIn == iProtocol then
            return false
        end
    end
    
    return true
end
g_ProtocolFilterFun = FilterProtocol

--[[
���������ص�½������ͻ���
������
����ֵ�� ��
--]]
local function sendLogInRst(iClientID, iRst)
    local tRtn = {}
    tRtn[ProtocolStr_Request] = Protocol.SC_LogIn
    tRtn[ProtocolStr_Rtn] = iRst
    
    local strMsg = cjson.encode(tRtn)
    g_objSessionManager:sendToByID(iClientID, strMsg, string.len(strMsg))
end

--[[
�������������
������
����ֵ�� bool
--]]
local function checkLogInParam(strAccount, strPSW)
    if not strAccount or not strPSW then       
        return false
    end
    
    if ((0 == string.len(strAccount)) or (0 == string.len(strPSW)))then      
        return false
    end
    
    return true
end

--[[
�������ͻ��������½
������
����ֵ�� ��
--]]
local function CSLogIn(tbMessage)
    local strAccount = tbMessage[ProtocolStr_Account]
    local strPSW = tbMessage[ProtocolStr_PSW]
    if not checkLogInParam(strAccount, strPSW) then
        return
    end
    
    local objCurSession = g_objSessionManager:getCurSession()
    
    --�Ƿ񳬹�������
    local iSessionID = objCurSession:getSessionID()
    local iOnLineNum = World:getPlayerMgr():getOnLineNum()
    if iOnLineNum >= MaxClientNum then
        Debug("CSLogIn server busy")
        Q_LOG(LOGLV_ERROR, 
            string.format("login error,server busy, online number %d, max number %d",
            iOnLineNum, MaxClientNum))
        sendLogInRst(iSessionID, ErrorCode.ServerBusy)
        
        return
    end
    
    --��ȡ�˺ŷ���������ID
    local iAccSessionID = RandOneSV(SVType.Account)
    if Q_INVALID_ID ==  iAccSessionID then
        Debug("CSLogIn not linked to any account server.")
        Q_LOG(LOGLV_ERROR, "login error,not linked to any account server")
        sendLogInRst(iSessionID, ErrorCode.ERROR)
        
        return
    end
    
    --���������Ϣ
    local strCheckID = GetID()
    objCurSession:setAccount(strAccount)
    objCurSession:setCheckID(strCheckID)
    objCurSession:setStatus(SessionStatus.Logining)
    
    --��������    
    local tSALogIn = {}
    tSALogIn[ProtocolStr_Request] = Protocol.SA_LogIn
    tSALogIn[ProtocolStr_ClientID] = iSessionID
    tSALogIn[ProtocolStr_CheckID] = strCheckID
    tSALogIn[ProtocolStr_Account] = strAccount
    tSALogIn[ProtocolStr_PSW] = tbMessage[ProtocolStr_PSW]
    
    local strMsg = cjson.encode(tSALogIn)
    g_objSessionManager:sendToByID(iAccSessionID, strMsg, string.len(strMsg))
end
RegNetEvent(Protocol.CS_LogIn, CSLogIn)

--[[
�����������˺�����Ҹ�Ҫ��Ϣ
������
����ֵ�� ��
--]]
local function SendAccountInfo(iClientID, tAllPlayer)
    local tMsg = {}
    local iNum = 0
    
    if tAllPlayer then
        iNum = TableLens(tAllPlayer)
    end
    
    tMsg[ProtocolStr_Request] = Protocol.SC_AccountInfo
    tMsg[ProtocolStr_Number] = iNum
    
    if 0 ~= iNum then
        tMsg[ProtocolStr_Info] = {}
        for _, val in pairs(tAllPlayer) do
            local tInfo = {}
            
            tInfo[ProtocolStr_Name] = val:getName()
            --tInfo[ProtocolStr_ID] = val:getID()
            
            table.insert(tMsg[ProtocolStr_Info], tInfo)
        end
    end
    
    local strMsg = cjson.encode(tMsg)    
    g_objSessionManager:sendToByID(iClientID, strMsg, string.len(strMsg))
end

--[[
�����������ݿ����������ʱ
������
����ֵ�� ��
--]]
local function Login_OnPlayerLoad(tbMessage)
    local iClientID = tbMessage[ProtocolStr_ClientID]
    local strCheckID = tbMessage[ProtocolStr_CheckID]
    
    --�ж�����session�Ƿ���
    local objSession = g_objSessionManager:getSessionByID(iClientID)
    if not objSession then
        Debug("Login_OnPlayerLoad not find session:" .. iClientID)
        return
    end
    
    --���У����
    if objSession:getCheckID() ~= strCheckID then
        Debug("Login_OnPlayerLoad check id not equal.")
        return
    end
    
    objSession:setStatus(SessionStatus.Loaded)
    
    local strAccount = objSession:getAccount()
    local tAllPlayer = World:getPlayerMgr():getPlayerByAccount(strAccount)
    
    SendAccountInfo(iClientID, tAllPlayer)
end

--[[
�������˺ŷ�������½��֤����
������
����ֵ�� ��
--]]
local function ASLogIn(tbMessage)
    local iClientID = tbMessage[ProtocolStr_ClientID]
    local strCheckID = tbMessage[ProtocolStr_CheckID]
    local iRtn = tbMessage[ProtocolStr_Rtn]
    
    --�ж�����session�Ƿ���
    local objSession = g_objSessionManager:getSessionByID(iClientID)
    if not objSession then
        Debug("ASLogIn not find session:" .. iClientID)
        return
    end
    
    --���У����
    if objSession:getCheckID() ~= strCheckID then
        Debug("ASLogIn check id not equal.")
        return
    end
    
    sendLogInRst(iClientID, iRtn)
    
    if ErrorCode.OK ~= iRtn then
        objSession:setStatus(SessionStatus.Unknown)
        Debug("ASLogIn login error")
        
        return
    end
    
    --�ɹ�    
    local strAccount = objSession:getAccount()
    local objPlayerMgr = World:getPlayerMgr()
    local tAllPlayer = objPlayerMgr:getPlayerByAccount(strAccount)
    
    --û�м��ص��ڴ�,�򲻴��ڽ�ɫ
    if not tAllPlayer then
        strCheckID = GetID()
        objSession:setCheckID(strCheckID)
        if RequestLoadPlayer(strAccount, iClientID, strCheckID) then
            objSession:setStatus(SessionStatus.Loading)
            RegPlayerLoaded(strCheckID, Login_OnPlayerLoad)
        end
        
        return
    end
    
    --�Ѿ����ڴ���
    objSession:setStatus(SessionStatus.Logined)
    SendAccountInfo(iClientID, tAllPlayer)
end
RegNetEvent(Protocol.AS_LogIn, ASLogIn)

--[[
���������������ң����ݿⷵ��
������
����ֵ�� ��
--]]
local function DBLoadPlayer(tbMessage)
    OnPlayerLoaded(tbMessage)
end
RegNetEvent(Protocol.DB_LoadPlayer, DBLoadPlayer)

--[[
��������ɫ���Ƽ��
������
����ֵ�� bool
--]]
local function checkName(strName)
    if not strName then
        return false
    end
    
    local iLens = string.len(strName)
    if ((0 == iLens) or (iLens >= MaxNameLens)) then
        return false
    end
    
    if CheckFilterWord(strName) then
        return false
    end
    
    local strCharset = GetCharset(strName)
    strCharset = string.upper(strCharset)
    if (("" ~= strCharset) and ("UTF-8" ~= strCharset)) then
        return false
    end
    
    return true
end

--[[
���������ؽ�ɫ����������ͻ���
������
����ֵ�� ��
--]]
local function sendCreatePlayerRst(iClientID, iRst)
    local tRtn = {}
    tRtn[ProtocolStr_Request] = Protocol.SC_CreatPlayer
    tRtn[ProtocolStr_Rtn] = iRst
    
    local strMsg = cjson.encode(tRtn)
    g_objSessionManager:sendToByID(iClientID, strMsg, string.len(strMsg))
end

--[[
�������ͻ��˴�����ɫ
������
����ֵ�� ��
--]]
local function CSCreatePlayer(tbMessage)
    local objCurSession = g_objSessionManager:getCurSession()
    local iSessionID = objCurSession:getSessionID()
    local strName = tbMessage[ProtocolStr_Name]
    
    local iDBSessionID = RandOneSV(SVType.DataBase)    
    if Q_INVALID_ID == iDBSessionID then
        Q_LOG(LOGLV_ERROR, "create player error,not linked to any database server")
        sendCreatePlayerRst(iSessionID, ErrorCode.FAILE)
        return
    end
    
    if not checkName(strName) then
        sendCreatePlayerRst(iSessionID, ErrorCode.FAILE)
        return
    end
    
    local strAccount = objCurSession:getAccount()  
    if World:getPlayerMgr():getRoleNum(strAccount) >= MaxRoleNum then
        sendCreatePlayerRst(iSessionID, ErrorCode.FAILE)
        return
    end
    
    local strCheckID = GetID()
    objCurSession:setCheckID(strCheckID)
    objCurSession:setStatus(SessionStatus.Createing)
    
    --�����ݿ�����
    local tReqMsg = {}
    local strPlayerID = GetID()
    tReqMsg[ProtocolStr_Request] = Protocol.DB_CreatPlayer
    tReqMsg[ProtocolStr_Account] = strAccount
    tReqMsg[ProtocolStr_ID] = strPlayerID
    tReqMsg[ProtocolStr_Name] = strName
    tReqMsg[ProtocolStr_ClientID] = iSessionID
    tReqMsg[ProtocolStr_CheckID] = strCheckID
    tReqMsg[ProtocolStr_Info] = {}
    
    ---
    local tPlayerInitAttr = {}
    tPlayerInitAttr[PlayerAttr_ID] = strPlayerID
    tPlayerInitAttr[PlayerAttr_Account] = strAccount
    tPlayerInitAttr[PlayerAttr_Name] = strName
    
    table.insert(tReqMsg[ProtocolStr_Info], tPlayerInitAttr)
    
    local strMsg = cjson.encode(tReqMsg)
    g_objSessionManager:sendToByID(iDBSessionID, strMsg, string.len(strMsg))
end
RegNetEvent(Protocol.CS_CreatPlayer, CSCreatePlayer)

--[[
������������ɫ���ݿⷵ��
������
����ֵ�� ��
--]]
local function DBCreatePlayer(tbMessage)
    local iClientID = tbMessage[ProtocolStr_ClientID]
    local strCheckID = tbMessage[ProtocolStr_CheckID]
    local strAccount = tbMessage[ProtocolStr_Account]
    local strPlayerID = tbMessage[ProtocolStr_ID]
    local strName = tbMessage[ProtocolStr_Name]
    local iRtn = tbMessage[ProtocolStr_Rtn]
    
    if ErrorCode.OK == iRtn then
        local objPlayer = Player:new(strPlayerID, strAccount, strName)
        World:getPlayerMgr():addPlayer(objPlayer)
        
        DBLog(DBLogType.CreatePlayer, strPlayerID)
    end
    
    --�ж�����session�Ƿ���
    local objSession = g_objSessionManager:getSessionByID(iClientID)
    if not objSession then
        return
    end
    
    --���У����
    if objSession:getCheckID() ~= strCheckID then
        return
    end
    
    objSession:setStatus(SessionStatus.Created)
    sendCreatePlayerRst(iClientID, iRtn)
end
RegNetEvent(Protocol.DB_CreatPlayer, DBCreatePlayer)

--[[
������ѡ���ɫ�������
������
����ֵ�� ��
--]]
local function sendSelectPlayerRst(iClientID, iRst)
    local tRtn = {}
    tRtn[ProtocolStr_Request] = Protocol.SC_SelectPlayer
    tRtn[ProtocolStr_Rtn] = iRst
    
    local strMsg = cjson.encode(tRtn)
    g_objSessionManager:sendToByID(iClientID, strMsg, string.len(strMsg))
end

--[[
������������Ϸ
������
����ֵ�� ��
--]]
local function EnterGame(objPlayer, objSession)
    local objPlayerMgr = World:getPlayerMgr()
    local strPlayerID = objPlayer:getID()
    local iSessionID = objSession:getSessionID()
    
    objSession:setID(strPlayerID)
    objSession:setStatus(SessionStatus.Playing)
    objPlayer:setSessionID(iSessionID)
    objPlayer:setSave(true)
    objPlayerMgr:setOnLineStatus(strPlayerID, true)
    
    sendSelectPlayerRst(iSessionID, ErrorCode.OK)
    
    OnGameEvent(GameEvent.LogIned, objPlayer)
    
    DBLog(DBLogType.LogIn, strPlayerID)
    Debug(string.format("player enter game,id %s, name %s", strPlayerID, objPlayer:getName()))
end

--[[
�������߳����ߵ�
������
����ֵ�� ��
--]]
local function KickPlayer(strAccount, iSessionID)
    local objPlayerMgr = World:getPlayerMgr()
    local tAllPlayer = objPlayerMgr:getPlayerByAccount(strAccount)
    
    --�ж��Ƿ�����,����������߳�
    for _, val in pairs(tAllPlayer) do
        if objPlayerMgr:checkOnLineStatus(val:getID())
            and (iSessionID ~= val:getSessionID()) then
            
            local tKickPlayer = {}
            tKickPlayer[ProtocolStr_Request] = Protocol.SC_KickPlayer
            
            val:sendMessage(tKickPlayer)
            CloseLink(val:getSessionID())
        end
    end
end

--[[
������ѡ���ɫ������Ϸ
������
����ֵ�� ��
--]]
local function CSSelectPlayer(tbMessage)
    local strName = tbMessage[ProtocolStr_Name]
    if not strName then
        return
    end
    
    local objCurSession = g_objSessionManager:getCurSession()
    local strAccount = objCurSession:getAccount()
    local iSessionID = objCurSession:getSessionID()
    
    local objPlayerMgr = World:getPlayerMgr()
    local objPlayer = objPlayerMgr:getPlayerByName(strName)
    if not objPlayer then
        return
    end
    
    if objPlayer:getAccount() ~= strAccount then
        return
    end
    
    KickPlayer(strAccount, iSessionID)
    EnterGame(objPlayer, objCurSession)
end
RegNetEvent(Protocol.CS_SelectPlayer, CSSelectPlayer)
