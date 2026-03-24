# Qt记事本项目 - 学习建议

## 一、当前项目存在的主要问题

### 1. 命名规范
- 类名 `widget` 应改为 `Widget`（Qt风格首字母大写）
- 成员变量建议加 `m_` 前缀，如 `m_file`

### 2. 内存与资源管理
- `QFile file` 放在 public 不安全，应为 private
- 析构函数中未检查文件是否已关闭
- 多文件打开时存在资源泄漏风险

### 3. 信号槽连接
- 使用了旧式 `SIGNAL()/SLOT()` 宏，应改用函数指针新语法
- 新语法有编译期检查，更安全

### 4. 编码处理
- `setCodec()` 在 Qt6 已废弃
- `toStdString().c_str()` 存在临时对象生命周期问题

### 5. 逻辑Bug
- 关闭文件时 `Cancel` 后仍会执行清理代码
- 多文件打开循环每次都 clear，只有最后一个文件显示
- 保存文件失败后未 return，继续执行写入

---

## 二、产品方向优化（面试加分项）

### 菜单栏 + 快捷键
```
文件(F): 新建(Ctrl+N) / 打开(Ctrl+O) / 保存(Ctrl+S) / 另存为
编辑(E): 撤销(Ctrl+Z) / 重做(Ctrl+Y) / 剪切 / 复制 / 粘贴 / 全选
```
实现方式：QMenuBar + QAction，用 connect 绑定快捷键

### 状态栏信息
- 当前行列号（已有）
- 文件编码格式
- 文件大小 / 字数统计
- 文件是否已修改（显示*号）

### 文件修改检测
- 内容变化时标题栏显示 `*` 标记
- 关闭窗口时弹出保存提示

### 查找替换
- Ctrl+F 弹出查找框
- 支持高亮匹配、上一个/下一个

### 多文档支持
- 用 QTabWidget 实现多标签页
- 每个标签页独立管理文件

---

## 三、完善记事本需要学的内容

### 任务1：菜单栏 + 快捷键

| 知识点 | 说明 | 示例代码 |
|--------|------|----------|
| QMenuBar | 菜单栏，窗口顶部那一排 | `menuBar()` 获取 |
| QMenu | 下拉菜单（文件、编辑、帮助） | `menuBar()->addMenu("文件(&F)")` |
| QAction | 菜单项（打开、保存等） | `menu->addAction("打开(&O)")` |
| 快捷键 | 给Action绑定键盘组合 | `action->setShortcut(QKeySequence("Ctrl+O"))` |
| &符号 | 给菜单加Alt快捷键 | "文件(&F)" 表示 Alt+F 打开 |
| 信号槽 | 点击菜单项触发函数 | `connect(action, &QAction::triggered, this, &Widget::onOpen)` |

### 任务2：撤销/重做 + 修改检测

| 知识点 | 说明 | 示例代码 |
|--------|------|----------|
| undo/redo | QTextEdit自带，不需要额外操作 | `ui->textEdit->undo()` |
| textChanged信号 | 文本变化时触发 | `connect(ui->textEdit, &QTextEdit::textChanged, this, &Widget::onTextChanged)` |
| windowModified | Qt窗口的修改标记 | `setWindowModified(true)` |
| closeEvent | 关闭窗口时的回调函数，需要重写 | `void closeEvent(QCloseEvent *event) override` |
| isModified | 判断文档是否被修改过 | `ui->textEdit->document()->isModified()` |

### 任务3：查找替换

| 知识点 | 说明 | 示例代码 |
|--------|------|----------|
| QDialog | 弹出对话框，自定义界面 | 新建FindDialog继承QDialog |
| QLineEdit | 输入框，用户输入查找关键词 | `QLineEdit *searchInput` |
| find() | QTextEdit自带的查找方法 | `ui->textEdit->find(keyword)` |
| ExtraSelection | 给文本加背景色高亮 | `QTextEdit::ExtraSelection selection` |
| QTextCursor | 文本光标，定位到匹配位置 | `selection.cursor = textEdit->textCursor()` |
| QList | 存储多个高亮区域 | `QList<QTextEdit::ExtraSelection> selections` |

### 补充：C++基础（边做边查）

| 知识点 | 什么时候用到 |
|--------|-------------|
| 函数指针 | connect信号槽的新语法 |
| override关键字 | 重写closeEvent时需要 |
| 构造函数初始化列表 | 创建FindDialog时初始化控件 |
| 引用&参数 | closeEvent(QCloseEvent *event)的参数 |
| 头文件包含 | 新建对话框类需要.h和.cpp |

---

## 三、Qt实习现状分析

### Qt岗位分布
| 领域 | 说明 |
|------|------|
| 嵌入式/工控 | Qt在嵌入式GUI领域是主流，岗位最多 |
| 军工/航天 | 大量使用Qt做桌面软件 |
| 医疗设备 | 超声、监护仪等设备界面 |
| 桌面工具 | 国内少数公司，岗位少 |

### 现实情况
- Qt岗位**相对较少**，远不如前后端、Java、Go
- 但**竞争也小**，掌握好更容易脱颖而出
- 双非硕走Qt方向，**嵌入式/工控/军工**是主战场

---

## 四、实习技术要求

### 最低门槛（能投简历）
- [ ] C++基础扎实（指针、内存、STL、多态）
- [ ] Qt基本控件 + 信号槽
- [ ] 会用QSS做简单界面美化
- [ ] 能独立完成一个桌面小工具（记事本、计算器）

### 加分项（面试有话说）
- [ ] 多线程（QThread、线程池）
- [ ] 网络编程（QTcpSocket、QHttp）
- [ ] 数据库（QSqlDatabase 增删改查）
- [ ] 音视频播放（QMediaPlayer、FFmpeg）
- [ ] 自定义控件（继承QWidget自己画）

### 面试常问
| 问题 | 要点 |
|------|------|
| 信号槽原理 | 观察者模式，moc编译，事件队列 |
| Qt事件循环 | exec()阻塞，事件分发机制 |
| 父子对象树 | 父对象析构自动delete子对象 |
| QThread用法 | moveToThread vs 继承QThread |
| 内存管理 | QObject树 vs 智能指针 |

---

## 五、学习路线

```
你现在在这里
    ↓
阶段1：记事本完善（菜单栏、快捷键、状态栏）  ← 1-2周
    ↓
阶段2：C++补课（STL、智能指针、多线程）      ← 2-3周
    ↓
阶段3：做一个完整项目（聊天软件/音乐播放器）   ← 3-4周
    ↓
阶段4：刷面试题 + 简历包装                   ← 1-2周
    ↓
阶段5：投简历（嵌入式/工控/军工方向公司）
```

---

## 六、建议的实习项目

| 项目 | 技术点 | 难度 |
|------|--------|------|
| 记事本（当前） | 文件IO、编码、信号槽 | ★★☆ |
| 聊天软件 | TCP网络、多线程、数据库 | ★★★ |
| 音乐播放器 | QMediaPlayer、自定义控件 | ★★★ |
| 远程桌面 | 屏幕捕获、图像压缩、网络传输 | ★★★★ |
| 工控上位机 | Modbus协议、串口通信、实时曲线 | ★★★★ |

---

*最后更新: 2026-03-21*
