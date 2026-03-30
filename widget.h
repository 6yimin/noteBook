#ifndef WIDGET_H      // 如果 WIDGET_H 没定义过
#define WIDGET_H      // 那就定义它
// 没有#ifndef保护的话：
// 第一次include：class widget { ... };
// 第二次include：class widget { ... };  // 重复定义！报错！

// 有#ifndef保护的话：
// 第一次include：WIDGET_H没定义，定义它，执行内容
// 第二次include：WIDGET_H已定义了，跳过，不执行

#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QTextStream>
#include <QMenuBar>
#include <QUndoStack>
#include <QWidget>
#include <iostream>
#include <QDebug>
#include <QDialog>
#include <QString>
#include <QFile>
#include <QFileDialog>
#include <QObject>
#include <QMenuBar>  // 菜单栏
#include <QMenu>     // 菜单
#include <QAction>   // 菜单项
#include <QCloseEvent>  // 关闭事件
#include <QTextOption>  // 自动换行模式
#include <QKeyEvent>    // 键盘事件（用于翻页）
#include <QShortcut>    // 快捷键（用于翻页）
// [优化1] 头文件精简: 只保留必要的头文件，其他放到.cpp中
// [优化2] 移除 <iostream>，Qt项目使用 qDebug/qInfo 替代 cout
// [优化3] <QDialog>/<QString>/<QFileDialog>/<QObject> 移到.cpp按需包含
// [优化4] QObject 已被 QWidget 继承，无需重复包含

QT_BEGIN_NAMESPACE
namespace Ui {
    class widget;
}
QT_END_NAMESPACE

class Widget : public QWidget //定义一个叫widget的类,继承QWidget（Qt的窗口基类）
// [优化5] 类名建议: widget -> Widget (Qt风格首字母大写)
{
    Q_OBJECT //Qt的魔法宏，加上它才能用信号槽机制。不加的话 connect 会报错。

public:
    Widget(QWidget *parent = nullptr);// 构造函数，=nullptr表示可以不传参数
                                      // 构造函数参数是QWidget *parent
                                      // 这是Qt的规定。所有QWidget的子类构造函数都要接收一个 QWidget *parent 参数。
                                      // 作用：告诉Qt"这个窗口是谁的子窗口"。
    ~Widget();                        // 析构函数，前面多个~，对象销毁时执行

    QFile file;
    // [优化6] 访问权限: QFile file 应该是 private，外部不应直接操作
    // [优化7] 建议添加文件状态管理成员变量，如 bool m_isModified

private slots:
    void on_btnopenfile_clicked();

    void on_btnsavefile_clicked();

    void on_btnclosefile_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_CursorPositionChanged();

    void onTextChanged();          // 监听文本变化，标题加*
    void onFind();                  // 查找功能
    void onReplace();               // 替换功能

protected:
    void closeEvent(QCloseEvent *event) override;  // 重写关闭事件

private:
    Ui::widget *ui;// 声明一个指针，类型是"Ui命名空间里的widget类" Ui::widget 是指QT的类
                   // 人话：我要在我的类widget里用一个指针，指向Qt生成的UI对象。

    //    你的 widget 类
    //    ┌─────────────────────────┐
    //    │ Ui::widget *ui;   ─────────→  Qt的 Ui::widget 类
    //    │                         │     ┌─────────────────────┐
    //    │on_btnopenfile_clicked() │     │ QTextEdit *textEdit │
    //    │on_btnsavefile_clicked() │     │ QPushButton *btnopen│
    //    │on_btnclosefile_clicked()│     │ QPushButton *btnsave│
    //    │                         │     │ QComboBox *comboBox │
    //    └─────────────────────────┘     └─────────────────────┘
    QString m_lastSearchText;

    // ===== 分页加载 =====
    QStringList m_allLines;      // 存储所有行
    int m_currentPage = 0;       // 当前页码（从0开始）
    int m_linesPerPage = 50;     // 每页显示的行数（可根据字体大小调整）

    void showPage(int page);     // 显示指定页
    void nextPage();             // 下一页
    void prevPage();             // 上一页

signals:
    //void
    // [优化10] signals 区块为空可以删除，不要留注释占位

};

#endif                // 结束 WIDGET_H
