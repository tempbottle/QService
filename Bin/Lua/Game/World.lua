--[[
������Ϸ������
--]]

GameWorld = {}
GameWorld.__index = GameWorld

--[[
�������½�һ��Ϸ������
������
����ֵ�� ��Ϸ������
--]]
function GameWorld:new()
        local self = {}        
        setmetatable(self, GameWorld)
        self.PlayerMgr = PlayerManager:new()
        
        return self
end

--[[
��������ȡ��ҹ�����
������
����ֵ�� ��ҹ�����
--]]
function GameWorld:getPlayerMgr()
    return self.PlayerMgr
end

--[[
��������Ϸ��ʼ��
������
����ֵ:
--]]
function GameWorld:initWorld()
    Debug("begin init game world.")
    Debug("end init game world.")
end

--[[
��������Ϸ�ر�
������
����ֵ:
--]]
function GameWorld:shutDown()
    Debug("begin shutdown game world.")
    Debug("end shutdown game world.")
end

--����һ��Ϸ����
if not World then
    World = GameWorld:new()
end

--�¼�ע��
local function InitWorld()
    World:initWorld()
end
RegGameEvent(GameEvent_StartUp, "InitWorld", InitWorld)

local function ShutDownWorld()
    World:shutDown()
end
RegGameEvent(GameEvent_ShutDown, "ShutDownWorld", ShutDownWorld)
