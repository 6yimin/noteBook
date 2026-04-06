#include "widget.h"
#include "ui_widget.h"
#include "finddialog.h"
#include "replacedialog.h"
#include <QTextEdit>
#include <QTextCursor>
#include <QTextDocument>
#include <QListWidget>  // 列表控件（用于目录）
#include <QFontComboBox>
#include <QApplication>  // 新增：获取系统DPI/字体缩放
#include <QScreen>       // 新增：获取屏幕DPI


//这是 Widget 类里面的 Widget 函数
//类  :: 函数名，和类名相同，所以它是构造函数 (  (QWidget *parent)参数：一个叫parent的指针，指向QWidget )
Widget::Widget(QWidget *parent)
    //构造函数参数是QWidget *parent
    //这是Qt的规定。所有QWidget的子类构造函数都要接收一个 QWidget *parent 参数。
    //作用：告诉Qt"这个窗口是谁的子窗口"。
    // 比如你创建一个对话框，指定主窗口为父窗口
    //QDialog *dialog = new QDialog(this);  // this就是父窗口
    : QWidget(parent)   // 冒号 = 初始化列表，意思：先调用父类的构造函数，把parent传给它
    , ui(new Ui::widget)// 逗号继续：new创建一个Ui对象，地址存到ui里
    //头文件有 Ui::widget *ui; 不用写 Ui::widget *ui = new Ui::widget;

    //后面有括号差别就是
    //  Student *s = new Student;     // 调用无参构造函数
    //  Student *s = new Student();   // 调用无参构造函数
    //  , m_file()          // ③ 逗号分隔，第三个初始化项（假设有的话）
    //  , m_count(0)        // ④ 逗号分隔，第四个初始化项
    // 必须在初始化列表的：
    // 1. 父类（继承的类）
    // 2. const 成员
    // 3. 引用成员 &
{
    ui->setupUi(this);
    this->setLayout(ui->verticalLayout);
    ui->widget_3->setLayout(ui->horizontalLayout_2);//作用：把 horizontalLayout_2 这个布局设置给 widget_3
    ui->widget_2->setLayout(ui->horizontalLayout);
    //connect(ui->textEdit,SIGNAL(cursorPositionChanged()),this,SLOT(on_CursorPositionChanged()));
    // [优化1] 信号槽连接方式: 建议用新语法(函数指针)替代旧式SIGNAL/SLOT宏
    // 旧式:
    //connect(ui->textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(on_CursorPositionChanged()));
    // 新式:
    connect(ui->textEdit, &QTextEdit::cursorPositionChanged, this, &Widget::on_CursorPositionChanged);
    // 优点: 编译期检查参数类型，避免运行时才发现连接失败

    // [优化2] 设置窗口标题和初始大小
    this->setWindowTitle("小刘记事本");
    this->resize(800, 1200);  // 设置窗口初始大小：宽800，高1200

    // [重点记] 设置 textEdit 初始字体大小为12
    QFont font = ui->textEdit->font();
    font.setPointSize(12);
    ui->textEdit->setFont(font);



    // ============================================================
    // 【任务1】菜单栏 + 快捷键
    // 学习点: QMenuBar, QAction, QShortcut
    // 目标:   文件(F): 新建(Ctrl+N) / 打开(Ctrl+O) / 保存(Ctrl+S) / 另存为 / 退出
    //         编辑(E): 撤销(Ctrl+Z) / 重做(Ctrl+Y) / 剪切 / 复制 / 粘贴 / 全选
    //         帮助(H): 关于
    // 实现步骤:
    //   1. 构造函数中: menuBar()->addMenu("文件(&F)") 创建菜单
    //   2. menu->addAction("打开(&O)", this, SLOT(on_btnopenfile_clicked())) 绑定
    //   3. QAction自带 setShortcut(QKeySequence("Ctrl+O")) 设置快捷键
    // ============================================================
    //    我想做一个菜单栏
    //        ↓
    //    查手册：QMenuBar 怎么用？
    //        ↓
    //    手册说：menuBar->addMenu() 可以添加菜单
    //        ↓
    //    我写：menuBar->addMenu("文件")
    
    //    我想在菜单里加一个"打开"选项
    //        ↓
    //    查手册：QMenu 怎么加菜单项？
    //        ↓
    //    手册说：menu->addAction() 可以添加动作
    //        ↓
    //    我写：fileMenu->addAction("打开")

    // [重点记] QMenuBar创建菜单栏, QMenu创建下拉菜单, QAction创建菜单项
    QMenuBar *menuBar = new QMenuBar();                  // 创建菜单栏（不写this，后面手动放到布局里）

//    // 1. 强制固定菜单栏高度为40px（满足你最初的需求）
//    int menuHeight = 40;
//    menuBar->setFixedHeight(menuHeight);  // 固定高度，不再留余量
//    ui->widget_2->setFixedHeight(menuHeight); // 容器高度和菜单栏完全一致

//    // 2. 字体大小固定为14px（视觉舒适，也可根据需要调整为15/16）
//    // 若要适配分辨率，可改为：int menuFontSize = qRound(14 * QApplication::primaryScreen()->devicePixelRatioF());
//    int menuFontSize = 14;

//    // 3. 优化样式表：高度占满+字体放大+垂直居中
//    QString menuStyle = QString(R"(
//        /* 菜单栏：高度占满+背景+边框 */
//        QMenuBar {
//            font-size: %1px;       /* 放大字体，视觉清晰 */
//            background-color: #f8f8f8;
//            border: 1px solid #e0e0e0;
//            spacing: 0px;          /* 去掉菜单项之间的默认间距 */
//        }
//        /* 一级菜单项（文件/编辑/帮助）：高度占满+垂直居中+足够内边距 */
//        QMenuBar::item {
//            padding: 0 20px;       /* 加大左右内边距，点击区域更宽 */
//            height: %2px;          /* 和菜单栏高度完全一致 */
//            align-items: center;   /* 文字垂直居中 */
//            display: flex;         /* 开启flex布局，保证垂直居中生效 */
//            justify-content: center; /* 文字水平居中 */
//        }
//        QMenuBar::item:selected {
//            background-color: #e8e8e8;
//            border-radius: 2px;
//        }
//        /* 下拉菜单：字体和一级菜单一致，避免过小 */
//        QMenu {
//            font-size: %1px;
//            padding: 4px 0;
//            min-width: 120px;
//            border: 1px solid #e0e0e0;
//        }
//        /* 下拉菜单项：加大内边距，文字不挤 */
//        QMenu::item {
//            padding: 8px 25px;     /* 上下8px，左右25px，视觉更舒适 */
//            min-height: 30px;      /* 下拉项高度加大 */
//        }
//        QMenu::item:selected {
//            background-color: #e8e8e8;
//        }
//    )").arg(menuFontSize)    // 字体大小（14px）
//      .arg(menuHeight);      // 菜单栏高度（40px）

//    menuBar->setStyleSheet(menuStyle);

    // 4. 布局：强制菜单栏占满widget_2的宽度和高度
//    ui->horizontalLayout->insertWidget(0, menuBar);
//    ui->horizontalLayout->setContentsMargins(0, 0, 0, 0);
//    ui->horizontalLayout->setSpacing(0); // 去掉布局内边距
//    ui->horizontalLayout->setStretch(0, 1); // 让菜单栏占满布局宽度

    // 替换原来的 setStyleSheet
    menuBar->setStyleSheet(
        "QMenuBar { min-height: 40px; max-height: 40px; } "  // 菜单栏高度
        "QMenuBar::item { padding: 0px 3px; font-size: 14px; }" // 菜单项：上下 0px 左右边距0px，字号 14
    );

    // ===== 菜单栏高度设置 =====
    // [第1版代码] 固定高度38，但不同屏幕显示效果不同
    //menuBar->setFixedHeight(38);

    // [第2版代码] 设置菜单栏固定高度40，和widget_2一致
    //menuBar->setStyleSheet("QMenuBar { min-height: 40px; max-height: 40px; }");
    //menuBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    //menuBar->setFixedHeight(40);  // 固定高度40，和widget_2一样
    //ui->widget_2->setFixedHeight(40);  // widget_2也固定40

    // ===== 菜单栏位置 =====
    // [重点记] 把菜单栏放到widget_2的布局里
    ui->horizontalLayout->insertWidget(0, menuBar);  // 把menuBar插入到horizontalLayout的第0个位置（最左边）
    //ui->horizontalLayout->addWidget(menuBar);  // 0=最左边，改成1=第二个位置

    // ===== 布局设置（影响所有控件） =====
    //ui->horizontalLayout->setContentsMargins(0, 0, 0, 0);  // 内边距：上 右 下 左
    //ui->horizontalLayout->setSpacing(0);                      // 控件之间的间距

    // [重点记] addMenu("文件(&F)") 添加菜单，(&F)表示Alt+F快捷键
    QMenu *fileMenu = menuBar->addMenu("文件(&F)");      // 添加"文件"菜单
    QMenu *editMenu = menuBar->addMenu("编辑(&E)");      // 添加"编辑"菜单
    QMenu *helpMenu = menuBar->addMenu("帮助(&H)");      // 添加"帮助"菜单

    //fileMenu->setFixedHeight(40);
    //editMenu->setFixedHeight(40);
    //helpMenu->setFixedHeight(40);

    // ===== 文件菜单 =====
    // [重点记] 三步走：1.创建菜单项 2.设快捷键 3.绑定函数
    QAction *openAction = fileMenu->addAction("打开(&O)");           // 创建"打开"菜单项
    openAction->setShortcut(QKeySequence("Ctrl+O"));                // 设置快捷键Ctrl+O
    connect(openAction, &QAction::triggered, this, &Widget::on_btnopenfile_clicked);  // 点击时调用打开函数

    QAction *saveAction = fileMenu->addAction("保存(&S)");
    saveAction->setShortcut(QKeySequence("Ctrl+S"));
    connect(saveAction, &QAction::triggered, this, &Widget::on_btnsavefile_clicked);

    // ===== 编辑菜单 =====
    // [重点记] textEdit自带undo/redo/cut/copy/paste/selectAll，直接绑定
    QAction *undoAction = editMenu->addAction("撤销(&Z)");
    undoAction->setShortcut(QKeySequence("Ctrl+Z"));
    connect(undoAction, &QAction::triggered, ui->textEdit, &QTextEdit::undo);  // 直接调用textEdit的undo

    QAction *redoAction = editMenu->addAction("重做(&Y)");
    redoAction->setShortcut(QKeySequence("Ctrl+Y"));
    connect(redoAction, &QAction::triggered, ui->textEdit, &QTextEdit::redo);

    QAction *cutAction = editMenu->addAction("剪切(&X)");
    cutAction->setShortcut(QKeySequence("Ctrl+X"));
    connect(cutAction, &QAction::triggered, ui->textEdit, &QTextEdit::cut);

    QAction *copyAction = editMenu->addAction("复制(&C)");
    copyAction->setShortcut(QKeySequence("Ctrl+C"));
    connect(copyAction, &QAction::triggered, ui->textEdit, &QTextEdit::copy);

    QAction *pasteAction = editMenu->addAction("粘贴(&V)");
    pasteAction->setShortcut(QKeySequence("Ctrl+V"));
    connect(pasteAction, &QAction::triggered, ui->textEdit, &QTextEdit::paste);

    QAction *selectAllAction = editMenu->addAction("全选(&A)");
    selectAllAction->setShortcut(QKeySequence("Ctrl+A"));
    connect(selectAllAction, &QAction::triggered, ui->textEdit, &QTextEdit::selectAll);

    // ===== 帮助菜单 =====
    QAction *aboutAction = helpMenu->addAction("关于");
    aboutAction->setShortcut(QKeySequence("Ctrl+T"));
    //connect(aboutAction, &QAction::triggered, ui->textEdit, );

    // [重点记] 在帮助菜单里添加查找和替换
    QAction *findAction = helpMenu->addAction("查找(&F)");
    findAction->setShortcut(QKeySequence("Ctrl+F"));
    connect(findAction, &QAction::triggered, this, &Widget::onFind);

    QAction *replaceAction = helpMenu->addAction("替换(&H)");
    replaceAction->setShortcut(QKeySequence("Ctrl+H"));
    connect(replaceAction, &QAction::triggered, this, &Widget::onReplace);

    // ===== 自动换行 =====
    QAction *wordWrapAction = editMenu->addAction("自动换行(&W)");
    wordWrapAction->setShortcut(QKeySequence("Ctrl+L"));  // 快捷键改为Ctrl+L
    wordWrapAction->setCheckable(true);   // 设置为可勾选
    wordWrapAction->setChecked(true);     // 默认勾选（开启自动换行）

    connect(wordWrapAction, &QAction::toggled, this, [this](bool checked){
        ui->textEdit->setWordWrapMode(checked ? QTextOption::WrapAtWordBoundaryOrAnywhere : QTextOption::NoWrap);
    });  // [重点记] 注意：lambda 结尾是 }); 不是 }


    // ===== 任务2：文件修改检测 =====
    // [重点记] 监听textChanged信号，内容变化时标题加*
    connect(ui->textEdit, &QTextEdit::textChanged, this, &Widget::onTextChanged);




    // ===== 字体大小子菜单 =====
    QMenu *fontSizeMenu = editMenu->addMenu("字体大小");  // 添加子菜单

    // 添加几个固定大小选项
    QAction *smallFont = fontSizeMenu->addAction("小 (12号)");
    QAction *mediumFont = fontSizeMenu->addAction("中 (16号)");
    QAction *largeFont = fontSizeMenu->addAction("大 (20号)");
    QAction *xlargeFont = fontSizeMenu->addAction("特大 (24号)");

    // 设置为可勾选
    smallFont->setCheckable(true);
    mediumFont->setCheckable(true);
    largeFont->setCheckable(true);
    xlargeFont->setCheckable(true);

    // 默认选中"小 (12号)"，和 textEdit 初始字体一致
    smallFont->setChecked(true);

    // [重点记] 使用QActionGroup让它们互斥（只能选一个）
    // 创建互斥组
    // QActionGroup *group = new QActionGroup(this);
    // group->addAction(action1);
    // group->addAction(action2);
    QActionGroup *fontGroup = new QActionGroup(this);
    fontGroup->addAction(smallFont);
    fontGroup->addAction(mediumFont);
    fontGroup->addAction(largeFont);
    fontGroup->addAction(xlargeFont);



    // 不需要写任何互斥代码
    // Qt 内部自动处理：点 action1 → action2 取消选中


    // 连接信号 - 点击时调整字体大小
    // [重点记] 调整字体前先清除高亮，防止崩溃
    connect(smallFont, &QAction::triggered, this, [this](){
        ui->textEdit->setExtraSelections({});  // 清除高亮
        QFont font = ui->textEdit->font();
        font.setPointSize(12);
        ui->textEdit->setFont(font);
    });

    connect(mediumFont, &QAction::triggered, this, [this](){
        ui->textEdit->setExtraSelections({});  // 清除高亮
        QFont font = ui->textEdit->font();
        font.setPointSize(16);
        ui->textEdit->setFont(font);
    });

    connect(largeFont, &QAction::triggered, this, [this](){
        ui->textEdit->setExtraSelections({});  // 清除高亮
        QFont font = ui->textEdit->font();
        font.setPointSize(20);
        ui->textEdit->setFont(font);
    });

    connect(xlargeFont, &QAction::triggered, this, [this](){
        ui->textEdit->setExtraSelections({});  // 清除高亮
        QFont font = ui->textEdit->font();
        font.setPointSize(24);
        ui->textEdit->setFont(font);
    });

    // ===== 分页快捷键 =====
    // [重点记] 用QShortcut而不是keyPressEvent，避免被QTextEdit覆盖
    QShortcut *nextPageShortcut = new QShortcut(QKeySequence("Ctrl+Down"), this);
    QShortcut *prevPageShortcut = new QShortcut(QKeySequence("Ctrl+Up"), this);
    QShortcut *nextPageShortcut2 = new QShortcut(QKeySequence(Qt::Key_PageDown), this);
    QShortcut *prevPageShortcut2 = new QShortcut(QKeySequence(Qt::Key_PageUp), this);

    // [重点记] 根据模式调用不同函数：章节模式调用 nextChapter，普通模式调用 nextPage
    connect(nextPageShortcut, &QShortcut::activated, this, [this](){
        if(m_chapterMode) nextChapter(); else nextPage();
    });
    connect(prevPageShortcut, &QShortcut::activated, this, [this](){
        if(m_chapterMode) prevChapter(); else prevPage();
    });
    connect(nextPageShortcut2, &QShortcut::activated, this, [this](){
        if(m_chapterMode) nextChapter(); else nextPage();
    });
    connect(prevPageShortcut2, &QShortcut::activated, this, [this](){
        if(m_chapterMode) prevChapter(); else prevPage();
    });

    // ===== 目录按钮（放在底部状态栏左边） =====
    QPushButton *tocButton = new QPushButton("目录", this);
    tocButton->setMinimumSize(60, 30);
    ui->horizontalLayout_2->insertWidget(0, tocButton);  // 插入到第0个位置（最左边）
    connect(tocButton, &QPushButton::clicked, this, &Widget::showChapterMenu);

    QMenu *themeMeum = menuBar->addMenu("主题(&T)");
    QAction *normalAction = themeMeum->addAction("普通模式");
    QAction *eyeAction = themeMeum->addAction("护眼模式");
    QAction *darkAction = themeMeum->addAction("深色模式");

//    connect(normalAction,&QAction::triggered,this,[this](){ setTheme(0); });
//    connect(eyeAction,&QAction::triggered,this,[this](){ setTheme(1); });
//    connect(darkAction,&QAction::triggered,this,[this](){ setTheme(2); });
    connect(normalAction,&QAction::triggered,this,[this](){ ui->theme_comboBox->setCurrentIndex(0); });
    connect(eyeAction,&QAction::triggered,this,[this](){ ui->theme_comboBox->setCurrentIndex(1); });
    connect(darkAction,&QAction::triggered,this,[this](){ ui->theme_comboBox->setCurrentIndex(2); });

    QFontComboBox *fontbox = new QFontComboBox(this);
    fontbox->setFixedWidth(160);
    fontbox->setFixedHeight(40);
    ui->horizontalLayout->addWidget(fontbox);
                                   //currentFontChanged(const QFont &font);
    connect(fontbox, &QFontComboBox::currentFontChanged, this, [this](const QFont &font){
        // 用户一下拉选择新字体，这个 lambda 就会立刻执行
        QFont newFont = font;
        int currentSize = ui->textEdit->font().pointSize();
        if(currentSize <= 0) currentSize = 12;
        newFont.setPointSize(currentSize);

        // font 参数就是用户刚刚选中的那个字体
        ui->textEdit->setFont(newFont);
    });


}

