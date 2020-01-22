#include "sockettest.h"

SocketTest::SocketTest(QObject *parent) :
    QObject(parent)
{
}

QString SocketTest::Connect(QString adress, int port )
{
    socket = new QTcpSocket(this);
    //socket->connectToHost("bogotobogo.com", 80);
    socket->connectToHost(adress, port);

    if(socket->waitForConnected(3000))
    {
        qDebug() << "Connected!";

        // send
        socket->write("hello server\r\n\r\n\r\n\r\n");
        socket->waitForBytesWritten(1000);
        socket->waitForReadyRead(3000);
        qDebug() << "Reading: " << socket->bytesAvailable();


        QByteArray byteArray = socket->readAll();
        qDebug() << byteArray;

        QString result = QString::fromStdString(byteArray.toStdString());

        socket->close();
        return result;
    }
    else
    {
        qDebug() << "Not connected!";
        return "Not connected";
    }

    // sent

    // got

    // closed
}
