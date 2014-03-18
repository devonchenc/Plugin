#pragma once

class CLog
{
public:
	CLog();
	virtual ~CLog();

public:
	//-日志文件-
	//如果没有指定，则为exe所在路径下的log.log文件
	static CString GetLogFile();

	static short SetLogFile(LPCTSTR strPath);

	static short ViewLogFile();

	// 前缀
	// 如果多个进程往同一个文件输出日志，可以为每个进程设置一个前缀
	// 前缀出现在日期时间之前
	static short SetPrefix(LPCTSTR strPrefix);

	//-日志信息-
	//-获取日志字符串,可以另外-
	static CString sOutV(LPCTSTR strType, LPCTSTR strFormat = NULL, va_list valist = NULL);

	static CString sOut0(LPCTSTR strType, LPCTSTR strFormat = NULL,...);
	
	static CString sOut ( LPCTSTR strFormat = NULL,...);

	//将日志信息输出到文件
	static short OutV(LPCTSTR strType, LPCTSTR strFormat = NULL, va_list valist = NULL);

	static short Out0(LPCTSTR strType, LPCTSTR strFormat = NULL, ...);

	static short Out(LPCTSTR strFormat = NULL, ...);

protected:
	static CString m_strLogFile;

	static CString m_strLogPrefix;

	static HANDLE m_hWriteEvent;
};