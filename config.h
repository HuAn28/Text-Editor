#ifndef CONFIG_H
#define CONFIG_H

#define X_SPACE       24  // 列间距
#define Y_SPACE       36  // 行间距
#define X_ADD          5  // 字符到左边的距离
#define Y_ADD         52  // 字符到顶部的距离
#define CURSOR_TIME  400  // 光标闪烁事件
#define CURSOR_WIDTH   2  // 光标的宽度
#define CURSOR_HEIGHT 30  // 光标的高度

// 定义的绘制内容
#define DRAW_BACKGROUND \
    for(int x = 0; x < this->width(); x++) { \
        for(int y = 0; y < this->height(); y++) { \
            painter.drawPoint(x, y); \
        } \
    }
#define DRAW_CURSOR_BLACK \
    int x_add = 3; \
    int y_add = 28; \
    if(cursor_show == true) { \
        painter.setPen(QColor(0, 0, 0)); \
        for(int x = x_add + X_SPACE * (cursor_x - start_x); x < x_add + X_SPACE * (cursor_x - start_x) + CURSOR_WIDTH; x++) { \
            for(int y = y_add + Y_SPACE * (cursor_y - start_y); y < y_add + Y_SPACE * (cursor_y - start_y) + CURSOR_HEIGHT; y++) { \
                painter.drawPoint(x, y); \
            } \
        } \
    } \
    else { \
        painter.setPen(QColor(255, 255, 255)); \
        for(int x = x_add + X_SPACE * (cursor_x - start_x); x < x_add + X_SPACE * (cursor_x - start_x) + CURSOR_WIDTH; x++) { \
            for(int y = y_add + Y_SPACE * (cursor_y - start_y); y < y_add + Y_SPACE * (cursor_y - start_y) + CURSOR_HEIGHT; y++) { \
                painter.drawPoint(x, y); \
            } \
        } \
    }
#define DRAW_INFORMATION \
    for(int y = height() - 25; y < height(); y++) { \
        for(int x = 0; x < width(); x++) { \
            painter.drawPoint(x, y); \
        } \
    }

// 声明工具函数
int findMin(int x, int y);

#endif // CONFIG_H
