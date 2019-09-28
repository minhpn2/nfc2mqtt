#ifndef HIDRAWDEVICE_H
#define HIDRAWDEVICE_H

#include <QObject>
#include <QFuture>

class HidrawDevice : public QObject
{
    Q_OBJECT
public:
    explicit HidrawDevice(const QString& name, bool forceHexData = false, QObject *parent = 0);

    void openDevice(quint16 vendor_id_, quint16 product_id_);
    void closeDevice();
    bool isOpen() {return _connected;}
signals:

    void onData(const QString& data);
public slots:
protected:
    void startDeviceMonitor();
    void stopDeviceMonitor();
    static void deviceMonitorFunc(HidrawDevice *devData);

    static bool mappingChar(unsigned char* buffer, int len, char& out);
private:
    bool _running;
    QFuture<void> _threadFuture;
    quint16 vendor_id;
    quint16 product_id;
    QString _name;
    bool _connected;
    bool _forceHexData;
};

#endif // HIDRAWDEVICE_H
