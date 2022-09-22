#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    color    =  0;   // 色彩样式为1
    start_x  =  0;   // 文章从第1列开始绘制
    start_y  =  0;   // 文章从第1行开始绘制
    choice_x = -1;   // 用于标记选择开始的列
    choice_y = -1;   // 用于标记选择开始的行
    cursor_x =  0;   // 光标在第1列
    cursor_y =  0;   // 光标在第1行
    clip = QApplication::clipboard();

    cursor_show = true;  // 光标处于显示的状态
    ctrl_down    = false; // alt按键没有按下

    // 创建定时器
    cursor_timer = new QTimer(this);
    // 启动定时器
    cursor_timer->start(CURSOR_TIME);
    // 执行内容
    connect(cursor_timer, &QTimer::timeout, [=](){
        cursor_show = !cursor_show;
        update();
    });

    // 初始化允许输入法
    setAttribute(Qt::WA_InputMethodEnabled, true);

    // 初始化底部提示栏
    statusbar = statusBar();
    setStatusBar(statusbar);
    QString str = "页面: 0-0   当前: 0   总: 0";
    statusbar_label = new QLabel(str, this);
    article.append("欢迎使用此文本编辑器！");
    article.append("按住alt可实现选择文本，配合滚轮实现左右滑动");

    calculationGrid();
    modifyStatusBarText();
}

void MainWindow::calculationGrid()
{
    x_max = 0;    // 屏幕最多绘制多少列
    while(x_max * X_SPACE + X_ADD < width()) {
        x_max++;
    }
    x_max = x_max - 1;

    y_max = 0;    // 屏幕最多绘制多少行
    while(y_max * Y_SPACE + Y_ADD < height()) {
        y_max++;
    }
    y_max = y_max - 1;
}

