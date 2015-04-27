--[[
Э����
--]]

--Э����
Protocol = {
    "Sys_Debug", --����
    "Sys_DebugRtn",--���Է���    
    "CS_Ping",   --ping 
    "SC_Ping",   --ping 
}
Protocol = table.enum(Protocol, 1)

--Э���Ӧ��protobuf�ַ���
local tProtoStr = {
    {Protocol.Sys_Debug, "Debug.CSDebug"},
    {Protocol.Sys_DebugRtn, "Debug.SCDebug"},
}

--protobuf�ļ�
local tProtoFile = {
    "Debug.proto",
    "Echo.proto",
}

local function loadProto()
    local strProtoDir = Q_GetModulPath() .. "ProtobufStr"
    parser.register(tProtoFile, strProtoDir)
end

local function initProtoStr()
    for _, val in pairs(tProtoStr) do
        setProtoStr(val[1], val[2])
    end
end

loadProto()
initProtoStr()
