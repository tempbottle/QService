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
��������ȡ�������ID
������
����ֵ�� ��
--]]
function GetOnLineID()
    return g_objSessionManager:getOnLineID()
end
