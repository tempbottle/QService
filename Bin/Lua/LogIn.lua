--[[
��½����
--]]

--[[
���������ص�½������ͻ���
������
����ֵ�� ��
--]]
local function sendLogInRst(iClientID, iRst)
    local tRtn = {}
    tRtn[Protocol_Request] = SC_LogIn
    tRtn[Protocol_RTN] = iRst
    
    local strMsg = cjson.encode(tRtn)
    g_objSessionManager:sendToByID(iClientID, strMsg, string.len(strMsg))
end

--[[
��������½��ʱ����
������
����ֵ�� ��
--]]
local function loginTimeOut(iClientID, strCheckID)
    local objSession = g_objSessionManager:getSessionByID(iClientID)
    if not objSession then
        return
    end
    
    if objSession:getCheckID() ~= strCheckID then
        return
    end
    
    if LinkStatus_LogIning ~= objSession:getStatus() then
        return
    end
    
    sendLogInRst(iClientID, Game_LoginTimeOut)
    RegDelayEvent(DelayCloseTime, CloseLink, iClientID)
end

--[[
�������ͻ��������½
������
����ֵ�� ��
--]]
function CSLogIn(tbMessage)
    if not tbMessage[Protocol_Account] then
        Debug("CSLogIn account is nil")
        g_objSessionManager:closeCurLink()
        
        return
    end
    
    local objCurSession = g_objSessionManager:getCurSession()
    local iSessionID = objCurSession:getSessionID()
    Debug("CSLogIn current session id:".. iSessionID)
    
    --�Ƿ񳬹�������
    local iOnLineNum = GetOnLineNum()
    if iOnLineNum >= MaxClintNum then
        Debug("CSLogIn server busy")
        sendLogInRst(iSessionID, Game_ServerBusy)
        RegDelayEvent(DelayCloseTime, CloseLink, iSessionID)
        
        return
    end
    
    --��ȡ�˺ŷ�������������
    local tAccountSVName = g_objSessionManager:getSVLinkerNameByType(SVType_Account)
    local iAccountCount = TableLens(tAccountSVName)
    if 0 ==  iAccountCount then
        Debug("CSLogIn not linked to any account server.")
        sendLogInRst(iSessionID, Q_RTN_ERROR)
        RegDelayEvent(DelayCloseTime, CloseLink, iSessionID)
        
        return
    end
    
    --���������Ϣ    
    objCurSession:setAccount(tbMessage[Protocol_Account])
    Debug("CSLogIn current session account:".. objCurSession:getAccount())
    
    local strCheckID = GetID()
    objCurSession:setCheckID(strCheckID)
    Debug("CSLogIn current session login check id:".. objCurSession:getCheckID())
    objCurSession:setStatus(LinkStatus_LogIning)
    
    --��������    
    local tSALogIn = {}
    tSALogIn[Protocol_Request] = SA_LogIn
    tSALogIn[Protocol_ClientID] = iSessionID
    tSALogIn[Protocol_CheckID] = strCheckID
    tSALogIn[Protocol_Account] = tbMessage[Protocol_Account]
    tSALogIn[Protocol_PSW] = tbMessage[Protocol_PSW]
    
    local strMsg = cjson.encode(tSALogIn)
    Debug("CSLogIn send message "..strMsg.." to account server")
    
    --�����ȡһ�˺ŷ�����,��������֤����
    local iRand = math.random(iAccountCount)
    Debug("send login request to account server:"..tAccountSVName[iRand])
    
    local iAccSessionID = g_objSessionManager:getServerLinkerSession(tAccountSVName[iRand]):getSessionID()
    g_objSessionManager:sendToByID(iAccSessionID, strMsg, string.len(strMsg))
    
    RegDelayEvent(LoginTimeOut, loginTimeOut, iSessionID, strCheckID)
end
RegNetEvent(CS_LogIn, CSLogIn)

--[[
�������˺ŷ�������½��֤����
������
����ֵ�� ��
--]]
function ASLogIn(tbMessage)
    local iClientID = tbMessage[Protocol_ClientID]
    local strCheckID = tbMessage[Protocol_CheckID]
    local iRtn = tbMessage[Protocol_RTN]
    
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
    
    --����Ѿ���½�򲻴���
    if LinkStatus_LogIned == objSession:getStatus() then
        Debug("ASLogIn already logined.")
        return
    end
    
    sendLogInRst(iClientID, iRtn)
    
    if Q_RTN_OK ~= iRtn then
        Debug("ASLogIn login error")
        return
    end
    
    objSession:setStatus(LinkStatus_LogIned)
    
    --���ݸ��˺������������Ϣִ�в���
    --�޽�ɫ
    --1����ɫ
    --���ɫ
end
RegNetEvent(AS_LogIn, ASLogIn)
