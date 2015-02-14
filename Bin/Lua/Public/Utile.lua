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
        print(string.format("[%s][Debug] %s", os.date(), tostring(msg)))
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
    g_objSessionManager:closeLinkByID(iClentID)
end

--[[
�����������ȡһָ�����͵ķ�����
������
����ֵ�� session id
--]]
function randSV(iType)
    local tSVName = g_objSessionManager:getSVLinkerNameByType(iType)
    local iCount = TableLens(tSVName)
    if 0 == iCount then
        return Q_INVALID_ID
    end
    
    local iRand = 1
    if iCount > 1  then
        iRand = math.random(iCount)
    end
    
    return g_objSessionManager:getServerLinkerSession(tSVName[iRand]):getSessionID()
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
    if "function" ~= type(Func) then
        Debug("in function callFunc param Func is not function.")
        return
    end

    local strMsg = ''
    --local strParam = cjson.encode({...})

    local function onExcept(sMsg)
        strMsg = sMsg
        local strStack = debug.traceback()
        Debug("error message:" .. strMsg)
        Debug("stack:" .. strStack)
        --Debug("param:" .. strParam)
        Q_LOG(LOGLV_ERROR, "error message:" .. strMsg)
        Q_LOG(LOGLV_ERROR, "stack:" .. strStack)
        --Q_LOG(LOGLV_ERROR, "param:" .. strParam)
    end
    
    local bRtn, objMsg = xpcall(Func, onExcept, table.unpack({...}))
    
    return bRtn, objMsg
end

--[[
������table��ӡ
������
����ֵ����
--]]
function printTable(lua_table, indent)
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
            printTable(v, indent + 1)
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
function isTableEmpty(lua_table)
    if "table" ~= type(lua_table) then
        return true
    end
    
    for _,_ in pairs(lua_table) do
        return false
    end
    
    return true
end

--[[
������table����
������
����ֵ��int
--]]
function tableLens(lua_table)
    if "table" ~= type(lua_table) then
        return 0
    end
    
    local iCount = 0
    for _,_ in pairs(lua_table) do
        iCount = iCount + 1
    end
    
    return iCount
end

--[[
������table����,�����Ǵ�����
������
����ֵ��table
--]]
function copyTable(tTable)
    if "table" ~= type(tTable)  then  
        return nil  
    end
    
    local tNewTab = {}  
    for i, v in pairs(tTable) do  
        local vtyp = type(v)
        
        if ("table" == vtyp) then  
            tNewTab[i] = CopyTable(v)  
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
function creatEnumTable(tMsg, iBegin) 
    assert("table" == type(tMsg)) 
    
    local tEnum = {} 
    local iEnumIndex = iBegin or 0 
    for key, val in pairs(tMsg) do 
        tEnum[val] = iEnumIndex + key - 1
    end 
    
    return tEnum 
end 

--[[
�������ַ������
������
����ֵ��table
--]]
function string.split(str, delimiter)
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

local function mkoffset_tab(offset)
    local offset_tab = ""
    if offset and type(offset) == "number" then
        for i = 0, offset - 1,1 do
            offset_tab = offset_tab.."\t"
        end
    end

    return offset_tab
end

local function dump(obj, offset)
    local getIndent, quoteStr, wrapKey, wrapVal, isArray, dumpObj

    offset = offset or 0
    local offset_tab = mkoffset_tab(offset + 1)

    getIndent = function(level)
                    return string.rep("\t", level)
                end
    quoteStr = function(str)
                    str = string.gsub(str, "[%c\\\"]", {
                        ["\t"] = "\\t",
                        ["\r"] = "\\r",
                        ["\n"] = "\\n",
                        ["\""] = "\\\"",
                        ["\\"] = "\\\\",
                    })
                    return '"' .. str .. '"'
                end
    wrapKey = function(val)
                if type(val) == "number" then
                    return "[" .. val .. "]"
                elseif type(val) == "string" then
                    return "[" .. quoteStr(val) .. "]"
                else
                    return "[" .. tostring(val) .. "]"
                end
             end
    wrapVal = function(val, level)
                if type(val) == "table" then
                    return dumpObj(val, level)
                elseif type(val) == "number" then
                    return val
                elseif type(val) == "string" then
                    return quoteStr(val)
                else
                    return tostring(val)
                end
             end
    local isArray = function(arr)
                        local count = 0 
                        for k, v in pairs(arr) do
                            count = count + 1 
                        end 
                        for i = 1, count do
                            if arr[i] == nil then
                                return false
                            end 
                        end 
                        
                        return true, count
                    end
    dumpObj = function(obj, level)
                if type(obj) ~= "table" then
                    return wrapVal(obj)
                end
                level = level + 1
                local tokens = {}
                tokens[#tokens + 1] = offset_tab.."{"
                local ret, count = isArray(obj)
                if ret then
                    for i = 1, count do
                        tokens[#tokens + 1] = offset_tab..getIndent(level) .. wrapVal(obj[i], level) .. ","
                    end
                else
                    for k, v in pairs(obj) do
                        tokens[#tokens + 1] = offset_tab..getIndent(level) .. wrapKey(k) .. " = " .. wrapVal(v, level) .. ","
                    end
                end
                tokens[#tokens + 1] = offset_tab..getIndent(level - 1) .. "}"
                
                return table.concat(tokens, "\n")
             end
             
    return dumpObj(obj, 0)
end

function debug.var_export(obj, prt)
    if prt then
        print(dump(obj))
    else
        return dump(obj)
    end
end

function debug.var_dump(obj)
    print(dump(obj))
end

function debug.trace(depth, not_prt, offset)
    local lstr = ""
    local offset_tab = mkoffset_tab(offset or 0)

    local function traceex(cur_level, base_level)
        local stack = debug.getinfo(cur_level)
        if not stack then 
            return false 
        end

        if stack.what == "C" then
            lstr = lstr..offset_tab..string.format("%d.[C FUNCTION]",cur_level - base_level + 1).."\n"
        else
            lstr = lstr..offset_tab..string.format("%d.[%s:%d]:%s:",cur_level - base_level + 1, stack.short_src, stack.linedefined, stack.name or "").."\n"
        end

        local i = 1
        while true do
            local name, value = debug.getlocal(cur_level, i)
            if not name then break end

            lstr = lstr..offset_tab.."\t"..name.." = "..dump(value, offset).."\n"
            i = i + 1
        end

        return true
    end
    
    local function trace_all()
        local base_level = 4
        local i = base_level

        repeat
            local ret = traceex(i, base_level, 1)
            i = i + 1
        until ret ~= true
    end

    local function trace_dep(depth)
        local base_level = 4
        local i = base_level

        repeat
            local ret = traceex(i, base_level, 1)
            i = i + 1
            if i >= depth + base_level then break end
        until ret ~= true
    end

    if not depth or depth == 0 or type(depth) ~= "number" then
        trace_all()
    elseif type(depth) == "number" then
        trace_dep(depth)
    end
  
    print(lstr)
end
