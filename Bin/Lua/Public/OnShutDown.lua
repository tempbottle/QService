--[[
�������ر�ʱ����  
--]]

function onShutDown()
    --ȷ���˳�������û��������˳�
    shutDown()
end
regGameEvent(GameEvent.ShutDown, onShutDown)
