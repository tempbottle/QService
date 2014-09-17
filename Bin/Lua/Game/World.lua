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

local function On10Minute()
    World:getPlayerMgr():Save(nil, nil)
end
RegGameEvent(GameEvent_1Minute, "On10Minute", On10Minute)

local function OnPlayerEnterGame(objPlayer)
    --add your code
end
RegGameEvent(GameEvent_LogIned, "OnPlayerEnterGame", OnPlayerEnterGame)

local function OnPlayerLogOut()
    local objPlayer = getCurPlayer()
    if not objPlayer then
        return
    end
    
    --add your code
    
    World:getPlayerMgr():setOnLineStatus(objPlayer:getID(), false) 
    objPlayer:setSessionID(Q_INVALID_ID)
    
    DBLog(DBLogType_LogOut, objPlayer:getID())
    Debug(string.format("player logout,id %s, name %s", objPlayer:getID(), objPlayer:getName()))
end
RegGameEvent(GameEvent_Close, "OnPlayerLogOut", OnPlayerLogOut)
