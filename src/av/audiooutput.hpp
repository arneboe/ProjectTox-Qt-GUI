#ifndef AUDIOOUTPUT_HPP
#define AUDIOOUTPUT_HPP

#include "ringbuffer.hpp"

#include <QAudioOutput>

class AudioOutput : public QAudioOutput
{
    Q_OBJECT
public:
    explicit AudioOutput(const QAudioFormat format, QObject *parent = 0);
    QIODevice* start();

private:
    QIODevice* device;

public slots:
    void output(Ringbuffer* buffer);

};

#endif // AUDIOOUTPUT_HPP
