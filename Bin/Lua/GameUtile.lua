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
