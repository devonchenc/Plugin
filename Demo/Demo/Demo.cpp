
// Demo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Demo.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "DemoDoc.h"
#include "DemoView.h"
#include "Log.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDemoApp

BEGIN_MESSAGE_MAP(CDemoApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CDemoApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_COMMAND(ID_LANGUAGE_ENG, &CDemoApp::OnLanguageEng)
	ON_UPDATE_COMMAND_UI(ID_LANGUAGE_ENG, &CDemoApp::OnUpdateLanguageEng)
	ON_COMMAND(ID_LANGUAGE_CHS, &CDemoApp::OnLanguageChs)
	ON_UPDATE_COMMAND_UI(ID_LANGUAGE_CHS, &CDemoApp::OnUpdateLanguageChs)
END_MESSAGE_MAP()

void CDemoApp::OnFileNew()
{
	CWinAppEx::OnFileNew();
}

// CDemoApp construction
CDemoApp::CDemoApp()
{
	m_bHiColorIcons = TRUE;

	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Demo.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CDemoApp object

CDemoApp theApp;


// CDemoApp initialization

BOOL CDemoApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

	// switch language
	SwitchLanguage();

	// replace document manager
	ReplaceDocManager();

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
/*	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_MAINFRAME,
		RUNTIME_CLASS(CDemoDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CDemoView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);*/

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	PIInitPlugin(this);

	// register plugin document templates
	PIRegisterDocTemplates();

	return TRUE;
}

int CDemoApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

//	int nResult = _CrtCheckMemory();
//	_CrtDumpMemoryLeaks();

	// removes all UI information about the application from registry
	CleanState();

	return CWinAppEx::ExitInstance();
}

// CDemoApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CDemoApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CDemoApp customization load/save methods

void CDemoApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CDemoApp::LoadCustomState()
{

}

void CDemoApp::SaveCustomState()
{

}

// CDemoApp message handlers

// write log
void CDemoApp::WriteLog(CString strLog)
{
	CLog::Out(strLog);
}

// replace default document manager
void CDemoApp::ReplaceDocManager()
{
	// make sure m_pDocManager hasn't been created
	ASSERT(m_pDocManager == NULL);
	// MFC only creates its own one if m_pDocManager is NULL
	// we replace the default doc manager
	m_pDocManager = new CPIDocManager;
}

void CDemoApp::RemovePluginDocTemplate()
{
	CPIDocManager* pDocManager = (CPIDocManager*)m_pDocManager;
	pDocManager->RemovePluginDocTemplate();
}

// switch language
void CDemoApp::SwitchLanguage()
{
	int nLanguage = GetProfileInt(_T("Settings"), _T("Language"), 0);

	LANGID id;
	if (nLanguage == 0)
	{
		id = MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT);
	}
	else
	{
		id = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_DEFAULT);
	}
	SetThreadUILanguage(id);
}

void CDemoApp::OnLanguageEng()
{
	WriteProfileInt(_T("Settings"), _T("Language"), 0);
}

void CDemoApp::OnUpdateLanguageEng(CCmdUI* pCmdUI)
{
	int nLanguage = GetProfileInt(_T("Settings"), _T("Language"), 0);
	pCmdUI->SetCheck(nLanguage == 0);
}

void CDemoApp::OnLanguageChs()
{
	WriteProfileInt(_T("Settings"), _T("Language"), 1);
}

void CDemoApp::OnUpdateLanguageChs(CCmdUI* pCmdUI)
{
	int nLanguage = GetProfileInt(_T("Settings"), _T("Language"), 0);
	pCmdUI->SetCheck(nLanguage == 1);
}
