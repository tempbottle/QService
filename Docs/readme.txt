ˮƽ���ޣ��緢��bug����ʲô�������������ϵ�ң����һ�����������ܡ�email:200309129@163.com��

һ������QService
windowsʹ��vs2010���߸��߰汾��
linuxʹ��mklib����������⣬Ȼ����mk��

����Lua�ļ�
    1��Main.lua
        Main.luaΪ��lua�ļ���ִ��һЩ������¼�������Ϣ����ַ������ӶϿ��ȡ�
    2��RegFuncs.lua
        RegFuncs.luaʵ���¼�ע�ᣬ�ص���
    3��Utile.lua 
        һЩ���ú���
    4��RegSV.lua
       ������ע��
    5��Protocol.lua
        �����롢������
    6��Macros.lua
        �ꡢö��
    7��ErrorCodes.lua
        ������
    8��Debug.lua
        debug
    9��DBManager.lua
        ���ݿ�
    10������Lua�ļ�
�����¼�
    1���¼�����
        �¼���ö����Macros.lua�е�GameEvent���û������Զ����¼�����������ȡ�
    2���¼�ִ��
        ���������������ҵȼ�������ִ��onGameEvent(GameEvent.LevelUp, objPlayer)
    3���¼�ע��
        ���������,regGameEvent(GameEvent.LevelUp, taskOnLevelUp) taskOnLevelUpΪ���
    �ȼ������Ժ�����������

�ġ�Lua��ʹ�õ�c++����
    ��lua�����ĺ����ο�Reg2Lua.cpp �е�void Register(void)����
    1������
        Q_GetModulPath ��ȡQServiceִ���ļ�����·��
        Q_GetPathSeparator ��ȡ·���ָ���("/"��"\")
        Q_LOG �ı���־
        getServerID ��ȡ���ñ��еķ�����ID
        getServerType ��ȡ���ñ��еķ���������
    2���� ������Ҫ����session��session����
        CSession session��ÿ�����Ӷ�Ӧһsession ��Lua�в�����ʵ����
            getSessionID ��ȡsession ID��
            setID  �������ⲿID �����ID,��Ҫͨ����ID����Ҷ�Ӧ
            getID  ��ȡ�������ⲿID
            setCheckID ����У��ID
            getCheckID ��ȡУ��ID
            setPing  ����ping
            setAccount ���ù������˺�
            getAccount ��ȡ�������˺�
            setStatus ����״̬ ֵΪMacros.lua�е�SessionStatusö��
            getStatus ��ȡ״̬ 
            getType ��ȡsession���� ֵΪMacros.lua�е�SessionTypeö��
        CSessionManager session���� ��Lua�в�����ʵ���� ��Ӧ��Main.lua�е�g_objSessionManager����
            closeLinkByID �ر����Ӹ���session ID
            closeCurLink �رյ�ǰsession����
            getCurSession ��ȡ��ǰsession���� CSession����  ������ɶ��ͶϿ���ʱ����ֵ����ʱ��Ϊnil
            getSessionByID ����session ID��ȡsession����
            getSVLinkerNameByType ��ȡĳ�����ͣ�SVType���ķ���������������
            getServerLinkerSession �������ƻ�ȡ�����������ӵ�session����
            sendToCur ��ǰsession������Ϣ
            sendToByID ��ָ��ID��session������Ϣ
            checkPing ping��飬����Ϊ��ʱʱ��
            getTimer ��ȡ��ʱ������ʱ����
            getCount ��ȡ��ʱ�������Ĵ���             
        

    



