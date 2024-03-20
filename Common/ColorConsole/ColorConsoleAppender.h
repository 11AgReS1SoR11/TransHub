#ifndef COLORCONSOLEAPPENDER_H
#define COLORCONSOLEAPPENDER_H

#include <log4qt/writerappender.h>

class QTextStream;

//!
//! \brief Класс вывода Qt сообщений в консоль с подсветкой текста.
//! \author Горбачёв Иван
//!
class ColorConsoleAppender : public Log4Qt::WriterAppender
{
    Q_OBJECT
    Q_PROPERTY(QString target READ target WRITE setTarget)

public:
    enum Target
    {
        STDOUT_TARGET,
        STDERR_TARGET
    };
#if QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
    Q_ENUMS(Target)
#else
    Q_ENUM(Target)
#endif

    //! \brief Конструктор.
    //! \param pParent Родительский объект.
    explicit ColorConsoleAppender (QObject *pParent = nullptr);

    //! \brief Конструктор.
    //! \param pLayout Компоновщик.
    //! \param pParent Родительский объект.
    explicit ColorConsoleAppender (const Log4Qt::LayoutSharedPtr &pLayout,
                                   QObject *pParent = nullptr);

    //! \brief Конструктор.
    //! \param pLayout Компоновщик.
    //! \param rTarget Определение вывода.
    //! \param pParent Родительский объект.
    explicit ColorConsoleAppender (const Log4Qt::LayoutSharedPtr &pLayout,
                                   const QString &rTarget, QObject *pParent = nullptr);

    //! \brief Конструктор.
    //! \param pLayout Компоновщик.
    //! \param target Определение вывода.
    //! \param pParent Родительский объект.
    ColorConsoleAppender (const Log4Qt::LayoutSharedPtr &pLayout,
                          Target target, QObject *pParent = nullptr);

    //! \brief Деструткор.
    virtual ~ColorConsoleAppender ();

public:
    QString target () const;
    void setTarget (const QString &rTarget);
    void setTarget (Target target);

    virtual void activateOptions () override;
    virtual void close () override;

protected:
    virtual void append (const Log4Qt::LoggingEvent &rEvent) override;
    void closeStream ();

private:
    volatile Target _target;
    QTextStream *_textStream {nullptr};

    Q_DISABLE_COPY (ColorConsoleAppender)
};

#endif // _CONSOLEAPPENDER_H
