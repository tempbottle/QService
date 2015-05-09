
#ifndef Q_LUABINARY_H_
#define Q_LUABINARY_H_

#include "BufferMgr.h"
#include "QString.h"
#include "LuaBridge/LuaBridge.h"

//lua �����ƽ���
class CLuaBinary
{
public:
    CLuaBinary(void);
    ~CLuaBinary(void);

    //����lua���
    void setLua(struct lua_State *pLua);
    //����Ҫ����������
    void setBuffer(const char *pszBuf, const size_t iLens);

    //��ȡҪ���������ݳ���
    size_t getLens(void) const;

    //���ƺ���
    //����д
    void reSetWrite(void);
    //��  ����ָ���ֽ�
    void skipRead(const size_t iLens);
    //д  ����ָ���ֽ������ֽ���0���
    bool skipWrite(const size_t iLens);

    //char
    bool setSint8(const short sVal);
    short getSint8(void);
    bool setUint8(const unsigned short usVal);
    unsigned short getUint8(void);

    //bool
    bool setBool(const bool bVal);
    bool getBool(void);

    //short
    bool setSint16(const short sVal);
    short getSint16(void);
    bool setUint16(const unsigned short usVal);
    unsigned short getUint16(void);

    //int
    bool setSint32(const int iVal);
    int getSint32(void);
    bool setUint32(const unsigned int uiVal);
    unsigned int getUint32(void);

    //int64_t
    bool setSint64(const char *pszVal);
    const char *getSint64(void);
    bool setUint64(const char *pszVal);
    const char *getUint64(void);

    //double
    bool setDouble(const double dVal);
    double getDouble(void);

    //float
    bool setFloat(const float fVal);
    float getFloat(void);

    //string ��֤��/0����
    bool setString(const char *pszVal);
    std::string getString(void);

    //byte
    bool setByte(const char *pszVal, const size_t iLens);
    std::string getByte(const size_t iLens);

    //struct ��֧��λ��
    /* objAttr �ṹ������ StructAttrö��*/
    bool setStruct(luabridge::LuaRef objVal, luabridge::LuaRef objAttr);
    luabridge::LuaRef getStruct(luabridge::LuaRef objAttr);

    //��ȡд���buffer
    std::string getBuffer(void) const;

    CBuffer *getBufferMgr(void)
    {
        return &m_objWritBuffer;
    }

private:
    bool setVal(const char *pszBuf, const size_t iLens);
    luabridge::LuaRef getStructAttr(luabridge::LuaRef &objAttr,
        std::string &strName, DataType &emType, int &iSize);

    template<typename T>
    T getLuaNumber(luabridge::LuaRef &objTmp)
    {
        T tVal = Q_INIT_NUMBER;
        if (objTmp.isNumber())
        {
            tVal = objTmp.cast<T>();
        }
        else if (objTmp.isString())
        {
            tVal = Q_ToNumber<T>(objTmp.cast<const char*>());
        }
        else if (objTmp.isNil())
        {
            tVal = Q_INIT_NUMBER;
        }
        else
        {
            tVal = Q_INIT_NUMBER;
        }
        
        return tVal;
    };

    template<typename T>
    T readNumber(void)
    {
        T tVal = Q_INIT_NUMBER;
        if ((m_iCurParseLens + sizeof(T)) > m_iParseBufLens)
        {
            return tVal;
        }

        tVal = *((T*)(m_pParseBuffer + m_iCurParseLens));
        m_iCurParseLens += sizeof(T);

        return tVal;
    };

private:
    char *m_pParseBuffer;//Ҫ������buffer
    struct lua_State *m_pLua;//lua���
    size_t m_iParseBufLens;//Ҫ������buffer����
    size_t m_iCurParseLens;//�Ѿ������ĳ���
    char acEmpty[1];
    char acTmp[Q_ONEK];
    CBuffer m_objWritBuffer;//дbuffer
    std::string m_strVali64;
    std::stringstream m_objStream;
};

#endif//Q_LUABINARY_H_
