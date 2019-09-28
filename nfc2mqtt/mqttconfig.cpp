#include "mqttconfig.h"

mqttconfig::mqttconfig(const QString& host, int port)
{
    this->mqttHost = host;
    this->mqttPort = port;

    _timerTryConnectToBroker = new QTimer(this);

    if(_timerTryConnectToBroker)
    {
        // Per 3s, call function tryConnectedToBroker will be called
        _timerTryConnectToBroker->setInterval(SER_TIME_TRY_CONNECT_BROKER);

        connect(_timerTryConnectToBroker, SIGNAL(timeout()),
                this, SLOT(tryConnectedToBroker()));
    }
}

mqttconfig::~mqttconfig()
{
    // Do nothing
}

void mqttconfig::setDefaultTopic(const QString& topic)
{
    this->mqttDefaultTopic = topic;
}

int mqttconfig::connectToBroker()
{
    qDebug() << "connect to host: " << mqttHost << ", port: " << mqttPort;

    mqttClient.setHostname(this->mqttHost);
    mqttClient.setPort(quint16(this->mqttPort));
    mqttClient.setProtocolVersion(QMqttClient::ProtocolVersion::MQTT_3_1);

    connect(&mqttClient,&QMqttClient::connected,this,&mqttconfig::brokerConnected);
    connect(&mqttClient,&QMqttClient::disconnected,this,&mqttconfig::brokerDisconnected);
    connect(&mqttClient,&QMqttClient::messageReceived,this,&mqttconfig::receiverMessage);
    connect(&mqttClient,&QMqttClient::pingResponseReceived,this,&mqttconfig::pingResponse);

    mqttClient.connectToHost();

    return 0;
}

// operation
int mqttconfig::publish(void*data, int size)
{
    if (size <=0)
        return -1;

    QByteArray qdata((char*) data, size);
    return mqttClient.publish(mqttDefaultTopic, qdata);
}

void mqttconfig::clientPublish(QString topic, QString msg)
{
    mqttClient.publish(topic,  msg.toUtf8());
}

void mqttconfig::clientSubscribe()
{

}

void mqttconfig::clientPing()
{
    mqttClient.requestPing();
}

void mqttconfig::brokerDisconnected()
{
    qDebug() << "Disconnected with Broker, try to connected with it";
    // Call function which will try connect to Broker when disconnect
    _timerTryConnectToBroker->start();
}

void mqttconfig::tryConnectedToBroker()
{
    // Connect to Host when this function was called
    mqttClient.connectToHost();
}

void mqttconfig::brokerConnected()
{
    qDebug() << "brokerConnected";
    // try subscribe
    qDebug() << "connected to host: " << mqttHost << ",port: " << mqttPort;

    // Stop timer is used to call function tryConnectedToBroker()
    _timerTryConnectToBroker->stop();
}

void mqttconfig::pingResponse()
{
    // just debug
    qDebug() << "pong received";
}

void mqttconfig::receiverMessage(const QByteArray &message)
{

}
