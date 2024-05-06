#include "StatusBarInfoWidget.h"
#include "../MainWindow.h"

#include <QtCore/QDebug>

#define ACTIVE_COLOR            0x42aaff
#define HIDE_AND_STOP_POP_UP    3 // sec.

StatusBarInfoWidget::StatusBarInfoWidget (MainWindow *mainWindow, QWidget *parent)
    : QLabel (parent)
{
    _popupModel = nullptr;
    _dataWindow = nullptr;
    _infoToolTip = new StatusBarInfoWidgetToolTip(_actionElements,
                                                    _popupElements.size(),
                                                    this);
    _infoToolTip->setWindowFlags(Qt::ToolTip);

    _mainWindow = mainWindow;
    _actionElements = 0;

    _isMute = false;
    _isStopped = false;

    _currentSound = ISystemGuiCorePopUpElement::ISystemGuiCorePopUpNotType;
    _currentSoundRe = ISystemGuiCorePopUpElement::ISystemGuiCorePopUpNotType;

    connect (&_soundTimerPlay, SIGNAL(timeout()), this, SLOT(playSound()));
    connect (&_soundTimerStop, SIGNAL(timeout()), this, SLOT(stopPlaySound()));
    connect (&_soundTimerAdd, SIGNAL(timeout()), this, SLOT(addSound()));

    _notActiveMakeData.load (":/icons/icons/information.png");
    _notActiveNoData.load (":/icons/icons/information_mono.png");

    _activeMakeData = highlighPixmap (_notActiveMakeData);
    _activeNoData = highlighPixmap (_notActiveNoData);

    setPixmap(_notActiveNoData);
    setFixedSize(_activeNoData.width() + 10, _activeNoData.height() + 10);
    setAlignment(Qt::AlignCenter);
    setMouseTracking(true);

    initSamples();
    initAudio();
    initModel();

    connect(this, SIGNAL(signalOffsetPopups()), this, SLOT(offsetPopups()));
}

StatusBarInfoWidget::~StatusBarInfoWidget()
{
    if(_dataWindow != nullptr)
    {
        _dataWindow->close();
        delete _dataWindow;
        _dataWindow = nullptr;
    }

    if(_popupModel != nullptr)
    {
        _popupModel->clear();
        delete _popupModel;
        _popupModel = nullptr;
    }

    for(int i = 0; i < _popupElements.size(); i++)
    {
        SystemGuiCorePopUpElement* popupElement = _popupElements.operator [](i);
        delete popupElement;
        popupElement = nullptr;
    }
    _popupElements.clear();

    // stop popUpWindow timers
    for(int i = 0; i < _allPopUps.size(); i++)
        _allPopUps[i]->stopTimers();

    // delete...
    for(int i = 0; i < _allPopUps.size(); i++) {
        StatusBarInfoWidgetPopupWindow *popUpW = _allPopUps[i];
        delete popUpW;
        popUpW = nullptr;
    }

    _replaySounds.clear();
    stopPlaySound();


    _mainWindow = nullptr;
}

QPixmap StatusBarInfoWidget::highlighPixmap (const QPixmap &pix)
{
    qDebug () << "[StatusBarInfoWidget][highlighPixmap] Under construction";
    //-- В прошлом getActivePixmap
    return pix;

    //    int _widthW = pix.width () + 2;
    //    int _heightW = pix.height () + 2;

    //    QPixmap m_openedPix = pix.scaled (_widthW, _heightW);
    //    QPixmap alpha = m_openedPix.alphaChannel();
    //    QImage img = m_openedPix.toImage();

    //    QColor activeColor;
    //    activeColor.setRgb(ACTIVE_COLOR);

    //    for(int w = 0; w < img.rect().right() + 1; w++)
    //    {
    //        for(int h = 0; h < img.rect().bottom() + 1; h++)
    //        {
    //            QColor col(img.pixel(w,h));
    //            col.setHsv(col.hue(), 0, col.value(), col.alpha());
    //            img.setPixel(w,h, activeColor.rgb());
    //        }
    //    }
    //    m_openedPix = QPixmap::fromImage(img);
    //    m_openedPix.setAlphaChannel(alpha);

    //    // формируем итоговую картинку
    //    QPixmap tempIcon(_widthW, _heightW);
    //    tempIcon.fill(Qt::transparent);
    //    QPainter painter(&tempIcon);
    //    painter.fillRect(0, 0, _widthW, _heightW, Qt::transparent);
    //    painter.drawPixmap(0, 0, _widthW, _heightW, m_openedPix);
    //    painter.drawPixmap(1, 1, _widthW - 2, _heightW - 2, pix);
    //    painter.end();

    //    return tempIcon;
}

