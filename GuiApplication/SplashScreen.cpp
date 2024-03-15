#include "SplashScreen.h"
#include "LoadDialog.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QProgressBar>

#include <Application.h>

SplashScreen::SplashScreen (const QFileInfoList &iconsList, Application *app, QWidget *parent)
    : QSplashScreen (parent)
    , _iconsList (iconsList)
{
    QPixmap logoImage = searchLogo (iconsList, QSize (128,128));
    setPixmap (logoImage);

    setAutoFillBackground (false);
    setAttribute (Qt::WA_NoBackground, true);
    setMask (logoImage.mask ());

    Qt::WindowFlags flags = 0;
    flags |= Qt::CustomizeWindowHint;
    flags |= Qt::WindowStaysOnTopHint;

    setWindowFlags (flags);
    _app = app;
}

SplashScreen::~SplashScreen () {
    _app = nullptr;
}

void SplashScreen::loadData (int userMode, const QString &userLogin)
{
    ISystemGuiCore *guiCore = ComponentManager::getObject<ISystemGuiCore> ();
    if (guiCore == nullptr)
    {
        qCritical () << "[SplashScreen][loadData] Could not find 'ISystemGuiCore'";

        QMessageBox msgBox;
        msgBox.setText (tr ("Не найден компонент ядра графического интерфейса системы!\n\n\t    Запуск остановлен!"));
        msgBox.setWindowTitle (tr ("Ошибка ядра графического интерфейса!"));
        msgBox.setWindowIcon (QIcon (":/Images/Images/phoenix_icon__v2_16x16.png"));
        msgBox.setIcon (QMessageBox::Warning);
        msgBox.setStandardButtons (QMessageBox::Ok);
        msgBox.exec ();

        emit loadError ();
        close ();
        return;
    }

    // -- задаем иконки логотипа
    ISystemIconManager* iconManager = guiCore->getIconManager ();
    for (int i = 0; i < _iconsList.size (); i++) {
        QPixmap buffPix = QPixmap (_iconsList[i].absoluteFilePath ());
        iconManager->setSystemLogo (buffPix.size (), QIcon (buffPix));
    }

    // -- запускаем приложение
    connect (guiCore, &ISystemGuiCore::finishInitGui, this, &SplashScreen::continueWhile);

    // -- инициализируем Gui
    _initGuiCore = false;

    if (!QMetaObject::invokeMethod (guiCore, "initGui", Q_ARG (int,userMode), Q_ARG (QString,userLogin)))
        qCritical () << "[SplashScreen][loadData] Could not execute 'QMetaObject::invokeMethod'";

    while (!_initGuiCore)
        QCoreApplication::processEvents ();

    QVector <ISystemGuiCoreMainWindow*> windowsVector = guiCore->getMainWindows ();
    if (windowsVector.size () != 0)
    {
        for (int i = 0; i < windowsVector.size (); i++)
            windowsVector[i]->show ();
    }
    else
    {
        qCritical () << "[SplashScreen][loadData] Could not find 'ISystemGuiCoreMainWindow'";

        QMessageBox msgBox;
        msgBox.setText (tr ("Не найдены конфигурации для главных окон системы!\nПроверьте конфигурационный файл!\n\n                     Запуск остановлен!"));
        msgBox.setWindowTitle (tr ("Ошибка конфигурации!"));
        msgBox.setWindowIcon (QIcon (":/Images/Images/phoenix_icon__v2_16x16.png"));
        msgBox.setIcon (QMessageBox::Warning);
        msgBox.setStandardButtons (QMessageBox::Ok);
        msgBox.exec ();

        emit loadError ();
        close ();
        return;
    }

    close ();
}

QPixmap SplashScreen::searchLogo (const QFileInfoList &iconsList, const QSize &iconSize)
{
    for (int i = 0; i < iconsList.size(); i++) {
        QPixmap buffPix = QPixmap (iconsList[i].absoluteFilePath());
        if (buffPix.size () == iconSize)
            return buffPix;
    }

    if (iconSize == QSize(16,16))
        return QPixmap (":/Images/Images/phoenix_logo_16.png");
    if (iconSize == QSize(32,32))
        return QPixmap (":/Images/Images/phoenix_logo_32.png");
    if (iconSize == QSize(64,64))
        return QPixmap (":/Images/Images/phoenix_logo_64.png");
    if (iconSize == QSize(128,128))
        return QPixmap (":/Images/Images/phoenix_logo_128.png");
    if (iconSize == QSize(256,256))
        return QPixmap (":/Images/Images/phoenix_logo_256.png");

    return QPixmap (":/Images/Images/");
}

void SplashScreen::show (int userMode, const QString &userLogin)
{
    //TODO: gim mod - перепроверить, отображается ли этот виджет
    QSplashScreen::show ();
    loadData (userMode, userLogin);
}

void SplashScreen::showAndInit () {
    loadData (0, "");
}

void SplashScreen::continueWhile () {
    _initGuiCore = true;
}

bool SplashScreen::eventFilter (QObject *target, QEvent *event)
{
    Q_UNUSED (target)
    Q_UNUSED (event)

    return true; // WTF
}
