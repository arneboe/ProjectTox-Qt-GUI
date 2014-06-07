#ifndef AV_HPP
#define AV_HPP

#include "ringbuffer.hpp"

#include <QObject>
#include <tox/tox.h>

class AV : public QObject
{
    Q_OBJECT
public:
    explicit AV(Tox* tox, QObject *parent = 0);
    ~AV();
    void start();

private:
    Tox* tox;
    QThread* audioThread;

};

#endif // AV_HPP
