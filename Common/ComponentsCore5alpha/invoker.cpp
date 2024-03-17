#include "invoker.h"
#include <QtCore/QDebug>

InvokerBase::InvokerBase ()
    : _connectionType (Qt::AutoConnection)
{}

InvokerBase::~InvokerBase ()
{
    if (!_success && nag)
        qWarning ("[ComponentsCore] Could not invoke function '%s' in object of type '%s'.",
            _sig.constData (), _target->metaObject ()->className ());
}

bool InvokerBase::wasSuccessful () const
{
    nag = false;
    return _success;
}

void InvokerBase::setConnectionType (Qt::ConnectionType c) {
    _connectionType = c;
}

void InvokerBase::invoke (QObject *t, const char *slot)
{
    _target = t;
    _success = false;
    _sig.append (slot, qstrlen(slot));
    _sig.append ('(');
    for (int paramCount = 0; paramCount < lastArg; ++paramCount) {
        if (paramCount)
            _sig.append (',');
        const char *type = _arg[paramCount].name ();
        _sig.append (type, int (strlen (type)));
    }
    _sig.append(')');
    _sig.append('\0');
    int idx = _target->metaObject ()->indexOfMethod (_sig.constData ());
    if (idx < 0) {
        qWarning () << "[ComponentsCore] Can't index of the method:"
                    << _sig.constData ();
        return;
    }
    QMetaMethod method = _target->metaObject ()->method (idx);
    if (_useRet)
        _success = method.invoke(_target, _connectionType, _ret,
           _arg[0], _arg[1], _arg[2], _arg[3], _arg[4],
           _arg[5], _arg[6], _arg[7], _arg[8], _arg[9]);
    else
        _success = method.invoke(_target, _connectionType,
           _arg[0], _arg[1], _arg[2], _arg[3], _arg[4],
           _arg[5], _arg[6], _arg[7], _arg[8], _arg[9]);
}
