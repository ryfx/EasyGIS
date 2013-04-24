#include "playthread.h"

PlayThread::PlayThread(QObject *parent, QString soundPath, bool isSilenced) :
    QThread(parent)
{
    qDebug() << "playing sound: " << soundPath;
    QUrl soundUrl(soundPath);

    mediaObject = new Phonon::MediaObject(this);
    mediaObject->setCurrentSource(Phonon::MediaSource(soundUrl));

    audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, 0);

    if(isSilenced)
    {
        audioOutput->setMuted(true);
    }

    Phonon::createPath(mediaObject, audioOutput);

}


void PlayThread::run()
{
    mediaObject->play();
}
