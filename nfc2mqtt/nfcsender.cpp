#include "nfcsender.h"

nfcsender::nfcsender(quint16 qrpid, quint16 qrvid)
{
    // Initialize NFC
    readHidQrcode = new DeviceManager();
    connect(readHidQrcode, SIGNAL(onMifare(const QString)),
            this,SLOT(nfcDataChanged(const QString)),Qt::QueuedConnection);

    readHidQrcode->value_pid = qrpid;
    readHidQrcode->value_vid = qrvid;
    readHidQrcode->start();

#ifdef nfcDebug
    qDebug() << "HID Initialized!!!";
#endif
}

void nfcsender::nfcDataChanged(const QString &nfc)
{
#ifdef nfcDebug
    qDebug() << "HID QR data:" << nfc;
#endif
    if(!nfc.isEmpty())
    {
#ifdef nfcDebug
        qDebug() << nfc;
#endif

        emit nfcDataUpdated(nfcEncode(nfc));
    }
}

QString nfcsender::nfcEncode(QString nfcData)
{
    // Chuyen doi sang ma Hex
    QString result = QString::number(nfcData.toUInt(), 16).toUpper();
#ifdef nfcDebug
    qDebug() << "result:" << result;
#endif

    /*  Thuc hien ma hoa chuoi data cua nfc
        Solution:
        - Cắt hai kí tự liên tiếp --> lưu vào một thanh ghi để lưu trữ
        - Đổi thứ tự của các kí tự để mã hóa đúng như yêu cầu

        Explain:
        - C480748A
    */

    QString resultEcode;
    // Tao list QString de luu cac tu ma hoa
    QStringList listContainEcodeNfc;
    for(int position = 0; position < result.size(); position ++)
    {
        if(0 == position%2) {
            resultEcode.insert(0, QString(result.mid(position, 2)));
            if(position < (result.size() - 2)) {
                resultEcode.insert(0, ":");
            }
        }
    }

    resultEcode =resultEcode.insert(0, "00:00:00:");
    if(resultEcode.size() > 20)
    {
        QString lastResultEcode = resultEcode.mid((resultEcode.size() - 20 - 1) ,resultEcode.size());
#ifdef nfcDebug
    qDebug() << "lastResultEcode:" << lastResultEcode;
#endif
        return lastResultEcode;
    }
#ifdef nfcDebug
    qDebug() << "result:" << resultEcode;
#endif

    return resultEcode;
}
