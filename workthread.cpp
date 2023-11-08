#include "workthread.h"
#include "lcDealImg.h"
#include <QFile>
#include <QFileDialog>
#include <QDebug>

WorkThread::WorkThread()
{

}

void WorkThread::setValue(QString &srcPath, QString &tarPath)
{
    mSrcPath = srcPath;
    mTarPath = tarPath;
}

void WorkThread::run()
{
    QDir *dir=new QDir(mSrcPath);
    QStringList filter;
    filter<<QString("*.bmp")<<QString("*.png")<<QString("*.jpg")<<QString("*.jpeg");
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>(dir->entryInfoList(filter));
    for(int i = 0;i<fileInfo->count(); i++)
    {
        QImage img(fileInfo->at(i).filePath());
        // 处理图片数据
        CDealImg::dealPix(img);
        //保存到目标路径下
        img.save(mTarPath+"/"+fileInfo->at(i).fileName());
        emit updateDealProBar(i+1);
    }
}
