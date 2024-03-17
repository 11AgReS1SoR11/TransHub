# pragma once

#include <QtWidgets/QWidget>
#include <QApplication>
#include <QString>
#include <QScreen>
#include <QPoint>
#include <QSettings>

//!
//! \file IFloatingWidget.h
//! \class IFloatingWidget
//! \brief Базовый класс для всплывающих окон с запоминание позиции и размера
//!
class IFloatingWidget : public QWidget
{
    Q_OBJECT

public:
explicit IFloatingWidget (QWidget *parent = nullptr)
    : QWidget (parent)
{
    setObjectName ("IFloatingWidget");
}

virtual ~IFloatingWidget () {}

public:
    //! \file IFloatingWidget.h
    //! \brief Сохранение/восстановление геометрии окна
    //! \param show: если true - чтение геометрии из settings; false - запись геометрии
    //! \param groupSetName - наименование подгруппы settings
    //! \param poff - сдвиг от верхнего левого угла parent Widget
    //!
    static bool AdjustWndGeometry(QWidget* w, bool show, const QString& groupSetName = QString ("/floatingGeometry"), QPoint poff = QPoint(0, 0))
    {
        if (w && !w->windowTitle().isEmpty())
        {
            QSettings settings (qApp->organizationName (), qApp->applicationName ());
            QString strregKey = w->windowTitle();
            QString regKey = QString::number(qChecksum(strregKey.toLocal8Bit().data(), strregKey.length()));
            settings.beginGroup (groupSetName);
            if (show)
            {
                QByteArray barr = settings.value(regKey).toByteArray();
                if (barr.count())
                    w->restoreGeometry(barr);
                else if (poff != QPoint(0, 0) && w->parentWidget())
                    w->move(w->parentWidget()->mapToGlobal(poff));
                else
                    w->move(QGuiApplication::screens().at(0)->geometry().center() - w->frameGeometry().center());
                return true;
            }
            else
                settings.setValue(regKey, w->saveGeometry());
            settings.endGroup();
            return true;
        }
        return false;
    }

protected:
void showEvent(QShowEvent *event) override
{
    AdjustWndGeometry(this, true);
  //  if (AdjustGeometry(this, true))
  //         showMaximized();
    QWidget::showEvent(event);
}

void closeEvent(QCloseEvent *event) override
{
    AdjustWndGeometry(this, false);
    QWidget::closeEvent(event);
}

};
