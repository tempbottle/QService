/*****************************************************************************
* Copyright (c) 2011-2012. Qifu Luo All Rights Reserved.200309129@163.com 
* svn:http://asuraserver.googlecode.com/svn/
* github:https://github.com/QService/QService
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
* 3. The name of the author may not be used to endorse or promote products
*    derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
* NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#include "DispWithLua.h"

#define LUA_EVENT_ONSTARTUP   "Lua_OnStartUp"
#define LUA_EVENT_ONSHUTDOWN  "Lua_OnShutDown"
#define LUA_EVENT_ONCONNECTED "Lua_OnConnected"
#define LUA_EVENT_ONCLOSE     "Lua_OnClose"
#define LUA_EVENT_ONTIMER     "Lua_OnTimer"
#define LUA_EVENT_ONREAD      "Lua_OnRead"
#define LUA_EVENT_ONHTTPREAD  "Lua_OnHttpRead"
#define LUA_EVENT_ONSVLINKED  "Lua_OnLinkedServer"

CDisposeEvent::CDisposeEvent(const char *pszLuaFile) : m_usOpCode(Q_INIT_NUMBER), 
    m_usMsgLens(Q_INIT_NUMBER), m_pLua(NULL)
{
    m_pLua = luaL_newstate();
    if (NULL == m_pLua)
    {
        Q_EXCEPTION(Q_ERROR_ALLOCMEMORY, "%s", "luaL_newstate error.");
    }

    luaL_openlibs(m_pLua);

    m_objReg2Lua.setLState(m_pLua);
    m_objReg2Lua.Register();

    int iRtn = luaL_dofile(m_pLua, pszLuaFile);
	if (Q_RTN_OK != iRtn)
	{
		const char *pError = lua_tostring(m_pLua, -1);
		std::string strLuaError = ((NULL == pError) ? "" : pError);
		if (NULL != m_pLua)
		{
			lua_close(m_pLua);
			m_pLua = NULL;
		}

		Q_EXCEPTION(Q_RTN_FAILE, strLuaError.c_str());      
	}
}

CDisposeEvent::~CDisposeEvent(void)
{
    if (NULL != m_pLua)
    {
        lua_close(m_pLua);
        m_pLua = NULL;
    }
}

void CDisposeEvent::onSerciveStartUp(void)
{
    try
    {
        getSessionManager()->setLua(m_pLua);
        luabridge::getGlobal(m_pLua, LUA_EVENT_ONSTARTUP)(getSessionManager());
        startSVLinker();
    }
    catch(luabridge::LuaException &e)
    {
        Q_Printf("%s", e.what());
        Q_SYSLOG(LOGLV_ERROR, "%s", e.what());
    }
    catch(CException &e)
    {
        Q_Printf("exception. code %d, message %s", 
            e.getErrorCode(), e.getErrorMsg());
        Q_SYSLOG(LOGLV_ERROR, "exception. code %d, message %s", 
            e.getErrorCode(), e.getErrorMsg());
    }
}

void CDisposeEvent::onSerciveShutDown(void)
{
    try
    {
        luabridge::getGlobal(m_pLua, LUA_EVENT_ONSHUTDOWN)();
    }
    catch(luabridge::LuaException &e)
    {
        Q_Printf("%s", e.what());
        Q_SYSLOG(LOGLV_ERROR, "%s", e.what());
    }
    catch(CException &e)
    {
        Q_Printf("exception. code %d, message %s", 
            e.getErrorCode(), e.getErrorMsg());
        Q_SYSLOG(LOGLV_ERROR, "exception. code %d, message %s", 
            e.getErrorCode(), e.getErrorMsg());
    }
}

void CDisposeEvent::onSocketClose(void)
{
    try
    {
        luabridge::getGlobal(m_pLua, LUA_EVENT_ONCLOSE)();
    }
    catch(luabridge::LuaException &e)
    {
        Q_Printf("%s", e.what());
        Q_SYSLOG(LOGLV_ERROR, "%s", e.what());
    }
    catch(CException &e)
    {
        Q_Printf("exception. code %d, message %s", 
            e.getErrorCode(), e.getErrorMsg());
        Q_SYSLOG(LOGLV_ERROR, "exception. code %d, message %s", 
            e.getErrorCode(), e.getErrorMsg());
    }
}

void CDisposeEvent::onTimerEvent(void)
{
    try
    {
        luabridge::getGlobal(m_pLua, LUA_EVENT_ONTIMER)();
    }
    catch(luabridge::LuaException &e)
    {
        Q_Printf("%s", e.what());
        Q_SYSLOG(LOGLV_ERROR, "%s", e.what());
    }
    catch(CException &e)
    {
        Q_Printf("exception. code %d, message %s", 
            e.getErrorCode(), e.getErrorMsg());
        Q_SYSLOG(LOGLV_ERROR, "exception. code %d, message %s", 
            e.getErrorCode(), e.getErrorMsg());
    }
}

void CDisposeEvent::onSocketRead(const char *pszMsg, const Q_PackHeadType &iLens)
{
    try
    {
        if (iLens < sizeof(m_usOpCode))
        {
            return;
        }

        m_usOpCode = ntohs(*((unsigned short *)(pszMsg)));
        m_usMsgLens = iLens - sizeof(m_usOpCode);
        if (0 == m_usMsgLens)
        {
            luabridge::getGlobal(m_pLua, LUA_EVENT_ONREAD)(m_usOpCode, "", m_usMsgLens);
        }
        else
        {
            m_stBinaryStr.pBuf = (char*)(pszMsg + sizeof(m_usOpCode));
            m_stBinaryStr.iLens = m_usMsgLens;
            luabridge::getGlobal(m_pLua, LUA_EVENT_ONREAD)(m_usOpCode, m_stBinaryStr, m_usMsgLens);
        }
    }
    catch(luabridge::LuaException &e)
    {
        Q_Printf("%s", e.what());
        Q_SYSLOG(LOGLV_ERROR, "%s", e.what());
    }
    catch(CException &e)
    {
        Q_Printf("exception. code %d, message %s", 
            e.getErrorCode(), e.getErrorMsg());
        Q_SYSLOG(LOGLV_ERROR, "exception. code %d, message %s", 
            e.getErrorCode(), e.getErrorMsg());
    }
}

void CDisposeEvent::onHttpRead(class CHttpBuffer *pHttpBuffer)
{
    try
    {
        luabridge::getGlobal(m_pLua, LUA_EVENT_ONHTTPREAD)(pHttpBuffer);
    }
    catch(luabridge::LuaException &e)
    {
        Q_Printf("%s", e.what());
        Q_SYSLOG(LOGLV_ERROR, "%s", e.what());
    }
    catch(CException &e)
    {
        Q_Printf("exception. code %d, message %s", 
            e.getErrorCode(), e.getErrorMsg());
        Q_SYSLOG(LOGLV_ERROR, "exception. code %d, message %s", 
            e.getErrorCode(), e.getErrorMsg());
    }
}

void CDisposeEvent::onLinkedServer(class CSession *pSession)
{
    try
    {
        luabridge::getGlobal(m_pLua, LUA_EVENT_ONSVLINKED)(pSession);
    }
    catch(luabridge::LuaException &e)
    {
        Q_Printf("%s", e.what());
        Q_SYSLOG(LOGLV_ERROR, "%s", e.what());
    }
    catch(CException &e)
    {
        Q_Printf("exception. code %d, message %s", 
            e.getErrorCode(), e.getErrorMsg());
        Q_SYSLOG(LOGLV_ERROR, "exception. code %d, message %s", 
            e.getErrorCode(), e.getErrorMsg());
    }
}
