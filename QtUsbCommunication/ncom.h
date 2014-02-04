
#ifndef NCOM_H
#define NCOM_H

#include <QObject>
#include <QQueue>
#include <QMap>

#include "nxtcommfantom.h"

namespace QNxt {

class NCom : public QObject
{
    Q_OBJECT
private:
    union header {
        struct {
            quint8 disconReq :1;
            quint8 datatype :4;
            quint8 mode :3;
        };
        quint8 asU8;
    };

    union floatUnion_t {
        float f;
        unsigned char bytes[4];
    };

    ViChar nxt_id[MAX_DEV_ID];
    nFANTOM100_iNXT nxt;

    ViChar name[MAX_DEV_NAME];

    quint8 data[MAX_PACKAGE_LEN];

    //QQueue<QMap<int, quint32>> buf_stack;
public:
    static const quint8 MAX_DATA_LEN = MAX_PACKAGE_LEN - 2;

    enum disconReq {
        noDisconnect, disconnect
    };
    enum comDatatype {
        typeU32 = 1,
        typeS32 = 2,
        typeBool = 3,
        typeFloat = 4,
        typeChar = 5,
        typeString = 6
    };
    enum comNModes {
        modeBasic = 0,
        modeSingle = 1,
        modePackage = 2,
        modePart = 3,
        modeStream = 4
        //free = 		5  //
        //free = 		6  //
        //free = 		7  //
    };

    NCom();
    ~NCom();

    bool isConnected();

    void clear() {
        memset(data, 0, MAX_PACKAGE_LEN);
    }

public slots:
    bool findNxt();
    bool open();
    void close();

    quint32 send(quint8 *data, quint8 idx, comDatatype datatype, comNModes nmode);
    quint32 receive(quint8 *data, quint8 &idx, comDatatype &datatype, comNModes &nmode);

    void handler();

    quint32 send(quint32 n, quint8 idx = 0);
    quint32 send(qint32 n, quint8 idx = 0);
    quint32 send(bool b, quint8 idx = 0);
    quint32 send(float num, quint8 idx = 0);
    quint32 send(char ch, quint8 idx = 0);
    quint32 send(const QString &string, quint8 idx = 0);

signals:
    void opend();
    void closed();

    void received(quint32 data, quint8 idx);
    void received(qint32 data,  quint8 idx);
    void received(bool data,    quint8 idx);
    void received(float data,   quint8 idx);
    void received(char data,   quint8 idx);
    void received(QString data, quint8 idx);
    void received(QVector<quint32> data, quint8 idx);
};


}

#endif // NCOM_H
