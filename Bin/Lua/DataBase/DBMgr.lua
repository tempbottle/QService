--[[
���ݿ����ӹ���
--]]

DBType_Game = 0
DBType_Log = DBType_Game + 1

DBManager = {}
DBManager.__index = DBManager

--[[
�������½�һ���ݿ����ӹ���
������
����ֵ�� ���ݿ����ӹ���
--]]
function DBManager:new()
        local self = {}        
        setmetatable(self, DBManager)
        self.Linkers = {}
        
        return self
end

--[[
���������һ����
������
����ֵ�� ��
--]]
function DBManager:addLinker(DBType, strHost, strDBName, strUser, strPSW, usPort)
    local dbUrl = CDBUrl()
    dbUrl:setUrl(string.format("DB=%s;UserName=%s;PassWord=%s;Port=%d;Host=%s", 
    strDBName, strUser, strPSW, usPort, strHost))
    
    local Linker = CMySQLLink()
    Linker:open(dbUrl)
        
    self.Linkers[DBType] = Linker
end

--[[
��������ȡһ����
������
����ֵ�� ����
--]]
function DBManager:getLinker(DBType)
    return self.Linkers[DBType]
end
