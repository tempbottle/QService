--[[
���ж������ 
--]]

Object = {}
Object.__index = Object

--varSerialize ���л���ʶ �ַ����Ҳ�Ϊ�������л�Ϊprotobuf��table���л�Ϊstruct ����Ϊjson
function Object:new(varSerialize)
    local self = {}
    setmetatable(self, Object)
    
    self.SoleID = ""    
    self.Serialize = varSerialize
    
    return self
end

--ΨһID
function Object:setSoleID(strID)
    self.SoleID = strID
end

function Object:getSoleID()
    return self.SoleID
end

--��Ҫ��д
function Object:getInfo()
    return nil
end
--��Ҫ��д
function Object:setInfo(tMsg)
    
end

--���ַ�������
function Object:fromString(strMsg)
    local tMsg = {}
    if not self.Serialize then
        tMsg = cjson.decode(strMsg)
        Debug("deserialization json message.")
    else
        if "string" == type(self.Serialize) then
            if 0 ~= string.len(self.Serialize) then
                tMsg = protobuf.decode(self.Serialize, strMsg, string.len(strMsg))
                Debug("deserialization protobuf message.")
            else
                assert(false)
            end
        elseif "table" == type(self.Serialize) then
            g_objSerBinary:setBuffer(strMsg, string.len(strMsg))
            tMsg = g_objSerBinary:getStruct(self.Serialize)
            Debug("deserialization struct message.")
        else
            assert(false)
        end
    end
    
    assert(tMsg)
    self:setInfo(tMsg)
end

--תΪ�ַ��� strProtoΪnilʹ��json
function Object:toString()
    if not self.Serialize then
        Debug("serialize json message.")
        return cjson.encode(self:getInfo())
    else
        if "string" == type(self.Serialize) then
            if 0 ~= string.len(self.Serialize) then
                Debug("serialize protobuf message.")
                return protobuf.encode(self.Serialize, self:getInfo())
            else
                assert(false)
            end
        elseif "table" == type(self.Serialize) then
            Debug("serialize struct message.")
            g_objSerBinary:reSetWrite()
            assert(g_objSerBinary:setStruct(self:getInfo(), self.Serialize))
            return g_objSerBinary:getBuffer()
        else
            assert(false)
        end
    end
end
