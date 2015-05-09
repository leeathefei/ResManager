
/////////////////////////////////////////////////////////////////////////////
2015年4月28日23:49:34
1.exitinstance失败，以为是没有在cdockapp类里面没有加入消息循环，加入后，发现还存在问题。消息循环可以去掉。
并进一步查询问题原因可能是controlbar没有释放？！

2015年4月29日10:39:04
1.将mainframe里面的界面移植到Module1中去。
2.

2015年5月6日23:19:55
1.各个对话框需要将自己注册到wndmanager里面去。每个窗口类在cpp文件中需要定义一个静态全局变量完成注册。
  程序启动的时候，会完成注册。为什么在程序启动，因为，静态函数指针必须由类自己完成。
  wndManager存储了所有的需要创建的资源的名称和创建函数。在真正使用的时候，才会去查询，
2.第一次show出配置框，会解析xml文件的时候，此时需要创建资源。目前只需要知道，窗口类名。不需要知道创建的函数地址，因为上面已经完成注册了。
3.每个窗口类还需要实现CBaseObj中的虚函数，完成窗口的创建。。。
4.xml文件暂时还不支持多次的嵌套。
5.xml文件，如果用户在创建一个类，而这个类的父亲窗口还没有创建，那么就需要创建父窗口！！！需要提示用户先创建父窗口？！runtimeclass？！
6.xml文件的root节点必须为<config>
7.sampleviewer 和 samplepanel的函数名注册，发生在loadlibrary之后。在InitInstance--pMainFrm->LoadMainMenus函数中完成了dll的加载。从而静态函数注册完成。
我们的策略是，对于写在配置文件中的dll，在解析完成后，就全部加载到内存中，从而完成函数名的注册。
8.约定：对于Mainframe+viewer这些系统自带的窗口。我们默认就先创建好。其他的窗口和控件可以随时创建。
9.提供外部接口，类似Init函数，主程序保存dllHInstance。以便随时得到函数process指针，从而调用，来创建相应的窗口。
10.任何CreateWnd函数需要转换到当前的dll资源环境：USE_CUSTOM_RESOURCE(_T("SamplePanel.dll"));

11.策略：因为dll的Viewer和ChildFrame类的特殊性，考虑到和相应的Doc类的绑定，所以在Dll加载的时候，就默认将模板注册起来。
但相应的toolbar和菜单？！不会立即注册。CMainFrame在程序一开始就创建好的。

12.在创建之前，在勾选父窗口的时候，如果父窗口还没创建（常见在dll模块的View和ChildFrame和doc还没注册），
	就提醒用户是否创建父窗口，否则创建基于CMainFrame的窗口。

13.????但有个问题：对于多View界面的窗口，在勾选父窗口的时候，不能通过类名类区分谁可以作为父窗口，应该是实例才对！！
14.我们约定，不同dll里面的frame+view是不允许同名的，否则在dll-classes逻辑判断时会混淆。并且不清楚frame和view是属于哪个dll的资源。
15.对于父窗口的指定，实际上指定的是已经创建好的 窗口实例。那么这个时候，不能用类名来告诉他而应该用树形的结构来指定。important!!!!!\
    树形结构的实例在指定tab的各个page中。创建好一个实例后，需要实时更新这个tree。对于实例的命名，加上引用计数，格式为：memoryaddres(Cclassname)_Index

tips:命名注册了文档模板，却没有显示view呢？先查看下你的csv文件中的参数是否正确。
tips:mainframe提供两种接口
	1.CreateDockWnd用于创建非frame/view的窗口。每个实现了CBaseObj接口的窗口类都需要在重载函数里面调用CXXXmanager::RegisterDockPane对象里面的接口创建自己。
	2.LoadDllByName是创建frame+view（直接调用注册文档模板对象创建框架）

tips:对于创建的窗口，可以监听并受到IObjCreatedEvent回调用于更新父亲窗口列表，但是，对于Frame+View却没有通知机制。需要注意哦！应该在LoadDllByname中完成消息的发布。
tips:每个View类需要在OnCreate函数中将自己的内存地址写入到xml文件共其他控件分享数据。
tips:xml的读取中，设计到递归的地方主要是嵌套。可以对增加节点：<bHasChild>来达到递归的更好的读取。to be continued.===>由于树形结构遍历很复杂，现在简化工作，采用
		listctrl来显示，两列显示：className+hInstance。
TIPS：对于创建的子窗口和float窗口，不要调用默认的CSampleViewerManager::Instance()->RegisterDockPane(this, _T("DlgTest4InSV"),eDockType, TRUE);
	改为调用默认的retrun __super::Create(IDD, pParent);直接返回。否则，会出现奇怪的现象。
TIPS:期间出现一次dynamic cast 到cbaseObj的时候失败了，以为是没有将对话框类导出，其实不需要导出对话框类。不是这个原因。