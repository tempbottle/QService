--[[
一些常用函数
--]]

--调试开关
local bDebugSwitch = true

--打印调用耗时开关
local btElapsedTime = true

--计时器
local objCtimer
if btElapsedTime then
	objCtimer = CTimer()
end

--唯一ID
local objSnowflakeID = CSnowflakeID()
objSnowflakeID:setMachineID(getServerID())

--过滤敏感字
local objFilter = CFilter()

--字符集
local objCharset = CCharset()

--[[
描述：调试信息打印
参数：
返回值：无
--]]
function Debug(msg)
	if bDebugSwitch then
		print(string.format("[%s][Debug] %s", os.date(), tostring(msg)))
	end
end

--[[
描述：获取字符串编码格式
参数：
返回值：编码格式
--]]
function GetCharset(strWord)
	return objCharset:getStrCharset(strWord, string.len(strWord))
end

--[[
描述：获取一不重复的ID
参数：
返回值：无
--]]
function GetID()
	return objSnowflakeID:getSnowflakeID()
end

--[[
描述：添加敏感词
参数：
返回值：无
--]]
function AddFilterWord(strWord)
	objFilter:addSensitiveWord(strWord, string.len(strWord))
end

--[[
描述：是否包含敏感词
参数：
返回值：无
--]]
function CheckFilterWord(strWord)
	return objFilter:checkHave(strWord, string.len(strWord))
end

--[[
描述：过滤敏感词
参数：
返回值：无
--]]
function Filter(strWord)
	return objFilter:Filter(strWord, string.len(strWord))
end

--[[
描述：重置计时器
参数：
返回值：无
--]]
function TimerReStart()
	if not btElapsedTime then
		return
	end
	
	objCtimer:reStart()
end

--[[
描述：获取耗时
参数：
返回值：无
--]]
function TimerElapsed()
	if not btElapsedTime then
		return 0
	end
	
	return objCtimer:Elapsed()
end

--[[
描述：保护模式中调用函数
参数：
返回值：无
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
		Q_LOG(Macros.LOGLV_ERROR, strMsg)
		Q_LOG(Macros.LOGLV_ERROR, strStack)
	end
end

--[[
描述：table打印
参数：
返回值：无
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
描述：table是否为空
参数：
返回值：无
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
描述：table长度
参数：
返回值：无
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
描述：table拷贝,不考虑带环的
参数：
返回值：无
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
