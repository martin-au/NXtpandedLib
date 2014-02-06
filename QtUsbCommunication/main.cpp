

#include "nxtcommfantom.h"
#include "ncom.h"

#include <QCoreApplication>
#include <QDebug>
#include <QTimer>

#include "logg.h"

#include "Windows.h"



int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);
    qDebug() << "Starting";

    QNxt::NCom com;

    do {
        // find a NXT
        if (!com.findNxt())
        {
            qDebug() << "NXT not found.";
            return 0;
        }

        // open the first found NXT
        if (!com.open())
        {
            qDebug() << "NXT found, but failed to open.";
            return 0;
        }
        Sleep(100);
    } while(!com.isConnected());

    qDebug() << "Connected!";

    /* unique device name can be given to each NXT
         * and it allows to communicate with multiple NXTs
         */
    //	if (NXTCommFantom_getName(nxt, name) == 1)
    //	{
    //		printf("NXT Device Name: %s\n", name);
    //	}


    /* now, this program could be connected with a NXT which uses
         * ECRobot USB API
         */


    /* echo back keyboard input */


    auto *logger = new Logg();
    QTimer *timer2 = new QTimer;

    QObject::connect(&com, SIGNAL(received(quint32,quint8)), logger, SLOT(out(quint32,quint8)));
    QObject::connect(&com, SIGNAL(received(qint32,quint8)), logger, SLOT(out(qint32,quint8)));
    QObject::connect(&com, SIGNAL(received(bool,quint8)), logger, SLOT(out(bool,quint8)));
    QObject::connect(&com, SIGNAL(received(float,quint8)),logger, SLOT(out(float,quint8)));
    QObject::connect(&com, SIGNAL(received(char,quint8)), logger, SLOT(out(char,quint8)));
    QObject::connect(&com, SIGNAL(received(QString,quint8)), logger, SLOT(out(QString,quint8)));
    QObject::connect(&com, SIGNAL(received(QVector<quint32>,quint8)), logger, SLOT(out(QVector<quint32>,quint8)));

    QObject::connect(timer2, SIGNAL(timeout()), logger, SLOT(send()));

    QObject::connect(logger, SIGNAL(sendThis(quint32,quint8)), &com, SLOT(send(quint32,quint8)));
    QObject::connect(logger, SIGNAL(sendThis(qint32,quint8)), &com, SLOT(send(qint32,quint8)));
    QObject::connect(logger, SIGNAL(sendThis(bool,quint8)), &com, SLOT(send(bool,quint8)));
    QObject::connect(logger, SIGNAL(sendThis(float,quint8)), &com, SLOT(send(float,quint8)));
    QObject::connect(logger, SIGNAL(sendThis(char,quint8)), &com, SLOT(send(char,quint8)));
    QObject::connect(logger, SIGNAL(sendThis(QString,quint8)), &com, SLOT(send(QString,quint8)));
    QObject::connect(logger, SIGNAL(sendThis(QVector<qint32>,quint8)), &com, SLOT(send(QVector<qint32>,quint8)));

    timer2->start(901);

    com.startHandler(999);
    QObject::connect(&a, SIGNAL(aboutToQuit()), &com, SLOT(close()));

    return a.exec();
}
