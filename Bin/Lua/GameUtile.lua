--[[
��Ϸ���ú���
--]]

--[[
��������ȡ���������
������
����ֵ����
--]]
function GetOnLineNum()
    return g_objSessionManager:getSessionSize() - g_objSessionManager:getGetSVLinkerNum()
end

--[[
�������ر�
������
����ֵ�� ��
--]]
function CloseLink(iClentID)
    g_objSessionManager:closeLinkByID(iClentID)
end

--[[
��������ȡ�������ID
������
����ֵ�� ��
--]]
function GetOnLineID()
    return g_objSessionManager:getOnLineID()
end