// ===== onTextChanged：文本变化时触发 =====
void Widget::onTextChanged()
{
    // 如果标题还没有*号，就加上
    if(!this->windowTitle().startsWith("*")){
        this->setWindowTitle("*" + this->windowTitle());
    }
}


// ===== closeEvent：关闭窗口时触发 =====
void Widget::closeEvent(QCloseEvent *event)
{
    // 判断标题是否以*开头（表示有未保存的修改）
    if(this->windowTitle().startsWith("*")){
        int ret = QMessageBox::warning(this, "提示",
                                       "当前文档未保存，是否保存？",
                                       QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                                       QMessageBox::Save);
        if(ret == QMessageBox::Save){
            on_btnsavefile_clicked();  // 保存
            // 点击"打开"时，调用你已有的 on_btnopenfile_clicked 函数
            event->accept();           // 允许关闭
        } else if(ret == QMessageBox::Discard){
            event->accept();           // 不保存，直接关闭
        } else {
            event->ignore();           // 取消，不关闭
        }
    } else {
        event->accept();               // 没有修改，直接关闭
    }
}



Widget::~Widget()
{
    delete ui;
    // *[优化3] 析构时应确保文件正确关闭，避免资源泄漏
    if(file.isOpen()){
        file.close();
    }
}

//命名规律：on_控件名_信号名 是Qt的自动连接规则，不用手动写connect。
//combobox 组合框
void Widget::on_comboBox_currentIndexChanged(int index)
{
    // 打印：用户选了第几个选项（调试用）
    qDebug() << index << ui->comboBox->currentText();

    if(file.isOpen()){
        // 移动光标到文件开头
        file.seek(0);

        // 用新编码重新读取
        QTextStream read(&file);
        auto codec = ui->comboBox->currentText().toStdString();
        read.setCodec(codec.c_str());

        // [重点记] 使用分页加载，避免大文件卡顿
        // 读取所有行存储到 m_allLines
        QString allText = read.readAll();
        m_allLines = allText.split("\n");

        // 显示当前页（保持当前页码不变）
        showPage(m_currentPage);
    }
}


