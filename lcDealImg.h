#ifndef CDEALIMG_H
#define CDEALIMG_H

#include <QPixmap>
#include <QImage>

class CDealImg
{
public:
    explicit CDealImg()=delete;
    ~CDealImg()=delete;

    static void dealPix(QPixmap &pm);

    static void dealPix(QImage &img);
};

#endif // CDEALIMG_H
