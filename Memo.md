================================================================================
关于插件的app
app就是全局变量
CImageApp theApp;
dllmain会调用CImageApp::InitInstance()


================================================================================
== 插件基础类
# 先看所有插件的基础PluginSupport.dll输出哪些内容
  查找PLUGIN_EXT_CLASS，封装了一些MFC的class
    class PLUGIN_EXT_CLASS CPIDocManager : public CDocManager
    class PLUGIN_EXT_CLASS CPIDocument : public CDocument
    class PLUGIN_EXT_CLASS CPIMDIChildWndEx : public CMDIChildWndEx
    class PLUGIN_EXT_CLASS CPIMDIFrameWndEx : public CMDIFrameWndEx
    class PLUGIN_EXT_CLASS CPIMultiDocTemplate : public CMultiDocTemplate
    class PLUGIN_EXT_CLASS CPIToolBar : public CMFCToolBar
    class PLUGIN_EXT_CLASS CPIView : public CScrollView
    
 # 大致了解PluginSupport.dll模块：
  // dll输出 
  PluginDefine.h：定义类型：结构，模板，自定义消息 
  PluginLibrary.h: 输出一些c方法。
  PluginImpl.h: 输出一些c方法。
    // 创建pluginSupport, 加载所有插件
    PLUGIN_EXPORT void PIInitPlugin(CWinApp* pApp);
  // dll实现
  PluginSupport.h: dll的app。保存插件封装的数组。
    class CPluginSupportApp : public CWinApp            
  PluginWrapper.h: 封装插件的管理接口、描述等。CPluginInfo m_PluginInfo;CPlugin* m_pPlugin;
  PluginClass.h: 插件的管理接口，每个插件必须实现。class PLUGIN_EXT_CLASS CPlugin？   
  // 重写各种控件
  PI****.h: class PLUGIN_EXT_CLASS CPIView : public CScrollView

# 注意PluginSupport.dll不是1个可调用的插件。
  没有声明自己是插件：DECLARE_PLUGIN，IMPLEMENT_PLUGIN
  在插件浏览器里看不到这个dll。

# 问题：可调用插件怎么引用PluginSupport？
  在每个插件的stdafx.h里，引用插件基础类。
    #include "..\\..\\PluginSupport\\PluginSupport\\PluginLibrary.h"
  在PluginLibrary.h里，加载动态库
    #pragma comment(lib, "..\\..\\PluginSupport\\Debug\\PluginSupport.lib")
		#pragma message("Automatically link with PluginSupport Debug")
    

================================================================================
== 创建插件
# 在stdafx.h里引用PluginLibLibrary.h, PluginSupport.lib。

# 在插件类里继承插件基础类，用宏快速实现插件类方法。
    class CImagePlugin : public CPlugin
    DECLARE_PLUGIN(CImagePlugin)
    IMPLEMENT_PLUGIN(CImagePlugin)

# IMPLEMENT_PLUGIN定义了1个输出函数，创建这个静态实例。
  插件容器调用这个方法，创建1个插件。
  #define IMPLEMENT_PLUGIN(class_name) \
	PLUGIN_EXPORT class_name* GetInstance(){ return class_name::Instance(); }
      
# DECLARE_PLUGIN定义了1个插件类的静态实例。 
  #define DECLARE_PLUGIN(class_name) \
	static class_name* Instance(){ static class_name _instance; return &_instance; } \
  
# 看怎么写1个插件？比如Image插件
  class CImagePlugin : public CPlugin，准备文档模板，提供本插件描述，用来插件管理？==》export
  class CImageApp : public CWinApp：创建1个Plugin系统的DocTemplate
  class CImageDocument : public CPIDocument，继承CPI***, 从框架调用docment==》export
  class CImageView : public CScrollView，为什么不是CPIView？不从框架调用view。
# MEMO: 容器要什么模块，就输出什么模块

# 再看典型1个插件 PluginBrowser
  class CBrowserPlugin : public CPlugin, 添加菜单，提供本插件描述 ==》export
  其他模块都不继承CPI***，因为从菜单调用插件功能。
  
