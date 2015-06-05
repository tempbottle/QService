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
    {"Public/pbc/initpbc", true},
    {"Public/luasocket/initluasocket", true},
    {"Public/redis/initredis", true},
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
    {"Public/Entity", true},
    
    --���
    {"Game/Component/Comp_Name", true},
    {"Game/Component/Comp_Attr", true},
    --ʵ��
    
    
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
    local luaDir = Q_GetModulPath() .. "Lua" .. Q_GetPathSeparator()
    local strPath = ""
    
    if strFile then
        strPath = luaDir..strFile..".lua"
        Debug("reload file %s", strPath)
        dofile(strPath)
        
        return
    end
    
    for _, val in pairs(tLoad) do
        if val[2] then
            strPath = luaDir..val[1]..".lua"
            Debug("reload file %s", strPath)
            dofile(strPath)
        end
    end
end
