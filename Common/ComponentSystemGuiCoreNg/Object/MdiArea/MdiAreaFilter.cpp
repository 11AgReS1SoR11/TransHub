#include "MdiAreaFilter.h"
#include <QtCore/QDebug>
#include <QtGui/QPainter>

#define BASE_COLOR_RED      121
#define BASE_COLOR_GREEN    113
#define BASE_COLOR_BLUE     116

MdiAreaFilter::MdiAreaFilter(QObject *parent)
    : QObject(parent)
{
    _mdiArea = nullptr;
    _iconManager = nullptr;
    _logoSize = 128;
    _baseDifference = 5;
}

MdiAreaFilter::~MdiAreaFilter()
{
    _mdiArea = nullptr;
    _iconManager = nullptr;
}

void MdiAreaFilter::setMdiArea (QMdiArea *mdiArea)
{
    _mdiArea = mdiArea;
    _baseColor = _mdiArea->background().color();
}

void MdiAreaFilter::setIconManager (ISystemIconManager *iconManager)
{
    if (!iconManager) {
        qWarning () << "[MdiAreaFilter][setIconManager] Invalid icon manager object";
        return;
    }

    _iconManager = iconManager;
    _logo = _iconManager->getSystemLogo (_logoSize, _logoSize).pixmap(_logoSize, _logoSize);
    _logo = convertPixmapToMonoxrom (_logo);
}

void MdiAreaFilter::updateMdiBrush ()
{
    if(!_mdiArea || !_iconManager)
        return;

    int buffW = 0;
    int buffH = 0;
    QSize mdiSize = _mdiArea->size();
    QSize logoSize = _logo.size();

    if(mdiSize.width() < logoSize.width())
        buffW = logoSize.width();
    else
        buffW = mdiSize.width();

    if(mdiSize.height() < logoSize.height())
        buffH = logoSize.height();
    else
        buffH = mdiSize.height();

    QPixmap buffPix(buffW, buffH);
    buffPix.fill(Qt::transparent); // задаем прозрачный фон
    QPainter painter(&buffPix);
    painter.setPen(_baseColor);
    painter.fillRect(0, 0, buffW, buffH, _baseColor);

    QColor col1 = _baseColor;
    QColor col2;

    int buffRed = _baseColor.red() - _baseDifference;
    if(buffRed < 0)
        buffRed = 0;

    int buffGreen = _baseColor.green() - _baseDifference;
    if(buffGreen < 0)
        buffGreen = 0;

    int buffBlue = _baseColor.blue() - _baseDifference;
    if(buffBlue < 0)
        buffBlue = 0;

    col2.setRed(buffRed);
    col2.setGreen(buffGreen);
    col2.setBlue(buffBlue);

    // draw rects
    QColor buffFirstRectColor;
    int buffSize = _logoSize / 2;
    int x = buffW;
    int y = buffH;
    int index = 0;
    while(true) {
        x = buffW;
        QColor firstColor = col1;
        QColor secondColor = col2;
        while(true) {
            QColor buffColor = firstColor;
            if(index%2)
                buffColor = secondColor;

            if(x == buffW) {
                if(buffColor == buffFirstRectColor) {
                    firstColor = col2;
                    secondColor = col1;

                    buffColor = firstColor;
                    if(index%2)
                        buffColor = secondColor;
                }
                buffFirstRectColor = buffColor;
            }

            painter.setBrush(QBrush(buffColor));
            painter.drawRect(x - buffSize, y - buffSize, buffSize, buffSize);
            x -= buffSize;
            index++;
            if(x < 0)
                break;
        }
        y -= buffSize;
        if(y < 0)
            break;
    }

    // draw lines
    int lineNum = 12;
    int buffInterval = buffSize / lineNum;
    x = buffW;
    y = buffH;
    index = 0;
    int indexRow = 0;
    while(true) {
        x = buffW;
        index = 0;
        while(true) {
            QColor buffColor = col2;
            if(indexRow%2) {
                if(!index%2) {
                    x -= buffSize;
                    index++;
                    if(x < 0)
                        break;
                    continue;
                }
            } else {
                if(index%2) {
                    x -= buffSize;
                    index++;
                    if(x < 0)
                        break;
                    continue;
                }
            }

            painter.setBrush(QBrush(buffColor));
            painter.setPen(QPen(QBrush(buffColor), 1));
            if(indexRow%2) {
                for(int i = 0; i <= lineNum; i++) {
                    painter.drawLine(x, y - (buffInterval * i), x - buffSize, y - (buffInterval * i));
                }
            } else {
                for(int i = 0; i <= lineNum; i++) {
                    painter.drawLine(x - (buffInterval * i), y, x - (buffInterval * i), y - buffSize);
                }
            }

            x -= buffSize;
            index++;
            if(x < 0)
                break;
        }
        indexRow++;
        y -= buffSize;
        if(y < 0)
            break;
    }

    // вставляем логотип
    painter.drawPixmap(buffW - _logo.width() - _logo.width()/4,
                       buffH - _logo.height() - _logo.height()/4,
                       _logo.width(),
                       _logo.height(),
                       _logo);
    painter.end();

    _mdiArea->setBackground(QBrush(buffPix));
}