void Widget::on_btnopenfile_clicked()
{
    //std::cout<< "UIbtuopenfileClicked"<<std::endl;
    // [优化8] 被注释掉的调试代码应该删除，保持代码整洁

    // 之前的问题：
    //QFileDialog qFileDialog;
    // 创建空对话框
    //QStringList files = getOpenFileNames(...);
    // 用户在这选了文件
    //qFileDialog.exec();
    // 又弹出另一个空对话框
    //qFileDialog.selectedFiles();
    // *[优化9] 用静态方法更简洁: QFileDialog::getOpenFileNames()
    // 第三个参数""是默认路径，空表示用上次打开的目录，也可填"D:/我的文档"固定路径
    QStringList files = QFileDialog::getOpenFileNames(this, "打开文件", "D:/Qt/code/noteBook", "文本文件 (*.txt)");

    if(!files.isEmpty()){
        // 遍历用户选的每个文件
        for(QString str : files){
            qDebug() << str;

            // [重点记] 先断开textChanged信号，避免clear和setText触发onTextChanged导致标题加*
            disconnect(ui->textEdit, &QTextEdit::textChanged, this, &Widget::onTextChanged);

            ui->textEdit->clear();
            // [优化10] 多文件打开逻辑Bug: 循环内每次都clear，只有最后一个文件内容会显示
            // 如果要支持多文件，应该使用 QTabWidget 或 每次打开新窗口
            // 如果只需要打开单个文件，应使用 getOpenFileName (单数)

            //QFile file(str);
            // 打开文件前先关闭之前的文件
            if(file.isOpen()){
                file.close();
            }

            //告诉QFile要操作哪个文件（只是设置名字，还没打开）
            file.setFileName(str);
            if(!file.open(QIODevice::ReadWrite | QIODevice::Text)){
                qDebug() << "open file name error!" << endl;
                // *[优化11] 打开失败应给用户提示，不只是qDebug
                QMessageBox::warning(this, "错误", "无法打开文件: " + str);
                // *[优化12] 失败后应continue跳过，而不是继续执行
                continue;
            }

            this->setWindowTitle(str+"-小刘记事本");

            QTextStream read(&file);
            //QString openLoad = ui->comboBox->currentText();
            //const char *code = openLoad.toStdString().c_str();
            //read.setCodec(code);
            auto openLoad = ui->comboBox->currentText().toStdString();
            read.setCodec(openLoad.c_str());

            // *[优化13] 使用分页加载，避免大文件卡顿
            // 读取所有行存储到 m_allLines
            QString allText = read.readAll();
            m_allLines = allText.split("\n");

            // [重点记] 解析章节
            parseChapters();

            if(m_chapterMode) {
                // 章节模式：显示第一章
                showChapter(0);
            } else {
                // 普通模式：显示第一页
                m_currentPage = 0;
                showPage(0);
            }

            // 读取完后重新连接textChanged信号
            connect(ui->textEdit, &QTextEdit::textChanged, this, &Widget::onTextChanged);

            // *[优化14] 不关闭文件，保持打开状态
            // 这样点保存时file.isOpen()是true，直接覆盖保存，不用再弹窗选文件
            //file.close();
        }
    }



//        // ...打开文件...

//        // 用 m_themeMode 恢复主题
//        setTheme(m_themeMode);


}


