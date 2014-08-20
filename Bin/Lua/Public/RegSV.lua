--[[
������ע��
--]]

if not g_RegSV then
    g_RegSV = {}
end

--[[
����������ע����������
������
����ֵ����
--]]
local function sendRegSV(iSessionID, iRtn)
    local tRegSVRtn = {}
    tRegSVRtn[Protocol_Request] = Server_RegServerRtn
    tRegSVRtn[Protocol_RTN] = iRtn
    
    local strMsg = cjson.encode(tRegSVRtn)
    g_objSessionManager:sendToByID(iSessionID, strMsg, string.len(strMsg))
end

--[[
������ע�������
������
����ֵ����
--]]
local function RegSV(tbMessage)
    local iSVID = tbMessage[Protocol_ServerID]
    if not iSVID then
        Debug("RegSV server id is nil")
        g_objSessionManager:closeCurLink()
        
        return
    end
    
    local objCurSession = g_objSessionManager:getCurSession()
    local iSessionID = objCurSession:getSessionID()
    if g_RegSV[iSVID] then
        Debug("RegSV server id "..tostring(iSVID) .. " already exist.")
        sendRegSV(iSessionID, Game_RegSV_Repeat)
        RegDelayEvent(DelayCloseTime, CloseLink, iSessionID)
        
        return
    end
    
    g_RegSV[iSVID] = iSessionID
    sendRegSV(iSessionID, Q_RTN_OK)
    Debug("RegSV register server "..tostring(iSVID).." successfully.")
end
RegNetEvent(Server_RegServer, RegSV)

--[[
������ע�����������
������
����ֵ����
--]]
local function RegSVRtn(tbMessage)
    local iRtn = tbMessage[Protocol_RTN]
    if not iRtn then
        return
    end
    
    if Q_RTN_OK ~= iRtn then
        local strMsg = "RegSVRtn register server error, code:"..tostring(iRtn)
        Debug(strMsg)
        Q_LOG(LOGLV_ERROR, strMsg)
        
        return
    end
end
RegNetEvent(Server_RegServerRtn, RegSVRtn)

--[[
������ע�����������
������
����ֵ����
--]]
local function OnSVClose()
    local objCurSession = g_objSessionManager:getCurSession()
    local iSessionID = objCurSession:getSessionID()
    
    for key, val in pairs(g_RegSV) do
        if val == iSessionID then
            g_RegSV[key] = nil
            Debug("OnSVClose remove server "..key)
            break
        end
    end
end
RegGameEvent(GameEvent_LogOut, "OnSVClose", OnSVClose)
