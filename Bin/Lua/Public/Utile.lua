--[[
һЩ���ú���
--]]

--Debug��ӡ����
local bDebugSwitch = true

--��ʱ��
local objCtimer = CTimer()

--ΨһID
local objSnowflakeID = CSnowflakeID()
objSnowflakeID:setMachineID(getServerID())

--����������
if not objFilter then
    objFilter = CFilter()
end

--�ַ���
local objCharset = CCharset()

--[[
������������Ϣ��ӡ
������
����ֵ����
--]]
function Debug(msg)
    if bDebugSwitch then
        print(string.format("[%s][Lua_Debug] %s", os.date(), tostring(msg)))
    end
end

--[[
��������ȡ�ַ��������ʽ
������
����ֵ�������ʽ
--]]
function getCharset(strWord)
    return objCharset:getStrCharset(strWord, string.len(strWord))
end

--[[
��������ȡһ���ظ���ID
������
����ֵ��ID
--]]
function getID()
    return objSnowflakeID:getSnowflakeID()
end

--[[
�������ر�
������
����ֵ�� ��
--]]
function closeLink(iClentID)
    g_objSessionMgr:closeLinkByID(iClentID)
end

--[[
������������д�
������
����ֵ����
--]]
function addFilterWord(strWord)
    objFilter:addSensitiveWord(strWord, string.len(strWord))
end

--[[
�������Ƿ�������д�
������
����ֵ��bool
--]]
function checkFilterWord(strWord)
    return objFilter:checkHave(strWord, string.len(strWord))
end

--[[
�������������д�
������
����ֵ�����˺�Ľ��
--]]
function Filter(strWord)
    return objFilter:Filter(strWord, string.len(strWord))
end

--[[
���������ü�ʱ��
������
����ֵ����
--]]
function timerReStart()
    objCtimer:reStart()
end

--[[
��������ȡ��ʱ
������
����ֵ��double
--]]
function timerElapsed()
    return objCtimer:Elapsed()
end

--[[
������д�ļ�
������
����ֵ����
--]]
function fileWrite(strFile, strFormat, ...)
    local pFile = io.open(strFile, "a")
    if not pFile then
        return
    end
    
    pFile:write(string.format(strFormat, table.unpack({...})))
    pFile:close()
end

--[[
����������ģʽ�е��ú���
������
����ֵ��bool ��������ֵ
--]]
function callFunc(Func, ...)
    assert("function" == type(Func))

    local function onExcept(strMsg)
        local strStack = debug.trace(3, true, 2)
        Debug("error message:" .. strMsg)
        Debug("trace:" .. strStack)
        Q_LOG(LOGLV_ERROR, "error message:" .. strMsg)
        Q_LOG(LOGLV_ERROR, "trace:" .. strStack)
    end
    
    return xpcall(Func, onExcept, table.unpack({...}))
end

--[[
������lua sql
������
����ֵ����ȡ����ֵ
--]]
function Rows(objConn, strSql)
    local cursor = assert (objConn:execute (strSql))
    return function ()
        return cursor:fetch()
    end
end

--[[
����: ����ִ��sql
����: 
����ֵ: 
--]]
function sqlExecAll(objConn, tSqls)
    if table.empty(tSqls) then
        return
    end
    
    local bRtn = objConn:setautocommit(false)
    if bRtn then
        for _, sql in pairs(tSqls) do
            local Rtn, strError = objConn:execute(sql)
            if not Rtn then
                objConn:rollback()
                objConn:setautocommit(true)
                assert(Rtn, strError)
                
                return
            end
        end
            
        objConn:commit()
        objConn:setautocommit(true)
        
        return
    end
  
    for _, sql in pairs(tSqls) do
        assert(objConn:execute(sql))
    end
end

--[[
�������ַ������
������
����ֵ��table
--]]
function string.split(str, delimiter)
    assert("string" == str)
    assert("string" == delimiter)
    
    if ('' == delimiter) then 
        return {str} 
    end
    
    local pos,arr = 0, {}
    for st,sp in function() return string.find(str, delimiter, pos, true) end do
        table.insert(arr, string.sub(str, pos, st - 1))
        pos = sp + 1
    end
    
    table.insert(arr, string.sub(str, pos))
    
    return arr
end

--[[
������table��ӡ
������
����ֵ����
--]]
function table.print(lua_table, indent)
    assert("table" == type(lua_table))    
    indent = indent or 0
    for k, v in pairs(lua_table) do
        if type(k) == "string" then
            k = string.format("%q", k)
        end
        
        local szSuffix = ""
        if type(v) == "table" then
            szSuffix = "{"
        end
        
        local szPrefix = string.rep("    ", indent)
        formatting = szPrefix.."["..k.."]".." = "..szSuffix
        
        if type(v) == "table" then
            print(formatting)
            table.print(v, indent + 1)
            print(szPrefix.."},")
        else
            local szValue = ""
            if type(v) == "string" then
                szValue = string.format("%q", v)
            else
                szValue = tostring(v)
            end
            
            print(formatting..szValue..",")
        end
    end
end

--[[
������table�Ƿ�Ϊ��
������
����ֵ��bool
--]]
function table.empty(lua_table)
    assert("table" == type(lua_table))    
    for _, _ in pairs(lua_table) do
        return false
    end
    
    return true
end

--[[
������table����
������
����ֵ��int
--]]
function table.len(lua_table)
    assert("table" == type(lua_table))    
    local iCount = 0
    for _, _ in pairs(lua_table) do
        iCount = iCount + 1
    end
    
    return iCount
end

--[[
������table����,�����Ǵ�����
������
����ֵ��table
--]]
function table.copy(tTable)
    assert("table" == type(lua_table))    
    local tNewTab = {}  
    for i, v in pairs(tTable) do  
        local vtyp = type(v)
        
        if ("table" == vtyp) then  
            tNewTab[i] = table.copy(v)  
        elseif ("thread" == vtyp) then  
            tNewTab[i] = v  
        elseif ("userdata" == vtyp) then  
            tNewTab[i] = v  
        else  
            tNewTab[i] = v  
        end
    end  
    
    return tNewTab
end

--[[
��������������enum��table
������
����ֵ��table
--]]
function table.enum(tMsg, iBegin) 
    assert("table" == type(tMsg))    
    local tEnum = {} 
    local iEnumIndex = iBegin or 0 
    for key, val in pairs(tMsg) do 
        tEnum[val] = iEnumIndex + key - 1
    end 
    
    return tEnum 
end 
