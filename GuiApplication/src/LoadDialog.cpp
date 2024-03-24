#include "LoadDialog.h"
#include "ui_LoadDialog.h"

#include <QtCore/QDebug>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QDesktopWidget>
#include <QtGui/QScreen>
#include <QtCore/QTimer>

#include <ComponentsCore5alpha/ComponentManager>
using namespace ComponentsCore;
#include <IObject/ISystemGuiCore/ISystemGuiCore.h>

LoadDialog::LoadDialog (const QFileInfoList &iconsList, QWidget *parent)
    : QDialog (parent)
    , ui (new Ui::LoadDialog)
{
    ui->setupUi (this);
    setWindowFlags (Qt::WindowMinMaxButtonsHint);

    using namespace ComponentsCore;

    connect (ComponentManager::instance (), &ComponentManager::signalInitDone,
             this, &LoadDialog::slotInitDone);
    connect (ComponentManager::instance (), &ComponentManager::signalShowProgressBar,
             this, &LoadDialog::showProgressBar);
    connect (ComponentManager::instance (), &ComponentManager::signalChangeProgressBarMax,
             this, &LoadDialog::changeProgressBarMax);
    connect (ComponentManager::instance (), &ComponentManager::signalChangeProgressBarText,
             this, &LoadDialog::changeProgressBarText);
    connect (ComponentManager::instance (), &ComponentManager::signalChangeProgressBarValue,
             this, &LoadDialog::changeProgressBarValue);

    connect (ComponentManager::instance (), &ComponentManager::signalComponentLoaded,
             this, &LoadDialog::slotComponentLoaded);
    connect (ComponentManager::instance (), &ComponentManager::signalComponentsLoaded,
             this, &LoadDialog::slotComponentsLoaded);
    connect (ComponentManager::instance (), &ComponentManager::signalComponentInitialized,
             this, &LoadDialog::slotComponentInitialized);
    connect (ComponentManager::instance (), &ComponentManager::signalComponentsInitialized,
             this, &LoadDialog::slotComponentsInitialized);

    setWindowIcon (searchLogo (iconsList, QSize (16,16)));
    setWindowTitle (tr ("Loading ..."));
    setFixedHeight (150);
    setFixedWidth (width());

    _infoWidget = new InformWidget (iconsList, width (), this);
    _infoWidget->progBarLoad ()->setMaximum (ComponentManager::instance ()->components ().size ());
    _infoWidget->progBarInit ()->setMaximum (ComponentManager::instance ()->components ().size ());
    _infoWidget->progBarInit ()->setValue (0);
    _infoWidget->progBarLoad ()->setValue(0);
    _infoWidget->progBarOther ()->setValue(0);
    ui->gridLayoutForWidget->addWidget (_infoWidget, 0,0);

    //-- центр окна
    if (auto primary_screen = QGuiApplication::primaryScreen ()) {
        int tempX = primary_screen->geometry ().width ();
        int tempY = primary_screen->geometry ().height();
        setGeometry ((tempX/2 - width()/2), (tempY/2 - height ()), width (), height ());
    }

    //-- формируем инфо-виджет
    _infoWidget->buttonCancel ()->setIcon (QIcon (":/Images/Images/cross.png"));
    _infoWidget->buttonCancel ()->setToolTip (tr ("Cancel"));

    _infoWidget->buttonNext ()->setIcon (QIcon(":/Images/Images/tick.png"));
    _infoWidget->buttonNext ()->setToolTip (tr ("Next"));

    _infoWidget->buttonSettings ()->setIcon (QIcon (":/Images/Images/gear.png"));
    _infoWidget->buttonSettings ()->setToolTip (tr ("Settings"));

    _infoWidget->buttonLog ()->setIcon (QIcon (":/Images/Images/information-white.png"));
    _infoWidget->buttonLog ()->setToolTip (tr ("Information"));

    connect (_infoWidget->buttonNext (), &QToolButton::clicked,
             this, &LoadDialog::clickNext);
    connect (_infoWidget->buttonCancel (), &QToolButton::clicked,
             this, &LoadDialog::clickCancel);
    connect (_infoWidget->buttonLog (), &QToolButton::toggled,
             this, &LoadDialog::clickLogg);
    connect (_infoWidget->buttonSettings (), &QToolButton::toggled,
             this, &LoadDialog::clickSettings);

    ui->checkBoxInitError->setIcon (QIcon (":/Images/Images/arrow-warning-init.png"));
    ui->checkBoxLoadError->setIcon (QIcon (":/Images/Images/arrow-warning-load.png"));

    //-- скрываем элементы
    ui->groupBoxSettings->setVisible (false);
    ui->textEditData->setVisible (false);

    _infoWidget->progBarOther ()->setVisible(false);
    _infoWidget->labelOther ()->setVisible(false);

    _infoWidget->labelInitIcon ()->setVisible(false);
    _infoWidget->labelInitText ()->setVisible(false);

    _infoWidget->labelLoadIcon ()->setVisible(false);
    _infoWidget->labelLoadText ()->setVisible(false);

    _infoWidget->buttonLog ()->setVisible (false);
    _infoWidget->buttonNext ()->setVisible (false);
    _infoWidget->buttonCancel ()->setVisible (false);
    _infoWidget->buttonSettings ()->setVisible (false);

    _infoWidget->progBarLoad ()->setVisible (false);
    _infoWidget->progBarInit ()->setVisible (false);

    loadStart ();
}

