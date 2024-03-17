#ifndef IHELPDESIGNER_H
#define IHELPDESIGNER_H

#include <QtCore/QString>

#include "IHelpSection.h"

//!
//! \file IHelpDesigner.h
//! \class IHelpDesigner
//! \brief Базовый класс конструктора файла справки
//!
class IHelpDesigner
{
public:
    //! \brief Деструктор класса
    virtual ~IHelpDesigner() {}

    //! \brief Запросить раздел файла справки
    //!        Если раздел не найден, то он будет создан.
    //! \param title Заголовок раздела
    //! \return IHelpSection
    virtual IHelpSection* helpSection (const QString &title) = 0;

    //! \brief Добавить раздел в файл справки
    //! \param section IHelpSection
    //! \return true / false
    virtual bool addSectionInHelp (IHelpSection *section) = 0;
};

#endif // IHELPDESIGNER_H