// *[优化] 这个include应该放到文件顶部，不要分散在函数中间
void Widget::on_btnsavefile_clicked()
{
    if(!file.isOpen()){
        QString fileName = QFileDialog::getSaveFileName(this,tr("Save file"),
                                                        //"D:/Qt/code/SigSlot",
                                                        // [优化15] 硬编码路径"D:/Qt/code/SigSlot"不灵活
                                                        // 应使用 QDir::homePath()或QApplication::applicationDirPath(),
                                                        QDir::homePath(),
                                                        tr("Text (*.txt *.doc)"));
        qDebug() << fileName;
        if(fileName.isEmpty()){
            qDebug() << "用户取消了文件保存!";
            return;
        }
        //QFile file(fileName);
        file.setFileName(fileName);
        this->setWindowTitle(fileName+"-小刘记事本");


    }

    // *先关闭已有文件（避免模式冲突）
    if(file.isOpen()){
        file.close();
    }

    // 强制以「覆盖模式」打开：WriteOnly(只写) + Text(文本) + Truncate(清空原有内容) | QIODevice::Truncate
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)){
        qDebug() << "open file error!" << endl;
        // *增加提示，更友好
        QMessageBox::warning(this, "错误", "文件保存失败！");
        // *[优化16] 保存模式应使用 WriteOnly，更符合语义
        // *[优化17] 失败后应return，避免继续写
        return;
    }

    QTextStream contextOfSave(&file);
    auto load = ui->comboBox->currentText().toStdString();
    contextOfSave.setCodec(load.c_str());
    // *[优化18] toStdString().c_str()生命周期问题，同优化6
    QString context = ui->textEdit->toPlainText();
    contextOfSave << context;

    //file.close();
    // *[优化19] 保存后应 flush 确保数据写入磁盘
    //不调用 flush：数据可能还在缓冲区，没真正写到硬盘。如果程序崩溃或断电，数据就丢了。
    file.flush();
    // *[优化20] 保存成功可给用户反馈
    QMessageBox::information(this, "提示", "文件保存成功！");
    qDebug() << "文件保存成功!";

    // [重点记] 保存成功后去掉标题的*号
    QString title = this->windowTitle();
    if(title.startsWith("*")){
        this->setWindowTitle(title.mid(1));  // mid(1)去掉第一个字符*
        //例:QString mid(int position) const;   //从第 position 个位置开始,截取到字符串最后
        //字符位置：0 1  2  3  4
        //        * 未 保 存
        //mid(n) = 去掉前 n 个字符
    }
}


