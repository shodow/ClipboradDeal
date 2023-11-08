#include "lcDealImg.h"
#include <QPainter>
#include <QRandomGenerator>

#define MODIFY

void CDealImg::dealPix(QPixmap &pm)
{
    QPainter painter(&pm);

#if defined WATERLOGO
    int fontSize = 8;
    if (pm.width() < 520) fontSize = 4;
    if (pm.width() < 290) fontSize = 2;
    QFont font("微软雅黑", fontSize, QFont::Thin);
    painter.setFont(font);
    painter.setPen(QColor(255, 0, 0, 255));


    switch (pos) {
        case TOPLEFT:
        {
            painter.drawText(QPointF(pm.width()*0.1, pm.height()*0.1), QString::fromLocal8Bit("@克克龙"));
            break;
        }
        case TOPRIGHT:
        {
            painter.drawText(QPointF(pm.width()*0.9, pm.height()*0.95), QString::fromLocal8Bit("@克克龙"));
            break;
        }
        case BOTTOMLEFT:
        {
            painter.drawText(QPointF(pm.width()*0.9, pm.height()*0.95), QString::fromLocal8Bit("@克克龙"));
            break;
        }
        case BOTTOMRIGHT:
        {
            painter.drawText(QPointF(pm.width()*0.9, pm.height()*0.95), QString::fromLocal8Bit("@克克龙"));
            break;
        }
        case CENTER:
        {
            painter.drawText(QPointF(pm.width()*0.5, pm.height()*0.5), QString::fromLocal8Bit("@克克龙"));
            break;
        }
    }
#elif defined EXIF
    // 修改图片EXIF信息
    pm.data_ptr();

#elif defined POINT
    int width = pm.width(); // 图像宽
    int height = pm.height(); // 图像高

    // 图片(0,0)点一个黑点
    painter.setPen(QColor(0, 0, 0));
    painter.drawPoint(QPoint(0, 0));

    // 图片点一个白点
    painter.setPen(QColor(255, 255, 255));
    painter.drawPoint(QPoint(width-1, 0));

    // 图片点一个红点
    painter.setPen(QColor(255, 0, 0));
    painter.drawPoint(QPoint(0, height-1));

    // 图片点一个蓝点
    painter.setPen(QColor(0, 0, 255));
    painter.drawPoint(QPoint(width-1, height-1));

    // 图片点一个绿点
    painter.setPen(QColor(0, 255, 0));
    painter.drawPoint(QPoint(width/2, height/2));

#elif defined MODIFY
    int width = pm.width(); // 图像宽
    int height = pm.height(); // 图像高

    // 获取图像宽高以内的一个随机点（x,y）
    int x = QRandomGenerator::global()->bounded(width);
    int y = QRandomGenerator::global()->bounded(height);

    // 获取xy点处的像素点的rgb值并加10，大于244的设置为244
    QImage image = pm.toImage();//将像素图转换为QImage
    QColor color = image.pixel(x, y);
    int r = color.red() > 244 ? 244 : (color.red() + 10);
    int g = color.green() > 244 ? 244 : (color.green() + 10);
    int b = color.blue() > 244 ? 244 : (color.blue() + 10);

    // 将处理后的rgb值画在原来的位置上
    painter.setPen(QColor(r, g, b));
    painter.drawPoint(QPoint(x, y));
#endif
}

void CDealImg::dealPix(QImage &img)
{
    int width = img.width(); // 图像宽
    int height = img.height(); // 图像高

    // 获取图像宽高以内的一个随机点（x,y）
    int x = QRandomGenerator::global()->bounded(width);
    int y = QRandomGenerator::global()->bounded(height);

    int offset = QRandomGenerator::global()->bounded(10);

    QColor color = img.pixel(x, y);
    int r = color.red() > 244 ? 244 : (color.red() + offset);
    int g = color.green() > 244 ? 244 : (color.green() + offset);
    int b = color.blue() > 244 ? 244 : (color.blue() + offset);
    img.setPixelColor(x, y, QColor(r,g,b));
}
