#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lcDealImg.h"
#include "lcLog.h"

#include <QClipboard>
#include <QFile>
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>

static const QString EXIT = "Exit";
static const QString DEBUG_STR = "Debug";
static const QString DEAL = QString::fromLocal8Bit("批处理");
static const QString VERSION = "FuckTX_v1.2";
static const QString MIMETYPE = "image/*";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , preText("")
    , preImage()
    , preMimeData(nullptr)
    , pos(TOPLEFT)
    , m_isDebug(false)
{
    ui->setupUi(this);

    // 设置托盘图标菜单
    menu = new QMenu();
    // 批处理按钮
    menu->addAction(DEAL);
    // debug按钮
    QAction *pdebug = menu->addAction(DEBUG_STR);
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

    m_thread = new WorkThread();
    connect(m_thread, &WorkThread::updateDealProBar, this, &MainWindow::on_updateDealProBar, Qt::QueuedConnection); // 监听系统剪切板数据变化事件
    connect(m_thread, &WorkThread::finished, this, &MainWindow::on_workThreadFinished, Qt::QueuedConnection); // 监听系统剪切板数据变化事件
}

MainWindow::~MainWindow()
{
    int i = PLACEHOLDER_SIZE;
    LOG_INFO("~MainWindow()");
    systemTrayIcon->hide();
    disconnect(clipboard, &QClipboard::dataChanged, this, &MainWindow::on_clipboard_update);
    delete systemTrayIcon;
    delete ui;
}

/**
 * @brief 测试函数
 */
void MainWindow::test()
{
    QMimeData *mimeData = const_cast<QMimeData *>(clipboard->mimeData());
    if (preMimeData == mimeData) return;
    LOG_INFO("on_clipboard_update");
    preMimeData = mimeData;
    if (mimeData->hasImage())
    {
        QMimeData *mimeData2 = new QMimeData();
        for (QString format:mimeData->formats())
        {
            LOG_INFO("format = {}", format.toStdString());
            const QByteArray data = mimeData->data(format);
            LOG_INFO("data len = {}", data.length());
            if (format.contains("PNG"))
            {
                QFile file("D:/testmmm");
                bool isOpen = file.open(QIODevice::WriteOnly);
                if (isOpen)
                {
                    LOG_INFO("success");
                    file.write(data);
                    file.close();
                }
                else
                {
                    LOG_INFO("error");
                }
            }
            mimeData2->setData(format, data);
//            mimeData->clear();
//            mimeData->setData(MIMETYPE, data);
//            clipboard->setMimeData(mimeData);
        }
        clipboard->setMimeData(mimeData2);
        //disconnect(clipboard, &QClipboard::dataChanged, this, &MainWindow::on_clipboard_update);
    }
}

/**
 * @brief 处理剪切板数据
 */
void MainWindow::dealClipboardImage(bool isDebug)
{
    // 从剪切板获取数据
    const QMimeData *mimeData = clipboard->mimeData();
    if (mimeData->hasImage())
    {
        // 图片数据，进行加工处理，改变数据的md5值
        QImage img = clipboard->image();
        if (preImage.cacheKey() == img.cacheKey()) return; // 已经处理过了

        LOG_INFO("on_clipboard_update");
        if (isDebug) img.save(QString("d://1_%1.png").arg(img.cacheKey()));// 开启debug模式会保存处理前后的图片到d盘
        // 处理图片数据
        CDealImg::dealPix(img);
        if (isDebug) img.save(QString("d://2_%1.png").arg(img.cacheKey()));// 开启debug模式会保存处理前后的图片到d盘
        preImage = img;
        // 处理后的数据重新放回剪切板
        clipboard->setImage(img);
    }
}

/**
 * @brief 处理本地文件夹下的图片
 * @param isDebug
 */
