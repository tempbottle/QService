
#ifndef Q_QSTRING_H_
#define Q_QSTRING_H_

#include "Macros.h"

/*ת��д*/
void Q_Convert2Upper(char *pszWord);
std::string &Q_Convert2Upper(std::string &strWord);
/*תСд*/
void Q_Convert2Lower(char *pszWord);
std::string &Q_Convert2Lower(std::string &strWord);
/*���ַ�����iBg��iEnd֮����ַ�����(iBg, iEnd��0��ʼ)*/
void Q_ReverseStr(char *pszWord, const size_t iBg, const size_t iEnd);
std::string &Q_ReverseStr(std::string &strWord, const size_t iBg, const size_t iEnd);
/*�����������n���ַ��Ƶ��ַ���ǰ��*/
void Q_MoveN2Before(char *pszWord, const size_t n);
std::string &Q_MoveN2Before(std::string &strWord, const size_t n);

/*wchar_t ת char*/
int Q_WideCharToMultiByte(const wchar_t *pszSrc, char *pszDest);
/*char ת wchar_t*/
int Q_MultiByteToWideChar(const char *pszSrc, wchar_t *pszDest);

/*ת�ַ���*/
template<typename T>
std::string Q_ToString(T number)
{
    std::stringstream objStream;

    objStream << number;

    return objStream.str();
};

/*ת����*/
template<typename T>
T Q_ToNumber(const char *pszNumber)
{
    T  iValue = Q_INIT_NUMBER;
    std::stringstream objStream;

    objStream << pszNumber;
    objStream >> iValue;

    return iValue;
};

/*ȥ���ַ��������пո�,\n,\r,\t*/
std::string &Q_TrimAll(std::string &strSource);
/*ȥ������ַ��������пո�,\n,\r,\t*/
std::string &Q_TrimLeft(std::string &strSource);
/*ȥ���ұ��ַ��������пո�,\n,\r,\t*/
std::string &Q_TrimRight(std::string &strSource);
/*ȥ�������ַ��������пո�,\n,\r,\t*/
std::string &Q_Trim(std::string &strSource);

/*�ַ������*/
void Q_Split(const std::string &strSource, const char *pszFlag,
    std::list<std::string> &lstRst);

/*ȡ�ñ�ǵ�һ�γ���֮ǰ���ַ���*/
std::string Q_GetFrontOfFlag(const std::string &strSource, const char *pszFlag);
/*ȡ�ñ�����һ�γ���֮����ַ���*/
std::string Q_GetLastOfFlag(const std::string &strSource, const char *pszFlag);

/*�ַ����滻,�򵥽�ԭ�ַ����е��ַ��滻Ϊָ���ַ�*/
std::string &Q_Replace(std::string &strSource, const char *pszFlag, 
    const char *pszReFlag);
/*�ַ����滻,�滻ԭ�ַ������ַ���ָ���ַ�������Ҫ�滻���ַ���ԭ�ַ��в�����*/
std::string &Q_Replace_Depth(std::string &strSource, const char *pszFlag, 
    const char *pszReFlag);

/*�ַ���ʽ��*/
std::string Q_FormatVa(const char *pcFormat, va_list args);
/*�ַ���ʽ��*/
std::string Q_FormatStr(const char *pcFormat, ...);

#endif//Q_QSTRING_H_
