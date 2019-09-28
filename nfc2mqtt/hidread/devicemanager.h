#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>
#include <QMutex>

class SerialConnection;
//class USBPort;
class ReceiptESCPrinter;
class BarcodeReader;
class HidrawDevice;
class SerialDevice;
class DeviceManager : public QObject
{
    Q_OBJECT
public:
    enum DeviceError {
        Success = 0,
        Print_Error,
        Print_Not_Ready,
        UnknownError
    };

    static DeviceManager* getInstance() {
        if (instance == nullptr) {
            QMutexLocker locker(&mutex);
            instance = new DeviceManager();
        }
        return instance;
    }

    void start();
    void stop();


signals:
    void onBarcode(const QString& barcode);
    void onMifare(const QString& barcode);

public slots:
    QString deviceStatus();


    void detect_scan_qr(QString bc);

public:
    explicit DeviceManager(QObject *parent = nullptr);
    quint16 value_pid;
    quint16 value_vid;
    ~DeviceManager();
private:
    static QMutex mutex;
    static DeviceManager* instance;

    SerialDevice *_serialReader;
    //USBPort *_usbPrinter;
    ReceiptESCPrinter* _receiptPrinter;
    HidrawDevice* _barcodeReader;
    HidrawDevice* _mifareReader;

    bool _printer_ready;

    QString _readerDeviceName;
};

#endif // DEVICEMANAGER_H
