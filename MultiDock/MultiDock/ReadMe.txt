
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

tips:明明注册了文档模板，却没有显示view呢？先查看下你的csv文件中的参数是否正确。
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
TIPS:如果该窗口类想要成为container容器窗口类，需要写自己的AdjustLayout函数：从wndmanager中拿到自己的指针，并看是否有child，并在wm_size消息中调整layout
TIPS:每个窗口资源需要在onClose函数中销毁掉自己的窗口资源。完成父亲窗口列表。注意改对象没有free，会有内存泄露，==>谁创建谁free。

TIPS:对于不同dll和主程序，资源菜单需要分配不同的id号，如果菜单定义相同的id，就会出现诡异的现象。


TODOS
TIPS:如果子窗口点了ok按钮，销毁了，那么wndManager和listctrl里面的都要更新哦,否则选择的父窗口是无效的===to be continued
TIPS:对于每个类，需要增加AdjustLayout函数。读取wndmanager里面的数据，并调整。
TIPS:为了方便记忆和修改，在创建child实例的时候，让用户可以给实例取名。用于记忆。不填写的默认是空白列。因为有时候要修改child的size，这个时候，
	可能会选择错误。
TIPS:在修改config\Modules.csv的工程名的是，修改后的值应该和代码里面的 CSampleViewerManager::RegisterDocTemplate()的最后一个参数名一致，否则
	  加载dll失败！



2015年5月9日15:39:52
todo：通过主界面菜单加载toolbar。
todo：xml文件的保存，和workspace的保存。
todo：adjustlayout在一个dialog里面的添加并完成测试是否可以作为parent添加子对话框。
todo：child窗口是否可以设置为resize style。
todo：是否可以过滤将CMainFrame给过滤掉。不让其作为parent！！！
todo：将子窗口设置为Resize style以后，如果用户鼠标拖拽了子对话框的大小，那么需要将新的rect更新到内存中去。
	  否则，用户在添加新的子对话框的时候，刷新界面，就会出现拖拽没有生效的情况。---经过调研，发现getclientrect和GetWindowREct都不能准确的酸楚child对话框的
	  crect值，即使调用ScreenToClient也不可以。所以还是让用户通过“更新位置”来修改child的尺寸。
todo：在创建自对话框的时候，可以设置child name一栏让用户填写，一是为了在列表中好辨认，另外还可以给对话框设置
	  名称。
todo：为了刷新好看，是否考虑在带有bitmap的对话框中使用内存dc绘制图片。
todo：如果子对话框关闭了，需要告知创建的三个page对话框，进行树形列表更新。删除掉那个关闭的自对话框实例。
2015年5月11日10:24:36
task
1.菜单乱码=done
2.演示一个child对话框嵌套child的实例。
3.对话框关闭了，对应的树列表，更新。===>done!
4.对于父窗口中的NULL和Mainframe是否需要？==>done!:null会缓存，但不显示
5.float窗口的研究，如何float，不停靠。
6.如果是父亲窗口关闭了，那么它下面的所有的child窗口都要关闭！！！===>done,如果父亲窗口关闭了，那么子窗口，全部DestroyWindow，并清除响应的缓存。
7.menu ,toolbar的加载。
8.多文档的sampleviewerview的演示？！
9.增加了菜单和toolbar的加载，对于samplepanel工程，由于加载文档模板失败了，所有的关于samplePanelView的菜单/toolbar的响应函数都不起作用了。
	现在的问题是解决samplePanelViewer的加载成功，将其属性修改为和SampleViewerView一样的：带View的测试工程！！！
	否则Menu的加载就需要独立出来(这个是在LoadDocTemplate时候动态加载的)， toolbar的响应函数应该写入到Mainframe里面去，因为View没有注册成功

2015年5月13日17:02:54 任务列表
1.考虑使用宏注册完成动态创建工作。
2.workspace功能
3.增加“删除窗口”，并显示存活窗口
4.menu 和toolbar，status bar的能独立加载，不跟文档模板绑定。
5.bcg库的依赖。
6.用户管理的逻辑实现。
