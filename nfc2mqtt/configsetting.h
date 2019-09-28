#ifndef CONFIGSETTING_H
#define CONFIGSETTING_H

#include <QObject>

class ConfigSetting : public QObject
{
    Q_OBJECT
public:
    static ConfigSetting& instance();
    explicit ConfigSetting(QObject *parent = nullptr);

    // Send Pid and Vid
    int* getQrPid()
    {
        return &qrpid;
    }

    int* getQrVid()
    {
        return &qrvid;
    }

    const QString& getMqttHost()
    {
        return hostmqtt;
    }

    int getMqttPort()
    {
        return portmqtt;
    }

    const QString& getMqttTopic()
    {
        return topicpubnfcidcard;
    }

private:
    static ConfigSetting *m_instance;

    bool    m_autoSave;

    int qrpid;
    int qrvid;

    QString hostmqtt;
    int     portmqtt;
    QString topicpubnfcidcard;

signals:

public slots:
};

#endif // CONFIGSETTING_H
