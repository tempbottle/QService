--[[
����ҵ��ģ��
--]]

cjson = require("cjson")
--ϵͳ��Ϣ��ȡ
sigar = require("sigar")
--�ļ�ϵͳ 
lfs = require("lfs")

local tLoad = 
{
    --����
    {"Public/pbc/initpbc", false},
    {"Public/luasocket/initluasocket", false},
    {"Public/redis/initredis", false},
    {"Public/Utile", true},
    {"Public/Macros", true},
    {"Public/ErrorCodes", true},
    {"Public/TimeWheel", true},
    {"Public/RegFuncs", true},
    {"Public/Protocol", true},
    {"Public/ProtobufDesp", true},
    {"Public/StructDesp", true},
    {"Public/NetMsgParser", true}, 
    {"Public/SessionUtile", true}, 
    {"Public/Mail", true},
    {"Public/Debug", false},--�����Ϊ�棬�ؼ��ػ�ѭ������      
    {"Public/LogLuaMemory", true},
    {"Public/Ping", true},    
    {"Public/Object", true},
    {"Public/Component", true},
    {"Public/Component/InitComp", true},
    {"Public/Entity", true},
    {"Public/Entity/InitEntity", true},
    
    
    {"Game/LogIn", true},
    
    {"Game/Test", true},
}

local function loadAllFile()
    for _, val in pairs(tLoad) do
        require(val[1])
    end
end

loadAllFile()

--[[
�������ؼ���lua�ļ�
������strFile --�ļ� Ϊnil�����tLoad�е�����
����ֵ�� ��
--]]
function reLoad(strFile)
    local tTmp = nil
    
    if strFile then
        tTmp = {strFile, true}
    else
        tTmp = tLoad
    end
    
    loadFile(tTmp)
end
