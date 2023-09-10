#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QClipboard>
#include <QMimeData>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

static const QString EXIT = "Exit";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , preText("")
    , pos(TOPLEFT)
{
    ui->setupUi(this);

    // 设置托盘图标菜单
    menu = new QMenu();
    menu->addAction(EXIT);
    connect(menu, &QMenu::triggered, this, &MainWindow::on_systemTrayIcon_Menu_Click);

    // 设置最小化托盘
    QIcon icon(":/resource/logo.png");
    systemTrayIcon = new QSystemTrayIcon(icon);
    systemTrayIcon->setContextMenu(menu);
    systemTrayIcon->setToolTip("FuckImg");
    systemTrayIcon->show();

    // 设置窗口属性
    setWindowTitle("fuckImg"); // 窗口名
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
        //img.save("d://bad.png");
        dealPix(img);
        //img.save("d://good.png");
        preImage = img;
        clipboard->setPixmap(img);
    }
}

void MainWindow::dealPix(QPixmap &pm){
    QPainter painter(&pm);
#ifdef WATERLOGO
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
#else
    // 图片点一个比较透明的黑点
    painter.setPen(QColor(0, 0, 0, 10));
    painter.drawPoint(QPoint(0, 0));
#endif
}

void MainWindow::on_systemTrayIcon_Menu_Click(QAction *action)
{
    QString text = action->text();
    if (text == EXIT)
    {
        close();
    }
}
