--[[
��ҹ���
--]]

--�����������{id = {}}
g_tAllPlayer
--�˺����ӳ��{account = {playername = id}
g_tAccountPlayerMap

if not g_tAllPlayer then
    g_tAllPlayer = {}
end

if not g_tAccountPlayerMap then
    g_tAccountPlayerMap = {}
end

--[[
������������
������
����ֵ����
--]]

--[[
������������
������
����ֵ����
--]]
function AddPlayer(strAccount, strID, strPlayerName, tInfo)
    if g_tAllPlayer[strID] then
        Debug("AddPlayer player id:"..strID.." already exits.")
        return
    else
        g_tAllPlayer[strID] = tInfo
    end
    
    if not g_tAccountPlayerMap[strAccount] then
        g_tAccountPlayerMap[strAccount] = {}
    end
    
    if not g_tAccountPlayerMap[strAccount][strPlayerName] then
        g_tAccountPlayerMap[strAccount][strPlayerName] = strID
    end
end

--[[
�������Ƴ����
������
����ֵ����
--]]
function RemovePlayer(strAccount, strID)
    g_tAllPlayer[strID] = nil
    
    if g_tAccountPlayerMap[strAccount] then
        for key, val in pairs(g_tAccountPlayerMap[strAccount]) do
            if val == strID then
                g_tAccountPlayerMap[strAccount][key] = nil
                
                if IsTableEmpty(g_tAccountPlayerMap[strAccount]) then
                    g_tAccountPlayerMap[strAccount] = nil
                end
                
                break
            end
        end
    end
end


