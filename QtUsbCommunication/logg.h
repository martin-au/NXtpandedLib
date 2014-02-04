#ifndef LOGG_H
#define LOGG_H


#include <QDebug>
#include <QObject>

class Logg : public QObject
{
    Q_OBJECT
private:
    int state;
public:
    explicit Logg(QObject *parent = 0);

public slots:
    void out(quint32 data, quint8 idx) {
        qDebug() << "idx: " << idx <<  "Received: " << data;
    }

    void out(qint32 data, quint8 idx) {
        qDebug() << "idx: " << idx <<  "Received: " << data;
    }

    void out(bool data, quint8 idx) {
        qDebug() << "idx: " << idx <<  "Received: " << data;
    }

    void out(float data, quint8 idx) {
        qDebug() << "idx: " << idx <<  "Received: " << data;
    }

    void out(char data, quint8 idx) {
        qDebug() << "idx: " << idx <<  "Received: " << data;
    }

    void out(QString data, quint8 idx) {
        qDebug() << "idx: " << idx <<  "Received: " << data;
    }
    void out(QVector<quint32> data,quint8 idx) {
        qDebug() << "idx: " << idx <<  "Received: " << data;
    }

    void send() {
        switch(state) {
        case 0: emit sendThis(static_cast<quint32>(12345), 1); break;
        case 1: emit sendThis(static_cast<qint32>(-1234), 1); break;
        case 2: emit sendThis(true, 1); break;
        case 3: emit sendThis(static_cast<float>(-12.34), 1); break;
        case 4: emit sendThis('M', 1); break;
        case 5: QString str("C++"); emit sendThis(str, 1); break;
        }
        state++;
    }
signals:
    void sendThis(quint32 n, quint8 idx);
    void sendThis(qint32 n, quint8 idx);
    void sendThis(bool n, quint8 idx);
    void sendThis(float n, quint8 idx);
    void sendThis(char n, quint8 idx);
    void sendThis(QString n, quint8 idx);
};
#endif // LOGG_H
