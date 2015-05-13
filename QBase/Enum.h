
#ifndef Q_ENUM_H_
#define Q_ENUM_H_

enum  WebSockOpCode
{
    CONTINUATION = 0x00,
    TEXT_FRAME = 0x01,
    BINARY_FRAME = 0x02,
    CLOSE = 0x08,
    PING = 0x09,
    PONG = 0x0A
};

/*aes key����*/
enum AESKeyType
{
    KeyNone = 0,
    Key128 = 128,
    Key192 = 192,
    Key256 = 256
};

/*��־����,ԽСԽ��*/
enum LOG_LEVEL
{
    LOGLV_NOLOG  = 0,
    LOGLV_ERROR  = 300, 
    LOGLV_WARN   = 400,
    LOGLV_INFO   = 500,
    LOGLV_DEBUG  = 600
};

/*���񼶱�*/
enum TaskLevel
{
    Q_ThreadLV_High = 0,
    Q_ThreadLV_Normal,
    Q_ThreadLV_Low
};

/*���ݿ�����*/
enum DBType
{
    DBType_Sqlite3 = 0,
    DBType_MySql
};

//��������
enum SessionType
{
    STYPE_TCP = 0,//tcp
    STYPE_TCPCLIENT,//tcp����
    STYPE_WEBSOCK//websocket
};

/*session״̬*/
enum SessionStatus
{
    SessionStatus_Closed = -1,
    SessionStatus_Connect,
    SessionStatus_Linked,
    SessionStatus_Logining,
    SessionStatus_Logined,
    SessionStatus_Playing,
    SessionStatus_GM
};

/*����*/
enum RunStatus
{
    RunStatus_Unknown = 0,//δ֪
    RunStatus_Error,//��������
    RunStatus_Starting,//��������
    RunStatus_Runing,//����
    RunStatus_Stopping,//����ֹͣ
    RunStatus_Stopped//ֹͣ
};

/*��������*/
enum DataType
{
    DType_SINT8 = 0,//char 
    DType_UINT8,//unsigned char
    DType_BOOL,//bool
    DType_SINT16,//short
    DType_UINT16,//unsigned short
    DType_SINT32,//int
    DType_UINT32,//unsigned int
    DType_SINT64,//int64_t 
    DType_UINT64,//uint64_t
    DType_FLOAT,//float
    DType_DOUBLE,//double
    DType_STRING,//string
    DType_BYTE,//byte
    DType_STRUCT,//struct
    DType_SKIP//��������������
};

/*�ṹ������*/
enum StructAttr
{
    StAttr_Name = 1,//������
    StAttr_Type,//��������
    StAttr_Size,//�ֽ� DType_STRING �� DType_SKIP����
    StAttr_StAttr//�ṹ������
};

#endif//Q_ENUM_H_
