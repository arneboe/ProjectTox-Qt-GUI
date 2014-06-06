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

private slots:
    void handleAudioInput(Ringbuffer* buffer);

signals:
    void audioOutputReceived(Ringbuffer* buffer);

private:
    Tox* tox;
    Ringbuffer outputBuffer;
    QThread* audioThread;

};

#endif // AV_HPP
