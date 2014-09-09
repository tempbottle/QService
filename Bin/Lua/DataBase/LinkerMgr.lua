--[[
���ݿ����ӹ���
--]]

LinkerManager = {}
LinkerManager.__index = LinkerManager

--[[
�������½�һ���ݿ����ӹ���
������
����ֵ�� ���ݿ����ӹ���
--]]
function LinkerManager:new()
        local self = {}        
        setmetatable(self, LinkerManager)
        self.Linkers = {}
        self.DBName = {}
        
        return self
end

--[[
���������һ����
������
����ֵ�� ��
--]]
function LinkerManager:addLinker(DBType, strHost, strDBName, strUser, strPSW, usPort)
    local dbUrl = CDBUrl()
    dbUrl:setUrl(string.format("DB=%s;UserName=%s;PassWord=%s;Port=%d;Host=%s", 
    strDBName, strUser, strPSW, usPort, strHost))
    
    local Linker = CMySQLLink()
    Linker:open(dbUrl)
    
    self.DBName[DBType] = strDBName
    self.Linkers[DBType] = Linker
end

--[[
��������ȡһ����
������
����ֵ�� ����
--]]
function LinkerManager:getLinker(DBType)
    return self.Linkers[DBType]
end

--[[
��������ȡ���ݿ���
������
����ֵ�� ����
--]]
function LinkerManager:getDBName(DBType)
    return self.DBName[DBType]
end

--[[
��������ȡ��������
������
����ֵ�� table
--]]
function LinkerManager:getAllLinker()
    return self.Linkers
end
