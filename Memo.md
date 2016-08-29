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
  PluginDefine.h：定义类型：结构，模板，自定义消息
  PluginImpl.h: 输出一些c方法。
  PluginLibrary.h: 输出一些c方法。
  PluginClass.h:  是插件的接口？class PLUGIN_EXT_CLASS CPlugin？
  PluginSupport.h: dll的app。class CPluginSupportApp : public CWinApp
  PI****.h: 重写各种控件。class PLUGIN_EXT_CLASS CPIView : public CScrollView

# 再看怎么写1个插件？比如Image插件
  class CImagePlugin : public CPlugin，重写了其中的几个方法，每个插件都要重写这个类型，用来插件管理？？？
  class CImageApp : public CWinApp：创建1个Plugin系统的DocTemplate
  class CImageDocument : public CPIDocument
  class CImageView : public CScrollView，为什么不是CPIView？？？
  
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






  