================================================================================
== 加载插件
# 容器怎么加载插件？
  在容器App里，BOOL CDemoApp::InitInstance()， PIInitPlugin(this); 
  PIInitPlugin是PluginSupport里的1个方法
    PLUGIN_EXPORT void PIInitPlugin(CWinApp* pApp)
  进入PluginSupport的App
        CPluginSupportApp* pThisApp = (CPluginSupportApp*)AfxGetApp();
	      // load plugin
	      pThisApp->SetMainApp(pApp);
	      pThisApp->LoadPlugin(pApp)

# 搜索、初始化插件
  void CPluginSupportApp::LoadPlugin(CWinApp* pApp)
  加载exe目录下的dll。m_hInstance = LoadLibrary(strFileName);
  找到GetInstance方法。m_pfnGetInstance = (GETINSTANCE)GetProcAddress(m_hInstance, "GetInstance");
  创建插件实例。m_pPlugin = m_pfnGetInstance(); 
  插件被封装在CPluginWrapper里。
  这些wrapper被保存在插件数组里。CPluginSupportApp::m_PluginArray
  轮询插件数组，初始化每个插件。
    m_PluginArray.GetAt(i)->InitPlugin(pApp, i);
      每个插件要编号i
      设置commandID的第1个序号：#define PLUGIN_COMMAND_BEGIN	50000
      UINT CPluginWrapper::m_nCommandIDIndex = PLUGIN_COMMAND_BEGIN;
      初始化插件；m_pPlugin->Init(); 总是向容器主菜单添加菜单项。
      例如DrawPlugin: void CDrawPlugin::Init()
		m_PluginArray.GetAt(i)->QueryPlugin();

================================================================================
== 调用插件、容器
# 容器app调用插件：从c dll函数
# 插件调用自己：windows消息
# 插件调用容器app：PluginLibrary.h
	   CPluginSupportApp* pApp = (CPluginSupportApp*)AfxGetApp();
	   CWinApp* pMainApp = pApp->GetMainApp();
	   CWnd* pMainWnd = pMainApp->m_pMainWnd;
     pMainWnd->SendMessage();
     
# 2个错误的方法
/*  直接创建的DocTemplate ===> 错误
//	因为在创建CMDIChildWnd::Create时，找不到MainFrame，ENSURE_VALID(pMainWnd);
	if (!m_pDocTemplate)
		m_pDocTemplate = new CPIMultiDocTemplate(IDR_IMAGE,
			RUNTIME_CLASS(CImageDocument),
			RUNTIME_CLASS(CPIMDIChildWndEx), // 自定义 MDI 子框架
			RUNTIME_CLASS(CImageView));
	m_pDocTemplate->OpenDocumentFile(_T(""), FALSE, TRUE);
*/
/*	
// 从PluginLibrary到容器App ===> 错误
//	因为在创建CMDIChildWnd::Create时，找不到MainFrame，ENSURE_VALID(pMainWnd);
//	BOOL CMDIChildWnd::Create(LPCTSTR lpszClassName,
//		CWinThread *pThread = AfxGetThread();
//		ENSURE_VALID(pThread);
//		CWnd* pMainWnd = pThread->m_pMainWnd;
//		ENSURE_VALID(pMainWnd);
	CWinApp* pMainApp = PIGetMainWinApp();
	POSITION pos = pMainApp->GetFirstDocTemplatePosition();
	CDocTemplate* pDocTemplate = pMainApp->GetNextDocTemplate(pos);
	pDocTemplate->OpenDocumentFile(NULL);
*/

================================================================================
# 关于文档模板DocumentTemplate的
  1，初始化时，在BOOL CDemoApp::InitInstance()中
  替换插件的文档模板管理器：ReplaceDocManager();
      m_pDocManager = new CPIDocManager;
  注册插件的文档模板：PIRegisterDocTemplates();
      遍历所有插件，取出文档模板
      CDocTemplate* pDocTemplate = pPluginWrapper->GetDocTemplate(j);
      pMainApp->AddDocTemplate(pDocTemplate);   // pMainApp是容器app
  2，关闭程序时，CMainFrame::OnClose()
      pDocManager->RemovePluginDocTemplate();

