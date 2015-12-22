#ifndef VISION_LOGPLAYER_H
#define VISION_LOGPLAYER_H

#include <QObject>
#include <QFile>
#include "worldmodel.h"
#include "logplayer.h"
#include "proto/sslvision/messages_robocup_ssl_wrapper.pb.h"
#include "proto/sslvision/vision_log.pb.h"

class Vision_logPlayer : public logPlayer
{
    Q_OBJECT
public:
    explicit Vision_logPlayer(WorldModel *worldmodel, QString address, QObject *parent = 0);
    virtual void playLog();
    virtual bool loadLog();
    bool loadLog(QString address);
    virtual bool saveLog();
    void recordLog(SSL_WrapperPacket input);
    void restartTime();
    SSL_WrapperPacket returnCurrentPacket();

public slots:
    void timerShot();

private:
    WorldModel *wm;
    QList<Vision_chunk> chunks;
    QTime *elapsedTime;
    QString fileAddress;
    Vision_chunk current_chunk;

signals:
    void dataReady();

public slots:
};

#endif // VISION_LOGPLAYER_H