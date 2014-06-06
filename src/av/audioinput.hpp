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

private:
    Ringbuffer buffer;
    QIODevice* device;

signals:
    void inputRecorded(Ringbuffer* buffer);

public slots:
    void readChunk();

};

#endif // AUDIOINPUT_HPP
