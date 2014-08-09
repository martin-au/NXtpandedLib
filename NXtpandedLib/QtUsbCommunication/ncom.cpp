#include "ncom.h"
#include <QSharedPointer>

#include <QDebug>

namespace QNxt {

inline unsigned char* num2Bytes (qint32 num, unsigned char *pstart) {
    *pstart 	=  (num >> 24) & 0xFF;
    *(pstart+1) =  (num >> 16) & 0xFF;
    *(pstart+2) =  (num >> 8) & 0xFF;
    *(pstart+3) =   num & 0xFF;
    return (pstart+4);
}

inline unsigned char* num2Bytes (quint32 num, unsigned char *pstart) {
    *pstart 	=  (num >> 24) & 0xFF;
    *(pstart+1) =  (num >> 16) & 0xFF;
    *(pstart+2) =  (num >> 8) & 0xFF;
    *(pstart+3) =   num & 0xFF;
    return (pstart+4);
}

inline unsigned char* bytes2Num (qint32 &num, unsigned char *pstart) {
    num = (*pstart << 24) | (*(pstart+1) << 16) | (*(pstart+2) << 8) | (*(pstart+3));
    return (pstart+4);
}

inline unsigned char* bytes2Num (quint32 &num, unsigned char *pstart) {
    num = (*pstart << 24) | (*(pstart+1) << 16) | (*(pstart+2) << 8) | (*(pstart+3));
    return (pstart+4);
}



NCom::NCom()
    : pollTimer(new QTimer(this))
{
    clear();
}

NCom::~NCom() {
    stopHandler();
    if(isConnected()) {
        close();
    }
}

bool NCom::findNxt() {
    if (NXTCommFantom_find(nxt_id) == 0) {
        return false;
    }
    return true;
}

bool NCom::open() {
    /* open the first found NXT */
    nxt = NXTCommFantom_open(nxt_id);
    if(nxt == 0) return false;

    /* connect with ECRobot USB API */
    if (NXTCommFantom_connect(nxt) == 0) {
        NXTCommFantom_close(nxt);
        return false;
    }
    emit opend();
    return true;
}


void NCom::close() {
    header mHeader;
    mHeader.disconReq = disconnect;
    data[headerByteIdx] = mHeader.asU8;
    send(data, 0, comDatatype::typeU32, comNModes::modeBasic);
    NXTCommFantom_close(nxt);
    emit closed();
}


bool NCom::isConnected() {
    if (nxt == NULL) {
        return false;
    }
    return true;
}


void NCom::startHandler(int pollIntervall) {
    QObject::connect(pollTimer, SIGNAL(timeout()), this, SLOT(handler()));
    pollTimer->start(pollIntervall);
}

void NCom::stopHandler() {
    pollTimer->stop();
    QObject::disconnect(pollTimer, SIGNAL(timeout()), this, SLOT(handler()));
}


quint32 NCom::send(quint8 *data, quint8 idx, comDatatype datatype, comNModes nmode, int len) {
    if(!isConnected() || data == 0) {
        return 0;
    }
    header mHeader;
    mHeader.disconReq = noDisconnect;
    mHeader.datatype = datatype;
    mHeader.mode = nmode;

    data[headerByteIdx] = mHeader.asU8;
    data[idxByteIdx] = idx;

    quint32 retlen = NXTCommFantom_send(nxt, static_cast<const unsigned char*>(data), 0, headerOverhead+len);
    clear();
    return retlen;
}


quint32 NCom::receive(quint8 *data, quint8 &idx, comDatatype &datatype, comNModes &nmode) {
    quint32 len = NXTCommFantom_receive(nxt, static_cast<unsigned char*>(data), 0, MAX_PACKAGE_LEN);
    if(len > 0) {
        header mHeader;
        mHeader.asU8 = data[headerByteIdx];
        if(mHeader.disconReq == disconnect) {
            close();
            return 0;
        }
        idx = data[idxByteIdx];
        datatype = static_cast<comDatatype>(mHeader.datatype);
        nmode = static_cast<comNModes>(mHeader.mode);
    }
    return len;
}


quint32 NCom::send(quint32 n, quint8 idx) {
    NCom::comDatatype type = NCom::typeU32;
    NCom::comNModes mode = NCom::modeSingle;

    num2Bytes(n, &data[data0ByteIdx]);
    return this->send(data, idx, type, mode, 4);
}


quint32 NCom::send(qint32 n, quint8 idx) {
    NCom::comDatatype type = NCom::typeS32;
    NCom::comNModes mode = NCom::modeSingle;

    num2Bytes(n, &data[data0ByteIdx]);
    return this->send(data, idx, type, mode, 4);
}

quint32 NCom::send(bool b, quint8 idx) {
    NCom::comDatatype type = NCom::typeBool;
    NCom::comNModes mode = NCom::modeSingle;

    data[data0ByteIdx] = static_cast<unsigned char>(b);
    return this->send(data, idx, type, mode, 1);
}


quint32 NCom::send(float num, quint8 idx) {
    NCom::comDatatype type = NCom::typeFloat;
    NCom::comNModes mode = NCom::modeSingle;

    if(sizeof(float) == 4) {
        floatUnion_t fu;
        fu.f = num;

        memcpy(&data[data0ByteIdx], fu.bytes, 4);
        return this->send(data, idx, type, mode, 4);
    }
    return -1;
}

quint32 NCom::send(char ch, quint8 idx) {
    NCom::comDatatype type = NCom::typeChar;
    NCom::comNModes mode = NCom::modeSingle;

    data[data0ByteIdx] = static_cast<unsigned char>(ch);
    return this->send(data, idx, type, mode, 1);
}


quint32 NCom::send(const QString &string, quint8 idx) {
    NCom::comDatatype type = NCom::typeString;
    NCom::comNModes mode = NCom::modeSingle;

    //const char *str = string.toStdString().c_str();

    //qDebug() << "Sending: " << static_cast<int>(str[1]);

    // Problems with converting, this is not nice but it solves the problem..
    char *str = new char[string.size()+1];
    for(int i=0; i<string.size(); ++i) {
        char ch = string.at(i).toLatin1();
        str[i] = ch;
    }
    str[string.size()] = '\0';

    if(string.size()+1 <= MAX_DATA_LEN) {
        memcpy(&data[data0ByteIdx], str, string.size()+1);
    } else {
        data[MAX_PACKAGE_LEN-1] = static_cast<unsigned char>('\0');
        memcpy(&data[data0ByteIdx], str, NCom::MAX_DATA_LEN-1);
    }
    delete[] str;
    return this->send(data, idx, type, mode, string.size()+1); // low level send will cut away to big len
}



quint32 NCom::send(const QVector<quint32> &vector, quint8 idx) {
    NCom::comDatatype type = NCom::typeU32;
    NCom::comNModes mode = NCom::modePackage;

    const int maxlen = ((NCom::MAX_DATA_LEN/sizeof(quint32)) >= vector.size()) ?
    vector.size() : NCom::MAX_DATA_LEN/sizeof(quint32);

    unsigned char *pstart = &data[NCom::data0ByteIdx];

    for(int i=0; i < maxlen; ++i) {
        pstart = num2Bytes(vector.at(i), pstart);
    }
    int retlen = this->send(data, idx, type, mode, (maxlen*4));
    return retlen;
}


quint32 NCom::send(const QVector<qint32> &vector, quint8 idx) {
    NCom::comDatatype type = NCom::typeS32;
    NCom::comNModes mode = NCom::modePackage;

    const int maxlen = ((NCom::MAX_DATA_LEN/sizeof(qint32)) >= vector.size()) ?
    vector.size() : NCom::MAX_DATA_LEN/sizeof(qint32);

    unsigned char *pstart = &data[NCom::data0ByteIdx];

    for(int i=0; i < maxlen; ++i) {
        pstart = num2Bytes(vector.at(i), pstart);
    }
    int retlen = this->send(data, idx, type, mode, (maxlen*4));
    return retlen;
}


void NCom::handler() {
    if(isConnected()) {
        quint8 idx = 0;
        comDatatype type;
        comNModes mode;

        quint32 len = receive(data, idx, type, mode);
        if(len > 0) {
            if(type == typeU32 && mode == modeSingle) {
                quint32 ret = 0;
                bytes2Num(ret, &data[data0ByteIdx]);
                emit received(ret, idx);
            } else if(type == typeS32 && mode == modeSingle) {
                qint32 ret = 0;
                bytes2Num(ret, &data[data0ByteIdx]);
                emit received(ret, idx);
            } else if(type == typeBool && mode == modeSingle) {
                bool ret = (data[data0ByteIdx] > 0) ? true : false;
                emit received(ret, idx);
            } else if(type == typeFloat && mode == modeSingle) {
                if(sizeof(float) == 4) {
                    floatUnion_t fu;
                    memcpy(fu.bytes , &data[data0ByteIdx], 4);
                    emit received(fu.f, idx);
                }
            } else if(type == typeChar && mode == modeSingle) {
                emit received(static_cast<char>(data[data0ByteIdx]), idx);
            } else if(type == typeString && mode == modeSingle) {
                QString ret("");
                for(int i=2; ; i++) {
                    // just in case someone try to send a not 0 terminated string!
                    if(i >= MAX_PACKAGE_LEN) {
                        data[i-1] = '\0';
                        break;
                    }
                    char ch = static_cast<char>(data[i]);
                    if(ch == '\0') break;
                    ret += ch;
                }
                emit received(ret, idx);
            } else if(type == typeU32 && mode == modePackage) {
                int numPackage = (len-headerOverhead)/4;
                //qDebug() << "numPackage: " << numPackage;
                QVector<quint32> vec(numPackage);

                unsigned char *pstart = &data[data0ByteIdx];
                for(int i=0; i<numPackage; ++i) {
                    quint32 num = 0;
                    pstart = bytes2Num(num, pstart);
                    vec.insert(i, num);
                }
                emit received(vec, idx);
            } else if(type == typeS32 && mode == modePackage) {
                int numPackage = (len-headerOverhead)/4;
                //qDebug() << "numPackage: " << numPackage;
                QVector<qint32> vec(numPackage);

                unsigned char *pstart = &data[data0ByteIdx];
                for(int i=0; i<numPackage; ++i) {
                    qint32 num = 0;
                    pstart = bytes2Num(num, pstart);
                    vec.insert(i, num);
                }
                emit received(vec, idx);
            } else {
                qDebug() << "Handler Error:";
                qDebug() << "Received:";
                qDebug() << "Len: " << len;
                qDebug() << "Idx: " << idx;
                qDebug() << "Type: " << type;
                qDebug() << "Mode: " << mode;
                qDebug() << "Header: " << static_cast<int>(data[0]);
            }

        clear();
        }
    }
}

}
