#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QMimeData>
#include "workthread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
enum POS
{
    TOPLEFT,
    TOPRIGHT,
    BOTTOMLEFT,
    BOTTOMRIGHT,
    CENTER
};

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void test();
    void dealClipboardImage(bool isDebug);

    void dealLocalDirImage(QString srcDir, QString tarDir);

private slots:
    void on_clipboard_update(); // 剪切板更新

    void on_systemTrayIcon_Menu_Click(QAction *action); // 系统状态栏菜单点击
    void on_beginPb_clicked(); // 开始批处理

    void on_srcPb_clicked();

    void on_tarPb_clicked();

    void on_exitPb_clicked();

    void on_updateDealProBar(int value);

    void on_workThreadFinished();

private:
    Ui::MainWindow *ui;
    QClipboard *clipboard;
    QString preText;
    QImage preImage;
    QMimeData *preMimeData; //
    bool m_bDrag;
    QPoint mouseStartPoint;
    QPoint windowTopLeftPoint;
    POS pos;

    QSystemTrayIcon *systemTrayIcon;
    QMenu *menu;
    bool m_isDebug;
    WorkThread *m_thread;
};
#endif // MAINWINDOW_H
