#ifndef AV_HPP
#define AV_HPP

#include "ringbuffer.hpp"
#include "audioinput.hpp"
#include "audiooutput.hpp"

#include <QObject>
#include <tox/tox.h>

class AudioInput;
class AudioOutput;

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
    AudioInput* audioInput;
    AudioOutput* audioOutput;

private slots:
    void handleAudioInput(int nbytes);

};

#endif // AV_HPP
