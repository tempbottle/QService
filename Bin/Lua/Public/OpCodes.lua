--[[
�����붨��
--]]

local OpCodes = {}

OpCodes.Invalid = -1

OpCodes.CS_LogIn = 1                    --�ͻ��˵���Ϸ��������½����
OpCodes.SA_LogIn = OpCodes.CS_LogIn + 1 --��Ϸ���������˺ŷ�������֤����
OpCodes.AS_LogIn = OpCodes.SA_LogIn + 1 --�˺ŷ�����Ϸ��������֤����
OpCodes.SC_LogIn = OpCodes.AS_LogIn + 1 --��Ϸ���������ص�½��֤���


return OpCodes
