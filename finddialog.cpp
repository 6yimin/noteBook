#include "finddialog.h"


// 构造函数：创建窗口时执行
FindDialog::FindDialog(QWidget *parent)
    : QDialog(parent)   // 先调用父类构造函数
{
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
    hLayout->addWidget(caseCheckBox);        // 左边放复选框
    hLayout->addStretch();                   // 中间留空
    hLayout->addWidget(findNextBtn);         // 右边放按钮

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
QString FindDialog::getSearchText() const
{
    return searchEdit->text();  // 从输入框取文字
}

// isCaseSensitive：返回是否勾选了区分大小写
bool FindDialog::isCaseSensitive() const
{
    return caseCheckBox->isChecked();  // 返回复选框状态
}
