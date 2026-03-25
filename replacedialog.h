#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

class ReplaceDialog : public QDialog
{
    Q_OBJECT

public:
    ReplaceDialog(QWidget *parent = nullptr);
    QString getSearchText() const { return searchEdit->text(); }
    QString getReplaceText() const { return replaceEdit->text(); }
    bool isCaseSensitive() const { return caseCheckBox->isChecked(); }

private:
    QLineEdit *searchEdit;      // 查找输入框
    QLineEdit *replaceEdit;     // 替换输入框
    QPushButton *replaceBtn;    // 逐个替换
    QPushButton *replaceAllBtn; // 全部替换
    QPushButton *cancelBtn;     // 取消
    QCheckBox *caseCheckBox;    // 区分大小写
signals:
    void replaceAll();          // 替换功能
};

#endif

