#include "devicemanager.h"
#include "hidrawdevice.h"

#include <QDebug>
#include <QThreadPool>

#if QT_VERSION >= 0x050000
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#else
#include "qt4json/QJsonObject.h"
#include "qt4json/QJsonArray.h"
#include "qt4json/QJsonDocument.h"
#endif

// Big paper
//#define PAPER_80

DeviceManager* DeviceManager::instance = nullptr;
QMutex DeviceManager::mutex;
void DeviceManager::start()
{
    QThreadPool::globalInstance()->setMaxThreadCount(5);


#if 0
    _barcodeReader = new HidrawDevice("BARCODE_READER");
    connect(_barcodeReader, SIGNAL(onData(QString)), this, SIGNAL(onBarcode(QString)), Qt::QueuedConnection);
    _barcodeReader->openDevice(0x24ea, 0x0197);
#endif

#if 1
    _mifareReader = new HidrawDevice("MIFARE_READER", true);
    _mifareReader->openDevice(value_pid,value_vid);
    connect(_mifareReader, SIGNAL(onData(QString)), this, SIGNAL(onMifare(QString)), Qt::QueuedConnection);
#endif
}

void DeviceManager::stop()
{
    if (_mifareReader) {
        _mifareReader->closeDevice();
        delete _mifareReader;
    }

    if (_barcodeReader) {
        _barcodeReader->closeDevice();
        delete _barcodeReader;
    }

}

QString DeviceManager::deviceStatus()
{
    QString status = QString("{posdevice: %1, barcode: %2, printer: %3, mifare: %4}")
                            // .arg(_serialReader && _serialReader->isOpen()?"connected":"disconnected")
                             .arg(_barcodeReader && _barcodeReader->isOpen()?"connected":"disconnected")
                             //.arg(_receiptPrinter && (_receiptPrinter->readDeviceStatus() == 0)?"connected":"disconnected")
                             .arg(_mifareReader && _mifareReader->isOpen()?"connected":"disconnected");

    return status;
}




void DeviceManager::detect_scan_qr(QString bc)
{
  qDebug() << "scan qr resuft " << bc;
}

DeviceManager::DeviceManager(QObject *parent) : QObject(parent),

    _barcodeReader(Q_NULLPTR),
    _mifareReader(Q_NULLPTR)
{

}

DeviceManager::~DeviceManager()
{

}
