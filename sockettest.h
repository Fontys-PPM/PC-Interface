#ifndef SOCKETTEST_H
#define SOCKETTEST_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>



class SocketTest : public QObject
{
    Q_OBJECT
public:
    explicit SocketTest(QObject *parent = 0);

    QString Connect(QString adress, int port );

    QString Connect(QString adress, int port, QString message );

signals:

public slots:

private:
    QTcpSocket *socket;
    
};

#endif // SOCKETTEST_H
