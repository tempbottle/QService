--[[
����ҵ��ģ��
--]]

cjson = require("cjson")

local tLoad = 
{
    {"Public/pbc/initpbc", true},
    {"Public/luasocket/initluasocket", true},
    {"Public/redis/initredis", true},
    {"Public/Utile", true},
    {"Public/Macros", true},    
    {"Public/ErrorCodes", true},
    {"Public/TimeWheel", true},
    {"Public/RegFuncs", true},
    {"Public/Protocol", true},
    {"Public/DispPack", true},
    {"Public/Debug", true},
    {"Public/LogLuaMemory", true},
    
    {"Game/Ping", true},
    {"Game/Test", true},
    --{"Game/EchoSV", true},
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
        Debug("reload file "..strPath)
        dofile(strPath)
        
        return
    end
    
    for _, val in pairs(tLoad) do
        if val[2] then
            strPath = luaDir..val[1]..".lua"
            Debug("reload file "..strPath)
            dofile(strPath)
        end
    end
end
