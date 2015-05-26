--[[
Protobuf��ʼ��
--]]

--Э���Ӧ��protobuf�ַ���
local tProtoStr = {
    {Protocol.CS_Login, "PK_Login.CS_Login"},
    {Protocol.SC_Login, "PK_RtnCode.RtnCode"},
}

--protobuf�ļ�
local tProtoFile = {
    "Login.proto",
    "ReturnCode.proto",
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