LoadDialog::~LoadDialog ()
{
    delete _infoWidget;
    _infoWidget = nullptr;

    delete ui;
}

void LoadDialog::loadStart ()
{
    QSettings settings (qApp->organizationName (), qApp->applicationName ());
    settings.beginGroup (QString ("/dialogLoadComponents"));
    ui->checkBoxLoadError->setChecked (settings.value ("ignoreLoadError", false).toBool());
    ui->checkBoxInitError->setChecked (settings.value ("ignoreInitError", false).toBool());

    int errorNum = settings.value ("errorComponentsNum", 0).toInt();
    for (int i = 0; i < errorNum; i++)
    {
        QString component = settings.value (QString ("errorComponent_%1").arg (i), "").toString ();
        if (component.isEmpty () == false)
            _prevErrors.push_back (component);
    }

    settings.endGroup ();
}

void LoadDialog::saveStart ()
{
    QSettings settings (qApp->organizationName (), qApp->applicationName ());
    settings.beginGroup ("/dialogLoadComponents");

    //-- чистим все в данной ветке
    QStringList allKeysInGroup = settings.allKeys();
    for(int ii = 0; ii < allKeysInGroup.size(); ii++)
        settings.remove(allKeysInGroup.operator [](ii));

    settings.setValue ("ignoreLoadError", ui->checkBoxLoadError->isChecked());
    settings.setValue ("ignoreInitError", ui->checkBoxInitError->isChecked());

    settings.setValue ("errorComponentsNum", _currentErrors.size());
    for (int i = 0; i < _currentErrors.size(); i++)
        settings.setValue (QString("errorComponent_%1").arg(i), _currentErrors.operator [](i));

    settings.endGroup ();
}

