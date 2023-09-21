#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>

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
    void dealPix(QPixmap &pm);

private slots:
    void on_clipboard_update();

    void on_systemTrayIcon_Menu_Click(QAction *action);
private:
    Ui::MainWindow *ui;
    QClipboard *clipboard;
    QString preText;
    QPixmap preImage;
    bool m_bDrag;
    QPoint mouseStartPoint;
    QPoint windowTopLeftPoint;
    POS pos;

    QSystemTrayIcon *systemTrayIcon;
    QMenu *menu;
    bool m_isDebug;
};
#endif // MAINWINDOW_H
