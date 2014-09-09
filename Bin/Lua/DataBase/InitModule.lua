--[[
����ҵ��ģ��
--]]

local tLoad = 
{
    {"Public/Macros", true},
    {"Public/Protocol", true},
    {"Public/ErrorCodes", true},
    {"Public/Utile", true},
    {"Public/RegFuncs", true},
    {"Public/Debug", true},
    {"Public/RegSV", true},
    
    {"DataBase/LinkerMgr", true},
    {"DataBase/InitDB", true},
    {"DataBase/DBMessage", true},
    {"DataBase/DBMgr", true},
}

local function LoadAllFile()
    for _, val in pairs(tLoad) do
        require(val[1])
    end
end

LoadAllFile()

--[[
�������ؼ���lua�ļ�
������strFile --�ļ� Ϊnil�����tLoad�е�����
����ֵ�� ��
--]]
function ReLoad(strFile)
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
