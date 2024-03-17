#include "ColorConsoleAppender.h"


#include <log4qt/loggingevent.h>
#include <log4qt/layout.h>
#include <log4qt/helpers/optionconverter.h>

#include <QtCore/QTextStream>

//-- описание цветов в консоли
// https://gist.github.com/JBlond/2fea43a3049b38287e5e9cefc87b2124

//#define CONSOLE_COLOR_WARNING   "\e[1;4;33m"
//#define CONSOLE_COLOR_INFO      "\e[1;4;32m"
//#define CONSOLE_COLOR_CRITICAL  "\e[1;4;31m"
//#define CONSOLE_COLOR_FATAL     "\e[1;4;41m"
#define CONSOLE_COLOR_WARNING   "\e[0;33m"
#define CONSOLE_COLOR_INFO      "\e[0;32m"
#define CONSOLE_COLOR_CRITICAL  "\e[0;31m"
#define CONSOLE_COLOR_FATAL     "\e[1;4;41m"
#define CONSOLE_COLOR_END       "\e[0m"

ColorConsoleAppender::ColorConsoleAppender (QObject *pParent)
    : Log4Qt::WriterAppender (pParent)
    , _target (STDOUT_TARGET)
{}

ColorConsoleAppender::ColorConsoleAppender (const Log4Qt::LayoutSharedPtr &pLayout,
                                            QObject *pParent)
    : Log4Qt::WriterAppender (pLayout, pParent)
    , _target (STDOUT_TARGET)
{}

ColorConsoleAppender::ColorConsoleAppender (const Log4Qt::LayoutSharedPtr &pLayout,
                                            const QString &rTarget,
                                            QObject *pParent)
    : Log4Qt::WriterAppender (pLayout, pParent)
    , _target (STDOUT_TARGET)
{
    setTarget (rTarget);
}

ColorConsoleAppender::ColorConsoleAppender (const Log4Qt::LayoutSharedPtr &pLayout,
                                           Target target,
                                           QObject *pParent)
    : Log4Qt::WriterAppender (pLayout, pParent)
    , _target (target)
    , _textStream (nullptr)
{}

ColorConsoleAppender::~ColorConsoleAppender () {
    ColorConsoleAppender::close ();
}

QString ColorConsoleAppender::target () const
{
    if (_target == STDOUT_TARGET)
        return QLatin1String ("STDOUT_TARGET");
    else
        return QLatin1String ("STDERR_TARGET");
}

void ColorConsoleAppender::setTarget (const QString &rTarget)
{
    bool ok;
    Target target = static_cast<Target>(Log4Qt::OptionConverter::toTarget (rTarget, &ok));
    if (ok)
        setTarget(target);
}

void ColorConsoleAppender::activateOptions ()
{
    QMutexLocker locker (&mObjectGuard);

    closeStream ();

    if (_target == STDOUT_TARGET)
        _textStream = new QTextStream (stdout);
    else
        _textStream = new QTextStream (stderr);
    setWriter (_textStream);

    Log4Qt::WriterAppender::activateOptions ();
}

void ColorConsoleAppender::close ()
{
    QMutexLocker locker (&mObjectGuard);

    if (isClosed ())
        return;

    Log4Qt::WriterAppender::close ();
    closeStream ();
}

void ColorConsoleAppender::append (const Log4Qt::LoggingEvent &rEvent)
{
    Q_ASSERT_X (layout (), "[ColorConsoleAppender][append]", "Layout must not be null");

    if (!_textStream)
        return;

    QString message = layout ()->format (rEvent);

    if (rEvent.level () == Log4Qt::Level::DEBUG_INT)
    {}
    else if (rEvent.level () == Log4Qt::Level::INFO_INT)
    {
        message = CONSOLE_COLOR_INFO + message + CONSOLE_COLOR_END;
    }
    else if (rEvent.level () == Log4Qt::Level::WARN_INT)
    {
        message = CONSOLE_COLOR_WARNING + message + CONSOLE_COLOR_END;
    }
    else if (rEvent.level () == Log4Qt::Level::ERROR_INT)
    {
        message = CONSOLE_COLOR_CRITICAL + message + CONSOLE_COLOR_END;
    }
    else if (rEvent.level () == Log4Qt::Level::FATAL_INT)
    {
        message = CONSOLE_COLOR_FATAL + message + CONSOLE_COLOR_END;
    }

    *_textStream << message;
    if (handleIoErrors ())
        return;

    if (immediateFlush ())
    {
        _textStream->flush ();
        if (handleIoErrors ())
            return;
    }
}

void ColorConsoleAppender::closeStream ()
{
    setWriter (nullptr);
    delete _textStream;
    _textStream = nullptr;
}

void ColorConsoleAppender::setTarget (Target target) {
    _target = target;
}