void StatusBarInfoWidget::initSamples ()
{
    QFile inputFile;
    QByteArray ba;

    QDir dr = QDir::current ();
    if (!dr.cd (QLatin1String ("resources"))) {
        qWarning () << "[StatusBarInfoWidget][initSamples] Couln't find resources directory:\n\t\t"
                    << qPrintable (dr.path () + QDir::separator () + QLatin1String ("resources"));
        return;
    }

    if (!dr.cd (QLatin1String ("sounds"))) {
        qWarning () << "[StatusBarInfoWidget][initSamples] Couln't find sounds directory (no samples sounds):\n\t\t"
                    << qPrintable (dr.path () + QDir::separator () + QLatin1String ("sounds"));
        return;
    }

#if defined(Q_OS_LINUX)
    // load info
    inputFile.setFileName (dr.path () + QDir::separator () + "Sound_Info.wav");
    if (inputFile.open (QIODevice::ReadOnly)) {
        ba = inputFile.readAll();
        inputFile.close();
        _allSamples.insert(ISystemGuiCorePopUpElement::ISystemGuiCorePopUpInfo, ba);
        _allSamples.insert(ISystemGuiCorePopUpElement::ISystemGuiCorePopUpNotType, ba);
    }

    // load ok
    inputFile.setFileName (dr.path () + QDir::separator () + "Sound_Ok.wav");
    if(inputFile.open(QIODevice::ReadOnly)) {
        ba = inputFile.readAll();
        inputFile.close();
        _allSamples.insert(ISystemGuiCorePopUpElement::ISystemGuiCorePopUpOk, ba);
    }

    // load warning
    inputFile.setFileName (dr.path () + QDir::separator () + "Sound_Warning.wav");
    if(inputFile.open(QIODevice::ReadOnly)) {
        ba = inputFile.readAll();
        inputFile.close();
        _allSamples.insert(ISystemGuiCorePopUpElement::ISystemGuiCorePopUpWarning, ba);
    }

    // load critical
    inputFile.setFileName (dr.path () + QDir::separator () + "Sound_Critical.wav");
    if(inputFile.open(QIODevice::ReadOnly)) {
        ba = inputFile.readAll();
        inputFile.close();
        _allSamples.insert(ISystemGuiCorePopUpElement::ISystemGuiCorePopUpCritical, ba);
    }
#endif

#if defined(Q_OS_WIN)
    // load info
    inputFile.setFileName(":/sounds/sounds/Sound_Info.raw");
    if(inputFile.open(QIODevice::ReadOnly)) {
        ba = inputFile.readAll();
        inputFile.close();
        _allSamples.insert(ISystemGuiCorePopUpElement::ISystemGuiCorePopUpInfo, ba);
        _allSamples.insert(ISystemGuiCorePopUpElement::ISystemGuiCorePopUpNotType, ba);
    }

    // load ok
    inputFile.setFileName(":/sounds/sounds/Sound_Ok.raw");
    if(inputFile.open(QIODevice::ReadOnly)) {
        ba = inputFile.readAll();
        inputFile.close();
        _allSamples.insert(ISystemGuiCorePopUpElement::ISystemGuiCorePopUpOk, ba);
    }

    // load warning
    inputFile.setFileName(":/sounds/sounds/Sound_Warning.raw");
    if(inputFile.open(QIODevice::ReadOnly)) {
        ba = inputFile.readAll();
        inputFile.close();
        _allSamples.insert(ISystemGuiCorePopUpElement::ISystemGuiCorePopUpWarning, ba);
    }

    // load critical
    inputFile.setFileName(":/sounds/sounds/Sound_Critical.raw");
    if(inputFile.open(QIODevice::ReadOnly)) {
        ba = inputFile.readAll();
        inputFile.close();
        _allSamples.insert(ISystemGuiCorePopUpElement::ISystemGuiCorePopUpCritical, ba);
    }
#endif
}

