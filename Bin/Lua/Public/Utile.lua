--[[
һЩ���ú���
--]]

--Debug��ӡ����
local bDebugSwitch = true

--��ӡ���ú�ʱ����
local btElapsedTime = true

--��ʱ��
local objCtimer
if btElapsedTime then
    objCtimer = CTimer()
end

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
function GetCharset(strWord)
    return objCharset:getStrCharset(strWord, string.len(strWord))
end

--[[
��������ȡһ���ظ���ID
������
����ֵ����
--]]
function GetID()
    return objSnowflakeID:getSnowflakeID()
end

--[[
�������ر�
������
����ֵ�� ��
--]]
function CloseLink(iClentID)
    g_objSessionManager:closeLinkByID(iClentID)
end

--[[
������������д�
������
����ֵ����
--]]
function AddFilterWord(strWord)
    objFilter:addSensitiveWord(strWord, string.len(strWord))
end

--[[
�������Ƿ�������д�
������
����ֵ����
--]]
function CheckFilterWord(strWord)
    return objFilter:checkHave(strWord, string.len(strWord))
end

--[[
�������������д�
������
����ֵ����
--]]
function Filter(strWord)
    return objFilter:Filter(strWord, string.len(strWord))
end

--[[
���������ü�ʱ��
������
����ֵ����
--]]
function TimerReStart()
    if not btElapsedTime then
        return
    end
    
    objCtimer:reStart()
end

--[[
��������ȡ��ʱ
������
����ֵ����
--]]
function TimerElapsed()
    if not btElapsedTime then
        return 0
    end
    
    return objCtimer:Elapsed()
end

--[[
������д�ļ�
������
����ֵ����
--]]
function FileWrite(strFile, strFormat, ...)
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
����ֵ����
--]]
function CallFunc(Func, ...)
    if "function" ~= type(Func) then
        Debug("in function CallFunc param Func is not function.")
        return
    end
    
    local bRtn, strMsg = pcall(Func, table.unpack({...}))
    if not bRtn then
        local strStack = debug.traceback()
        Debug(strMsg)
        Debug(strStack)
        Q_LOG(LOGLV_ERROR, strMsg)
        Q_LOG(LOGLV_ERROR, strStack)
    end
    
    return bRtn, strMsg
end

--[[
������table��ӡ
������
����ֵ����
--]]
function PrintTable (lua_table, indent)
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
            PrintTable(v, indent + 1)
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
����ֵ����
--]]
function IsTableEmpty(lua_table)
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
����ֵ����
--]]
function TableLens(lua_table)
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
����ֵ����
--]]
function CopyTable(tTable)
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
