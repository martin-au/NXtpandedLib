#include "ncom.h"

#include <QDebug>

namespace QNxt {

NCom::NCom()
{
    clear();
}

NCom::~NCom() {
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
    quint8 data[MAX_DATA_LEN];
    header mHeader;
    mHeader.disconReq = disconnect;
    data[0] = mHeader.asU8;
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


quint32 NCom::send(quint8 *data, quint8 idx, comDatatype datatype, comNModes nmode) {
    if(!isConnected() || data == 0) {
        return 0;
    }
    header mHeader;
    mHeader.disconReq = noDisconnect;
    mHeader.datatype = datatype;
    mHeader.mode = nmode;

    data[0] = mHeader.asU8;
    data[1] = idx;

    quint32 len = NXTCommFantom_send(nxt, static_cast<const unsigned char*>(data), 0, MAX_DATA_LEN);
    clear();
    return len;
}


quint32 NCom::receive(quint8 *data, quint8 &idx, comDatatype &datatype, comNModes &nmode) {
    quint32 len = NXTCommFantom_receive(nxt, static_cast<unsigned char*>(data), 0, MAX_DATA_LEN);
    if(len > 0) {
        header mHeader;
        mHeader.asU8 = data[0];
        if(mHeader.disconReq == disconnect) {
            close();
            return 0;
        }
        idx = data[1];
        datatype = static_cast<comDatatype>(mHeader.datatype);
        nmode = static_cast<comNModes>(mHeader.mode);
    }
    return len;
}


quint32 NCom::send(quint32 n, quint8 idx) {
    NCom::comDatatype type = NCom::typeU32;
    NCom::comNModes mode = NCom::modeSingle;

    data[2] =  (n >> 24) & 0xFF;
    data[3] =  (n >> 16) & 0xFF;
    data[4] =  (n >> 8) & 0xFF;
    data[5] =  n & 0xFF;
    return this->send(data, idx, type, mode);
}


quint32 NCom::send(qint32 n, quint8 idx) {
    NCom::comDatatype type = NCom::typeS32;
    NCom::comNModes mode = NCom::modeSingle;

    data[2] =  (n >> 24) & 0xFF;
    data[3] =  (n >> 16) & 0xFF;
    data[4] =  (n >> 8) & 0xFF;
    data[5] =  n & 0xFF;
    return this->send(data, idx, type, mode);
}

quint32 NCom::send(bool b, quint8 idx) {
    NCom::comDatatype type = NCom::typeBool;
    NCom::comNModes mode = NCom::modeSingle;

    data[2] = static_cast<unsigned char>(b);
    return this->send(data, idx, type, mode);
}


quint32 NCom::send(float num, quint8 idx) {
    NCom::comDatatype type = NCom::typeFloat;
    NCom::comNModes mode = NCom::modeSingle;

    if(sizeof(float) == 4) {
        floatUnion_t fu;
        fu.f = num;

        memcpy(data+2, fu.bytes, 4);
        return this->send(data, idx, type, mode);
    }
    return -1;
}

quint32 NCom::send(char ch, quint8 idx) {
    NCom::comDatatype type = NCom::typeChar;
    NCom::comNModes mode = NCom::modeSingle;

    data[2] = static_cast<unsigned char>(ch);
    return this->send(data, idx, type, mode);
}


quint32 NCom::send(const QString &string, quint8 idx) {
    NCom::comDatatype type = NCom::typeString;
    NCom::comNModes mode = NCom::modeSingle;

    char *str = string.toLocal8Bit().data();

    if(string.size()+1 <= NCom::MAX_DATA_LEN) {
        memcpy((data+2), str, string.size()+1);
    } else {
        data[MAX_PACKAGE_LEN] = static_cast<unsigned char>('\0');
        memcpy((data+2), str, NCom::MAX_DATA_LEN-1);
    }
    return this->send(data, idx, type, mode);
}



void NCom::handler() {
    if(isConnected()) {
        quint8 idx = 0;
        comDatatype type;
        comNModes mode;

        if(receive(data, idx, type, mode) > 0) {
            if(type == typeU32 && mode == modeSingle) {
                quint32 ret = (data[2] << 24) | (data[3] << 16) | (data[4] << 8) | (data[5]);
                emit received(ret, idx);
                clear();
            } else if(type == typeS32 && mode == modeSingle) {
                qint32 ret = (data[2] << 24) | (data[3] << 16) | (data[4] << 8) | (data[5]);
                emit received(ret, idx);
                clear();
            } else if(type == typeBool && mode == modeSingle) {
                bool ret = (data[2] > 0) ? true : false;
                emit received(ret, idx);
                clear();
            } else if(type == typeFloat && mode == modeSingle) {
                if(sizeof(float) == 4) {
                    floatUnion_t fu;
                    memcpy(fu.bytes , data+2, 4);
                    emit received(fu.f, idx);
                }
                clear();
            } else if(type == typeChar && mode == modeSingle) {
                emit received(static_cast<char>(data[2]), idx);
            } else if(type == typeString && mode == modeSingle) {
                QString ret("");
                for(int i=2; ; i++) {
                    char ch = static_cast<char>(data[i]);
                    if(ch == '\0') break;
                    ret += ch;
                }
                emit received(ret, idx);
            }

        }
    }
}

}
