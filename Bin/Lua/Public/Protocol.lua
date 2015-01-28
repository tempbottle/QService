--[[
Э����
--]]

--Э����
Protocol = {
    "Sys_Debug", --����
    "Sys_DebugRtn",--���Է���
    "Sys_ReqRegSV", --������ע��
    "Sys_RegSVRtn",--������ע�᷵��
    
    "CS_Ping",   --ping 
}
Protocol = creatEnumTable(Protocol, 1)

--protobuf�ļ�
local tProtoFile = {
    "RegSV.proto",
}

--Э���Ӧ��protobuf�ַ���
local tProtoStr = {
    {Protocol.Sys_ReqRegSV, "RegSV.Request"},
    {Protocol.Sys_RegSVRtn, "RegSV.Response"},
}

local function loadProto()
    local strProtoDir = Q_GetModulPath() .. "Lua" .. Q_GetPathSeparator().."Proto"
    parser.register(tProtoFile, strProtoDir)
end

local function initProtoStr()
    for _, val in pairs(tProtoStr) do
        setProtoStr(val[1], val[2])
    end
end

loadProto()
initProtoStr()
