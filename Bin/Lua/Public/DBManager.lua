--[[
���ݿ����
--]]

--���ݿ�����
DBType = {
    "Game",
    "Log",
}
DBType = creatEnumTable(DBType)

--mysql��ʼ��
if not g_MysqlENV then
    g_MysqlENV = assert(mysql.mysql())
end

DBManager = {}
DBManager.__index = DBManager

--[[
���������ݿ����
������
����ֵ�� table
--]]
function DBManager:new()
    local self = {}
    setmetatable(self, DBManager)
        
    self.Linker = {}
        
    return self
end

--[[
����: �������
����: 
����ֵ: 
--]]
function DBManager:addLinker(DBType, strDBName, strUser, strPSW, strHost, usPort)
    self.Linker[DBType] = assert(g_MysqlENV:connect(strDBName, strUser, strPSW, strHost,usPort))
end

--[[
����: ��ȡ����
����: 
����ֵ: 
--]]
function DBManager:getLinker(DBType)
    return self.Linker[DBType]
end

--[[
����: �ر����ӣ�����
����: 
����ֵ: 
--]]
function DBManager:closeAll()
    for _, Linker in pairs(self.Linker) do
        Linker:close()
    end
    
    if g_MysqlENV then
        g_MysqlENV:close()
    end
end

--[[
����: ִ��sql
����: 
����ֵ: ִ�н��
--]]
function DBManager:executeSql(DBType, strSql)
    local objLinker = self:getLinker(DBType)
    assert(objLinker, string.format("get linker by type %d error.", DBType))
    
    return assert(objLinker:execute(strSql))
end

--[[
����: ����ִ��sql
����: 
����ֵ: 
--]]
function DBManager:executeAll(DBType, tSqls)
    local objLinker = self:getLinker(DBType)
    if not objLinker then
        Debug(string.format("get linker by type %d error.", DBType))
        return
    end
    
    if IsTableEmpty(tSqls) then
        return
    end
    
    local bRtn = objLinker:setautocommit(false)
    if bRtn then
        for _, sql in pairs(tSqls) do
            local Rtn, strError = objLinker:execute(sql)
            if not Rtn then
                objLinker:rollback()
                objLinker:setautocommit(true)
                assert(Rtn, strError)
                
                return
            end
        end
            
        objLinker:commit()
        objLinker:setautocommit(true)
        
        return
    end
  
    Debug("setautocommit error.")
    for _, sql in pairs(tSqls) do
        assert(objLinker:execute(sql))
    end
end
