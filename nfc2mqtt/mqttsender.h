#ifndef MQTTSENDER_H
#define MQTTSENDER_H

#include <qmqttclient.h>
#include <QGeoPositionInfoSource>
#include <QObject>
#include <QString>

#include "mqttconfig.h"

class mqttconfig;
class MqttSender
{
public:
    MqttSender(const QString& host, int port);
    ~MqttSender();

    void setDefaultTopic(const QString& topic);
    int sendUtf8String(const char* utf8str);
    int send(void*data, int size);
    int asyncSend(void* data, int size);

private:
    mqttconfig* _privateSender;

};

#endif // MQTTSENDER_H