void StatusBarInfoWidget::initAudio()
{
}

void StatusBarInfoWidget::initModel()
{
    if(_popupModel != nullptr)
    {
        _popupModel->removeRows(0, 0, QModelIndex()); //чистим модель
        _popupModel->setRowCount(0);
    }
    else
    {
        _popupModel = new QStandardItemModel();
        _popupModel->setColumnCount(5);

        QStandardItem* header = nullptr;

        header = new QStandardItem (tr ("Guid"));
        _popupModel->setHorizontalHeaderItem (0, header);

        header = new QStandardItem ("");
        _popupModel->setHorizontalHeaderItem (1, header);

        header = new QStandardItem (tr ("Время добавления"));
        _popupModel->setHorizontalHeaderItem (2, header);

        header = new QStandardItem (tr ("Название"));
        _popupModel->setHorizontalHeaderItem (3, header);

        header = new QStandardItem (tr ("Описание"));
        _popupModel->setHorizontalHeaderItem (4, header);
    }
}

void StatusBarInfoWidget::addStatusElementInModel(SystemGuiCorePopUpElement *popupElement)
{
    if(popupElement == nullptr)
        return;

    int column = 0;
    int rowNum = _popupModel->rowCount();

    QStandardItem* itemSource = new QStandardItem();
    itemSource->setText(popupElement->getElementGuid());
    _popupModel->setItem(rowNum, column++, itemSource);

    itemSource = new QStandardItem();
    itemSource->setIcon(popupElement->getElementIcon_16x16());
    _popupModel->setItem(rowNum, column++, itemSource);

    itemSource = new QStandardItem();
    itemSource->setText(popupElement->getElementAddTime().toString("dd.MM.yyyy - hh:mm:ss"));
    itemSource->setToolTip(popupElement->getElementAddTime().toString("dd.MM.yyyy - hh:mm:ss"));
    _popupModel->setItem(rowNum, column++, itemSource);

    QString buffCaption = popupElement->getElementCaption();
    buffCaption = buffCaption.replace("\n"," ");
    buffCaption = buffCaption.replace("\t"," ");
    itemSource = new QStandardItem();
    itemSource->setText(buffCaption);
    itemSource->setToolTip(buffCaption);
    _popupModel->setItem(rowNum, column++, itemSource);

    QString buffDataText = popupElement->getElementDataText();
    buffDataText = buffDataText.replace("\n"," ");
    buffDataText = buffDataText.replace("\t"," ");
    itemSource = new QStandardItem();
    itemSource->setText(buffDataText);
    itemSource->setToolTip(buffDataText);
    _popupModel->setItem(rowNum, column++, itemSource);
}

void StatusBarInfoWidget::removeStatusElementInModel(const QModelIndex &index)
{
    _popupModel->removeRow(index.row());
}