void MainWindow::modifyStatusBarText()
{
    QString str;
    if(start_y + y_max > article.size()) {
        str = "页面: " + (QString::number(start_y + 1)) + "-" + (QString::number(article.size())) +
              "   总: " + (QString::number(article.size())) + "   当前:" + (QString::number(cursor_y + 1));
    }
    else {
        str = "页面: " + (QString::number(start_y + 1)) + "-" + (QString::number(start_y + y_max)) +
              "   总: " + (QString::number(article.size())) + "   当前:" + (QString::number(cursor_y + 1));
    }
    delete statusbar_label;
    statusbar_label = new QLabel(str, this);
    statusbar->addWidget(statusbar_label);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        int x = (event->x()) / X_SPACE + start_x;
        int y = (event->y() - 20) / Y_SPACE + start_y;
        int list_len = article.length();
        if(y > list_len - 1) {
            y = list_len - 1;
        }
        int line_len = article[y].length();
        if(x > line_len) {
            x = line_len;
        }
        cursor_x = x;
        cursor_y = y;
        if(ctrl_down == false) {
            choice_x = x;
            choice_y = y;
        }
    }
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(ctrl_down == false) {
        choice_x = -1;
        choice_y = -1;
    }
    modifyStatusBarText();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    int _x = choice_x;
    int _y = choice_y;
    if(_x > -1 || _y > -1) {
        int x = (event->x()) / X_SPACE + start_x;
        int y = (event->y() - 20) / Y_SPACE + start_y;
        int list_len = article.length();
        if(y > list_len - 1) {
            y = list_len - 1;
        }
        int line_len = article[y].length();
        if(x > line_len) {
            x = line_len;
        }
        cursor_x = x;
        cursor_y = y;
        update();
    }
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    QPoint p = event->angleDelta();

    if(ctrl_down == false) {
        if(p.y() > 0) {
            if(start_y > 0) {
                start_y -= 3;
                if(start_y < 0) {
                    start_y = 0;
                }
            }
        }
        else if(p.y() < 0) {
            start_y += 3;
            if(start_y > article.length()) {
                start_y = article.length() - 1;
            }
        }
        modifyStatusBarText();
        update();
    }
    else {
        if(p.y() > 0) {
            start_x += 3;
        }
        else if(p.y() < 0) {
            if(start_x > 0) {
                start_x -= 3;
                if(start_x < 0) {
                    start_x = 0;
                }
            }
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    calculationGrid();
    int key = event->key();
    QString text = event->text();
    QString line = article[cursor_y];
    switch (key) {
    case Qt::Key_Left:
        if(cursor_x > 0) {
            cursor_x = cursor_x - 1;
            if(cursor_x < start_x) {
                start_x = start_x - 1;
            }
            modifyStatusBarText();
            cursor_show = true;
            update();
        }
        break;
    case Qt::Key_Right:
        if(cursor_x < line.length()) {
            cursor_x = cursor_x + 1;
            if(cursor_x > start_x + x_max) {
                start_x = start_x + 1;
            }
            modifyStatusBarText();
            cursor_show = true;
            update();
        }
        break;
    case Qt::Key_Up:
        if(cursor_y > 0) {
            if(cursor_x > article[cursor_y - 1].length()) {
                cursor_x = article[cursor_y - 1].length();
            }
            cursor_y = cursor_y - 1;
            if(cursor_y < start_y) {
                start_y = start_y - 1;
            }
            modifyStatusBarText();
            cursor_show = true;
            update();
        }
        break;
    case Qt::Key_Down:
        if(cursor_y < article.length() - 1) {
            if(cursor_x > article[cursor_y + 1].length()){
                cursor_x = article[cursor_y + 1].length();
            }
            cursor_y = cursor_y + 1;
            if(cursor_y > start_y + y_max - 1) {
                start_y = start_y + 1;
            }
            modifyStatusBarText();
            cursor_show = true;
            update();
        }
        break;
    case Qt::Key_Return:
    {
        QString right_str = line.right(line.length() - cursor_x);
        article.insert(cursor_y + 1, right_str);
        article[cursor_y] = line.left(cursor_x);
        cursor_x = 0;
        cursor_y = cursor_y + 1;
        if(cursor_y > start_y + y_max - 1) {
            start_y = start_y + 1;
        }
        modifyStatusBarText();
        cursor_show = true;
        update();
    }
        break;
    case Qt::Key_Backspace:
        if(cursor_x > 0) {
            article[cursor_y] = line.left(cursor_x - 1) + line.right(line.length() - cursor_x);
            cursor_x = cursor_x -1;
            if(cursor_x < start_x + 2) {
                start_x = start_x - 2;
                if(start_x < 0) {
                    start_x = 0;
                }
            }
            if(cursor_y < start_y) {
                start_y = cursor_y;
            }
            modifyStatusBarText();
            cursor_show = true;
            update();
        }
        else {
            if(cursor_y > 0) {
                int src_len = article[cursor_y - 1].length();
                article[cursor_y - 1] += line;
                article.removeAt(cursor_y);
                cursor_x = src_len;
                cursor_y = cursor_y -1;
                if(cursor_x > start_x + x_max) {
                    start_x = src_len - 5;
                    if(start_x < 0) {
                        start_x = 0;
                    }
                }
                if(cursor_y < start_y) {
                    start_y = cursor_y - 1;
                }
                modifyStatusBarText();
                cursor_show = true;
                update();
            }
        }
        break;
    case Qt::Key_Control:
    {
        ctrl_down = true;
    }
        break;
    case Qt::Key_C:
    {
        if(ctrl_down == true) {
            if(choice_x > -1 && choice_y > -1) {
                if(choice_y == cursor_y) {
                    QString line = article.at(choice_y).mid(choice_x, cursor_x - choice_x);
                    clip->setText(line);
                }
                else if(choice_y < cursor_y) {
                    QString line = article.at(choice_y);
                    line = line.mid(choice_x, line.length() - choice_x);
                    for(int y = choice_y - start_y + 1; y < cursor_y - start_y; y++) {
                        line = line + "\n" + article.at(y + start_y);
                    }
                    line = line + + "\n" + article.at(cursor_y).mid(0, cursor_x);
                    clip->setText(line);
                }
            }
        }
    }
        break;
    case Qt::Key_V:
    {
        QString str = clip->text();
        qDebug() << str;
        article[cursor_y] = line.left(cursor_x) + str + line.right(line.length() - cursor_x);
        cursor_x = cursor_x + str.length();
        cursor_show = true;
        update();
    }
    default:
        if (key >= 0 && key <= 127) {
            article[cursor_y] = line.left(cursor_x) + text + line.right(line.length() - cursor_x);
            cursor_x = cursor_x + text.length();
            if(cursor_x > start_x + x_max) {
                start_x = start_x + text.length();
            }
            cursor_show = true;
            update();
        }
        break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    int key = event->key();
    switch (key) {
    case Qt::Key_Control:
    {
        ctrl_down = false;
    }
        break;
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    calculationGrid();
    QPainter painter(this);

    if(color == 0) {  // color为0，表示绘制白色主题
        // 绘制白色背景
        painter.setPen(QColor(255, 255, 255));
        DRAW_BACKGROUND

        painter.setPen(QColor(200, 230, 250));
        if(choice_x > -1 && choice_y > -1) {
            if(choice_y == cursor_y) {
                for(int x = choice_x - start_x; x < cursor_x - start_x; x++) {
                    int y = choice_y - start_y;
                    for(int i = x * X_SPACE + 4; i < (x + 1) * X_SPACE + 4; i++) {
                        for (int j = y * Y_SPACE + 31; j < (y + 1) * Y_SPACE + 31 - 10; j++) {
                            painter.drawPoint(i, j);
                        }
                    }
                }
            }
            else if(choice_y < cursor_y) {
                QString line = article.at(choice_y);
                for(int x = choice_x - start_x; x < line.length(); x++) {
                    int y = choice_y - start_y;
                    for(int i = x * X_SPACE + 4; i < (x + 1) * X_SPACE + 4; i++) {
                        for (int j = y * Y_SPACE + 31; j < (y + 1) * Y_SPACE + 31 - 10; j++) {
                            painter.drawPoint(i, j);
                        }
                    }
                }
                for(int y = choice_y - start_y + 1; y < cursor_y - start_y; y++) {
                    line = article.at(y + start_y);
                    for(int x = start_x; x < line.length(); x++) {
                        for(int i = x * X_SPACE + 4; i < (x + 1) * X_SPACE + 4; i++) {
                            for (int j = y * Y_SPACE + 31; j < (y + 1) * Y_SPACE + 31 - 10; j++) {
                                painter.drawPoint(i, j);
                            }
                        }
                    }
                }
                for(int x = start_x; x < cursor_x; x++) {
                    int y = cursor_y - start_y;
                    for(int i = x * X_SPACE + 4; i < (x + 1) * X_SPACE + 4; i++) {
                        for (int j = y * Y_SPACE + 31; j < (y + 1) * Y_SPACE + 31 - 10; j++) {
                            painter.drawPoint(i, j);
                        }
                    }
                }
            }
        }

        // 绘制黑色的字
        painter.setPen(QColor(0, 0, 0));
        painter.setFont(QFont("微软雅黑", 14));
        int draw_y = findMin(article.length() - start_y, y_max);
        for(int y = start_y; y < start_y + draw_y; y++) {
            QString line = article.at(y);
            int draw_x = findMin(line.length() - start_x, x_max);
            for(int x = start_x; x < start_x + draw_x; x++) {
                painter.drawText((x - start_x) * X_SPACE + X_ADD, (y - start_y) * Y_SPACE + Y_ADD, line.at(x));
            }
        }

        // 绘制光标
        DRAW_CURSOR_BLACK
    }
    else if(color == 1) {

    }

    // 绘制底部菜单栏
    painter.setPen(QColor(240, 240, 240));
    DRAW_INFORMATION
}

void MainWindow::inputMethodEvent(QInputMethodEvent *event)
{
    QString text = event->commitString();
    if(text.length() > 0) {
        QString line = article[cursor_y];
        article[cursor_y] = line.left(cursor_x) + text + line.right(line.length() - cursor_x);
        cursor_x = cursor_x + text.length();
        if(cursor_x > start_x + x_max) {
            start_x = start_x + text.length();
        }
        cursor_show = true;
        update();
    }
}

