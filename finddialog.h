#ifndef FINDDIALOG_H
#define FINDDIALOG_H
// 第1步：写保护，防止重复包含

#include <QCheckBox>
#include <QDialog>
#include <QLineEdit>
#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
class FindDialog : public QDialog{
    Q_OBJECT  // Qt的类都要加这个
public:
    //构造函数
    FindDialog(QWidget *parent = nullptr);

    // 下面这些是"给别人用的函数"
    // 别人调用 getSearchText() 就能拿到用户输入的文字
    QString getSearchText() const;
    bool isCaseSensitive() const;

private:
    //这些东西属于这个窗口，不是全局的，不是别的窗口的。
    //所以必须放在 private: 下面，作为窗口的成员变量。
    QLineEdit *searchEdit;     // 输入框指针
    QPushButton *findNextBtn;  // 按钮指针
    QCheckBox *caseCheckBox;   // 复选框指针
    //Qt 控件必须用指针 → 加 *
};





#endif // FINDDIALOG_H