void Widget::on_btnclosefile_clicked()
{
//    QMessageBox::StandardButton res = QMessageBox::question(
//                this,
//                "确认清空",
//                "是否清空文本内容?",
//                QMessageBox::Yes | QMessageBox::No,
//                QMessageBox::No
//                );
//    if(res != QMessageBox::Yes){
//        return;
//    }
//    ui->textEdit->clear();
//    // 可选：重置光标位置
//    ui->textEdit->moveCursor(QTextCursor::Start);
// [优化21] 大段注释代码应删除，可用版本控制(Git)来管理历史代码

    QMessageBox msgBox;
    int ret = QMessageBox::warning(this, tr("My Application"),
                                   tr("当前文档未保存\n"
                                      "请问你是否保存?"),
                                   QMessageBox::Save | QMessageBox::Discard
                                   | QMessageBox::Cancel,
                                   QMessageBox::Save);
    switch (ret) {
      case QMessageBox::Save:
        // Save was clicked
        on_btnsavefile_clicked();
        //qDebug() << "文件已保存";
        // *[优化23] 这段代码在Discard和Cancel时也会执行，逻辑有误
        // 应该放在 case QMessageBox::Save 和 case QMessageBox::Discard 中
        if(file.isOpen()){
            file.close();
            ui->textEdit->clear();
            this->setWindowTitle("小刘记事本");
        }
          break;
      case QMessageBox::Discard:
        // Don't Save was clicked
        ui->textEdit->clear();
        if(file.isOpen()){
            file.close();
        }
        this->setWindowTitle("小刘记事本");
        qDebug() << "用户忽略文件";
          break;
      case QMessageBox::Cancel:
        // Cancel was clicked
        qDebug() << "取消关闭文件";
        // *[优化22] Cancel时应该直接return，不应该继续执行后面的清理代码
          return;
      default:
          // should never be reached
          break;
    }

    //ui->textEdit->clear();
}

