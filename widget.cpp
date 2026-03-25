#include "widget.h"
#include "ui_widget.h"
#include "finddialog.h"
#include "replacedialog.h"
#include <QTextEdit>
#include <QTextCursor>
#include <QTextDocument>



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

    // [优化2] 建议初始化时设置窗口标题和默认编码
    this->setWindowTitle("小刘记事本");



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

    // ===== 菜单栏高度设置 =====
    // [第1版代码] 固定高度38，但不同屏幕显示效果不同
    //menuBar->setFixedHeight(38);

    // [第2版代码] 设置菜单栏最小高度40，让菜单栏跟随widget_2高度
    //menuBar->setFixedHeight(40);  // 最小高度40

    //ui->horizontalLayout->insertWidget(0, menuBar);
    //ui->horizontalLayout->setContentsMargins(0, 0, 0, 0);  // 去掉布局的内边距（上下左右都为0）
    //ui->horizontalLayout->setSpacing(0);   // 去掉控件之间的间距
    // ===== 菜单栏大小 =====
    menuBar->setMinimumHeight(40);  // 最小高度 → 改这个数字
    menuBar->setMinimumWidth(200);  // 最小宽度 → 加这个可以控制宽度

    // ===== 菜单栏位置 =====
    ui->horizontalLayout->insertWidget(0, menuBar);  // 0=最左边，改成1=第二个位置

    // ===== 布局设置（影响所有控件） =====
    ui->horizontalLayout->setContentsMargins(0, 0, 0, 0);  // 内边距：上 右 下 左
    ui->horizontalLayout->setSpacing(0);                      // 控件之间的间距

    // [重点记] addMenu("文件(&F)") 添加菜单，(&F)表示Alt+F快捷键
    QMenu *fileMenu = menuBar->addMenu("文件(&F)");      // 添加"文件"菜单
    QMenu *editMenu = menuBar->addMenu("编辑(&E)");      // 添加"编辑"菜单
    QMenu *helpMenu = menuBar->addMenu("帮助(&H)");      // 添加"帮助"菜单

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
//    connect(aboutAction, &QAction::triggered, ui->textEdit, );

    // [重点记] 在帮助菜单里添加查找和替换
    QAction *findAction = helpMenu->addAction("查找(&F)");
    findAction->setShortcut(QKeySequence("Ctrl+F"));
    connect(findAction, &QAction::triggered, this, &Widget::onFind);

    QAction *replaceAction = helpMenu->addAction("替换(&H)");
    replaceAction->setShortcut(QKeySequence("Ctrl+H"));
    connect(replaceAction, &QAction::triggered, this, &Widget::onReplace);

    // [重点记] 把菜单栏放到widget_2的布局里
    ui->horizontalLayout->insertWidget(0, menuBar);  // 把menuBar插入到horizontalLayout的第0个位置（最左边）

    // ===== 任务2：文件修改检测 =====
    // [重点记] 监听textChanged信号，内容变化时标题加*
    connect(ui->textEdit, &QTextEdit::textChanged, this, &Widget::onTextChanged);
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
    // *[优化4] 这行返回值未使用，属于无意义代码，建议删除

    if(file.isOpen()){

        ui->textEdit->clear();
        //ui->textEdit   指访问ui指向的对象里的textEdit成员

        //移动光标到文件开头
        file.seek(0);

        //先移动光标再读取
        // 文件就像磁带，读到一半想重新听，得倒带回去
        QTextStream read(&file);

        //QString c_str = ui->comboBox->currentText();
        //const char *code = c_str.toStdString().c_str();
        //read.setCodec(ui->comboBox->currentText().toStdString().c_str());

        // *[优化5] setCodec在Qt6已废弃，Qt6使用setEncoding()
        // *[优化6] toStdString().c_str() 的生命周期问题:
        // c_str()返回的指针在临时string对象销毁后可能无效
        // 应该先保存std::string对象:
        auto codec = ui->comboBox->currentText().toStdString();
        // read.setCodec(codec.c_str());
        read.setCodec(codec.c_str());

        //QString text = read.readLine();
        //ui->textEdit->setText(text);

//        while(!read.atEnd()){
//            QString text = read.readLine();
//            //ui->textEdit->setText(text);
//            ui->textEdit->append(text);
//        }
        // *[优化7] 被注释掉的 readAll() 方式更简洁，可以考虑使用
        QString allText = read.readAll();
        ui->textEdit->setText(allText);

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

            // *[优化13] readAll() 比逐行readLine + append更高效
            // 对于大文件，逐行append会频繁触发UI刷新
            //QString text = read.readLine();
            //ui->textEdit->setText(text);
//            while(!read.atEnd()){
//                QString text = read.readLine();
//                //ui->textEdit->setText(text);
//                ui->textEdit->append(text);
//            }
            QString allText = read.readAll();
            ui->textEdit->setText(allText);

            // 读取完后重新连接textChanged信号
            connect(ui->textEdit, &QTextEdit::textChanged, this, &Widget::onTextChanged);

            // *[优化14] 不关闭文件，保持打开状态
            // 这样点保存时file.isOpen()是true，直接覆盖保存，不用再弹窗选文件
            //file.close();
        }
    }
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
    qDebug() << "第" << cursor.blockNumber() + 1 << "行" << "第" << cursor.columnNumber() + 1 << "列";
    // *[优化24] qDebug可删除，只保留UI显示即可
    QString blockNum = QString::number(cursor.blockNumber() + 1);
    QString columnNum = QString::number(cursor.columnNumber() + 1);
    const QString cursorText = "第"+blockNum+"行"+"第"+columnNum+"列";
    ui->horAndver->setText(cursorText);
    // [优化25] 字符串拼接用 QStringLiteral 或 tr() 包裹，便于后续国际化
    // [优化26] 可以用arg()方法更清晰:
    // QString cursorText = QString("第%1行 第%2列").arg(cursor.blockNumber()+1).arg(cursor.columnNumber()+1);
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
void Widget::onFind()
{
    static FindDialog *findDialog = nullptr;  // 静态变量，保持对话框状态

    if(!findDialog){
        findDialog = new FindDialog(this);

        // [重点记] 查找下一个：高亮所有匹配 + 光标跳到下一个
        connect(findDialog, &QDialog::accepted, this, [this](){
            QString searchText = findDialog->getSearchText();
            if(searchText.isEmpty()) return;

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


