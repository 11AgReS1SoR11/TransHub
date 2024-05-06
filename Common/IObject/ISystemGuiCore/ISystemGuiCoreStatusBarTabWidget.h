#ifndef ISYSTEMGUICORESTATUSBARTABWIDGET_H
#define ISYSTEMGUICORESTATUSBARTABWIDGET_H

#include <QLabel>

#include "ISystemGuiCoreStatusBarTabWindow.h"
#include <QMainWindow>
#include <QVariant>

//!
//! \brief Класс вкладки для окон в статус баре
//! \author Кромачев Тимофей
//!

class ISystemGuiCoreStatusBarTabWidget : public QLabel
{
    Q_OBJECT
public:

    //! \brief Конструктор класса
    //! \param icon Иконка для вкладки
    //! \param parent Виджет-родитель
    explicit ISystemGuiCoreStatusBarTabWidget( QMainWindow* mainWindow, const QPixmap & icon, QWidget * parent = nullptr )
        : QLabel( parent ), _activeIcon( icon )
    {
        setProperty("class", "TabWidget" );
        _mainWindow = mainWindow;
        _infoToolTip = nullptr;
        if ( _activeIcon.isNull() )
            throw std::invalid_argument( "Icon item is NULL" );

        setPixmap( _activeIcon );
        setFixedSize( _activeIcon.width() + 10, _activeIcon.height() + 10 );
        setAlignment(Qt::AlignCenter);
        setMouseTracking(true);
    }

    //! \brief Деструктор класса
    virtual ~ISystemGuiCoreStatusBarTabWidget ()
    {
        emit TabClosed();
    }

    //! \brief Установить иконку
    //! \param icon Базовая иконка
    //! \return Удалось ли установить
    bool SetPixmap ( const QPixmap & icon )
    {
        if ( icon.isNull() )
            return false;

        _activeIcon = icon;
        setPixmap( _activeIcon );
        setFixedSize( _activeIcon.width() + 10, _activeIcon.height() + 10 );
        return true;
    }

    //! \brief Связать вкладку с виджетом в окне
    //! \param StatusBarTabWindow виджет в окне
    virtual void ConnectTabWindow( ISystemGuiCoreStatusBarTabWindow * sbtwin )
    {
        connect( sbtwin, &ISystemGuiCoreStatusBarTabWindow::HideTabWindow, this, &ISystemGuiCoreStatusBarTabWidget::OnHideTabWindow );
        connect( sbtwin, &ISystemGuiCoreStatusBarTabWindow::CloseTabWindow, this, &ISystemGuiCoreStatusBarTabWidget::OnCloseTabWindow );
        connect( this, &ISystemGuiCoreStatusBarTabWidget::TabClicked, sbtwin, &ISystemGuiCoreStatusBarTabWindow::OnTabClicked );
    }

protected:
    //! \brief Метод обработки клика мыши
    //! \param event QMouseEvent
    void mousePressEvent (QMouseEvent * event ) override
    {
        emit TabClicked(false);
        QLabel::mousePressEvent( event );
    }

    //! \brief Метод обработки получения фокуса
    //! \param event QEvent
    void enterEvent (QEvent * event ) override
    {
        QString st = styleSheet();


        Q_UNUSED( event )
        setStyleSheet( "QLabel "
                       "{ "
                       "background-color: rgba(128, 128, 128, 60);"
                       "color: black;"
                       " border: none;"
                       " border-radius: 10px; "
                       "}" );

        if ( _infoToolTip )
        {
            _infoToolTip->setStyleSheet("QLabel{background-color: rgb(245, 245, 235); }");
            _infoToolTip->show();
        }
    }

    //! \brief Метод обработки потери фокуса
    //! \param event QEvent
    void leaveEvent (QEvent * event ) override
    {
        Q_UNUSED( event )
        setStyleSheet( ".TabWidget#shadowLabel {}" );

        if ( _infoToolTip )
        {
            if ( _infoToolTip )
                _infoToolTip->hide();
        }
    }

    QWidget * _infoToolTip; /**< виджет всплывающей*/
    QMainWindow* _mainWindow; /**< основное окно системы */

signals:
    /*!
     * \brief Сигнал, информирующий о нажатии на StatusBarTabWidget
     */
    void TabClicked(bool is_only_open);

    /*!
     * \brief Сигнал, информирующий о закрытии StatusBarTabWidget
     */
    void TabClosed();

protected slots:

    /*!
     * \brief Слот, реагирующий на закрытие связанного виджета
     */
    virtual void OnCloseTabWindow()
    {
        deleteLater();
    }

    /*!
     * \brief Слот, реагирующий на скрытие связанного виджета
     */
    virtual void OnHideTabWindow()
    {
        setStyleSheet( "QLabel "
                       "{ "
                       "background-color: rgba(135, 206, 250, 60);"
                       "color: black;"
                       " border: none;"
                       " border-radius: 10px; "
                       "}" );
    }

private:
    QPixmap _activeIcon;    /**< активная иконка */
};

#endif // ISYSTEMGUICORESTATUSBARTABWIDGET_H
