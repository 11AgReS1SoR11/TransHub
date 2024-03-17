#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QtCore/QDebug>
#include <QtCore/QFileInfoList>
#include <QtCore/QCoreApplication>
#include <QtWidgets/QApplication>
#include <QtCore/QDir>
#include <QtCore/QTranslator>
#include <QtCore/QLibraryInfo>
#include <QtNetwork/QNetworkInterface>


//! \brief Разбор аргументов конкретного компонента (component-args).
//! \param string Значение component-args.
//! \return Список аргументов, где первый параметр должен соотсветсвовать названию компонента.
QStringList parse (const QString &string)
{
    QStringList arguments;
    int index {0};

    //-- пропуск пробелов
    while (index < string.size () && string[index].isSpace ())
        index++;

    //-- название компонента
    QString componentName;
    while (index < string.size () && !string[index].isSpace ())
        componentName += string[index++];
    arguments.append (componentName);

    //-- пропуск пробелов
    while (index < string.size () && string[index].isSpace ())
        index++;

    while (index < string.size ())
    {
        if (string[index] == '"')
        {
            index++;
            QString arg;
            while (index < string.size () && string[index] != '"')
                arg += string[index++];

            //-- даже пустую строку добавляем, так как это может трактоваться как пустой параметр
            arguments.append (arg);

            if (index < string.size () && string[index] == '"')
                index++;

            //-- пропуск пробелов
            while (index < string.size () && string[index].isSpace ())
                index++;
        }
        else
        {
            QString arg;
            while (index < string.size () && !string[index].isSpace () && string[index] != '=')
                arg += string[index++];

            arguments.append (arg);

            if (index < string.size () && string[index] == '=')
                index++;

            //-- пропуск пробелов
            while (index < string.size () && string[index].isSpace ())
                index++;
        }
    }

    return arguments;
}

//! \brief Разбор аргументов.
//! \param string Входная строка.
//! \return Перечень аргументов.
QMap<QString, QString> parse1 (const QString &string)
{
    int index {0};
    QMap<QString, QString> arguments;

    //-- пропуск пробелов
    while (index < string.size () && string[index].isSpace ())
        index++;

    //--
    while (index < string.size ())
    {
        //-- аргумент начинается с символа '-'
        if (string[index] != '-')
            return arguments;

        //-- аргумент
        QString arg;
        while (index < string.size () && !string[index].isSpace () && string[index] != '=')
            arg += string[index++];

        if (index < string.size () && string[index] == '=')
            index++;

        //-- пропуск пробелов
        while (index < string.size () && string[index].isSpace ())
            index++;

        //-- аргумент начинается с символа '-'
        if ((index < string.size () && string[index] == '-') || index == string.size ()) {
            arguments.insert (arg, "");
            continue;
        }

        //-- значение аргумента
        QString value;
        if (index < string.size ())
        {
            QVector<QChar> simbols {'"', '\''};
            if (simbols.indexOf (string[index]) != -1)
            {
                auto sim = string[index];

                index++;
                while (index < string.size () && string[index] != sim)
                    value += string[index++];

                if (index < string.size () && string[index] == sim)
                    index++;

                //-- ошибка - конец строки но не ожидаемый символ
                if (index == string.size () && string[index] != sim)
                    return arguments;

                //-- пропуск пробелов
                while (index < string.size () && string[index].isSpace ())
                    index++;
            }
            else
            {
                while (index < string.size () && !string[index].isSpace ())
                    value += string[index++];

                if (index < string.size () && string[index] == '=')
                    index++;

                //-- пропуск пробелов
                while (index < string.size () && string[index].isSpace ())
                    index++;
            }
        }

        arguments.insert (arg, value);
    }

    return arguments;
}

//! \brief Разбор аргументов конкретного компонента (component-args).
//! \param string Значение component-args.
//! \param[out] component Название компонента.
//! \param[out] arguments Перечень аргументов.
//! \return true/false
bool parse1 (const QString &string, QString &component, QMap<QString, QString> &arguments)
{
    int index {0};

    //-- пропуск пробелов
    while (index < string.size () && string[index].isSpace ())
        index++;

    //-- название компонента
    while (index < string.size () && !string[index].isSpace ())
        component += string[index++];

    //-- пропуск пробелов
    while (index < string.size () && string[index].isSpace ())
        index++;

    //-- первый символ первого аргумента
    if (index < string.size () && string[index] != '-')
        return false;

    arguments = parse1 (string.mid (index));
    return true;
}

//! \brief Загрузка иконок из папки с ресурсами.
//! \return Список иконок.
QFileInfoList loadIcons ()
{
    QFileInfoList lst;
    QDir iconspath (qApp->applicationDirPath ());
    if (iconspath.cd ("resources/icons"))
        lst = iconspath.entryInfoList ({"*.png", "*.jpg", "*.jpeg", "*.bmp"}, QDir::Files);
    return lst;
}

//! \brief Загрузка и инсталяция файлов переводов.
//! \param app Приложение.
//! \return Список файлов переводов.
QList<QTranslator*> loadAndInstallTr (QApplication *app)
{
    QList<QTranslator*> translators;

    //-- set translator for the default widget's text (for example: QMessageBox's buttons)
    QTranslator *qtTranslator = new QTranslator (app);
    if (qtTranslator->load ("qt_ru", QLibraryInfo::location (QLibraryInfo::TranslationsPath))) {
        if (!app->installTranslator (qtTranslator))
            qWarning () << "[main] Could not installe translation 'qt_ru'";
        else {
            qInfo () << "[main] Successfuly loaded and installed translation 'qt_ru'";
            //translators.append (qtTranslator); //--> почему то при удалении этого перевода SIGFAULT
        }
    } else {
        qWarning () << "[main] Could not load translations file 'qt_ru'";
        delete qtTranslator;
    }

    //-- файлы переводов в папке с ресурсами
    QFileInfoList trfiles;
    QDir trDir (qApp->applicationDirPath ());
    if (trDir.cd ("resources/translations"))
        trfiles = trDir.entryInfoList ({"*.qm"}, QDir::Files);

    for (int i = 0; i < trfiles.size (); ++i)
    {
        QTranslator *translator = new QTranslator ();
        if (translator->load (trfiles[i].absoluteFilePath ()))
        {
            if (app->installTranslator (translator)) {
                translators.append (translator);
                qInfo () << "[main] Successfuly loaded and installed translation"
                         << trfiles[i].fileName ();
            } else {
                qWarning () << "[main] Could not install translation file"
                            << trfiles[i].fileName ();
                delete translator;
            }
        }
        else {
            qWarning () << "[main] Could not load translations file"
                        << trfiles[i].fileName ();
            delete translator;
        }
    }

    return translators;
}


#endif // FUNCTIONS_H
