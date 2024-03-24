#ifndef ABOUTCOMPONENTSWIDGET_H
#define ABOUTCOMPONENTSWIDGET_H

#include <QWidget>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QMutex>

#include <QtGui/QColor>
#include <QtGui/QPalette>
#include <QtGui/QIcon>
#include <QtWidgets/QListWidgetItem>
#include <QtWidgets/QMessageBox>

#include "../MainWindowDefines.h"

QT_BEGIN_NAMESPACE
class QTreeWidgetItem;
QT_END_NAMESPACE

namespace Ui {
class AboutComponentsWidget;
}

//!
//! \file FormAboutPlugins.h
//! \class FormAboutPlugins
//! \brief Класс диалогового окна с отображением всех компонентов и их статусов загрузки.
//! \author Кромачев Максим
//!
class AboutComponentsWidget : public QWidget
{
    Q_OBJECT

public:
    //! \brief Конструктор.
    //! \param parent Родительский виджет.
    explicit AboutComponentsWidget (QWidget *parent = nullptr);

    //! \brief Деструктор.
    ~AboutComponentsWidget ();

public slots:
    //! \brief Слот поиска компонента в дереве
    //! \param text Название
    void findComponent (const QString &text);

private:
    Ui::AboutComponentsWidget *ui {nullptr};  //!< объект графического интерфейса
    QTreeWidgetItem *_lastSelected {nullptr}; //!< последний выбранный элемент

    //! \brief Инициализация.
    void init ();

    //! \brief Очистка списка комопнентов.
    void clean ();

private slots:
    //! \brief Слот выбора элемента
    //! \param current Текущий элемент
    //! \param previous Предыдущий элемент
    void changeCurrentItem (QTreeWidgetItem *current, QTreeWidgetItem *previous);
};

#endif // ABOUTCOMPONENTSWIDGET_H