void StatusBarInfoWidget::addStatusElement(SystemGuiCorePopUpElement *popupElement, bool autoClose)
{
    QMutexLocker a(&_mutex);

    if(_dataWindow == nullptr)
        _actionElements++;

    int posX = 0;
    int posY = 0;

    StatusBarInfoWidgetPopupWindow* buffWindowInfo = new StatusBarInfoWidgetPopupWindow(popupElement, autoClose, this);
    buffWindowInfo->setFixedSize(buffWindowInfo->size());
    buffWindowInfo->setWindowFlags(Qt::ToolTip);

    StatusBarInfoWidgetPopupWindow* lastPopUp = getLastPopUp();
    if(lastPopUp == nullptr)
    {
        posX = (_mainWindow->pos().x() + _mainWindow->width()) - buffWindowInfo->width();
        posY = (_mainWindow->pos().y() + _mainWindow->height()) - buffWindowInfo->height() - height()/2;
    }
    else
    {
        //        posX = lastPopUp->pos().x();
        //        posY = lastPopUp->pos().y() - buffWindowInfo->height() /*- 1*/;
        //        if(posY <= 0)
        //        {
        //            posX = lastPopUp->pos().x() - buffWindowInfo->width() /*- 1*/;
        //            posY = (_mainWindow->pos().y() + _mainWindow->height()) - buffWindowInfo->height() - height()/2;
        //        }
        posX = lastPopUp->getPosX();
        posY = lastPopUp->getPosY() - buffWindowInfo->height() /*- 1*/;
        if(posY <= 0)
        {
            posX = lastPopUp->getPosX() - buffWindowInfo->width() /*- 1*/;
            posY = (_mainWindow->pos().y() + _mainWindow->height()) - buffWindowInfo->height() - height()/2;
        }
    }

    _allPopUps.push_back(buffWindowInfo);
    connect(buffWindowInfo, SIGNAL(signalClose(StatusBarInfoWidgetPopupWindow*)), this, SLOT(closePopupWindow(StatusBarInfoWidgetPopupWindow*)));
    connect(buffWindowInfo, SIGNAL(signalPlay(ISystemGuiCorePopUpElement::ISystemGuiCorePopUpType)), this, SLOT(replaySound(ISystemGuiCorePopUpElement::ISystemGuiCorePopUpType)));
    connect(buffWindowInfo, SIGNAL(signalMoved()), this, SLOT(offsetPopups()));

    buffWindowInfo->move(posX, posY);
    buffWindowInfo->show();

    _popupElements.push_back(popupElement);
    addStatusElementInModel(popupElement);
//    if(_alreadyShow == true)
        setPixmap(_activeMakeData);
//    else
//        setPixmap(_notActiveMakeData);

    playSound(popupElement->getElementType());
}

void StatusBarInfoWidget::removeStatusElement(QModelIndex index)
{
    QMutexLocker a(&_mutex);
    Q_UNUSED(a)

    SystemGuiCorePopUpElement* buffPopup = getPopupElementFromModel(index);
    if(buffPopup == nullptr)
        return;

    if(_popupElements.contains(buffPopup) == true)
    {
        int buffIndex = _popupElements.indexOf(buffPopup);
        _popupElements.remove(buffIndex, 1);

        delete buffPopup;
        buffPopup = nullptr;
    }

    removeStatusElementInModel(index);

    if(_popupElements.size() == 0)
    {
        //    if(_alreadyShow == true)
                setPixmap(_activeMakeData);
        //    else
        //        setPixmap(_notActiveMakeData);
    }
    else
    {
        //    if(_alreadyShow == true)
                setPixmap(_activeMakeData);
        //    else
        //        setPixmap(_notActiveMakeData);
    }
}

void StatusBarInfoWidget::removeAllElements()
{
    QMutexLocker a(&_mutex);
    Q_UNUSED(a)

    for(int i = 0; i < _popupElements.size(); i++)
    {
        SystemGuiCorePopUpElement* popupElement = _popupElements.operator [](i);
        delete popupElement;
        popupElement = nullptr;
    }
    _popupElements.clear();

    initModel();

    //    if(_alreadyShow == true)
            setPixmap(_activeMakeData);
    //    else
    //        setPixmap(_notActiveMakeData);
}

