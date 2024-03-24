#include "RunErrorDialog.h"
#include "ui_RunErrorDialog.h"

#include <QtGui/QColor>
#include <QtGui/QIcon>
#include <QtGui/QPainter>
#include <QtCore/QDebug>
#include <QtGui/QPixmap>

RunErrorDialog::RunErrorDialog (const QFileInfoList &iconsList,
                                const QString &errorCaption,
                                const QString &errorDataLog,
                                QWidget *parent)
    : QDialog (parent)
    , ui (new Ui::RunErrorDialog)
{
    ui->setupUi (this);

    QPixmap statusPix;
    statusPix.load (":/Images/Images/exclamation-red_12x12.png");

    QPixmap tempDataPix = /*convertPixmapToMonoxrom(*/searchLogo(iconsList, QSize(16,16))/*)*/;

    QPixmap tempIcon (18, 18);
    tempIcon.fill (Qt::transparent); // задаем прозрачный фон
    QPainter painter (&tempIcon);
    painter.fillRect (0, 0, 18, 18, Qt::transparent);
    painter.drawPixmap (0, 0, tempDataPix.width(), tempDataPix.height(), tempDataPix); // вставляем рисунок
    painter.drawPixmap (6, 6, statusPix.width(), statusPix.height(), statusPix); // вставляем иконку статуса
    painter.end ();

    setWindowIcon (QIcon (tempIcon));
    setWindowTitle (tr ("Аварийная остановка приложения"));

    ui->labelErrorData->setText (errorCaption);
    ui->textEditErrorLogData->setText (errorDataLog);

    ui->pushButtonClose->setIcon (QIcon (":/Images/Images/cross.png"));
    connect (ui->pushButtonClose, &QPushButton::clicked, this, &RunErrorDialog::close);
}

RunErrorDialog::~RunErrorDialog () {
    delete ui;
}

QPixmap RunErrorDialog::searchLogo (const QFileInfoList &iconsList, const QSize &iconSize)
{
    for (int i = 0; i < iconsList.size(); i++) {
        QPixmap buffPix = QPixmap (iconsList[i].absoluteFilePath ());
        if (buffPix.size () == iconSize)
            return buffPix;
    }

    if(iconSize == QSize(16,16))
        return QPixmap(":/Images/Images/truck_16px.png");
    if(iconSize == QSize(32,32))
        return QPixmap(":/Images/Images/truck_32px.png");
    if(iconSize == QSize(64,64))
        return QPixmap(":/Images/Images/truck_64px.png");
    if(iconSize == QSize(128,128))
        return QPixmap(":/Images/Images/truck_128px.png");
    if(iconSize == QSize(256,256))
        return QPixmap(":/Images/Images/truck_256px.png");

    return QPixmap (":/Images/Images/");
}

QPixmap RunErrorDialog::convertPixmapToMonoxrom (const QPixmap &pixmap) const
{
    return QPixmap::fromImage (pixmap.toImage (), Qt::MonoOnly);

    //----------------------------------------------------------------
    //    QPixmap m_openedPix = pixmap;
    //    QPixmap alpha = m_openedPix.alphaChannel();
    //    QImage img = m_openedPix.toImage();

    //    for(int w = 0; w < img.rect().right() + 1; w++)
    //    {
    //        for(int h = 0; h < img.rect().bottom() + 1; h++)
    //        {
    //            QColor col(img.pixel(w,h));
    //            //col.setHsv(col.hue(), 0, col.value(), col.alpha());
    //            col.setHsl(col.hue(), 0, col.lightness(), col.alpha());
    //            img.setPixel(w,h,col.rgb());
    //        }
    //    }
    //    m_openedPix = QPixmap::fromImage(img);
    //    m_openedPix.setAlphaChannel(alpha);
    //    return m_openedPix;
    //----------------------------------------------------------------
}
