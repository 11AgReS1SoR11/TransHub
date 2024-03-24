#include "InformWidget.h"
#include "ui_InformWidget.h"

#include <QPalette>
#include <QPainter>
#include <QPixmap>
#include <QBrush>
#include <QLinearGradient>

InformWidget::InformWidget (const QFileInfoList &iconsList, int w, QWidget *parent)
    : QWidget (parent)
    , ui (new Ui::InformWidget)
{
    ui->setupUi (this);
    resize (w, height ());

    // формируем фоновый рисунок
    QPixmap pix = searchLogo (iconsList, QSize (128,128));

    QPixmap tempIconPix (width (), height ());
    tempIconPix.fill(Qt::transparent); // задаем прозрачный фон
    QPainter painterOne(&tempIconPix);
    painterOne.fillRect(0, 0, width(), height(), Qt::transparent);

    QColor maxColor = QColor(searchRgb(pix));
    maxColor.setAlpha(100);

    QLinearGradient linearGrad1(QPointF(0, 0), QPointF(pix.width() + 20, 0));
    linearGrad1.setColorAt(0, Qt::transparent);
    linearGrad1.setColorAt(1, maxColor);

    QLinearGradient linearGrad2(QPointF(0, 0), QPointF(width() - 20, 0));
    linearGrad2.setColorAt(0, maxColor);
    linearGrad2.setColorAt(1, Qt::transparent);

    painterOne.fillRect(0, 0, pix.width() + 2, height(), linearGrad1);
    painterOne.fillRect(pix.width() + 2, 0, width() - 20, height(), linearGrad2);
    painterOne.drawPixmap(0, 0, pix.width(), pix.height(), pix); // вставляем рисунок
    painterOne.end();

    QPalette pal2;
    pal2.setBrush(backgroundRole(), QBrush(tempIconPix));
    setPalette(pal2);
    setAutoFillBackground(true);

    QPalette pal;
    pal.setBrush(ui->progressBarLoad->backgroundRole(), QBrush("#d6d2d2"));
    ui->progressBarOther->setPalette(pal);
    ui->progressBarInit->setPalette(pal);
    ui->progressBarLoad->setPalette(pal);
}

InformWidget::~InformWidget () {
    delete ui;
}

QToolButton* InformWidget::buttonCancel () {
    return ui->toolButtonCancel;
}

QToolButton* InformWidget::buttonNext()
{
    return ui->toolButtonNext;
}

QToolButton* InformWidget::buttonSettings()
{
    return ui->toolButtonSettings;
}

QToolButton* InformWidget::buttonLog()
{
    return ui->toolButtonLog;
}

QProgressBar *InformWidget::progBarOther()
{
    return ui->progressBarOther;
}

QProgressBar* InformWidget::progBarLoad()
{
    return ui->progressBarLoad;
}

QProgressBar* InformWidget::progBarInit()
{
    return ui->progressBarInit;
}

QLabel *InformWidget::labelOther()
{
    return ui->labelOther;
}

QLabel* InformWidget::labelLoadIcon()
{
    return ui->labelLoadDataIcon;
}

QLabel* InformWidget::labelLoadText()
{
    return ui->labelLoadDataText;
}

QLabel* InformWidget::labelInitIcon()
{
    return ui->labelInitDataIcon;
}

QLabel* InformWidget::labelInitText()
{
    return ui->labelInitDataText;
}

QPixmap InformWidget::searchLogo(const QFileInfoList &iconsList, QSize iconSize)
{
    for(int i = 0; i < iconsList.size(); i++) {
        QPixmap buffPix = QPixmap(iconsList[i].absoluteFilePath());
        if(buffPix.size() == iconSize)
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

    return QPixmap(":/Images/Images/");
}

QRgb InformWidget::searchRgb(const QPixmap &pixmap)
{
    QPixmap m_openedPix = pixmap;
    QImage img = m_openedPix.toImage();

    QMap<QRgb, int> rgbList;
    for(int w = 0; w < img.rect().right() + 1; w++)
    {
        for(int h = 0; h < img.rect().bottom() + 1; h++)
        {
            QColor col(img.pixel(w,h));
            QRgb buffRgb = col.rgb();
            if(qRed(buffRgb) == qGreen(buffRgb)
               && qRed(buffRgb) == qBlue(buffRgb)
               && qRed(buffRgb) == 255)
                continue;

            else if(qRed(buffRgb) == qGreen(buffRgb)
                    && qRed(buffRgb) == qBlue(buffRgb)
                    && qRed(buffRgb) == 0)
                     continue;

            if(rgbList.contains(buffRgb))
                rgbList.insert(buffRgb, rgbList.value(buffRgb) + 1);
            else
                rgbList.insert(buffRgb, 1);
        }
    }

    QRgb rgbData;
    int maxNum = 0;
    QMapIterator<QRgb, int> i(rgbList);
    while(i.hasNext()) {
        i.next();
        if(i.value() > maxNum) {
            rgbData = i.key();
            maxNum = i.value();
        }
    }

    // высветляем
    double rgbRD = static_cast<double>(qRed(rgbData));
    double rgbGD = static_cast<double>(qGreen(rgbData));
    double rgbBD = static_cast<double>(qBlue(rgbData));

    if(rgbRD == rgbGD && rgbRD == rgbBD) {
        // высветляем все
        rgbRD = rgbRD + ((rgbRD/255)*100);
        rgbGD = rgbGD + ((rgbGD/255)*100);
        rgbBD = rgbBD + ((rgbBD/255)*100);

    } else if(rgbRD > rgbGD && rgbRD > rgbBD) {
        // высветляем красный
        rgbRD = rgbRD + ((rgbRD/255)*100);

    } else if(rgbGD > rgbRD && rgbGD > rgbBD) {
        // высветляем зеленый
        rgbGD = rgbGD + ((rgbGD/255)*100);

    } else if(rgbBD > rgbGD && rgbBD > rgbRD) {
        // высветляем синий
        rgbBD = rgbBD + ((rgbBD/255)*100);
    }

    int rgbRed = rgbRD;
    int rgbGreen = rgbGD;
    int rgbBlue = rgbBD;

    if(rgbRed > 255)
        rgbRed = 255;
    if(rgbGreen > 255)
        rgbGreen = 255;
    if(rgbBlue > 255)
        rgbBlue = 255;

    return qRgb(rgbRed, rgbGreen, rgbBlue);
}