void MainWindow::dealLocalDirImage(QString srcDir, QString tarDir)
{
    QDir *dir=new QDir(srcDir);
    QStringList filter;
    filter<<QString("*.bmp")<<QString("*.png")<<QString("*.jpg")<<QString("*.jpeg");
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>(dir->entryInfoList(filter));
    if (0 == fileInfo->count())
    {
        QMessageBox::information(nullptr, QStringLiteral("Error"), QStringLiteral("源文件夹下没有bmp、png、jpg、jpeg格式文件！"));
        return;
    }
    ui->DealProBar->setMaximum(fileInfo->count());
    for(int i = 0;i<fileInfo->count(); i++)
    {
        //qDebug()<<fileInfo->at(i).filePath();//路径全名
        //qDebug()<<fileInfo->at(i).fileName();//文件名
        QImage img(fileInfo->at(i).filePath());
        // 处理图片数据
        CDealImg::dealPix(img);
        //保存到目标路径下
        img.save(tarDir+"/"+fileInfo->at(i).fileName());
        ui->DealProBar->setValue(i+1);
    }
    QMessageBox::information(nullptr, QStringLiteral("Info"), QStringLiteral("处理完成！"));

    hide();
    ui->srcPb->setEnabled(true);
    ui->tarPb->setEnabled(true);
    ui->beginPb->setEnabled(true);
    ui->exitPb->setEnabled(true);
}

/**
 * @brief 剪切板内容更新触发的槽函数
 */
void MainWindow::on_clipboard_update()
{
    dealClipboardImage(m_isDebug);
    //test();
}

/**
 * @brief 状态栏右键菜单点击事件触发的槽函数
 * @param action 事件
 */
void MainWindow::on_systemTrayIcon_Menu_Click(QAction *action)
{
    QString text = action->text();
    if (text == EXIT)
    {
        close();
    }
    else if (DEBUG == text)
    {
        // debug模式，会保存处理前后的图片
        m_isDebug = action->isChecked();
        LOG_INFO("DEBUG = {}", m_isDebug);
    }
    else if (DEAL == text)
    {
        // 批处理
        show();
    }
}

/**
 * @brief 开始批处理按钮点击触发的槽函数
 */
void MainWindow::on_beginPb_clicked()
{
    QString srcPath = ui->srcLab->text();
    QString tarPath = ui->tarLab->text();
    if (0 == srcPath.length() ||
            0 == tarPath.length())
    {
        QMessageBox::information(nullptr, QStringLiteral("Error"), QStringLiteral("请选择需要批处理的源文件路径和目标文件路径！"));
        return;
    }

    QDir *dir=new QDir(srcPath);
    QStringList filter;
    filter<<QString("*.bmp")<<QString("*.png")<<QString("*.jpg")<<QString("*.jpeg");
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>(dir->entryInfoList(filter));
    if (0 == fileInfo->count())
    {
        QMessageBox::information(nullptr, QStringLiteral("Error"), QStringLiteral("源文件夹下没有bmp、png、jpg、jpeg格式文件！"));
        return;
    }

    ui->srcPb->setEnabled(false);
    ui->tarPb->setEnabled(false);
    ui->beginPb->setEnabled(false);
    ui->exitPb->setEnabled(false);
    ui->DealProBar->setMaximum(fileInfo->count());
    m_thread->setValue(srcPath, tarPath);
    m_thread->start();
}

/**
 * @brief 打开源文件路径按钮触发的槽函数
 */
void MainWindow::on_srcPb_clicked()
{
    QString filename = QFileDialog::getExistingDirectory();
    ui->srcLab->setText(filename);
}

/**
 * @brief 打开目标文件路径按钮触发的槽函数
 */
void MainWindow::on_tarPb_clicked()
{
    QString filename = QFileDialog::getExistingDirectory();
    ui->tarLab->setText(filename);
}

/**
 * @brief 退出按钮
 */
void MainWindow::on_exitPb_clicked()
{
    hide();
}

void MainWindow::on_updateDealProBar(int value)
{
    ui->DealProBar->setValue(value);
}

void MainWindow::on_workThreadFinished()
{
    QMessageBox::information(nullptr, QStringLiteral("Info"), QStringLiteral("处理完成！"));

    hide();
    ui->srcPb->setEnabled(true);
    ui->tarPb->setEnabled(true);
    ui->beginPb->setEnabled(true);
    ui->exitPb->setEnabled(true);
}
