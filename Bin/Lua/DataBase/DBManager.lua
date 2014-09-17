--[[
���ݿ����
--]]

--���ݿ�����
DBType_Game = 0
DBType_Log = DBType_Game + 1

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
    self.DBName = {}
        
    return self
end

--[[
����: �������
����: 
����ֵ: 
--]]
function DBManager:addLinker(DBType, strDBName, strUser, strPSW, strHost, usPort)
    self.Linker[DBType] = assert(g_MysqlENV:connect(strDBName, strUser, strPSW, strHost,usPort))
    self.DBName[DBType] = strDBName
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
����: ����ִ��sql
����: 
����ֵ: 
--]]
function DBManager:executeAll(objLinker, tSqls)
    if IsTableEmpty(tSqls) then
        return
    end
    
    local bRtn = objLinker:setautocommit(false)
    if bRtn then
        for _, sql in pairs(tSqls) do
            if 0 == objLinker:execute(sql) then
                Debug(string.format("execute sql:%s error.", sql))
                
                objLinker:rollback()
                objLinker:setautocommit(true)
                
                return
            end
        end
            
        objLinker:commit()
        objLinker:setautocommit(true)
        
        return
    end
            
    Debug("setautocommit error.")
    for _, sql in pairs(tSqls) do
        if 0 == objLinker:execute(sql) then
            Debug(string.format("execute sql:%s error.", sql))
            return
        end
    end
        
end

--[[
����: ����
����: tAttrs -- {���ԡ��ֶ� = ֵ}
����ֵ: 
--]]
function DBManager:insert(DBType, strTable, tAttrs)
    if IsTableEmpty(tAttrs) then
        return
    end
    
    local objLinker = self:getLinker(DBType)
    if not objLinker then
        return
    end
    
    local tSqls = {}
    
    --�������
    for _, tAttr in pairs(tAttrs) do
        local strSql = string.format("INSERT INTO %s (", strTable)
        for strField, _ in pairs(tAttr) do
            strSql = string.format("%s %s,", strSql, strField)
        end
        
        strSql = string.sub(strSql, 1, string.len(strSql) - 1)
        strSql = string.format("%s ) VALUES (", strSql)
        
        for strFiled, Attr in pairs(tAttr) do
            local strType = type(Attr)
            if "number" ==  strType then
                strSql = string.format("%s %d,", strSql, Attr)
            elseif "boolean" == strType then
                if Attr then
                    strSql = string.format("%s %d,", strSql, 1)
                else
                    strSql = string.format("%s %d,", strSql, 0)
                end
            elseif "string" == strType then
                strSql = string.format("%s '%s',", strSql, Attr)
            elseif "table" == strType then
                strSql = string.format("%s '%s',", strSql, cjson.encode(AttrInfo))
            else
                Debug("unknown field type")
                return
            end
        end
        
        strSql = string.sub(strSql, 1, string.len(strSql) - 1)
        strSql = string.format("%s )", strSql)
        Debug(strSql)
        table.insert(tSqls, strSql)
    end
    
    self:executeAll(objLinker, tSqls)
end

--[[
����: ����,�������id�ֶ�
����: tAttrs -- id = {���ԡ��ֶ� = ֵ}
����ֵ: 
--]]
function DBManager:update(DBType, strTable, tAttrs)
    if IsTableEmpty(tAttrs) then
        return
    end
    
    local objLinker = self:getLinker(DBType)
    if not objLinker then
        return
    end
    
    local tSqls = {}
    
    for id, tAttr in pairs(tAttrs) do
        local strSql = string.format("UPDATE %s SET", strTable)
        for strFiled, Attr in pairs(tAttr) do
            local strType = type(Attr)
            if ("number" == strType) then
                strSql = string.format("%s %s=%d,", strSql, strFiled, Attr)
            elseif("boolean" == strType) then
                if AttrInfo then
                    strSql = string.format("%s %s=%d,", strSql, strFiled, 1)
                else
                    strSql = string.format("%s %s=%d,", strSql, strFiled, 0)
                end                
            elseif("string" == strType) then 
                strSql = string.format("%s %s='%s',", strSql, strFiled, Attr)
            elseif("table" == strType) then
                strSql = string.format("%s %s='%s',", strSql, strFiled, cjson.encode(Attr))
            else
                Debug("unknown field type")
                return
            end        
        end
        
        --ȥ������,��
        strSql = string.sub(strSql, 1, string.len(strSql) - 1)
        strSql = string.format("%s WHERE id=%s", strSql, id)
        Debug(strSql)
        table.insert(tSqls, strSql)
    end
    
    self:executeAll(objLinker, tSqls)
end