QPixmap MdiAreaFilter::convertPixmapToMonoxrom (const QPixmap &pixmap) const
{
    return QPixmap::fromImage (pixmap.toImage (), Qt::MonoOnly);

//---------------------------------------------------------------------
//    QPixmap m_openedPix = pixmap;
//    QPixmap alpha = m_openedPix.alphaChannel();
//    QImage img = m_openedPix.toImage();

//    // формируем карту красного цвета
//    QMap<int /*before*/, int /*after*/> buffRedList;
//    for(int w = 0; w < img.rect().right() + 1; w++) {
//        for(int h = 0; h < img.rect().bottom() + 1; h++) {
//            QColor col(img.pixel(w,h));
//            buffRedList.insert(col.red(), 0);
//        }
//    }

//    // получаем самый темный цвет
//    int buffRedBefore = -1;
//    QMapIterator<int /*before*/, int /*after*/> it(buffRedList);
//    while(it.hasNext()) {
//        it.next();
//        buffRedBefore = it.key();
//        break;
//    }

//    // расчитываем разницу цветов
//    int buffBaseRed = _baseColor.red() - (_baseDifference * 20);
//    if(buffBaseRed < 0)
//        buffBaseRed = 0;

//    int differenceRed = 0;
//    if(buffRedBefore < buffBaseRed) {
//        differenceRed = buffBaseRed - buffRedBefore;
//    }

//    // заменяем цвета
//    QList<int> keyRed = buffRedList.keys();
//    for(int ii = 0; ii < keyRed.size(); ii++) {
//        int afterRed = keyRed[ii] + differenceRed;
//        if(afterRed > 255)
//            afterRed = 255;

//        buffRedList.insert(keyRed[ii], afterRed);
//    }


//    // делаем изображение черно-белым
//    int baseG = _baseColor.red() - _baseColor.green();
//    int baseB = _baseColor.red() - _baseColor.blue();

//    for(int w = 0; w < img.rect().right() + 1; w++) {
//        for(int h = 0; h < img.rect().bottom() + 1; h++) {
//            QColor col(img.pixel(w,h));

////            int buffRed = col.red();

//            int buffRed = buffRedList.value(col.red(), 0);
//            int buffGreen = buffRed - baseG;
//            if(buffGreen < 0)
//                buffGreen = 0;
//            if(buffGreen > 255)
//                buffGreen = 255;

//            int buffBlue = buffRed - baseB;
//            if(buffBlue < 0)
//                buffBlue = 0;
//            if(buffBlue > 255)
//                buffBlue = 255;

//            if(buffRed == 0 || buffGreen == 0 || buffBlue == 0) {
//                buffRed = buffGreen = buffBlue = 0;
//            }

//            col.setRed(buffRed);
//            col.setGreen(buffGreen);
//            col.setBlue(buffBlue);

//            col.setAlpha(col.alpha());

////            col.setHsv(col.hue(), 0, col.value(), col.alpha());
////            col.setHsl(col.hue(), 0, col.lightness(), col.alpha());
//            img.setPixel(w,h,col.rgb());
//        }
//    }
//    m_openedPix = QPixmap::fromImage(img);
//    m_openedPix.setAlphaChannel(alpha);
//    return m_openedPix;
//--------------------------------------------------------------------
}

QRgb MdiAreaFilter::searchRgb (const QPixmap &pixmap)
{
    //QPixmap m_openedPix = pixmap;
    QImage img = pixmap.toImage();

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
    qDebug() << "RGB size" << rgbList.size();


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

    return qRgb (rgbRed, rgbGreen, rgbBlue);
}

bool MdiAreaFilter::eventFilter (QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseMove) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::NoButton) {
            emit mousePos (mouseEvent->pos());
        }
    }
    if (event->type() == QEvent::Resize) {
        this->updateMdiBrush();
    }
    return QObject::eventFilter(obj, event);
}

int MdiAreaFilter::fixer (int value) const
{
    if (value < 0)
        return 0;
    else if (value > 255)
        return 255;
    else
        return value;
}
