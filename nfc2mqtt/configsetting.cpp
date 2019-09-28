#include "configsetting.h"
#include <QMutex>
#include <QSettings>
#include <QDebug>
#include <QStandardPaths>
#include <QTextStream>

ConfigSetting* ConfigSetting::m_instance = nullptr;

ConfigSetting::ConfigSetting(QObject *parent) : QObject(parent)
{
    QSettings sets("vng", "vpos");

    // Setting param for NFC module
    qrpid  = sets.value(QString("option/valueqrpid"), 0xffff).toInt();
    qrvid  = sets.value(QString("option/valueqrvid"), 0x0035).toInt();

    // Setting param for connection with Broker
    hostmqtt     = sets.value(QString("option/hostmqtt"), QString("localhost")).toString();
    portmqtt     = sets.value(QString("option/portmqtt"), 1883).toInt();
    topicpubnfcidcard     = sets.value(QString("option/topicsub"), QString("bus/callserver")).toString();
}

ConfigSetting& ConfigSetting::instance()
{
    static QMutex mutex;

    if (m_instance == nullptr)
    {
        mutex.lock();

        if (m_instance == nullptr)
        {
            m_instance = new ConfigSetting();
        }

        mutex.unlock();
    }

    return *(m_instance);
}





