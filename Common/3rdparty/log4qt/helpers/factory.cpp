/******************************************************************************
 *
 * package:     Log4Qt
 * file:        factory.cpp
 * created:     September 2007
 * author:      Martin Heinrich
 *
 *
 * Copyright 2007 Martin Heinrich
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/


#include "helpers/factory.h"

#include "consoleappender.h"
#include "colorconsoleappender.h"
#include "dailyrollingfileappender.h"
#include "fileappender.h"
#include "helpers/logerror.h"
#include "helpers/initialisationhelper.h"
#include "helpers/optionconverter.h"
#include "patternlayout.h"
#include "rollingfileappender.h"
#include "signalappender.h"
#include "simplelayout.h"
#include "simpletimelayout.h"
#include "ttcclayout.h"
#include "binarylayout.h"
#include "binarytotextlayout.h"
#include "xmllayout.h"

#if defined(QT_NETWORK_LIB)
#include "telnetappender.h"
#endif

#if defined(QT_SQL_LIB)
#include "databaseappender.h"
#include "databaselayout.h"
#endif //#ifdef QT_SQL_LIB

#include "asyncappender.h"
#include "mainthreadappender.h"
#include "systemlogappender.h"
#include "binaryfileappender.h"
#include "rollingbinaryfileappender.h"
#include "dailyfileappender.h"
#ifdef Q_OS_WIN
#include "wdcappender.h"
#endif

#include "varia/debugappender.h"
#include "varia/denyallfilter.h"
#include "varia/levelmatchfilter.h"
#include "varia/levelrangefilter.h"
#include "varia/listappender.h"
#include "varia/nullappender.h"
#include "varia/stringmatchfilter.h"
#include "varia/binaryeventfilter.h"

#include <QMetaObject>
#include <QMetaProperty>

namespace Log4Qt
{

LOG4QT_DECLARE_STATIC_LOGGER(logger, Log4Qt::Factory)

// Appenders

Appender *console_file_appender()
{
    return new ConsoleAppender;
}

Appender *create_daily_rolling_file_appender()
{
    return new DailyRollingFileAppender;
}

Appender *create_debug_appender()
{
    return new DebugAppender;
}

Appender *create_file_appender()
{
    return new FileAppender;
}

Appender *create_list_appender()
{
    return new ListAppender;
}

Appender *create_null_appender()
{
    return new NullAppender;
}

Appender *create_rolling_file_appender()
{
    return new RollingFileAppender;
}

Appender *create_signal_appender()
{
    return new SignalAppender;
}

Appender *create_color_console_appender()
{
    return new ColorConsoleAppender;
}

//#if defined(QT_SQL_LIB)
//Appender *create_database_appender()
//{
//    return new DatabaseAppender;
//}
//#endif //#if defined(QT_SQL_LIB)

//#if defined(QT_NETWORK_LIB)
//Appender *create_telnet_appender()
//{
//    return new TelnetAppender;
//}
//#endif

Appender *create_async_appender()
{
    return new AsyncAppender;
}

Appender *create_mainthread_appender()
{
    return new MainThreadAppender;
}

Appender *create_systemlog_appender()
{
    return new SystemLogAppender;
}

Appender *create_binaryfile_appender()
{
    return new BinaryFileAppender;
}

Appender *create_rollingbinaryfile_appender()
{
    return new RollingBinaryFileAppender;
}

Appender *create_dailyrollingfile_appender()
{
    return new DailyFileAppender;
}

#ifdef Q_OS_WIN
Appender *create_wdc_appender()
{
    return new WDCAppender;
}
#endif

// Filters

Filter *create_deny_all_filter()
{
    return new DenyAllFilter;
}

Filter *create_level_match_filter()
{
    return new LevelMatchFilter;
}

Filter *create_level_range_filter()
{
    return new LevelRangeFilter;
}

Filter *create_string_match_filter()
{
    return new StringMatchFilter;
}

Filter *create_binaryevent_filter()
{
    return new BinaryEventFilter;
}

// Layouts

Layout *create_pattern_layout()
{
    return new PatternLayout;
}

Layout *create_simple_layout()
{
    return new SimpleLayout;
}

Layout *create_simple_time_layout()
{
    return new SimpleTimeLayout;
}

#if defined(QT_SQL_LIB)
Layout *create_database_layout()
{
    return new DatabaseLayout;
}
#endif //#if defined(QT_SQL_LIB)

Layout *create_ttcc_layout()
{
    return new TTCCLayout;
}
Layout *create_binary_layout()
{
    return new BinaryLayout;
}

Layout *create_binarytotext_layout()
{
    return new BinaryToTextLayout;
}

Layout *create_xml_layout()
{
    return new XMLLayout;
}

Factory::Factory() :
    mObjectGuard(),
    mAppenderRegistry(),
    mFilterRegistry(),
    mLayoutRegistry()
{
    registerDefaultAppenders();
    registerDefaultFilters();
    registerDefaultLayouts();
}


LOG4QT_IMPLEMENT_INSTANCE(Factory)


Appender *Factory::doCreateAppender(const QString &rAppenderClassName)
{
    QMutexLocker locker(&mObjectGuard);

    if (!mAppenderRegistry.contains(rAppenderClassName))
    {
        logger()->warn("Request for the creation of Appender with class '%1', which is not registered", rAppenderClassName);
        return nullptr;
    }
    return mAppenderRegistry.value(rAppenderClassName)();
}


Filter *Factory::doCreateFilter(const QString &rFilterClassName)
{
    QMutexLocker locker(&mObjectGuard);

    if (!mFilterRegistry.contains(rFilterClassName))
    {
        logger()->warn("Request for the creation of Filter with class '%1', which is not registered", rFilterClassName);
        return nullptr;
    }
    return mFilterRegistry.value(rFilterClassName)();
}


Layout *Factory::doCreateLayout(const QString &rLayoutClassName)
{
    QMutexLocker locker(&mObjectGuard);

    if (!mLayoutRegistry.contains(rLayoutClassName))
    {
        logger()->warn("Request for the creation of Layout with class '%1', which is not registered", rLayoutClassName);
        return nullptr;
    }
    return mLayoutRegistry.value(rLayoutClassName)();
}


void Factory::doRegisterAppender(const QString &rAppenderClassName,
                                 AppenderFactoryFunc pAppenderFactoryFunc)
{
    QMutexLocker locker(&mObjectGuard);

    if (rAppenderClassName.isEmpty())
    {
        logger()->warn("Registering Appender factory function with empty class name");
        return;
    }
    mAppenderRegistry.insert(rAppenderClassName, pAppenderFactoryFunc);
}


void Factory::doRegisterFilter(const QString &rFilterClassName,
                               FilterFactoryFunc pFilterFactoryFunc)
{
    QMutexLocker locker(&mObjectGuard);

    if (rFilterClassName.isEmpty())
    {
        logger()->warn("Registering Filter factory function with empty class name");
        return;
    }
    mFilterRegistry.insert(rFilterClassName, pFilterFactoryFunc);
}


void Factory::doRegisterLayout(const QString &rLayoutClassName,
                               LayoutFactoryFunc pLayoutFactoryFunc)
{
    QMutexLocker locker(&mObjectGuard);

    if (rLayoutClassName.isEmpty())
    {
        logger()->warn("Registering Layout factory function with empty class name");
        return;
    }
    mLayoutRegistry.insert(rLayoutClassName, pLayoutFactoryFunc);
}


void Factory::doSetObjectProperty(QObject *pObject,
                                  const QString &rProperty,
                                  const QString &rValue)
{
    // - Validate property
    // - Get correct property name from meta object
    // - Find specific property setter
    // - If no specfifc propery setter can be found,
    //   find general property setter
    // - Call property setter

    QMetaProperty meta_property;
    if (!validateObjectProperty(meta_property, rProperty, pObject))
        return;

    QString property = QLatin1String(meta_property.name());
    QString type = QLatin1String(meta_property.typeName());
    logger()->debug("Setting property '%1' on object of class '%2' to value '%3'",
                    property,
                    QLatin1String(pObject->metaObject()->className()),
                    rValue);

    QVariant value;
    bool ok = true;
    if (type == QLatin1String("bool"))
        value = OptionConverter::toBoolean(rValue, &ok);
    else if (type == QLatin1String("int"))
        value = OptionConverter::toInt(rValue, &ok);
    else if (type == QLatin1String("Log4Qt::Level"))
        value = QVariant::fromValue(OptionConverter::toLevel(rValue, &ok));
    else if (type == QLatin1String("QString"))
        value = rValue;
    else
    {
        LogError e = LOG4QT_ERROR("Cannot convert to type '%1' for property '%2' on object of class '%3'",
                                  CONFIGURATOR_UNKNOWN_TYPE_ERROR,
                                  "Log4Qt::Factory");
        e << type
          << property
          << QString::fromLatin1(pObject->metaObject()->className());
        logger()->error(e);
        return;
    }
    if (!ok)
        return;

    // Everything is checked and the type is the one of the property.
    // Write should never return false
    if (!meta_property.write(pObject, value))
        logger()->warn("Unxpected error result from QMetaProperty.write()");
}


void Factory::doUnregisterAppender(const QString &rAppenderClassName)
{
    QMutexLocker locker(&mObjectGuard);

    if (!mAppenderRegistry.contains(rAppenderClassName))
    {
        logger()->warn("Request to unregister not registered Appender factory function for class '%1'", rAppenderClassName);
        return;
    }
    mAppenderRegistry.remove(rAppenderClassName);
}


void Factory::doUnregisterFilter(const QString &rFilterClassName)
{
    QMutexLocker locker(&mObjectGuard);

    if (!mFilterRegistry.contains(rFilterClassName))
    {
        logger()->warn("Request to unregister not registered Filter factory function for class '%1'", rFilterClassName);
        return;
    }
    mFilterRegistry.remove(rFilterClassName);
}


void Factory::doUnregisterLayout(const QString &rLayoutClassName)
{
    QMutexLocker locker(&mObjectGuard);

    if (!mLayoutRegistry.contains(rLayoutClassName))
    {
        logger()->warn("Request to unregister not registered Layout factory function for class '%1'", rLayoutClassName);
        return;
    }
    mLayoutRegistry.remove(rLayoutClassName);
}


void Factory::registerDefaultAppenders()
{
    mAppenderRegistry.insert(QLatin1String("org.apache.log4j.ConsoleAppender"), console_file_appender);
    mAppenderRegistry.insert(QLatin1String("Log4Qt::ConsoleAppender"), console_file_appender);
    mAppenderRegistry.insert(QLatin1String("org.apache.log4j.DailyRollingFileAppender"), create_daily_rolling_file_appender);
    mAppenderRegistry.insert(QLatin1String("Log4Qt::DailyRollingFileAppender"), create_daily_rolling_file_appender);
    mAppenderRegistry.insert(QLatin1String("org.apache.log4j.varia.DebugAppender"), create_debug_appender);
    mAppenderRegistry.insert(QLatin1String("Log4Qt::DebugAppender"), create_debug_appender);
    mAppenderRegistry.insert(QLatin1String("org.apache.log4j.FileAppender"), create_file_appender);
    mAppenderRegistry.insert(QLatin1String("Log4Qt::FileAppender"), create_file_appender);
    mAppenderRegistry.insert(QLatin1String("org.apache.log4j.varia.ListAppender"), create_list_appender);
    mAppenderRegistry.insert(QLatin1String("Log4Qt::ListAppender"), create_list_appender);
    mAppenderRegistry.insert(QLatin1String("org.apache.log4j.varia.NullAppender"), create_null_appender);
    mAppenderRegistry.insert(QLatin1String("Log4Qt::NullAppender"), create_null_appender);
    mAppenderRegistry.insert(QLatin1String("org.apache.log4j.RollingFileAppender"), create_rolling_file_appender);
    mAppenderRegistry.insert(QLatin1String("Log4Qt::RollingFileAppender"), create_rolling_file_appender);

    mAppenderRegistry.insert(QLatin1String("org.apache.log4j.SignalAppender"), create_signal_appender);
    mAppenderRegistry.insert(QLatin1String("Log4Qt::SignalAppender"), create_signal_appender);
    mAppenderRegistry.insert(QLatin1String("org.apache.log4j.ColorConsoleAppender"), create_color_console_appender);
    mAppenderRegistry.insert(QLatin1String("Log4Qt::ColorConsoleAppender"), create_color_console_appender);

//#if defined(QT_SQL_LIB)
//    mAppenderRegistry.insert(QLatin1String("org.apache.log4j.DatabaseAppender"), create_database_appender);
//    mAppenderRegistry.insert(QLatin1String("Log4Qt::DatabaseAppender"), create_database_appender);
//#endif //#ifdef QT_SQL_LIB

//#if defined(QT_NETWORK_LIB)
//    mAppenderRegistry.insert(QLatin1String("org.apache.log4j.TelnetAppender"), create_telnet_appender);
//    mAppenderRegistry.insert(QLatin1String("Log4Qt::TelnetAppender"), create_telnet_appender);
//#endif
    mAppenderRegistry.insert(QLatin1String("org.apache.log4j.AsyncAppender"), create_async_appender);
    mAppenderRegistry.insert(QLatin1String("Log4Qt::AsyncAppender"), create_async_appender);

    mAppenderRegistry.insert(QLatin1String("org.apache.log4j.MainThreadAppender"), create_mainthread_appender);
    mAppenderRegistry.insert(QLatin1String("Log4Qt::MainThreadAppender"), create_mainthread_appender);

    mAppenderRegistry.insert(QLatin1String("org.apache.log4j.SystemLogAppender"), create_systemlog_appender);
    mAppenderRegistry.insert(QLatin1String("Log4Qt::SystemLogAppender"), create_systemlog_appender);

    mAppenderRegistry.insert(QLatin1String("org.apache.log4j.BinaryFileAppender"), create_binaryfile_appender);
    mAppenderRegistry.insert(QLatin1String("Log4Qt::BinaryFileAppender"), create_binaryfile_appender);

    mAppenderRegistry.insert(QLatin1String("org.apache.log4j.RollingBinaryFileAppender"), create_rollingbinaryfile_appender);
    mAppenderRegistry.insert(QLatin1String("Log4Qt::RollingBinaryFileAppender"), create_rollingbinaryfile_appender);

    mAppenderRegistry.insert(QLatin1String("org.apache.log4j.DailyFileAppender"), create_dailyrollingfile_appender);
    mAppenderRegistry.insert(QLatin1String("Log4Qt::DailyFileAppender"), create_dailyrollingfile_appender);
#ifdef Q_OS_WIN
    mAppenderRegistry.insert(QLatin1String("org.apache.log4j.WDCAppender"), create_wdc_appender);
    mAppenderRegistry.insert(QLatin1String("Log4Qt::WDCAppender"), create_wdc_appender);
#endif
}


void Factory::registerDefaultFilters()
{
    mFilterRegistry.insert(QLatin1String("org.apache.log4j.varia.DenyAllFilter"), create_deny_all_filter);
    mFilterRegistry.insert(QLatin1String("Log4Qt::DenyAllFilter"), create_deny_all_filter);
    mFilterRegistry.insert(QLatin1String("org.apache.log4j.varia.LevelMatchFilter"), create_level_match_filter);
    mFilterRegistry.insert(QLatin1String("Log4Qt::LevelMatchFilter"), create_level_match_filter);
    mFilterRegistry.insert(QLatin1String("org.apache.log4j.varia.LevelRangeFilter"), create_level_range_filter);
    mFilterRegistry.insert(QLatin1String("Log4Qt::LevelRangeFilter"), create_level_range_filter);
    mFilterRegistry.insert(QLatin1String("org.apache.log4j.varia.StringMatchFilter"), create_string_match_filter);
    mFilterRegistry.insert(QLatin1String("Log4Qt::StringMatchFilter"), create_string_match_filter);
    mFilterRegistry.insert(QLatin1String("org.apache.log4j.varia.BinaryEventFilter"), create_binaryevent_filter);
    mFilterRegistry.insert(QLatin1String("Log4Qt::BinaryEventFilter"), create_binaryevent_filter);
}


void Factory::registerDefaultLayouts()
{
    mLayoutRegistry.insert(QLatin1String("org.apache.log4j.PatternLayout"), create_pattern_layout);
    mLayoutRegistry.insert(QLatin1String("Log4Qt::PatternLayout"), create_pattern_layout);
    mLayoutRegistry.insert(QLatin1String("org.apache.log4j.SimpleLayout"), create_simple_layout);
    mLayoutRegistry.insert(QLatin1String("Log4Qt::SimpleLayout"), create_simple_layout);
    mLayoutRegistry.insert(QLatin1String("org.apache.log4j.TTCCLayout"), create_ttcc_layout);
    mLayoutRegistry.insert(QLatin1String("Log4Qt::TTCCLayout"), create_ttcc_layout);

    mLayoutRegistry.insert(QLatin1String("org.apache.log4j.SimpleTimeLayout"), create_simple_time_layout);
    mLayoutRegistry.insert(QLatin1String("Log4Qt::SimpleTimeLayout"), create_simple_time_layout);

#if defined(QT_SQL_LIB)
    mLayoutRegistry.insert(QLatin1String("org.apache.log4j.DatabaseLayout"), create_database_layout);
    mLayoutRegistry.insert(QLatin1String("Log4Qt::DatabaseLayout"), create_database_layout);
#endif //#ifdef (QT_SQL_LIB)

    mLayoutRegistry.insert(QLatin1String("org.apache.log4j.BinaryLayout"), create_binary_layout);
    mLayoutRegistry.insert(QLatin1String("Log4Qt::BinaryLayout"), create_binary_layout);

    mLayoutRegistry.insert(QLatin1String("org.apache.log4j.BinaryToTextLayout"), create_binarytotext_layout);
    mLayoutRegistry.insert(QLatin1String("Log4Qt::BinaryToTextLayout"), create_binarytotext_layout);

    mLayoutRegistry.insert(QLatin1String("org.apache.log4j.XMLLayout"), create_xml_layout);
    mLayoutRegistry.insert(QLatin1String("Log4Qt::XMLLayout"), create_xml_layout);
}


bool Factory::validateObjectProperty(QMetaProperty &rMetaProperty,
                                     const QString &rProperty,
                                     QObject *pObject)
{
    // Validate:
    // - No null object pointer
    // - No empty property name
    // - Property exists on the object (QT or Java name)
    // - Property is readable
    // - Property is writable

    const char *p_context = "Log4Qt::Factory";
    LogError e = LOG4QT_ERROR("Unable to set property value on object",
                              CONFIGURATOR_PROPERTY_ERROR,
                              p_context);

    if (!pObject)
    {
        LogError ce = LOG4QT_ERROR("Invalid null object pointer",
                                   0,
                                   p_context);
        e.addCausingError(ce);
        logger()->error(e);
        return false;
    }
    if (rProperty.isEmpty())
    {
        LogError ce = LOG4QT_ERROR("Invalid empty property name",
                                   0,
                                   p_context);
        e.addCausingError(ce);
        logger()->error(e);
        return false;
    }
    const QMetaObject *p_meta_object = pObject->metaObject();
    QString property = rProperty;
    int i = p_meta_object->indexOfProperty(property.toLatin1().constData());
    if (i < 0)
    {
        // Try name with lower case first character. Java properties names
        // start upper case
        property[0] = property[0].toLower();
        i = p_meta_object->indexOfProperty(property.toLatin1().constData());
        if (i < 0)
        {
            LogError ce = LOG4QT_ERROR("Property '%1' does not exist in class '%2'",
                                       0,
                                       p_context);
            ce << property
               << QString::fromLatin1(pObject->metaObject()->className());
            e.addCausingError(ce);
            logger()->error(e);
            return false;
        }
    }
    rMetaProperty = p_meta_object->property(i);
    if (!rMetaProperty.isWritable())
    {
        LogError ce = LOG4QT_ERROR("Property '%1' is not writable in class '%2'",
                                   0,
                                   p_context);
        ce << property
           << QString::fromLatin1(pObject->metaObject()->className());
        e.addCausingError(ce);
        logger()->error(e);
        return false;
    }

    return true;
}

} // namespace Log4Qt

