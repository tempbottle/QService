--[[
��Ϸ���ú���
--]]

--[[
��������ȡ���������
������
����ֵ����
--]]
function GetOnLineNum(objSessionManager)
    return objSessionManager:getSessionSize() - objSessionManager:getGetSVLinkerNum()
end

--[[
�������ر�
������
����ֵ�� ��
--]]
function CloseLink(objSessionManager, iClentID)
    objSessionManager:closeLinkByID(iClentID)
end
