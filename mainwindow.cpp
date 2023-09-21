#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QClipboard>
#include <QMimeData>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QRandomGenerator>

#define MODIFY
static const QString EXIT = "Exit";
static const QString DEBUG = "Debug";
static const QString RELEASE = "Release";
static const QString VERSION = "FuckImg_v1.1";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , preText("")
    , pos(TOPLEFT)
    , m_isDebug(false)
{
    ui->setupUi(this);

    // 设置托盘图标菜单
    menu = new QMenu();
    // debug按钮
    QAction *pdebug = menu->addAction(DEBUG);
    pdebug->setCheckable(true);
    pdebug->setChecked(false);
    // 退出按钮
    menu->addAction(EXIT);
    connect(menu, &QMenu::triggered, this, &MainWindow::on_systemTrayIcon_Menu_Click);

    // 设置最小化托盘
    QIcon icon(":/resource/logo.png");
    systemTrayIcon = new QSystemTrayIcon(icon);
    systemTrayIcon->setContextMenu(menu);
    systemTrayIcon->setToolTip(VERSION);
    systemTrayIcon->show();

    // 设置窗口属性
    setWindowTitle(VERSION); // 窗口名
    setWindowFlags(Qt::FramelessWindowHint); // 窗口不显示标题栏

    clipboard = QGuiApplication::clipboard(); // 创建系统剪切板对象
    connect(clipboard, &QClipboard::dataChanged, this, &MainWindow::on_clipboard_update, Qt::QueuedConnection); // 监听系统剪切板数据变化事件
}

MainWindow::~MainWindow()
{
    qDebug() << "~MainWindow()";
    systemTrayIcon->hide();
    disconnect(clipboard, &QClipboard::dataChanged, this, &MainWindow::on_clipboard_update);
    delete systemTrayIcon;
    delete ui;
}

void MainWindow::on_clipboard_update()
{
    // 从剪切板获取数据
    const QMimeData *mimeData = clipboard->mimeData();
    if (mimeData->hasImage()) {
        // 图片数据，进行加工处理，改变数据的md5值
        QPixmap img = qvariant_cast<QPixmap>(mimeData->imageData());
        if (preImage.cacheKey() == img.cacheKey()) return;
        if (m_isDebug) img.save(QString("d://1_%1.png").arg(img.cacheKey()));
        dealPix(img);
        if (m_isDebug) img.save(QString("d://2_%1.png").arg(img.cacheKey()));
        preImage = img;
        clipboard->setPixmap(img);
    }
}

void MainWindow::dealPix(QPixmap &pm){
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

void MainWindow::on_systemTrayIcon_Menu_Click(QAction *action)
{
    QString text = action->text();
    if (text == EXIT)
    {
        close();
    }
    else if (DEBUG == text)
    {
        m_isDebug = action->isChecked();
    }
}