void Widget::on_CursorPositionChanged()
{
    QTextCursor cursor = ui->textEdit->textCursor();

    // [重点记] 章节模式显示章节信息，分页模式显示页码，普通模式只显示行列
    if(!m_allLines.isEmpty()) {
        int columnNum = cursor.columnNumber() + 1;

        if(m_chapterMode && !m_chapters.isEmpty()) {
            // 章节模式：显示章节信息
            Chapter chapter = m_chapters[m_currentChapter];
            int realLine = chapter.startLine + cursor.blockNumber() + 1;  // 真实行号

            QString text = QString("第%1行 第%2列 | %3 (%4/%5)")
                           .arg(realLine)
                           .arg(columnNum)
                           .arg(chapter.name)           // 章节名
                           .arg(m_currentChapter + 1)   // 当前章节
                           .arg(m_chapters.count());    // 总章节
            ui->horAndver->setText(text);
        } else {
            // 普通分页模式：显示行列+页码
            int realLine = m_currentPage * m_linesPerPage + cursor.blockNumber() + 1;
            int totalPages = (m_allLines.count() + m_linesPerPage - 1) / m_linesPerPage;

            QString text = QString("第%1行 第%2列 | 第%3页/共%4页")
                           .arg(realLine)
                           .arg(columnNum)
                           .arg(m_currentPage + 1)
                           .arg(totalPages);
            ui->horAndver->setText(text);
        }
    } else {
        // 没有文件：只显示行列
        QString text = QString("第%1行 第%2列")
                       .arg(cursor.blockNumber() + 1)
                       .arg(cursor.columnNumber() + 1);
        ui->horAndver->setText(text);
    }
}


// ============================================================
// 【任务2】撤销/重做 + 文件修改检测
// 学习点: QUndoStack, QTextDocument::isModified()
// 目标:   Ctrl+Z撤销, Ctrl+Y重做
//         内容修改时标题栏显示"*"标记
//         关闭窗口时弹出"是否保存"提示
// 实现步骤:
//   1. QTextEdit自带undo/redo，直接 ui->textEdit->undo() 即可
//   2. 监听 textChanged() 信号，修改标题: setWindowTitle("*" + title)
//   3. 重写 closeEvent()，判断 isModified()，弹窗询问是否保存
// ============================================================

    //QUndoStack undo;
    //void QUndoStack::redoTextChanged(const QString &redoText)



// ============================================================
// 【任务3】查找替换
// 学习点: QDialog自定义, QTextCursor, 高亮显示
// 目标:   Ctrl+F 弹出查找框，支持"查找下一个"
//         高亮所有匹配项
//         Ctrl+H 弹出替换框，支持逐个替换/全部替换
// 实现步骤:
//   1. 新建 FindDialog 类(QDialog)，放QLineEdit + QPushButton
//   2. 用 textEdit->find(keyword) 查找下一个
//   3. 用 QTextEdit::ExtraSelection 高亮所有匹配项
// ============================================================


//为什么要分两步？
//因为FindDialog 和 Widget 是两个独立的类，它们不能直接互相访问对方的控件。

//如果直接在按钮点击时做高亮
// 在 FindDialog 里
//connect(findNextBtn, &QPushButton::clicked, this, [this](){
//    // 问题：FindDialog 里没有 ui->textEdit
//    // FindDialog 不知道主窗口的文本框在哪
//    ui->textEdit->find(...);  // ❌ 找不到 ui
//});
//FindDialog 只是一个弹窗，它不知道主窗口有什么控件。

//正确的做法：分工明确
//FindDialog（弹窗）
//  ├── 职责：收集用户输入（搜索文字）
//  ├── 用户点按钮 → 调用 accept() → 关闭弹窗 → 发出 accepted 信号
//  └── 不负责：查找和高亮（它不知道文本框在哪）

