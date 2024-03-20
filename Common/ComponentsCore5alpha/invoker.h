#ifndef COMPONENTSCORE_INVOKER_H
#define COMPONENTSCORE_INVOKER_H

#include <QMetaMethod>
#include <QMetaObject>
#include <QMetaType>
#include <QVarLengthArray>
#include <QtCore/QDebug>

class InvokerBase
{
public:
    InvokerBase ();
    virtual ~InvokerBase ();

    bool wasSuccessful () const;
    void setConnectionType (Qt::ConnectionType connectionType);

    void addArgument () {}

    template <typename T, typename ... Args>
    void addArgument (const T &t, const Args& ... args)
    {
        _arg[lastArg++] = QGenericArgument (typeName<T> (), &t);
        addArgument (args ...);
    }

    template <class T> void setReturnValue (T &t)
    {
        _useRet = true;
        _ret = QGenericReturnArgument (typeName<T> (), &t);
    }

    void invoke (QObject *target, const char *slot);

private:
    InvokerBase (const InvokerBase &);
    template <class T> const char *typeName () {
        return QMetaType::typeName (qMetaTypeId<T> ());
    }

    QObject *_target {nullptr};
    QGenericArgument _arg[10];
    QGenericReturnArgument _ret;
    QVarLengthArray<char, 512> _sig;
    int lastArg {0};
    bool _success {true};
    bool _useRet {false};
    Qt::ConnectionType _connectionType {Qt::AutoConnection};
    mutable bool nag {true};
};

template <class Result>
class Invoker : public InvokerBase
{
public:
    template<typename ... Args>
    Invoker (QObject *target, const char *slot, Args ... args)
    {
        static_assert ((sizeof... (Args)) <= 10, "Wrong arguments count! 10 is a max!");

        setReturnValue (result);
        addArgument (args ...);
        InvokerBase::invoke (target, slot);
    }

    operator Result () const { return result; }

private:
    Result result;
};

template<> class Invoker<void> : public InvokerBase
{
public:
    template<typename ... Args>
    Invoker (QObject *target, const char *slot, Args ... args)
    {
        static_assert ((sizeof... (Args)) <= 10, "Wrong arguments count! 10 is a max!");

        addArgument (args ...);
        InvokerBase::invoke (target, slot);
    }
};

#ifndef Q_QDOC
template <class Result>
Result invokeHelper (InvokerBase &in, QObject *target, const char *slot)
{
    Result result;
    in.setReturnValue(result);
    in.invoke(target, slot);
    return result;
}

template <>
inline void invokeHelper<void> (InvokerBase &in, QObject *target, const char *slot)
{
    in.invoke (target, slot);
}
#endif

template<class Result>
Result invoke (QObject *target, const char *slot)
{
    InvokerBase in;
    return invokeHelper<Result> (in, target, slot);
}

template<class Result, typename ... Args>
Result invoke (QObject *target, const char *slot, const Args& ... args)
{
    InvokerBase in;
    in.addArgument (args ...);
    return invokeHelper<Result> (in, target, slot);
}

#endif //COMPONENTSCORE_INVOKER_H
