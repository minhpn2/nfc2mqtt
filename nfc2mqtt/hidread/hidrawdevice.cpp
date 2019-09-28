#include "hidrawdevice.h"
#include "hidapi.h"

#include <QtConcurrent>


const char NewLineBreak[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const char NewTagBreak[] =  { 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00 };

#define MAX_BARCODE_PATTERN 512

#define VPOS_BASE_DIR "/root/vpos"

typedef struct barcodeMapping {
    char character;
    int size;
    uchar pattern[MAX_BARCODE_PATTERN];
} barcodeMapping;

const barcodeMapping CharacterMap[] = {
    #if 0
    {'A', 9, {0x20,0x00,0x04,0x00,0x00,0x00,0x00,0x00, 0x20}}, //A
    {'B', 9, {0x20,0x00,0x05,0x00,0x00,0x00,0x00,0x00, 0x20}}, //B
    {'C', 9, {0x20,0x00,0x06,0x00,0x00,0x00,0x00,0x00, 0x20}}, //C
    {'D', 9, {0x20,0x00,0x07,0x00,0x00,0x00,0x00,0x00, 0x20}}, //D
    {'E', 9, {0x20,0x00,0x08,0x00,0x00,0x00,0x00,0x00, 0x20}}, //E
    {'F', 9, {0x20,0x00,0x09,0x00,0x00,0x00,0x00,0x00, 0x20}}, //F
    {'G', 9, {0x20,0x00,0x0A,0x00,0x00,0x00,0x00,0x00, 0x20}}, //G
    {'H', 9, {0x20,0x00,0x0B,0x00,0x00,0x00,0x00,0x00, 0x20}}, //H
    {'I', 9, {0x20,0x00,0x0C,0x00,0x00,0x00,0x00,0x00, 0x20}}, //I
    {'J', 9, {0x20,0x00,0x0D,0x00,0x00,0x00,0x00,0x00, 0x20}}, //J
    {'K', 9, {0x20,0x00,0x0E,0x00,0x00,0x00,0x00,0x00, 0x20}}, //K
    {'L', 9, {0x20,0x00,0x0F,0x00,0x00,0x00,0x00,0x00, 0x20}}, //L
    {'M', 9, {0x20,0x00,0x10,0x00,0x00,0x00,0x00,0x00, 0x20}}, //M
    {'N', 9, {0x20,0x00,0x11,0x00,0x00,0x00,0x00,0x00, 0x20}}, //N
    {'O', 9, {0x20,0x00,0x12,0x00,0x00,0x00,0x00,0x00, 0x20}}, //O
    {'P', 9, {0x20,0x00,0x13,0x00,0x00,0x00,0x00,0x00, 0x20}}, //P
    {'Q', 9, {0x20,0x00,0x14,0x00,0x00,0x00,0x00,0x00, 0x20}}, //Q
    {'R', 9, {0x20,0x00,0x15,0x00,0x00,0x00,0x00,0x00, 0x20}}, //R
    {'S', 9, {0x20,0x00,0x16,0x00,0x00,0x00,0x00,0x00, 0x20}}, //S
    {'T', 9, {0x20,0x00,0x17,0x00,0x00,0x00,0x00,0x00, 0x20}}, //T
    {'U', 9, {0x20,0x00,0x18,0x00,0x00,0x00,0x00,0x00, 0x20}}, //U
    {'W', 9, {0x20,0x00,0x19,0x00,0x00,0x00,0x00,0x00, 0x20}}, //W

    {'V', 9, {0x20,0x00,0x1A,0x00,0x00,0x00,0x00,0x00, 0x20}}, //V
    {'X', 9, {0x20,0x00,0x1B,0x00,0x00,0x00,0x00,0x00, 0x20}}, //X
    {'Y', 9, {0x20,0x00,0x1C,0x00,0x00,0x00,0x00,0x00, 0x20}}, //Y
    {'Z', 9, {0x20,0x00,0x1D,0x00,0x00,0x00,0x00,0x00, 0x20}}, //Z
    #endif

    {'0', 8, {0x00,0x00,0x27,0x00,0x00,0x00,0x00, 0x00}},
    {'1', 8, {0x00,0x00,0x1e,0x00,0x00,0x00,0x00, 0x00}},
    {'!', 8, {0x02,0x00,0x1e,0x00,0x00,0x00,0x00, 0x00}},
    {'2', 8, {0x00,0x00,0x1f,0x00,0x00,0x00,0x00, 0x00}},
    {'@', 8, {0x02,0x00,0x1f,0x00,0x00,0x00,0x00, 0x00}},
    {'3', 8, {0x00,0x00,0x20,0x00,0x00,0x00,0x00, 0x00}},
    {'#', 8, {0x02,0x00,0x20,0x00,0x00,0x00,0x00, 0x00}},
    {'4', 8, {0x00,0x00,0x21,0x00,0x00,0x00,0x00, 0x00}},
    {'$', 8, {0x02,0x00,0x21,0x00,0x00,0x00,0x00, 0x00}},
    {'5', 8, {0x00,0x00,0x22,0x00,0x00,0x00,0x00, 0x00}},
    {'%', 8, {0x02,0x00,0x22,0x00,0x00,0x00,0x00, 0x00}},
    {'6', 8, {0x00,0x00,0x23,0x00,0x00,0x00,0x00, 0x00}},
    {'^', 8, {0x02,0x00,0x23,0x00,0x00,0x00,0x00, 0x00}},
    {'7', 8, {0x00,0x00,0x24,0x00,0x00,0x00,0x00, 0x00}},
    {'&', 8, {0x02,0x00,0x24,0x00,0x00,0x00,0x00, 0x00}},
    {'8', 8, {0x00,0x00,0x25,0x00,0x00,0x00,0x00, 0x00}},
    {'*', 8, {0x02,0x00,0x25,0x00,0x00,0x00,0x00, 0x00}},
    {'9', 8, {0x00,0x00,0x26,0x00,0x00,0x00,0x00, 0x00}},
    {':', 8, {0x02,0x00,0x33,0x00,0x00,0x00,0x00, 0x00}},
    {';', 8, {0x00,0x00,0x33,0x00,0x00,0x00,0x00, 0x00}},
    {'(', 8, {0x02,0x00,0x26,0x00,0x00,0x00,0x00, 0x00}},
    {')', 8, {0x02,0x00,0x27,0x00,0x00,0x00,0x00, 0x00}},
    {'{', 8, {0x02,0x00,0x2f,0x00,0x00,0x00,0x00, 0x00}},
    {'[', 8, {0x00,0x00,0x2f,0x00,0x00,0x00,0x00, 0x00}},
    {']', 8, {0x00,0x00,0x30,0x00,0x00,0x00,0x00, 0x00}},
    {'}', 8, {0x02,0x00,0x30,0x00,0x00,0x00,0x00, 0x00}},
    {'<', 8, {0x02,0x00,0x36,0x00,0x00,0x00,0x00, 0x00}},
    {',', 8, {0x00,0x00,0x36,0x00,0x00,0x00,0x00, 0x00}},
    {'>', 8, {0x02,0x00,0x37,0x00,0x00,0x00,0x00, 0x00}},
    {'.', 8, {0x00,0x00,0x37,0x00,0x00,0x00,0x00, 0x00}},
    {'?', 8, {0x02,0x00,0x38,0x00,0x00,0x00,0x00, 0x00}},
    {'/', 8, {0x00,0x00,0x38,0x00,0x00,0x00,0x00, 0x00}},
    {'\\', 8, {0x00,0x00,0x64,0x00,0x00,0x00,0x00, 0x00}},
    {'|', 8, {0x02,0x00,0x64,0x00,0x00,0x00,0x00, 0x00}},
    {'\'', 8, {0x00,0x00,0x34,0x00,0x00,0x00,0x00, 0x00}},
    {'"', 8, {0x02,0x00,0x34,0x00,0x00,0x00,0x00, 0x00}},
    {' ', 8, {0x02,0x00,0x2c,0x00,0x00,0x00,0x00, 0x00}},
    {' ', 8, {0x00,0x00,0x2c,0x00,0x00,0x00,0x00, 0x00}},
    {'-', 8, {0x00,0x00,0x2d,0x00,0x00,0x00,0x00, 0x00}},
    {'_', 8, {0x02,0x00,0x2d,0x00,0x00,0x00,0x00, 0x00}},
    {'=', 8, {0x00,0x00,0x2e,0x00,0x00,0x00,0x00, 0x00}},
    {'+', 8, {0x02,0x00,0x2e,0x00,0x00,0x00,0x00, 0x00}},
    {'`', 8, {0x00,0x00,0x35,0x00,0x00,0x00,0x00, 0x00}},
    {'~', 8, {0x02,0x00,0x35,0x00,0x00,0x00,0x00, 0x00}},




    #if 1
    {'A', 8, {0x02,0x00,0x04,0x00,0x00,0x00,0x00, 0x00}},
    {'B', 8, {0x02,0x00,0x05,0x00,0x00,0x00,0x00, 0x00}},
    {'C', 8, {0x02,0x00,0x06,0x00,0x00,0x00,0x00, 0x00}},
    {'D', 8, {0x02,0x00,0x07,0x00,0x00,0x00,0x00, 0x00}},
    {'E', 8, {0x02,0x00,0x08,0x00,0x00,0x00,0x00, 0x00}}, //E
    {'F', 8, {0x02,0x00,0x09,0x00,0x00,0x00,0x00, 0x00}}, //F
    {'G', 8, {0x02,0x00,0x0a,0x00,0x00,0x00,0x00, 0x00}}, //G
    {'H', 8, {0x02,0x00,0x0b,0x00,0x00,0x00,0x00, 0x00}}, //H
    {'I', 8, {0x02,0x00,0x0c,0x00,0x00,0x00,0x00, 0x00}}, //I
    {'J', 8, {0x02,0x00,0x0d,0x00,0x00,0x00,0x00, 0x00}}, //J
    {'K', 8, {0x02,0x00,0x0e,0x00,0x00,0x00,0x00, 0x00}}, //K
    {'L', 8, {0x02,0x00,0x0f,0x00,0x00,0x00,0x00, 0x00}}, //L
    {'M', 8, {0x02,0x00,0x10,0x00,0x00,0x00,0x00, 0x00}}, //M
    {'N', 8, {0x02,0x00,0x11,0x00,0x00,0x00,0x00, 0x00}}, //N
    {'O', 8, {0x02,0x00,0x12,0x00,0x00,0x00,0x00, 0x00}}, //O
    {'P', 8, {0x02,0x00,0x13,0x00,0x00,0x00,0x00, 0x00}}, //P
    {'Q', 8, {0x02,0x00,0x14,0x00,0x00,0x00,0x00, 0x00}}, //Q
    {'R', 8, {0x02,0x00,0x15,0x00,0x00,0x00,0x00, 0x00}}, //R
    {'S', 8, {0x02,0x00,0x16,0x00,0x00,0x00,0x00, 0x00}}, //S
    {'T', 8, {0x02,0x00,0x17,0x00,0x00,0x00,0x00, 0x00}}, //T
    {'U', 8, {0x02,0x00,0x18,0x00,0x00,0x00,0x00, 0x00}}, //U
    {'V', 8, {0x02,0x00,0x19,0x00,0x00,0x00,0x00, 0x00}}, //W
    {'W', 8, {0x02,0x00,0x1a,0x00,0x00,0x00,0x00, 0x00}}, //V
    {'X', 8, {0x02,0x00,0x1b,0x00,0x00,0x00,0x00, 0x00}}, //X
    {'Y', 8, {0x02,0x00,0x1c,0x00,0x00,0x00,0x00, 0x00}}, //Y
    {'Z', 8, {0x02,0x00,0x1d,0x00,0x00,0x00,0x00, 0x00}}, //Z

    {'a', 8, {0x00,0x00,0x04,0x00,0x00,0x00,0x00, 0x00}},
    {'b', 8, {0x00,0x00,0x05,0x00,0x00,0x00,0x00, 0x00}},
    {'c', 8, {0x00,0x00,0x06,0x00,0x00,0x00,0x00, 0x00}},
    {'d', 8, {0x00,0x00,0x07,0x00,0x00,0x00,0x00, 0x00}},
    {'e', 8, {0x00,0x00,0x08,0x00,0x00,0x00,0x00, 0x00}}, //E
    {'f', 8, {0x00,0x00,0x09,0x00,0x00,0x00,0x00, 0x00}}, //F
    {'g', 8, {0x00,0x00,0x0a,0x00,0x00,0x00,0x00, 0x00}}, //G
    {'h', 8, {0x00,0x00,0x0b,0x00,0x00,0x00,0x00, 0x00}}, //H
    {'i', 8, {0x00,0x00,0x0c,0x00,0x00,0x00,0x00, 0x00}}, //I
    {'j', 8, {0x00,0x00,0x0d,0x00,0x00,0x00,0x00, 0x00}}, //J
    {'k', 8, {0x00,0x00,0x0e,0x00,0x00,0x00,0x00, 0x00}}, //K
    {'l', 8, {0x00,0x00,0x0f,0x00,0x00,0x00,0x00, 0x00}}, //L
    {'m', 8, {0x00,0x00,0x10,0x00,0x00,0x00,0x00, 0x00}}, //M
    {'n', 8, {0x00,0x00,0x11,0x00,0x00,0x00,0x00, 0x00}}, //N
    {'o', 8, {0x00,0x00,0x12,0x00,0x00,0x00,0x00, 0x00}}, //O
    {'p', 8, {0x00,0x00,0x13,0x00,0x00,0x00,0x00, 0x00}}, //P
    {'q', 8, {0x00,0x00,0x14,0x00,0x00,0x00,0x00, 0x00}}, //Q
    {'r', 8, {0x00,0x00,0x15,0x00,0x00,0x00,0x00, 0x00}}, //R
    {'s', 8, {0x00,0x00,0x16,0x00,0x00,0x00,0x00, 0x00}}, //S
    {'t', 8, {0x00,0x00,0x17,0x00,0x00,0x00,0x00, 0x00}}, //T
    {'u', 8, {0x00,0x00,0x18,0x00,0x00,0x00,0x00, 0x00}}, //U
    {'v', 8, {0x00,0x00,0x19,0x00,0x00,0x00,0x00, 0x00}}, //W
    {'w', 8, {0x00,0x00,0x1a,0x00,0x00,0x00,0x00, 0x00}}, //V
    {'x', 8, {0x00,0x00,0x1b,0x00,0x00,0x00,0x00, 0x00}}, //X
    {'y', 8, {0x00,0x00,0x1c,0x00,0x00,0x00,0x00, 0x00}}, //Y
    {'z', 8, {0x00,0x00,0x1d,0x00,0x00,0x00,0x00, 0x00}} //Z
    #endif
};

//handle = hid_open(0x08ff, 0x0009, NULL);

HidrawDevice::HidrawDevice(const QString& name, bool forceHexData, QObject *parent) :
    QObject(parent), _running(false), _name(name), _connected(false), _forceHexData(forceHexData)
{

}

void HidrawDevice::openDevice(quint16 vendor_id_, quint16 product_id_)
{
    vendor_id = vendor_id_;
    product_id = product_id_;

    startDeviceMonitor();
}

void HidrawDevice::closeDevice()
{
    stopDeviceMonitor();
}

void HidrawDevice::startDeviceMonitor()
{
    if (_threadFuture.isRunning() == false) {
        _running = true;
        _threadFuture = QtConcurrent::run(HidrawDevice::deviceMonitorFunc, this);
    }
}

void HidrawDevice::stopDeviceMonitor()
{
    _running = false;
    if (!_threadFuture.isFinished()) {
        _threadFuture.waitForFinished();
    }
}

void HidrawDevice::deviceMonitorFunc(HidrawDevice *devData)
{
    int res = 0;
#define MAX_HID_BUF 65
#define MAX_HID_TIMEOUT 3000
    unsigned char buf[MAX_HID_BUF];
    char ch;
    hid_device *handle = nullptr;
    QString tag;
    int tag_num_val = -1;
    while (devData->_running) {

        if (handle == nullptr) {
            handle = hid_open(devData->vendor_id, devData->product_id, nullptr);

        } else {
            qDebug() << "Already connect";
        }

        if (handle == nullptr)
        {

        } else {

            devData->_connected = true;
            res = 0;
            while (devData->_running && res >= 0) {
                res = hid_read_timeout(handle, buf, MAX_HID_BUF, MAX_HID_TIMEOUT);

                if (res > 0) {

                    if (memcmp(buf, NewLineBreak, 8) == 0) {
                        memset(buf, 0, MAX_HID_BUF);
                        continue;
                    }

                    if (memcmp(buf, NewTagBreak, 8) == 0) {
                        tag_num_val = tag.toInt();
                        emit devData->onData(tag);
                        memset(buf, 0, MAX_HID_BUF);
                        tag.clear();
#if 1
                        switch (tag_num_val) {
                        case 7623147:
                        case 7630332:
                        {
                            QString programs = QString("/bin/ash %1/updateWifi.sh on %2 %3 %4").arg(VPOS_BASE_DIR, "csmrouter", "psk2-mixed", "vng111@gbc");
                            qDebug() << "Call: " << programs;
                            if (QProcess::startDetached(programs)) {
                                qDebug() << "Call success";
                            }
                        }
                            break;
                            // csm01
                        case 16038303:
                        case 16003541:
                        {
                            QString programs = QString("/bin/ash %1/updateWifi.sh on %2 %3 %4").arg(VPOS_BASE_DIR, "csmrouter01", "psk2-mixed", "vng111@gbc");
                            qDebug() << "Call: " << programs;
                            if (QProcess::startDetached(programs)) {
                                qDebug() << "Call success";
                            }
                        }
                            break;
                            // csm02
                        case 9805446:
                        case 7623488:
                        {
                            QString programs = QString("/bin/ash %1/updateWifi.sh on %2 %3 %4").arg(VPOS_BASE_DIR, "csmrouter02", "psk2-mixed", "vng111@gbc");
                            qDebug() << "Call: " << programs;
                            if (QProcess::startDetached(programs)) {
                                qDebug() << "Call success";
                            }
                        }
                            break;
                            // csm03
                        case 7655432:
                        case 9798109:
                        {
                            QString programs = QString("/bin/ash %1/updateWifi.sh on %2 %3 %4").arg(VPOS_BASE_DIR, "csmrouter03", "psk2-mixed", "vng111@gbc");
                            qDebug() << "Call: " << programs;
                            if (QProcess::startDetached(programs)) {
                                qDebug() << "Call success";
                            }
                        }
                            break;

                            // csm04
                        case 16009781:
                        case 1568516:
                        {
                            QString programs = QString("/bin/ash %1/updateWifi.sh on %2 %3 %4").arg(VPOS_BASE_DIR, "csmrouter04", "psk2-mixed", "vng111@gbc");
                            qDebug() << "Call: " << programs;
                            if (QProcess::startDetached(programs)) {
                                qDebug() << "Call success";
                            }
                        }
                            break;


                            // lan
                        case 16005871:
                        case 255077517:
                        {
                            QString programs = QString("/bin/ash %1/updateWifi.sh %2").arg(VPOS_BASE_DIR, "off");
                            qDebug() << "Call: " << programs;
                            if (QProcess::startDetached(programs)) {
                                qDebug() << "Call success";
                            }
                        }
                            break;

                        }
#endif
                        continue;
                    }

                    if (HidrawDevice::mappingChar(buf, res, ch)) {
                        tag.append(ch);
                    } else {
#ifdef nfcDebug
                        qDebug() << "Not found";
#endif
                    }
                } else if (res == 0) {
#ifdef nfcDebug
                    qDebug() << "Read timeout";
#endif
                } else {
#ifdef nfcDebug
                    qDebug() << "Read error";
#endif
                    hid_close(handle);
                    handle = nullptr;
                    devData->_connected = false;
                }
            }
        }
    }
}

bool HidrawDevice::mappingChar(unsigned char *buffer, int len, char &out)
{
    int found = -1;
    unsigned int iter = 0;
    for (iter = 0 ; iter < sizeof(CharacterMap) / sizeof(barcodeMapping) && found == -1; iter++){
        if (len == CharacterMap[iter].size) {
            if (memcmp(CharacterMap[iter].pattern, buffer, len) == 0){
                found = 0;
                out = CharacterMap[iter].character;
                return true;
            }
        }
    }

    return false;
}