//Widget（主窗口）
//  ├── 职责：监听 accepted 信号
//  ├── 收到信号 → 取到用户输入 → 执行查找和高亮
//  └── 负责：实际的查找逻辑（它有 ui->textEdit）
// 因为查找显示高亮是在ui->textEdit里显示出来的
void Widget::onFind()
{
    static FindDialog *findDialog = nullptr;  // 静态变量，保持对话框状态

    if(!findDialog){
        findDialog = new FindDialog(this);

        // [重点记] 查找下一个：高亮所有匹配 + 光标跳到下一个
        //connect(发送者, &发送者类::信号, 接收者, &接收者类::槽函数);
        //前面finddialog.cpp里还有个connect(findNextBtn, &QPushButton::clicked, this, &QDialog::accept);
        //相当于connect(findDialog,&QDialog::accepted,this,&QWidget::onAccept){
        connect(findDialog, &QDialog::accepted, this, [this](){
            QString searchText = findDialog->getSearchText();
            if(searchText.isEmpty()) return;  //跳出 lambda（匿名函数）

            m_lastSearchText = searchText;  // 记住搜索文本

            // 清除之前的高亮（不需要selectAll，那样会选中全部文本）
            ui->textEdit->setExtraSelections({});

            // [重点记] QTextEdit::ExtraSelection 用于高亮文本
            QList<QTextEdit::ExtraSelection> selections;
            QTextCursor cursor = ui->textEdit->textCursor();
            cursor.movePosition(QTextCursor::Start);

            // 查找所有匹配项并高亮
            QTextCharFormat highlightFormat;
            highlightFormat.setBackground(Qt::yellow);  // 黄色高亮

            while(!cursor.isNull() && !cursor.atEnd()){
                cursor = ui->textEdit->document()->find(searchText, cursor,
                    findDialog->isCaseSensitive() ? QTextDocument::FindCaseSensitively : QTextDocument::FindFlags());
                if(!cursor.isNull()){
                    QTextEdit::ExtraSelection selection;
                    selection.cursor = cursor;
                    selection.format = highlightFormat;
                    selections.append(selection);
                }
            }

            ui->textEdit->setExtraSelections(selections);

            // 光标跳到第一个匹配位置
            cursor = ui->textEdit->document()->find(searchText,
                ui->textEdit->textCursor(), QTextDocument::FindBackward);
            if(!cursor.isNull()){
                ui->textEdit->setTextCursor(cursor);
            }
        });
    }

    findDialog->show();
    findDialog->raise();
    findDialog->activateWindow();
}


void Widget::onReplace()
{
    static ReplaceDialog *replaceDialog = nullptr;

    if(!replaceDialog){
        replaceDialog = new ReplaceDialog(this);

        // 逐个替换
        connect(replaceDialog, &QDialog::accepted, this, [this](){
            QString searchText = replaceDialog->getSearchText();
            QString replaceText = replaceDialog->getReplaceText();
            if(searchText.isEmpty()) return;

            // 先尝试查找当前光标位置的匹配
            bool found = ui->textEdit->find(searchText,
                replaceDialog->isCaseSensitive() ? QTextDocument::FindCaseSensitively : QTextDocument::FindFlags());
            if(found){
                // 找到了，替换选中的文本
                QTextCursor cursor = ui->textEdit->textCursor();
                cursor.insertText(replaceText);

                // [重点记] 替换后自动查找下一个
                ui->textEdit->find(searchText,
                    replaceDialog->isCaseSensitive() ? QTextDocument::FindCaseSensitively : QTextDocument::FindFlags());
            }
        });

        // 全部替换
        connect(replaceDialog, &ReplaceDialog::replaceAll, this, [this](){
            QString searchText = replaceDialog->getSearchText();
            QString replaceText = replaceDialog->getReplaceText();
            if(searchText.isEmpty()) return;

            QTextCursor cursor(ui->textEdit->document());
            cursor.movePosition(QTextCursor::Start);

            int count = 0;
            while(!cursor.isNull() && !cursor.atEnd()){
                cursor = ui->textEdit->document()->find(searchText, cursor,
                    replaceDialog->isCaseSensitive() ? QTextDocument::FindCaseSensitively : QTextDocument::FindFlags());
                if(!cursor.isNull()){
                    cursor.insertText(replaceText);
                    count++;
                }
            }

            QMessageBox::information(this, "提示", QString("共替换 %1 处").arg(count));
        });
    }

    replaceDialog->show();
    replaceDialog->raise();
    replaceDialog->activateWindow();
}

// ===== 分页功能实现 =====

// [重点记] 显示指定页的内容
void Widget::showPage(int page)
{
    if(m_allLines.isEmpty()) return;

    int totalPages = (m_allLines.count() + m_linesPerPage - 1) / m_linesPerPage;  // 计算总页数

    // 页码范围检查
    if(page < 0) page = 0;
    if(page >= totalPages) page = totalPages - 1;

    m_currentPage = page;

    // 计算当前页的起始行和结束行
    int start = page * m_linesPerPage;
    int end = start + m_linesPerPage;
    if(end > m_allLines.count()) end = m_allLines.count();

    // 构建当前页的文本
    QString pageText;
    for(int i = start; i < end; i++) {
        pageText += m_allLines[i] + "\n";
    }

    // 断开textChanged信号，避免触发标题加*
    disconnect(ui->textEdit, &QTextEdit::textChanged, this, &Widget::onTextChanged);

    // 显示当前页
    ui->textEdit->setText(pageText);

    // 重新连接textChanged信号
    connect(ui->textEdit, &QTextEdit::textChanged, this, &Widget::onTextChanged);

    // [已删除] 状态栏显示由 on_CursorPositionChanged 统一处理，避免重复显示
}

