--[[
���ݿ���Ϣ����
--]]

--[[
������������ҷ���
������
����ֵ�� ��
--]]
local function DBSavePlayer(tbMessage)
    local strCheckID = tbMessage[ProtocolStr_CheckID]
    if not strCheckID then
        return
    end
    
    local objPlayerMgr = World:getPlayerMgr()
    objPlayerMgr:removSaveCheckID(strCheckID)
    if not objPlayerMgr:saveCheckIDEmpty() then
        return
    end
    
    Debug("all save task finished.")
    --do something
end
RegNetEvent(Protocol.DB_SavePlayer, DBSavePlayer)
