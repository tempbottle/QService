--[[
ִ������
--]]

--[[
������ִ���ַ�������
�������ַ���
����ֵ����
--]]
local function doString(strMsg)
    local Func = load(strMsg) 
    
    return callFunc(Func)
end
