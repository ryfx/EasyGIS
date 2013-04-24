#ifndef PLAYTHREAD_H
#define PLAYTHREAD_H

#include <QThread>
#include <QtGui>
#include <phonon>

class PlayThread : public QThread
{
    Q_OBJECT
public:
    explicit PlayThread(QObject *parent, QString soundPath, bool isSilenced);

    Phonon::MediaObject* mediaObject;
    Phonon::AudioOutput* audioOutput;
    
signals:
    
public slots:

protected:
    void run();
private:

    
};

#endif // PLAYTHREAD_H
