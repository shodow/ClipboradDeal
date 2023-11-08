#include "mainwindow.h"

#include <QApplication>
#include <QSharedMemory>
#include <QMessageBox>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSharedMemory shared("fuck");
    if (shared.attach())
    {
        QMessageBox::information(nullptr, QStringLiteral("Warning"), QStringLiteral("程序已运行"));
        return 0;
    }
    shared.create(1);

    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);

    MainWindow w;
    //w.show();
    return a.exec();
}