QIcon LoadDialog::searchLogo (const QFileInfoList &iconsList, const QSize &iconSize)
{
    for (int i = 0; i < iconsList.size(); i++) {
        QPixmap buffPix = QPixmap(iconsList[i].absoluteFilePath());
        if (buffPix.size () == iconSize)
            return QIcon (buffPix);
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

    return QIcon (":/Images/Images/");
}

void LoadDialog::execDialog ()
{
    QDialog::show ();
    QTimer::singleShot (100, this, &LoadDialog::slotStart);
}

void LoadDialog::slotStart ()
{
    //переделать:
    // 1 конектимся к менеджеру
    // 2 startup и ждём сигнала signalStartSuccess

    using namespace ComponentsCore;
    if (ComponentManager::instance () == nullptr)
    {
        qCritical () << "[LoadDialog][slotStart] Internal error: Could not find 'ComponentManager'";

        QMessageBox msgBox;
        msgBox.setText ("Ошибка менеджера комопнентов!\n\nЗапуск остановлен!");
        msgBox.setWindowTitle ("Ошибка менеджера компонентов!");
        msgBox.setWindowIcon (QIcon (":/Images/Images/phoenix_icon__v2_16x16.png"));
        msgBox.setIcon (QMessageBox::Warning);
        msgBox.setStandardButtons (QMessageBox::Ok);
        msgBox.exec ();

        reject ();
    }

    ui->textEditData->append ("<b>Загрузка компонентов:</b>\n");

    ComponentManager::instance ()->startup ();

//    if (!ComponentManager::instance()->startup ())
//    {
//        ui->textEditData->append(tr("<b><FONT COLOR=RED>Ошибка инициализации комплекса!</FONT></b>\n"));

//        QPixmap icon(":/Images/Images/cross.png");
//        _infoWidget->getLabelInitDataIcon()->setPixmap(icon);
//        _infoWidget->getLabelInitDataText()->setText(tr("Ошибка инициализации комплекса!"));

//        _infoWidget->getLabelInitDataIcon()->setVisible(true);
//        _infoWidget->getLabelInitDataText()->setVisible(true);

//        _infoWidget->getButtonLog()->setVisible(true);

//        _infoWidget->getProgBarLoad()->setVisible(false);

//        _infoWidget->buttonCancel()->setVisible(true);
//        return;
//    }

//    if (_initError == true || _loadError == true)
//    {
//        _infoWidget->getButtonNext()->setVisible(true);
//        _infoWidget->buttonCancel()->setVisible(true);
//        _infoWidget->getButtonLog()->setVisible(true);
//        _infoWidget->getButtonSettings()->setVisible(true);

//        // если есть ошибки старта
//        if(_loadError == true &&
//           ui->checkBoxLoadError->isChecked() == false)
//        {
//        }

//        else if(_initError == true &&
//                ui->checkBoxInitError->isChecked() == false)
//        {
//        }

//        else if(_newError > 0)
//        {
//        }

//        else if(_updateError > 0)
//        {
//        }
//        else
//            clickNext ();
//    }
//    else
//        clickNext ();
}

void LoadDialog::slotComponentLoaded (const QString &componentName, const QString &componentVer,
                                      bool status, const QString &error)
{
    _infoWidget->progBarLoad ()->setVisible (true);
    _infoWidget->progBarLoad ()->setValue (_infoWidget->progBarLoad ()->value () + 1);

    //-- поиск спецификации компонента
    ComponentsCore::ComponentSpec *spec {nullptr};
    auto list = ComponentManager::instance ()->components ();
    for (auto cm : qAsConst (list)) {
        if (cm->name () == componentName) {
            spec = cm;
            break;
        }
    }

    if (status)
    {
        if (spec)
            ui->textEditData->append (QString ("<b><FONT COLOR=GREEN>Загружен: </FONT></b> %1 (версия: %2)\n")
                                      .arg (spec->description (), componentVer));
        else
            ui->textEditData->append (QString ("<b><FONT COLOR=GREEN>Загружен: </FONT></b> %1 (версия: %2)\n")
                                      .arg (componentName, componentVer));
    }
    else {
        if (spec)
        {
            if (!error.isEmpty ())
                ui->textEditData->append (QString ("<b><FONT COLOR=RED>Ошибка загрузки компонента: %1 (версия: %2): </FONT></b>\n<FONT COLOR=RED>%3</FONT>\n")
                                          .arg (spec->description (), componentVer, error));
            else
                ui->textEditData->append (QString ("<b><FONT COLOR=RED>Ошибка загрузки компонента: %1 (версия: %2)</FONT></b>\n")
                                          .arg (spec->description (), componentVer));
        }
        else
        {
            if (!error.isEmpty ())
                ui->textEditData->append (QString ("<b><FONT COLOR=RED>Ошибка загрузки компонента: %1 (версия: %2): </FONT></b>\n<FONT COLOR=RED>%3</FONT>\n")
                                          .arg (componentName, componentVer, error));
            else
                ui->textEditData->append (QString ("<b><FONT COLOR=RED>Ошибка загрузки компонента: %1 (версия: %2)</FONT></b>\n")
                                          .arg (componentName, componentVer));
        }

        _loadError = true;

        if (_prevErrors.contains (componentName) == false)
            _newError++;

        _currentErrors.push_back (componentName);
    }
}

void LoadDialog::slotComponentsLoaded ()
{
    _infoWidget->progBarLoad ()->setVisible (false);

    _infoWidget->labelLoadIcon ()->setVisible (true);
    _infoWidget->labelLoadText ()->setVisible (true);

    if (_loadError) {
        QPixmap icon (":/Images/Images/cross.png");
        _infoWidget->labelLoadIcon ()->setPixmap (icon);
        _infoWidget->labelLoadText ()->setText (tr ("Сomponents loaded error"));
    } else {
        QPixmap icon (":/Images/Images/tick.png");
        _infoWidget->labelLoadIcon ()->setPixmap (icon);
        _infoWidget->labelLoadText ()->setText (tr ("Components loaded successfully"));
    }

    ui->textEditData->append (QString ());
    ui->textEditData->append ("\n<b>Инициализация компонентов:</b>\n");
}

void LoadDialog::slotComponentInitialized (const QString &componentName, const QString &componentVer,
                                           bool status, const QString &error)
{
    _infoWidget->progBarInit ()->setVisible (true);
    _infoWidget->progBarInit ()->setValue (_infoWidget->progBarInit ()->value () + 1);

    //-- поиск спецификации компонента
    ComponentsCore::ComponentSpec *spec {nullptr};
    auto list = ComponentManager::instance ()->components ();
    for (auto cm : qAsConst (list)) {
        if (cm->name () == componentName) {
            spec = cm;
            break;
        }
    }

    if (status == true)
    {
        if (spec)
            ui->textEditData->append (QString ("<b><FONT COLOR=GREEN>Запущен: </FONT></b> %1 (версия: %2)\n")
                                      .arg (spec->description (), componentVer));
        else
            ui->textEditData->append (QString ("<b><FONT COLOR=GREEN>Запущен: </FONT></b> %1 (версия: %2)\n")
                                      .arg (componentName, componentVer));
    }
    else
    {
        if (error.isEmpty () == false)
        {
            if (spec) {
                ui->textEditData->append (QString ("<b><FONT COLOR=RED>Ошибка запуска: %1 (версия: %2)</FONT></b>\n")
                                          .arg (spec->description (), componentVer));
                ui->textEditData->append (QString ("<FONT COLOR=RED>Ошибка: %1</FONT>\n")
                                          .arg (error));
            } else {
                ui->textEditData->append (QString ("<b><FONT COLOR=RED>Ошибка запуска: %1 (версия: %2)</FONT></b>\n")
                                          .arg (componentName, componentVer));
                ui->textEditData->append (QString ("<FONT COLOR=RED>&nbsp;Ошибка: %1</FONT>\n")
                                          .arg (error));
            }
        }
        else
        {
            if (spec)
                ui->textEditData->append (QString ("<b><FONT COLOR=RED>Ошибка запуска: </FONT></b> <FONT COLOR=RED>%1 (версия: %2)</FONT>\n")
                                          .arg (spec->description (), componentVer));
            else
                ui->textEditData->append (QString ("<b><FONT COLOR=RED>Ошибка запуска: </FONT></b> <FONT COLOR=RED>%1 (версия: %2)</FONT>\n")
                                          .arg (componentName, componentVer));
        }

        _initError = true;

        if (_prevErrors.contains (componentName) == false)
            _newError++;

        _currentErrors.push_back (componentName);
    }
}

void LoadDialog::slotComponentsInitialized ()
{
    _infoWidget->progBarInit ()->setVisible (false);

    _infoWidget->labelInitIcon ()->setVisible (true);
    _infoWidget->labelInitText ()->setVisible (true);

    if (_initError) {
        QPixmap icon (":/Images/Images/cross.png");
        _infoWidget->labelInitIcon ()->setPixmap (icon);
        _infoWidget->labelInitText ()->setText (tr ("Сomponents initialization error"));
    } else {
        QPixmap icon (":/Images/Images/tick.png");
        _infoWidget->labelInitIcon ()->setPixmap (icon);
        _infoWidget->labelInitText ()->setText (tr ("Components initialization successful"));
    }
}

//void LoadDialog::reject () {
//    QDialog::reject ();
//}

void LoadDialog::clickNext ()
{
    ISystemGuiCore* guiCore = ComponentManager::getObject<ISystemGuiCore> ();
    if (guiCore == nullptr)
    {
        qCritical () << "[LoadDialog][clickNext] Could not find 'ISystemGuiCore'";

        QMessageBox msgBox;
        msgBox.setText ("Не найден компонент ядра графического интерфейса системы!\n\n\t    Запуск остановлен!");
        msgBox.setWindowTitle ("Ошибка ядра графического интерфейса!");
        msgBox.setWindowIcon (QIcon (":/Images/Images/phoenix_icon__v2_16x16.png"));
        msgBox.setIcon (QMessageBox::Warning);
        msgBox.setStandardButtons (QMessageBox::Ok);
        msgBox.exec ();

        reject ();
    }

     accept ();
}

void LoadDialog::clickCancel () {
    reject ();
}

void LoadDialog::clickLogg (bool value)
{
    if (value == true)
        setFixedHeight (height () + 240);
    else
        setFixedHeight (height () - 240);

    ui->textEditData->setVisible (value);
}

void LoadDialog::clickSettings (bool value)
{
    if (value == true)
        setFixedHeight (height () + 86);
    else
        setFixedHeight (height () - 86);

    ui->groupBoxSettings->setVisible (value);
}

void LoadDialog::slotInitDone ()
{
    //-- deprecated
    //    if (_loadError && _initError)
    //    {
    //        if (_newError == 0)
    //        {
    //            QPixmap icon (":/Images/Images/exclamation--frame.png");
    //            _infoWidget->getLabelInitDataIcon ()->setPixmap (icon);
    //            _infoWidget->getLabelInitDataText ()->setText (tr ("Выполнена с ошибками!"));
    //        }
    //        else
    //        {
    //            QPixmap icon (":/Images/Images/exclamation-red.png");
    //            _infoWidget->getLabelInitDataIcon ()->setPixmap (icon);
    //            _infoWidget->getLabelInitDataText ()->setText (tr ("Выполнена с ошибками! Новых ошибок: %1").arg (_newError));
    //        }
    //    }
    //    else
    //    {
    //        QPixmap icon (":/Images/Images/tick.png");
    //        _infoWidget->getLabelInitDataIcon ()->setPixmap (icon);
    //        _infoWidget->getLabelInitDataText ()->setText (tr ("Выполнена успешно."));
    //    }

    //    _infoWidget->getLabelInitDataIcon ()->setVisible (true);
    //    _infoWidget->getLabelInitDataText ()->setVisible (true);

    //    _infoWidget->getProgBarLoad ()->setVisible (false);
    //    _infoWidget->getProgBarInit ()->setVisible (false);
    //-- deprecated

    if (_loadError || _initError)
    {
        _infoWidget->buttonNext ()->setVisible (true);
        _infoWidget->buttonCancel ()->setVisible (true);
        _infoWidget->buttonLog ()->setVisible (true);
        _infoWidget->buttonSettings ()->setVisible (true);

        if (_loadError == true &&
                ui->checkBoxLoadError->isChecked () == false)
        {}

        else if (_initError == true &&
                 ui->checkBoxInitError->isChecked () == false)
        {}

        else if (_newError > 0)
        {}

        else if (_updateError > 0)
        {}
    }

    if (!_loadError && !_initError)
        accept ();
}


void LoadDialog::showProgressBar (bool isShow)
{
    if (isShow) {
        _infoWidget->progBarOther ()->setMaximum (0);
        _infoWidget->progBarOther ()->setValue (0);
        _infoWidget->labelOther ()->setText ("");
        _infoWidget->labelOther ()->show ();
        _infoWidget->progBarOther ()->show ();
    } else {
        _infoWidget->progBarOther ()->hide ();
        _infoWidget->labelOther ()->hide ();
    }
}

void LoadDialog::changeProgressBarMax (int maxValue)
{
    _infoWidget->progBarOther ()->setValue (0);
    _infoWidget->progBarOther ()->setMaximum (maxValue);
}

void LoadDialog::changeProgressBarText (const QString &text)
{
    _infoWidget->labelOther ()->setText (text);
}

void LoadDialog::changeProgressBarValue (int value)
{
    _infoWidget->progBarOther ()->setValue (value);
}
