#ifndef _PROCESSINGGUI_H
#define _PROCESSINGGUI_H

#include <IObject/ISystemGuiCore/ISystemGuiCoreParentWidget.h>
#include <IObject/ISystemGuiCore/ISystemGuiCore.h>
#include <IObject/ISystemGuiCore/ISystemGuiCoreMenu.h>
#include <experimental/optional>

//!
//! \brief Класс формирования меню.
//! \author Кромачев Максим
//!

class Q_DECL_IMPORT ProcessingGui : public QObject, public ISystemGuiCoreParentWidget
{
    Q_OBJECT

public:
    //! \brief Конструктор.
    //! \param parent Родительский виджет.
    ProcessingGui (QObject* parent = nullptr);

    //! \brief Деструктор.
    virtual ~ProcessingGui () = default;

    //! \brief Инициализация графического интерфейса компонента.
    void initGui () override;

    //! \brief Вернуть виджет.
    //! \param actionName
    //! \param actionSignature
    //! \param type
    //! \return Виджет
    QWidget * getWidget (const QString& actionName, const QString &actionSignature,
                        WidgetType &type, WidgetShowType &showType) override;

protected:
    //! \brief Получить объект доступа к ядру графического интерфейса.
    ISystemGuiCore *guicore () const;

    //! \brief Получить главное окно приложения.
    ISystemGuiCoreMainWindow *mainWindow () const;
};

#endif // _PROCESSINGGUI_H
