#include "AboutSystemDialog.h"
#include "ui_AboutSystemDialog.h"

#include <ComponentsCore5alpha/ComponentManager>
using namespace ComponentsCore;

#include <QtWidgets/QDesktopWidget>
#include <QtGui/QtEvents>
#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QDebug>

#include <QtGui/QFont>
#include <QtGui/QFontMetrics>

AboutSystemDialog::AboutSystemDialog (const QPixmap &iconLogo, const QString &windowTitle, QWidget *parent)
    : QDialog (parent)
    , ui (new Ui::AboutSystemDialog)
{
    ui->setupUi (this);
    setAttribute (Qt::WA_DeleteOnClose);

    ui->_labelIcon->setPixmap (iconLogo);

    ui->_btnClose->setIcon (QIcon (":/icons/icons/icon_cancel.png"));
    connect (ui->_btnClose, &QPushButton::clicked, this, &AboutSystemDialog::accept);

    int invalidComponents = 0;
    int readComponents = 0;
    int resolvedComponents = 0;
    int loadedComponents = 0;
    int initializedComponents = 0;
    int runningComponents = 0;
    int stoppedComponents = 0;
    int deletedComponents = 0;

    using namespace ComponentsCore;
    const QList<ComponentSpec*> plugins = ComponentManager::components ();
    for (ComponentSpec *plugin : plugins)
    {
        if (!plugin) {
            qWarning () << "[AboutSystemDialog][AboutSystemDialog] Null ComponentSpec pointer";
            continue;
        }

        //-- Invalid,        //!< Не корретный
        //-- Read,           //!< Считана метаинформация
        //-- Resolved,       //!< Разрешены все зависимости
        //-- Loaded,         //!< Загружен
        //-- Initialized,    //!< Проинициализирован
        //-- Running,
        //-- Stopped,        //!< Остановлен
        //-- Deleted

        if (ComponentSpec::Invalid == plugin->state ())
            invalidComponents++;
        else if (ComponentSpec::Read == plugin->state ())
            readComponents++;
        else if (ComponentSpec::Resolved == plugin->state ())
            resolvedComponents++;
        else if (ComponentSpec::Loaded == plugin->state ())
            loadedComponents++;
        else if (ComponentSpec::Initialized == plugin->state ())
            initializedComponents++;
        else if (ComponentSpec::Running == plugin->state ())
            runningComponents++;
        else if (ComponentSpec::Stopped == plugin->state ())
            stoppedComponents++;
        else if (ComponentSpec::Deleted == plugin->state ())
            deletedComponents++;
        else
            qWarning () << qPrintable (QString ("[AboutSystemDialog][AboutSystemDialog] Unknown component '%1' state '%2'")
                                       .arg (plugin->name ())
                                       .arg (plugin->state ()));
    }

    (void)readComponents;
    (void)resolvedComponents;
    (void)initializedComponents;

    //    QString textDataVersion = tr (COMPONENT_SYSTEM_GUI_CORE_NG_NAME)
    //            + tr (" - версия: ")
    //            + tr (COMPONENT_SYSTEM_GUI_CORE_NG_VERSION);
    //QString textDataVersion;

    QString textData = windowTitle + "\n\n" +
            //textDataVersion + "\n\n" +
            tr ("Components status:") + "\n" +
            tr ("Loaded:") + " " + QString::number (loadedComponents) + "\n" +
            tr ("Running:") + " " + QString::number (runningComponents) + "\n" +
            tr ("Invalid:") + " " + QString::number (invalidComponents) + "\n" +
            tr ("Stopped:") + " " + QString::number (stoppedComponents) + "\n" +
            tr ("Deleted:") + " " + QString::number (deletedComponents) + "\n\n" +
            "© 2021-" + QString::number (QDateTime::currentDateTime ().date ().year ()) + " " + tr ("Inteltech Inc.");
    ui->_labelText->setText (textData);

    //        QFont fontDataText;
    //        fontDataText.fromString (textDataVersion);
    //        QFontMetrics fontDataMetrics (fontDataText);
    //        int w = fontDataMetrics.width (textDataVersion);

    //    QFont fontDataText;
    //    fontDataText.fromString (textData);
    //    QFontMetrics fontDataMetrics (textData);
    //    int w = fontDataMetrics.width (textData);

    QFontMetrics fm = qApp->fontMetrics ();
    QRect pixelR = fm.boundingRect (qApp->desktop ()->availableGeometry (), Qt::TextWordWrap, textData);
    //resize (iconLogo.width () + pixelW + 20 + 20, height ());
    resize (iconLogo.width () + pixelR.width () + 100 + 20, qMax (iconLogo.height (), pixelR.height () + 100));
}

AboutSystemDialog::~AboutSystemDialog () {
    delete ui;
}

void AboutSystemDialog::keyPressEvent (QKeyEvent *e)
{
    if (e->key () == Qt::Key_Escape)
        e->ignore ();
    else
        e->accept ();
}
