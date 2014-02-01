#ifndef LOGG_H
#define LOGG_H


#include <QDebug>
#include <QObject>

class Logg : public QObject
{
    Q_OBJECT
public:
    explicit Logg(QObject *parent = 0);

public slots:
    void out(quint8 idx, quint8 data) {
        qDebug() << "idx: " << idx <<  "Received: " << data;
    }
    void send() {
        emit sendThis(123, 1);

    }
signals:
    void sendThis(quint32 n, quint8 idx);
};
#endif // LOGG_H
