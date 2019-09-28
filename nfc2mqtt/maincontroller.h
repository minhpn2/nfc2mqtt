#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H
#include <QTimer>
#include <QObject>

#include <QtDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QTimer>
#include <QFile>

#include "configsetting.h"
#include "hidread/devicemanager.h"
#include "nfcsender.h"
#include "mqttsender.h"

class nfcsender;
class maincontroller : public QObject
{
    Q_OBJECT
public:
    explicit maincontroller(QObject *parent = nullptr);

    void start();

signals:

public slots:

private slots:
    void nfcDataUpdatedToBroker(const QString &nfc);

private:
    nfcsender   *_nfcsender;
    MqttSender  *m_sender;
    QStringList *listIdCard;

    int qrPid;
    int qrVid;

};

#endif // MAINCONTROLLER_H
