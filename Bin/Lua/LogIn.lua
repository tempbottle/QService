--[[
��½����
--]]

--[[
���������ص�½������ͻ���
������
����ֵ�� ��
--]]
local function sendLogInRst(objSessionManager, iClientID, iRst)
    local tRtn = {}
    tRtn[Macros.Protocol_Request] = OpCodes.SC_LogIn
    tRtn[Macros.Protocol_RTN] = iRst
    
    local strMsg = cjson.encode(tRtn)
    objSessionManager:sendToByID(iClientID, strMsg, string.len(strMsg))
end

--[[
�������ͻ��������½
������
����ֵ�� ��
--]]
function CSLogIn(objSessionManager, tbMessage)
    if not tbMessage[Macros.Protocol_Account] then
        Debug("CSLogIn account is nil")
        objSessionManager:closeCurLink()
        return
    end
    
    local objCurSession = objSessionManager:getCurSession()
    local iSessionID = objCurSession:getSessionID()
    Debug("CSLogIn current session id:".. iSessionID)
    
    --�Ƿ񳬹�������
    local iOnLineNum = GetOnLineNum(objSessionManager)
    if iOnLineNum >= Macros.MaxClintNum then
        Debug("CSLogIn server busy")
        sendLogInRst(objSessionManager, iSessionID, ErrorCodes.Game_ServerBusy)
        RegFuncs:RegDelayEvent(Macros.DelayCloseTime, CloseLink, objSessionManager, iSessionID)
        
        return
    end
    
    --��ȡ�˺ŷ�������������
    local tAccountSVName = objSessionManager:getSVLinkerNameByType(Macros.SVType_Account)
    local iAccountCount = TableLens(tAccountSVName)
    if 0 ==  iAccountCount then
        Debug("CSLogIn not linked to any account server.")
        sendLogInRst(objSessionManager, iSessionID, ErrorCodes.Q_RTN_ERROR)
        RegFuncs:RegDelayEvent(Macros.DelayCloseTime, CloseLink, objSessionManager, iSessionID)
        
        return
    end
    
    --���������Ϣ    
    objCurSession:setAccount(tbMessage[Macros.Protocol_Account])
    Debug("CSLogIn current session account:".. objCurSession:getAccount())
    
    local strCheckID = GetID()
    objCurSession:setCheckID(strCheckID)
    Debug("CSLogIn current session login check id:".. objCurSession:getCheckID())
    
    --��������    
    local tSALogIn = {}
    tSALogIn[Macros.Protocol_Request] = OpCodes.SA_LogIn
    tSALogIn[Macros.Protocol_ClientID] = iSessionID
    tSALogIn[Macros.Protocol_CheckID] = strCheckID
    tSALogIn[Macros.Protocol_Account] = tbMessage[Macros.Protocol_Account]
    tSALogIn[Macros.Protocol_PSW] = tbMessage[Macros.Protocol_PSW]
    
    local strMsg = cjson.encode(tSALogIn)
    Debug("CSLogIn send message "..strMsg.." to account server")
    
    --�����ȡһ�˺ŷ�����,��������֤����
    local iRand = math.random(iAccountCount)
    Debug("send login request to account server:"..tAccountSVName[iRand])
    
    iSessionID = objSessionManager:getServerLinkerSession(tAccountSVName[iRand]):getSessionID()
    objSessionManager:sendToByID(iSessionID, strMsg, string.len(strMsg))
end
RegFuncs:RegNetEvent(OpCodes.CS_LogIn, CSLogIn)

--[[
�������˺ŷ�������½��֤����
������
����ֵ�� ��
--]]
function ASLogIn(objSessionManager, tbMessage)
    local iClinetID = tbMessage[Macros.Protocol_ClientID]
    local iCheckID = tbMessage[Macros.Protocol_CheckID]
    local iRtn = tbMessage[Macros.Protocol_RTN]
    
    --�ж�����session�Ƿ���
    local objSession = objSessionManager:getSessionByID(iClinetID)
    if not objSession then
        Debug("ASLogIn not find session:" .. iClinetID)
        return
    end
    
    --���У����
    if objSession:getCheckID() ~= iCheckID then
        Debug("ASLogIn check id not equal.")
        return
    end
    
    --����Ѿ���½�򲻴���
    if Macros.LinkStatus_LogIned == objSession:getStatus() then
        Debug("ASLogIn already logined.")
        return
    end
    
    sendLogInRst(objSessionManager, iClinetID, iRtn)
    
    if ErrorCodes.Q_RTN_OK ~= iRtn then
        Debug("ASLogIn login error")
        return
    end
    
    objSession:setStatus(Macros.LinkStatus_LogIned)
    
    --���ݸ��˺������������Ϣִ�в���
end
RegFuncs:RegNetEvent(OpCodes.AS_LogIn, ASLogIn)
