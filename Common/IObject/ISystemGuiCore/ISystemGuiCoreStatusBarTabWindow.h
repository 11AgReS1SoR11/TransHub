#ifndef ISYSTEMGUICORESTATUSBARTABWINDOW_H
#define ISYSTEMGUICORESTATUSBARTABWINDOW_H

#include <QWidget>
#include <experimental/optional>

//!
//! \brief Класс оконных виджетов для вкладок
//! \author Кромачев Тимофей
//!

class ISystemGuiCoreStatusBarTabWindow : public QWidget
{
    Q_OBJECT
public:
    //! \brief Конструктор класса
    //! \param parent Виджет-родитель
    explicit ISystemGuiCoreStatusBarTabWindow( QWidget *parent = nullptr )
        : QWidget{ parent }
    {
    }

    //! \brief Деструктор класса
    virtual ~ISystemGuiCoreStatusBarTabWindow ()
    {
        emit CloseTabWindow( objectName() );
    }

    //! \brief Получение иконки для вкладки
    //! \return соответствующая иконка
    virtual QPixmap GetWindowIcon() = 0;

signals:
    /*!
     * \brief Сигнал, информирующий о скрытии StatusBarTabWindow
     */
    void HideTabWindow();

    /*!
     * \brief Сигнал, для сохранения настроек положения окна, содержащего StatusBarTabWindow
     */
    void SaveTabWindowSettings( const QString &, std::experimental::optional<QPair<QByteArray, QPoint>> );

    /*!
     * \brief Сигнал, информирующий о закрытии StatusBarTabWindow
     */
    void CloseTabWindow( const QString & );

    void OpenTabWindow();

public slots:
    virtual void OnTabClicked(bool is_only_open) = 0;
};

#endif // ISYSTEMGUICORESTATUSBARTABWINDOW_H
