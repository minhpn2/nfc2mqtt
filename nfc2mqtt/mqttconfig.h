#ifndef MQTTCONFIG_H
#define MQTTCONFIG_H


#include <qmqttclient.h>
#include <QGeoPositionInfoSource>
#include <QObject>
#include <QString>
#include <QTimer>

#define  SER_TIME_TRY_CONNECT_BROKER 3000 // 3s

class mqttconfig : public QObject
{
    Q_OBJECT
public:
    explicit mqttconfig(const QString& host, int port);

    ~mqttconfig();

    void setDefaultTopic(const QString& topic);

    int connectToBroker();

    int publish(void*data, int size);

    void clientPublish(QString topic, QString msg);

    void clientSubscribe();

    void clientPing();

private slots:
    void brokerDisconnected();

    void brokerConnected();

    void pingResponse();

    void receiverMessage(const QByteArray &message);

    void tryConnectedToBroker();
private:
    QTimer*     _timerTryConnectToBroker;
    QMqttClient mqttClient;
    QString     mqttHost;
    QString     mqttDefaultTopic;
    int         mqttPort;

    friend class MqttSender;
};

#endif // MQTTCONFIG_H
