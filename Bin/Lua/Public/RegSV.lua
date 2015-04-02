--[[
������ע��
--]]
--{svid{ProtocolStr_ClientID = ,ProtocolStr_ServerType = }}
if not g_RegService then
    g_RegService = {}
end

--[[
���������������ע��
������
����ֵ����
--]]
function requireRegSV(objSession)
    local strProro = getProtoStr(Protocol.Sys_ReqRegSV)
    local iClientID = objSession:getSessionID()
    local tRequest = {}
    local strCheckID = getID()    
    
    objSession:setCheckID(strCheckID)
    
    tRequest.uiServerID = getServerID()
    tRequest.uiServerType = getServerType()
    tRequest.strCheckID = strCheckID
    tRequest.uiClientID = iClientID
    
    local strMsg = protobuf.encode(strProro, tRequest)
    g_objSessionMgr:sendToByID(iClientID, Protocol.Sys_ReqRegSV, strMsg, string.len(strMsg))
end

--[[
������ע�������
������
����ֵ�� ��
--]]
local function RegServer(tbMessage)
    local strProro = getProtoStr(Protocol.Sys_RegSVRtn)
    local objCurSession = g_objSessionMgr:getCurSession()
    local iSessionID = objCurSession:getSessionID()
    local iSVID = tbMessage.uiServerID
    local iSVType = tbMessage.uiServerType
    local tResponse = {}
   
    tResponse.strCheckID = tbMessage.strCheckID
    tResponse.uiClientID = tbMessage.uiClientID
    
    if g_RegService[iSVID] then
        tResponse.iRtn = ErrorCode.FAILE
    else
        g_RegService[iSVID] = {iSessionID, iSVType}
        
        Debug(string.format("register server id %d, server type %d, client id %d.",
        iSVID, iSVType, iSessionID))
        
        tResponse.iRtn = ErrorCode.OK
    end
    
    local strMsg = protobuf.encode(strProro, tResponse)
    g_objSessionMgr:sendToCur(Protocol.Sys_RegSVRtn, strMsg, string.len(strMsg))
end
regNetEvent(Protocol.Sys_ReqRegSV, RegServer)

--[[
������ע�����������
������
����ֵ�� ��
--]]
local function RegServerRtn(tbMessage)
    local strCheckID = tbMessage.strCheckID
    local iClientID = tbMessage.uiClientID
    local iRtn = tbMessage.iRtn
    
    local objSession = g_objSessionMgr:getSessionByID(iClientID)
    if not objSession then
        return
    end
    
    if objSession:getCheckID() ~= strCheckID then
        return
    end
    
    objSession:setCheckID("")
    if ErrorCode.OK == iRtn then
        Debug("register service successfully.")
        return
    end
    
    Debug("register service error.")
    closeLink(iClientID)
end
regNetEvent(Protocol.Sys_RegSVRtn, RegServerRtn)

--[[
���������ӶϿ�ʱ
������
����ֵ�� ��
--]]
local function OnRegSVClose()
    local objCurSession = g_objSessionMgr:getCurSession()
    local iSessionID = objCurSession:getSessionID()
    
    for key, val in pairs(g_RegService) do
        if iSessionID == val[1] then
            g_RegService[key] = nil
            Debug("remove register server "..tonumber(key))
            break
        end
    end
end
regGameEvent(GameEvent.Close, OnRegSVClose)

--[[
�������Ƴ�������ע��,����ǿ���Ƴ�
������
����ֵ�� ��
--]]
function removeRegSV(iSVID)
    if not g_RegService[iSVID] then
        return
    end
    
    local iSessionID = g_RegService[iSVID][1]
    CloseLink(iSessionID)
    g_RegService[iSVID] = nil
end

--[[
�������������ͻ�ȡ����������ID
������
����ֵ�� table
--]]
function getRegSV(iSVType)
    local tRtn = {}
    for key, val in pairs(g_RegService) do
        if iSVType == val[2] then
            table.insert(tRtn, val[1])
        end
    end
    
    return tRtn
end
