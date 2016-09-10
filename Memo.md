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

# 看怎么写1个插件？比如Image插件
  class CImagePlugin : public CPlugin，准备文档模板，提供本插件描述，用来插件管理？==》export
  class CImageApp : public CWinApp：创建1个Plugin系统的DocTemplate
  class CImageDocument : public CPIDocument，继承CPI***, 从框架调用docment==》export
  class CImageView : public CScrollView，为什么不是CPIView？不从框架调用view。
# MEMO: 容器要什么模块，就输出什么模块

# 再看典型1个插件 PluginBrowser
  class CBrowserPlugin : public CPlugin, 添加菜单，提供本插件描述 ==》export
  其他模块都不继承CPI***，因为从菜单调用插件功能。

# 接着看容器怎么加载插件
  搜索插件：void CPluginBrowserDlg::SearchPlugin()，去找app目录下的dll
  动态加载dll：HINSTANCE hInstance = LoadLibrary(strFileName);
  找到c函数：GETINSTANCE pfnGetInstance = (GETINSTANCE)GetProcAddress(hInstance, "GetInstance"); 
  ===》 GetInstance在哪里？
  返回管理接口：CPlugin* pPlugin = pfnGetInstance();
  查询接口描述：pPlugin->Query(*pInfo)
  释放dll：FreeLibrary(hInstance);
  
# 关于插件的c函数：GetInstance
	每个插件都只有这个c函数，用来创建插件实例。
	定义：在PluginDefine.h中，定义了宏来实现GetInstance()。
    调用插件类的Instance()方法。返回插件管理接口CPlugin的类型
		#define IMPLEMENT_PLUGIN(class_name) \
		PLUGIN_EXPORT class_name* GetInstance(){ return class_name::Instance(); }
	实现：在ImagePlugin.cpp里，
		IMPLEMENT_PLUGIN(CImagePlugin)
		class CImagePlugin : public CPlugin
    PLUGIN_EXPORT CImagePlugin* GetInstance(){ return CImagePlugin::Instance(); }
    
    class CImagePlugin : public CPlugin
	   DECLARE_PLUGIN(CImagePlugin)
     #define DECLARE_PLUGIN(class_name) \
	     static class_name* Instance(){ static class_name _instance; return &_instance; } \
# MEMO: 每个插件，只有1个静态的实例？？？

================================================================================
# 容器怎么创建、加载、引用插件？
  在MainFrame里没有找到。 class CMainFrame : public CPIMDIFrameWndEx
  在App里，BOOL CDemoApp::InitInstance()， PIInitPlugin(this); 

# 容器加载插件
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

# 关于容器DocumentTemplate的操作
  1，初始化时，在BOOL CDemoApp::InitInstance()中
  替换插件的文档模板管理器：ReplaceDocManager();
      m_pDocManager = new CPIDocManager;
  注册插件的文档模板：PIRegisterDocTemplates();
      遍历所有插件，取出文档模板
      CDocTemplate* pDocTemplate = pPluginWrapper->GetDocTemplate(j);
      pMainApp->AddDocTemplate(pDocTemplate);  
  2，关闭程序时，CMainFrame::OnClose()
      pDocManager->RemovePluginDocTemplate();
  

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






  

