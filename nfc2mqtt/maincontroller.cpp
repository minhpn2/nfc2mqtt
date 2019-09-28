#include "maincontroller.h"
#include <QHostAddress>

maincontroller::maincontroller(QObject *parent) : QObject(parent)
{

}

void maincontroller::start()
{
    // Initialie value
    ConfigSetting* settings = &ConfigSetting::instance();

    // Setting param for NFC Module
    qrPid = *settings->getQrPid();
    qrVid = *settings->getQrVid();

    qDebug() << qrPid;
    qDebug() << qrVid;

    _nfcsender = new nfcsender(quint16(qrPid), quint16(qrVid));
    connect(_nfcsender, SIGNAL(nfcDataUpdated(const QString)),
            this, SLOT(nfcDataUpdatedToBroker(const QString)));

    // Setting param for Mqtt
    QString topic = settings->getMqttTopic();
    QString host = settings->getMqttHost();
    int port = settings->getMqttPort();

    m_sender = new MqttSender(host, port);
    m_sender->setDefaultTopic(topic);

    // Tao list danh sach nhan vien
    listIdCard = new QStringList();
    QFile file("/usr/local/qt5pi/vpos/20190925.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Error: Can not open file." ;
        return;
    }

    while (!file.atEnd())
    {
        listIdCard->append(QString(file.readLine()));
    }
}

void maincontroller::nfcDataUpdatedToBroker(const QString &nfc)
{
    // Chuyen doi form cua nfc
#ifdef nfcDebug
    qDebug() << "nfc:" << nfc;
#endif

    if(false == nfc.isEmpty())
    {
        QJsonObject jsonObj;
        jsonObj["id_card"] = nfc;
        jsonObj["id_card_checked"] = QString::number(listIdCard->contains(nfc + "\n"));

        QJsonDocument jsonDoc(jsonObj);
        if(m_sender)
        {
            this->m_sender->send(jsonDoc.toJson().data(), jsonDoc.toJson().size());
        }
    }
}
