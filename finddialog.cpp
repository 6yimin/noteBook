#include "finddialog.h"


// 构造函数：创建窗口时执行
FindDialog::FindDialog(QWidget *parent)
    : QDialog(parent)   // 先调用父类构造函数
{
    // 窗口大小
    setWindowTitle("查找");
    setFixedSize(400, 150);  // 改大窗口大小

    // 给输入框指定爸爸 = this（窗口自己）
    searchEdit = new QLineEdit(this);
    searchEdit->setMinimumHeight(30);  // 输入框最小高度

    // 给按钮指定爸爸 = this（窗口自己）
    findNextBtn = new QPushButton("查找下一个", this);
    findNextBtn->setMinimumSize(100, 35);  // 按钮最小大小

    // 给复选框指定爸爸 = this（窗口自己）
    caseCheckBox = new QCheckBox("区分大小写", this);
    caseCheckBox->setMinimumHeight(30);  // 复选框最小高度

    // 第3步：布局（控件放哪里）
    QHBoxLayout *hLayout = new QHBoxLayout;  // 横向布局
    hLayout->addWidget(caseCheckBox);   // 在横向布局里，第1个位置放复选框
    hLayout->addStretch();              // 在横向布局里，第2个位置放"弹簧"（空隙）
    hLayout->addWidget(findNextBtn);    // 在横向布局里，第3个位置放按钮

    QVBoxLayout *vLayout = new QVBoxLayout;  // 纵向布局
    vLayout->addWidget(new QLabel("查找内容:", this));  // 提示文字
    vLayout->addWidget(searchEdit);          // 输入框
    vLayout->addLayout(hLayout);             // 底部横排
    setLayout(vLayout);                      // 设置给窗口

    // 第4步：按钮点击时关闭窗口
    // [重点记] 按下回车也能触发查找
    connect(findNextBtn, &QPushButton::clicked, this, &QDialog::accept);
}

// getSearchText：返回用户输入的文字
//const QString getSearchText();            // const 在前面：返回值是const（只读）
QString FindDialog::getSearchText() const   // const 在后面：函数本身是const（只读）
{
    return searchEdit->text();  // 从输入框取文字
}

// isCaseSensitive：返回是否勾选了区分大小写
bool FindDialog::isCaseSensitive() const
{
    return caseCheckBox->isChecked();  // 返回复选框状态
}


//用一个简单例子解释函数加 const 加哪里
//class Student {
//public:
//    QString name;    // 成员变量：姓名

//    // const 在后面：这个函数不会修改 name
//    QString getName() const {
//        return name;         // 只是返回，不改 name
//    }

//    // 没有 const：这个函数会修改 name
//    void setName(QString n) {
//        name = n;            // 改了 name
//    }
//};

// 后面的 const：函数本身是只读的（常用）
//QString getSearchText() const;   // 我不会修改任何东西

// 前面的 const：返回值是只读的（很少用）
//const QString getSearchText();   // 返回的字符串你不能改