// [重点记] 下一页
void Widget::nextPage()
{
    showPage(m_currentPage + 1);
}

// [重点记] 上一页
void Widget::prevPage()
{
    showPage(m_currentPage - 1);
}

// ===== 章节分页实现 =====

// [重点记] 解析章节
void Widget::parseChapters()
{
    m_chapters.clear();

    // 正则表达式：匹配章节标题
    // 支持格式：第1章、第一章、Chapter 1、序章、楔子、尾声、5.标题、1. 第一章
    QRegularExpression re("^(第.{1,5}章|第.{1,5}节|Chapter\\s*\\d+|序章|楔子|尾声|\\d+\\.\\s*.+)");

    for(int i = 0; i < m_allLines.count(); i++) {
        QString line = m_allLines[i].trimmed();
        QRegularExpressionMatch match = re.match(line);

        if(match.hasMatch()) {
            // 找到新章节
            Chapter chapter;
            chapter.name = line;           // 章节名
            chapter.startLine = i;         // 起始行
            chapter.endLine = m_allLines.count() - 1;  // 先默认设定该章到文件末尾

            // 更新上一章的结束行
            if(!m_chapters.isEmpty()) {
                m_chapters.last().endLine = i - 1;
            }

            m_chapters.append(chapter);
        }
    }

    // 如果找到了章节，启用章节模式
    if(!m_chapters.isEmpty()) {
        m_chapterMode = true;
        m_currentChapter = 0;
        qDebug() << "找到" << m_chapters.count() << "个章节";
    } else {
        m_chapterMode = false;
        qDebug() << "未找到章节，使用普通分页模式";
    }
}

// [重点记] 显示指定章节
void Widget::showChapter(int index)
{
    if(m_chapters.isEmpty()) return;

    // 页码范围检查
    if(index < 0) index = 0;
    if(index >= m_chapters.count()) index = m_chapters.count() - 1;

    m_currentChapter = index;
    Chapter chapter = m_chapters[index];

    // 构建章节内容
    QString text;
    for(int i = chapter.startLine; i <= chapter.endLine; i++) {
        text += m_allLines[i] + "\n";
    }

    // 断开信号，避免触发标题加*
    disconnect(ui->textEdit, &QTextEdit::textChanged, this, &Widget::onTextChanged);
    ui->textEdit->setText(text);
    connect(ui->textEdit, &QTextEdit::textChanged, this, &Widget::onTextChanged);
}

// [重点记] 下一章
void Widget::nextChapter()
{
    showChapter(m_currentChapter + 1);
}

// [重点记] 上一章
void Widget::prevChapter()
{
    showChapter(m_currentChapter - 1);
}

// [重点记] 显示章节目录（点击目录按钮时触发）
void Widget::showChapterMenu()
{
    if(!m_chapterMode || m_chapters.isEmpty()) {
        //  m_chapterMode = 是否启用章节模式
        //  m_chapters.isEmpty() = 章节列表是否为空
        QMessageBox::information(this, "提示", "当前文件没有章节");
        return;
    }

    // 创建对话框
    QDialog dialog(this);               // 创建对话框，父对象是当前窗口
    dialog.setWindowTitle("章节目录");   // 设置对话框标题
    dialog.resize(500, 800);  // 对话框大小

    // 创建列表控件listWidget（可滚动）
    QListWidget *listWidget = new QListWidget(&dialog);
    listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);  // 始终显示垂直滚动条

    // 添加所有章节到列表（竖排，一行一个）
    for(int i = 0; i < m_chapters.count(); i++) {
        //  m_chapters.count() → 章节总数
        //  循环从第0个到最后一个章节
        listWidget->addItem(m_chapters[i].name);
        //  ↑                         ↑
        // 往列表添加一行          第i个章节的名字
    }

    // 当前章节-高亮显示
    listWidget->setCurrentRow(m_currentChapter);

    // 创建布局
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->addWidget(listWidget);
    dialog.setLayout(layout);

    // 双击章节跳转
    connect(listWidget, &QListWidget::itemDoubleClicked, [&](){
        int index = listWidget->currentRow();
        showChapter(index);
        dialog.accept();  // 关闭对话框
    });

    // 显示对话框
    dialog.exec();
}

//void Widget::setTheme(int mode){
//    m_themeMode = mode;
//    if(mode == 0){
//        ui->textEdit->setStyleSheet("background-color: white; color: black;");
//    }else if(mode == 1){
//        ui->textEdit->setStyleSheet("background-color: rgb(199, 237, 204); color: rgb(0, 0, 0);");
//    }else if(mode == 2){
//        ui->textEdit->setStyleSheet("background-color: rgb(30, 30, 30); color: rgb(200, 200, 200);");
//    }
//}




void Widget::on_theme_comboBox_currentIndexChanged(int index)
{
    m_themeMode = index;
        if(index == 0){
            ui->textEdit->setStyleSheet("background-color: white; color: black;");
        }else if(index == 1){
            ui->textEdit->setStyleSheet("background-color: rgb(199, 237, 204); color: rgb(0, 0, 0);");
        }else if(index == 2){
            ui->textEdit->setStyleSheet("background-color: rgb(30, 30, 30); color: rgb(200, 200, 200);");
        }
}
