--[[
��ҹ���
--]]

--�����������{id = {}}
if not g_tAllPlayer then
    g_tAllPlayer = {}
end

--�˺����ӳ��{account = {playername = id}
if not g_tAccountPlayerMap then
    g_tAccountPlayerMap = {}
end

--[[
�����������������ɹ������
������
����ֵ����
--]]
local function OnSVStartUp()
    Debug("Plyaer OnSVStartUp")
end

local function OnSVShutDown()
    Debug("Plyaer OnSVShutDown")
end

local function OnLinkClose()
    Debug("Plyaer OnLinkClose")
end

--[[
������������
������
����ֵ����
--]]

