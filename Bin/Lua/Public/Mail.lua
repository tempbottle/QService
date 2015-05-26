--[[
�ʼ�����
--]]

Mail = {}
Mail.__index = Mail

function Mail:new()
    local self = {}
    setmetatable(self, Mail)
    
    self.Subject = ""--����
    self.StrMsg = ""--�ַ�������
    self.HtmlStrMsg = ""--Html�ַ�������
    self.HtmlFileMsg = ""--Html�ļ�����
    self.Recipient = {}--������
    self.Attach = {}--���� 
    
    return self
end

--����
function Mail:Clear()
    self.Subject = ""
    self.StrMsg = ""
    self.HtmlStrMsg = ""
    self.HtmlFileMsg = ""
    self.Recipient = {}
    self.Attach = {} 
end

--����
function Mail:Send()
    assert(not table.empty(self.Recipient))
    assert(0 ~= string.len(self.Subject))
    
    local strJson = cjson.encode(self)
    sendMail(strJson)
end

--����
function Mail:setSubject(strSubject)
    self.Subject = strSubject
end

--��Ϣ
function Mail:setStrMsg(strMsg)
    self.StrMsg = strMsg
end

function Mail:setHtmlMsg(strHtmlMsg)
    self.HtmlStrMsg = strHtmlMsg
end

function Mail:setHtmlFileMsg(strHtmlFile)
    self.HtmlFileMsg = strHtmlFile
end

--������
function Mail:addRecipient(strAddr)
    for _, val in pairs(self.Recipient) do
        if val == strAddr then
            return
        end
    end
    
    table.insert(self.Recipient, strAddr)
end

function Mail:removeRecipient(strAddr)
    for key, val in pairs(self.Recipient) do
        if val == strAddr then
            table.remove(self.Recipient, key)
            return
        end
    end
end

function Mail:clearRecipient()
    self.Recipient = {}
end

--����
function Mail:addAttach(strFile)
    for _, val in pairs(self.Attach) do
        if val == strFile then
            return
        end
    end
    
    table.insert(self.Attach, strFile)
end

function Mail:removeAttach(strFile)
    for key, val in pairs(self.Attach) do
        if val == strFile then
            table.remove(self.Attach, key)
            return
        end
    end
end

function Mail:clearAttach()
    self.Attach = {}
end