void StatusBarInfoWidget::mousePressEvent (QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton)
    {
        if (_dataWindow == nullptr)
        {
            QMdiArea* mdiArea = dynamic_cast<QMdiArea*> (_mainWindow->centralWidget ());
            if (mdiArea != nullptr)
            {
                _actionElements = 0;
                _dataWindow = new StatusBarInfoWidgetData (_popupModel, this);
                connect (_dataWindow, SIGNAL(destroyed()), this, SLOT(closeDataWindow()));
                connect (_dataWindow, SIGNAL(removeElement(QModelIndex)), this, SLOT(removeStatusElement(QModelIndex)));
                connect (_dataWindow, SIGNAL(activateElement(QModelIndex)), this, SLOT(activateStatusElement(QModelIndex)));
                connect (_dataWindow, SIGNAL(removeAllElements()), this, SLOT(removeAllElements()));

                QMdiSubWindow* buffWIndow = mdiArea->addSubWindow (_dataWindow);
                buffWIndow->setAccessibleName (_dataWindow->windowTitle ());
                buffWIndow->resize (_dataWindow->size ());
                buffWIndow->setWindowIcon (_dataWindow->windowIcon ());
                buffWIndow->move (_mainWindow->centralWidget ()->width () - buffWIndow->width (),
                                  _mainWindow->centralWidget ()->height () - buffWIndow->height ());

                _dataWindow->show ();
            }
            else
            {
                _actionElements = 0;
                _dataWindow = new StatusBarInfoWidgetData (_popupModel, this);
                connect (_dataWindow, SIGNAL(destroyed()), this, SLOT(closeDataWindow()));
                connect (_dataWindow, SIGNAL(removeElement(QModelIndex)), this, SLOT(removeStatusElement(QModelIndex)));
                connect (_dataWindow, SIGNAL(activateElement(QModelIndex)), this, SLOT(activateStatusElement(QModelIndex)));
                connect (_dataWindow, SIGNAL(removeAllElements()), this, SLOT(removeAllElements()));

                QDialog *dlg = new QDialog (_mainWindow);
                dlg->setLayout (new QHBoxLayout ());
                dlg->setContentsMargins (2, 2, 2, 2);
                dlg->setAttribute (Qt::WA_DeleteOnClose);
                dlg->layout ()->addWidget (_dataWindow);
                dlg->setWindowTitle (_dataWindow->windowTitle ());
                dlg->setWindowIcon (_dataWindow->windowIcon ());

                dlg->move (_mainWindow->centralWidget ()->width () - dlg->width (),
                           _mainWindow->centralWidget ()->height () - dlg->height ());

                dlg->show ();
                _dataWindow->show ();
            }
        }
        else
            _dataWindow->setFocus ();
    }
}

void StatusBarInfoWidget::closeDataWindow()
{
    _dataWindow = nullptr;
}

void StatusBarInfoWidget::activateStatusElement(QModelIndex index)
{
    QMutexLocker a(&_mutex);
    Q_UNUSED(a)

    SystemGuiCorePopUpElement* buffPopup = getPopupElementFromModel(index);
    if(buffPopup == nullptr)
        return;

    buffPopup->activateElement();
}

void StatusBarInfoWidget::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event)

    if(_popupElements.size() == 0)
        setPixmap(_activeNoData);
    else
        setPixmap(_activeMakeData);

    if(_dataWindow != nullptr)
        return;

    int posX = (_mainWindow->pos().x() + _mainWindow->width()) - _infoToolTip->width() - 30;
    int posY = (_mainWindow->pos().y() + _mainWindow->height()) - _infoToolTip->height() - 5;

    _infoToolTip->show();
    _infoToolTip->move(posX, posY);
    _infoToolTip->setWindowOpacity(0.9);
}

void StatusBarInfoWidget::enterEvent(QEvent *event)
{
    Q_UNUSED(event)

    setStyleSheet("QLabel { background-color: rgba(128, 128, 128, 60); color: black; border: none; border-radius: 10px; }");
}

void StatusBarInfoWidget::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)

    if(_popupElements.size() == 0)
        setPixmap(_notActiveNoData);
    else
        setPixmap(_notActiveMakeData);

    QString styleSheet = "QLabel#shadowLabel {}";
    setStyleSheet(styleSheet);
    _infoToolTip->hide();
}