================================================================================
# 关于插件菜单
  1，创建菜单资源
  2，合并插件菜单到容器主菜单，CBrowserPlugin::Init()
       会有便利工具：CPlugin::MergeMenu
问题：怎么确定不同插件菜单的相对顺序。
  3，实现菜单处理，可以在插件app里，void CPluginBrowserApp::OnPluginBrowser()
  

================================================================================
  
# 这些插件都是“动态链接到 MFC 的规则 DLL”
  Regular DLLs Dynamically Linked to MFC
  是在内部使用 MFC 的 DLL，这类 DLL 中的导出函数可由 MFC 或非 MFC 可执行文件调用。
  这类 DLL 是使用 MFC 动态链接库版本（也称作 MFC 共享版本）生成的。
  函数通常是通过标准 C 接口从规则 DLL 导出的。
  必须在所有导出函数的开始处添加 AFX_MANAGE_STATE 宏，以将当前模块的状态设置为 DLL 的状态。
  AFX_MANAGE_STATE(AfxGetStaticModuleState( ))
  客户端可执行文件可以用任何支持使用 DLL 的语言（C、C++、Pascal、Visual Basic 等）编写；它不必是 MFC 应用程序。
  这类 DLL 必须实例化 CWinApp 派生类。
  与应用程序的 CWinApp 对象不同，DLL 的 CWinApp 对象没有主消息泵。
  如果 DLL 生成无模式对话框或有自己的主框架窗口，则应用程序的主消息泵必须调用从 DLL 导出的例程来调用 CWinApp::PreTranslateMessage。
  动态链接到 MFC 的 DLL 无法同时静态链接到 MFC。 像任何其他 DLL 一样，应用程序链接到动态链接到 MFC 的规则 DLL。
  符号通常是通过标准 C 接口从规则 DLL 导出的。
  extern "C" __declspec(dllexport) MyExportedFunction( );
  规则 DLL 内的所有内存分配都应在该 DLL 内进行；
  DLL 不应向调用可执行文件传递或从调用可执行文件接收下列任何指针：
  AFX_MANAGE_STATE宏不应当用于静态链接到 MFC 的规则 DLL 中，也不应当用于扩展 DLL 中
  
# 其他类型的MFC DLL
# MFC 扩展 DLL (Extension DLLs)
  是通常实现从现有 Microsoft 基础类库类派生的可重用类的 DLL。
# 静态链接到 MFC 的规则 DLL(Regular DLLs Statically Linked to MFC)  
  客户端可执行文件可以用任何支持使用 DLL 的语言（C、C++、Pascal、Visual Basic 等）编写；它不必是 MFC 应用程序。
  DLL 可以链接到由应用程序使用的同一 MFC 静态链接库。 已不再有单独用于 DLL 的静态链接库版本。
  这类 DLL 必须实例化从 CWinApp 派生的类。
  DLL 的 CWinApp 对象没有主消息泵
  如果 DLL 打开无模式对话框或有自己的主框架窗口，则应用程序的主消息泵必须调用由 DLL 导出的例程，而该例程应会反过来调用 DLL 应用程序对象的 CWinApp::PreTranslateMessage 成员函数。
  符号通常是通过标准 C 接口从规则 DLL 导出的。 从规则 DLL 导出的函数的声明形如：
  extern "C" __declspec(dllexport) MyExportedFunction( );
  规则 DLL 内的所有内存分配都应在该 DLL 内进行；DLL 不应向调用可执行文件传递或从调用可执行文件接收下列任何指针：
  仅当创建了数据副本后，在应用程序和 DLL 之间传递指向 C 运行库所分配的内存的指针才是安全的。 一定不要删除这些指针或调整它们的大小，也不要在没有创建内存副本的情况下使用这些指针。

# 2类规则DLL的差别：
  链接MFC DLL的方式，
  导出函数是否要保存、恢复状态，为什么？？？
  要特别注意dll和主程序之间的内存分配。






  

