#ifndef AUDIOINPUT_HPP
#define AUDIOINPUT_HPP

#include "ringbuffer.hpp"
#include "av.hpp"

#include <QAudioInput>

class AudioInput : public QAudioInput
{
    Q_OBJECT
public:
    explicit AudioInput(const QAudioFormat& format, QObject *parent = 0);

    QIODevice* start();
    QIODevice* getDevice();

private:
    Ringbuffer<512* 1024> device;

};

#endif // AUDIOINPUT_HPP
