#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QDebug>
#include <QPainter>
#include <QTimer>
#include <QPoint>
#include <QKeyEvent>
#include <QFile>
#include <QDir>
#include <QTextCodec>
#include <math.h>
#include <QFileDialog>
#include <QClipboard>
#include <QApplication>
#include "config.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void init();
    void calculationGrid();
    void modifyStatusBarText();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
    virtual void paintEvent(QPaintEvent *event);
    virtual void inputMethodEvent(QInputMethodEvent *event);

public:
    QTimer *cursor_timer;    // 光标闪烁用的定时器
    QStatusBar *statusbar;   // 状态栏
    QLabel *statusbar_label; // 状态栏里面显示的文本
    QClipboard *clip;        // 剪贴板
    QList<QString> article;  // 文章内容
    QString file_name;       // 文章所在的位置和文章名

    bool cursor_show; // 光标是否处于显示状态
    bool ctrl_down;    // 判断alt按键是否按下

    int color;    // 色彩样式
    int start_x;  // 文章从第几列开始绘制
    int x_max;    // 屏幕最多绘制多少列
    int start_y;  // 文章从第几行开始绘制
    int y_max;    // 屏幕最多绘制多少行
    int choice_x; // 用于标记选择开始的列
    int choice_y; // 用于标记选择开始的行
    int cursor_x; // 光标所在的列
    int cursor_y; // 光标所在的行

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