StatusBarInfoWidgetPopupWindow *StatusBarInfoWidget::getLastPopUp()
{
    if(_allPopUps.size() == 0)
        return nullptr;

    for(int i = _allPopUps.size() - 1; i >= 0; i--) {
        StatusBarInfoWidgetPopupWindow *buffW = _allPopUps[i];
        if(!buffW->isMoved())
            return buffW;
    }
    return nullptr;
}

void StatusBarInfoWidget::closePopupWindow(StatusBarInfoWidgetPopupWindow *window)
{
    _allPopUps.remove(_allPopUps.indexOf(window));
    delete window;
    emit signalOffsetPopups();
}

void StatusBarInfoWidget::offsetPopups()
{
    StatusBarInfoWidgetPopupWindow* lastPopUp = nullptr;
    for(int i = 0; i < _allPopUps.size(); i++) {
        int posX = 0;
        int posY = 0;

        StatusBarInfoWidgetPopupWindow* popUp = _allPopUps[i];
        if(popUp->isMoved())
            continue;

        if(lastPopUp == nullptr) {
            posX = (_mainWindow->pos().x() + _mainWindow->width()) - popUp->width();
            posY = (_mainWindow->pos().y() + _mainWindow->height()) - popUp->height() - height()/2;
        } else {
            posX = lastPopUp->getPosX();
            posY = lastPopUp->getPosY() - popUp->height() /*- 1*/;
            if(posY <= 0)
            {
                posX = lastPopUp->getPosX() - popUp->width() /*- 1*/;
                posY = (_mainWindow->pos().y() + _mainWindow->height()) - popUp->height() - height()/2;
            }
        }
        popUp->startMove(posX, posY);
        lastPopUp = popUp;
    }
}

void StatusBarInfoWidget::clearCount()
{
    _actionElements = 0;
}

void StatusBarInfoWidget::playSound()
{
    if(_isMute)
        return;

    stopPlaySound();

    QByteArray ba = _allSamples.value(_currentSound);
    _soundBuffer.setData(ba);
    _soundBuffer.open(QBuffer::ReadWrite);


    _soundTimerStop.start (HIDE_AND_STOP_POP_UP * 1000);
}

void StatusBarInfoWidget::playSound(ISystemGuiCorePopUpElement::ISystemGuiCorePopUpType type)
{
    if(_isMute)
        return;

    _currentSound = type;
    _soundTimerPlay.start(100); // 0.1 sec
}

void StatusBarInfoWidget::replaySound (ISystemGuiCorePopUpElement::ISystemGuiCorePopUpType type)
{
    if(_isMute)
        return;

    if(type != ISystemGuiCorePopUpElement::ISystemGuiCorePopUpWarning
            && type != ISystemGuiCorePopUpElement::ISystemGuiCorePopUpCritical)
        return;

}

void StatusBarInfoWidget::replaySound()
{
}

void StatusBarInfoWidget::stopPlaySound()
{

    _soundBuffer.close();
    _soundTimerPlay.stop();
    _soundTimerStop.stop();

    if(_replaySounds.size() != 0) {
        _currentSound = _replaySounds[0];
        _replaySounds.remove(0);
        _soundTimerPlay.start(100); // 0.1 sec
    }
}

void StatusBarInfoWidget::addSound()
{
    if(!_replaySounds.contains(_currentSound))
        _replaySounds.push_back(_currentSoundRe);

    _soundTimerAdd.stop();
}

SystemGuiCorePopUpElement *StatusBarInfoWidget::getPopupElementFromModel(const QModelIndex &index)
{
    int rowNum = index.row();
    QStandardItem* tempGuidItem = _popupModel->item(rowNum, 0);

    if(tempGuidItem == nullptr)
        return nullptr;

    QString tempGuid = tempGuidItem->text();

    for(int i = 0; i < _popupElements.size(); i++)
    {
        SystemGuiCorePopUpElement* buffItem = _popupElements.operator [](i);
        if(buffItem->getElementGuid() == tempGuid)
            return buffItem;
    }
    return nullptr;
}

void StatusBarInfoWidget::slotVolumeChanged(bool isMute)
{
    _isMute = isMute;
}
