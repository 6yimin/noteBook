#include "replacedialog.h"

ReplaceDialog::ReplaceDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("替换");
    setFixedSize(500, 200);  // 改大窗口大小

    searchEdit = new QLineEdit(this);
    searchEdit->setMinimumHeight(35);  // 输入框最小高度

    replaceEdit = new QLineEdit(this);
    replaceEdit->setMinimumHeight(35);  // 输入框最小高度

    replaceBtn = new QPushButton("替换", this);
    replaceBtn->setMinimumSize(80, 40);  // 按钮最小大小

    replaceAllBtn = new QPushButton("全部替换", this);
    replaceAllBtn->setMinimumSize(160, 40);  // 全部替换按钮是替换按钮的两倍

    cancelBtn = new QPushButton("取消", this);
    cancelBtn->setMinimumSize(80, 40);  // 按钮最小大小

    caseCheckBox = new QCheckBox("区分大小写", this);
    caseCheckBox->setMinimumHeight(40);  // 复选框最小高度

    // 布局
    QHBoxLayout *hLayout1 = new QHBoxLayout;
    hLayout1->addWidget(new QLabel("查找内容:", this));
    hLayout1->addWidget(searchEdit);

    QHBoxLayout *hLayout2 = new QHBoxLayout;
    hLayout2->addWidget(new QLabel("替换为:", this));
    hLayout2->addWidget(replaceEdit);

    QHBoxLayout *hLayout3 = new QHBoxLayout;
    hLayout3->addWidget(caseCheckBox);
    hLayout3->addStretch();
    hLayout3->addWidget(replaceBtn);
    hLayout3->addWidget(replaceAllBtn);
    hLayout3->addWidget(cancelBtn);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addLayout(hLayout1);
    vLayout->addLayout(hLayout2);
    vLayout->addLayout(hLayout3);
    setLayout(vLayout);

    // [重点记] 按下回车也能触发查找（替换框默认按钮是替换）
    connect(replaceBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(replaceAllBtn, &QPushButton::clicked, this, &ReplaceDialog::replaceAll);  // 全部替换按钮发出replaceAll信号
}
