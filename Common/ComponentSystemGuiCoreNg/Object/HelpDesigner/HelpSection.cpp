#include "HelpSection.h"

HelpSection::HelpSection(const QString &title, QObject *parent) :
    QObject(parent),
    _addedInHelp(false),
    _isSubsection(false),
    _title(title),
    _text("")
{
}

HelpSection::~HelpSection()
{
    _title.clear();
    _text.clear();
    _imageResources.clear();
}

void HelpSection::addedInHelp(bool isAdded)
{
    _addedInHelp = isAdded;
}

void HelpSection::setSubsection(bool isSubsection)
{
    _isSubsection = isSubsection;
}

bool HelpSection::isAdded()
{
    return _addedInHelp;
}

bool HelpSection::isSubsection() const
{
    return _isSubsection;
}

IHelpItem *HelpSection::item(const QString &itemTitle)
{
    QVector<IHelpItem*> buffList = this->items();
    for(int i = 0; i < buffList.size(); i++) {
        IHelpItem* buffItem = buffList[i];
        if(buffItem->title() == itemTitle) {
            return buffItem;
        }
    }
    return nullptr;
}

QVector<IHelpItem *> HelpSection::items()
{
    return _items;
}

QVector<HelpSection *> HelpSection::sections()
{
    return _subsections;
}

QString HelpSection::title() const
{
    QMutexLocker lock(&_mutex);
    Q_UNUSED(lock)

    return _title;
}

QString HelpSection::text() const
{
    QMutexLocker lock(&_mutex);
    Q_UNUSED(lock)

    return _text;
}

const QMap<QString, QImage> *HelpSection::imageResources()
{
    QMutexLocker lock(&_mutex);
    Q_UNUSED(lock)

    return &_imageResources;
}

void HelpSection::setText(const QString &text)
{
    QMutexLocker lock(&_mutex);
    Q_UNUSED(lock)

    _text = text;
}

void HelpSection::setTextFile(QFile *textFile)
{
    QMutexLocker lock(&_mutex);
    Q_UNUSED(lock)

    if(!textFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << QString("[HelpSection][setTextFile] Can not open file! Section title: %1; File: %2")
                       .arg(_title)
                       .arg(textFile->fileName());

        delete textFile;
        return;
    }

    _text = QString(textFile->readAll());
    textFile->close();

    if(_text.isEmpty())
        qWarning() << QString("[HelpSection][setTextFile] Text file is Empty! Section title: %1; File: %2")
                      .arg(_title)
                      .arg(textFile->fileName());

    delete textFile;
}

void HelpSection::setImageResources (const QMap<QString, QImage> &imageResources)
{
    QMutexLocker lock(&_mutex);
    Q_UNUSED(lock)

    _imageResources = imageResources;
    this->scaleImageMap();
}

bool HelpSection::addImageResource(const QString &imageName, const QImage &image)
{
    QMutexLocker lock(&_mutex);
    Q_UNUSED(lock)

    if(image.isNull()) {
        qCritical() << QString("[HelpSection][addImageResource] Image is NULL! Add failed! Section title: %1").arg(_title);
        return false;
    }
    if(_imageResources.contains(imageName)) {
        qCritical() << QString("[HelpSection][addImageResource] Image with this name already added! Add failed! Section title: %1").arg(_title);
        return false;
    }

    QImage buffImage = this->scaleImage(image);
    if(buffImage.isNull()) {
        qCritical() << QString("[HelpSection][addImageResource] Scale image failed! Image is NULL! Add failed! Item title: %1").arg(_title);
        return false;
    }

    _imageResources.insert(imageName, buffImage);
    return true;
}

bool HelpSection::addItem(IHelpItem *item)
{
    QMutexLocker lock(&_mutex);
    Q_UNUSED(lock)

    IHelpItem *buffItem = this->item(item->title());
    if(buffItem) {
        qCritical() << QString("[HelpSection][addItem] Help item \"%1\" already inserted in section \"%2\"! Add item failed!")
                       .arg(item->title())
                       .arg(_title);
        return false;
    }

    item->setParent(this);
    _items.push_back(item);
    return true;
}

bool HelpSection::addSubsection(IHelpSection *subsection)
{
    QMutexLocker lock(&_mutex);
    Q_UNUSED(lock)

    if(!subsection) {
        qCritical() << "[HelpSection][addSubsection] Subsection is NULL! Add failed!";
        return false;
    }

    HelpSection* buffSubsection = dynamic_cast<HelpSection*>(subsection);
    if(!buffSubsection) {
        qCritical() << "[HelpSection][addSubsection] Convert to HelpSection failed! Add subsection failed!";
        return false;
    }

    if(buffSubsection->isAdded()) {
        qWarning() << QString("[HelpSection][addSubsection] Section \"%1\" already added in help! Add subsection failed!")
                       .arg(buffSubsection->title());
        return false;
    }

    if(buffSubsection->isSubsection()) {
        qWarning() << QString("[HelpSection][addSubsection] Section \"%1\" is already subsection! Add subsection failed!")
                      .arg(buffSubsection->title());
        return false;
    }

    buffSubsection->setSubsection(true);
    _subsections.push_back(buffSubsection);
    return true;
}

QImage HelpSection::scaleImage(QImage img)
{
    int height = img.rect().height();
    int width = img.rect().width();

    if(width <= IMG_MAX_WIDTH)
        return img;

    double tempWidth = (double) width;
    double tempHeight = (double) height;
    double tempSize = tempWidth / ((double) IMG_MAX_WIDTH);
    double tempHeightImg = tempHeight/tempSize;

    return img.scaled(IMG_MAX_WIDTH, tempHeightImg);
}

void HelpSection::scaleImageMap()
{
    QMap<QString, QImage> buffImageResources;
    QMapIterator<QString, QImage> i(_imageResources);
    while(i.hasNext()) {
        i.next();
        QImage buffImage = this->scaleImage(i.value());
        if(buffImage.isNull()) {
            qCritical() << QString("[IHelpItem][scaleImageMap] Scale image failed! Image is NULL! Add failed! Item title: %1").arg(i.key());
            continue;
        }
        buffImageResources.insert(i.key(), buffImage);
    }
    _imageResources.clear();
    _imageResources = buffImageResources;
}
