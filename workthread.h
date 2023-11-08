#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QThread>

class WorkThread : public QThread
{
    Q_OBJECT
signals:
    void updateDealProBar(int value);
public:
    WorkThread();
    void setValue(QString &srcPath, QString &tarPath);
    void run() override;
private:
    QString mSrcPath;
    QString mTarPath;
};

#endif // WORKTHREAD_H
