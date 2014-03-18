#include "stdafx.h"
#include "Log.h"

// BOOL bIncludeSep -- 是否包含最后的分隔符"\"
CString GetExePath(BOOL bIncludeSep)
{
	// 得到当前的文件名
	CString strFileName;
	GetModuleFileName(AfxGetInstanceHandle(), strFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
	strFileName.ReleaseBuffer();

	// 得到当前目录
	strFileName = strFileName.Left(strFileName.ReverseFind(_T('\\')) + 1);

	if (bIncludeSep)
	{
		return strFileName;
	}
	else
	{
		return strFileName.Left(strFileName.GetLength() - 1);
	}
}

// 获取最后的文件名 如果给定文件不是全路径，就是相对于exe
CString GetFileForExePath(LPCTSTR strCurFileName)
{
	CString strPath = strCurFileName;
	if (!strPath.IsEmpty())
	{
		// 相对路径
		if (strPath.Find(_T(":")) <= 0)
		{
			strPath.Format(_T("%s%s"), GetExePath(TRUE), strCurFileName);
		}
	}

	return strPath;
}

#define LOG_EVENT _T("ChyLogWrite")

CString CLog::m_strLogFile = _T("");

CString CLog::m_strLogPrefix = _T("");

HANDLE CLog::m_hWriteEvent = NULL;

CLog::CLog()
{

}

CLog::~CLog()
{

}

short CLog::SetLogFile(LPCTSTR strPath)
{
	if (strPath == NULL || strPath[0] == 0)
	{
		m_strLogFile = GetFileForExePath(_T("log.txt"));
	}
	else
	{
		m_strLogFile = GetFileForExePath(strPath);
	}
	return 1;
}

CString CLog::GetLogFile()
{
	return m_strLogFile;
}

short CLog::ViewLogFile()
{
	CString strLogFile = GetLogFile();

	ShellExecute(NULL, _T("open"), strLogFile, NULL, NULL, SW_SHOW);

	return strLogFile.IsEmpty() ? 0 : 1;
}

short CLog::SetPrefix(LPCTSTR strPrefix)
{
	if (strPrefix && strPrefix[0])
	{
		m_strLogPrefix = strPrefix;
	}
	return 1;
}

CString CLog::sOutV(LPCTSTR strType, LPCTSTR strFormat, va_list valist)
{
	CString strPart_Prefix;

	if (!m_strLogPrefix.IsEmpty())
	{
		strPart_Prefix.Format(_T("[%s]"), m_strLogPrefix);
	}

	CString strPart_Time;
	{
		SYSTEMTIME sysTime = {0};
		GetLocalTime(&sysTime);
		strPart_Time.Format(_T("[%2.2d-%2.2d %2.2d:%2.2d:%2.2d.%3.3d]"), sysTime.wMonth, sysTime.wDay,
			sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);
	}

	CString strPart_Type;
	if (strType && strType[0])
	{
		strPart_Type.Format(_T("[%s]"), strType);
	}

	CString strPart_Info;
	{
		strPart_Info.FormatV(strFormat, valist);
	}

	CString str = strPart_Prefix + strPart_Time + strPart_Type + strPart_Info;

	return str;
}

CString CLog::sOut0(LPCTSTR strType, LPCTSTR strFormat, ...)
{
	va_list valist;

	va_start(valist, strFormat);

	CString strInfo = sOutV(strType, strFormat, valist);

	va_end(valist);

	return strInfo;
}

CString CLog::sOut(LPCTSTR strFormat, ...)
{
	va_list valist;

	va_start(valist, strFormat);

	CString strInfo = sOutV(NULL, strFormat, valist);

	va_end(valist);

	return strInfo;
}

short CLog::OutV(LPCTSTR strType, LPCTSTR strFormat, va_list valist)
{
	if (m_hWriteEvent == NULL)
	{
		m_hWriteEvent = OpenEvent(0, FALSE, LOG_EVENT);

		if (m_hWriteEvent == NULL)
		{
			m_hWriteEvent = CreateEvent(NULL, FALSE, TRUE, LOG_EVENT); 
		}
	}

	WaitForSingleObject(m_hWriteEvent, INFINITE);

	// 打开关闭文件
	if (m_strLogFile.IsEmpty())
	{
		SetLogFile(NULL);
	}

	CStdioFile file;

	if (file.Open(m_strLogFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite))
	{
		CString strPart_NewLine = _T("\n");

		CString strInfo = sOutV(strType, strFormat, valist);

		CString str = strInfo + _T("\n");

		file.SeekToEnd();

		file.WriteString(str);

		file.Close();
	}

	SetEvent(m_hWriteEvent);

	return 1;
}

short CLog::Out0(LPCTSTR strType, LPCTSTR strFormat, ...)
{
	va_list valist;
	va_start(valist, strFormat);

	short rtn = OutV(strType, strFormat, valist);

	va_end(valist);

	return rtn;
}

short CLog::Out(LPCTSTR strFormat, ...)
{
	va_list valist;
	va_start(valist, strFormat);

	short rtn = OutV(NULL, strFormat, valist);

	va_end(valist);

	return rtn;
}