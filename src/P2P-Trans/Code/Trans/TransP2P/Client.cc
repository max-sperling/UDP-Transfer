/************************/
/* Author: Max Sperling */
/************************/

#include "Client.hh"

#include <iostream>
#include <thread>
using namespace std;

// ***** Public ************************************************************************************
Client::Client()
{
    thread = new QThread();
    socket = new QTcpSocket();
    //moveToThread(thread);
}

bool Client::init(string addr, unsigned int port)
{
    this->addr = QString::fromStdString(addr);
    this->port = port;
    connect(socket, SIGNAL(readyRead()), this, SLOT(onGetTCPStream()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(thread, SIGNAL(started()), this, SLOT(onStartedThread()));
    thread->start();
    return true;
}
// *************************************************************************************************

// ***** Private ***********************************************************************************
bool Client::connectToServer()
{
    if(socket->state() == QTcpSocket::ConnectedState)
        return true;

    socket->connectToHost(addr, port);

    if(!socket->waitForConnected(2000))
    {
        qDebug() << "Can not connect";
        return false;
    }
    qDebug() << "Connected";

    waitForInput();
    return true;
}

void Client::waitForInput()
{
    sendTCPStream(QByteArray::fromStdString("register"));
}

bool Client::sendTCPStream(QByteArray data)
{
    if(socket->state() != QTcpSocket::ConnectedState)
        return false;

    socket->write(data);
    return true;
}
// *************************************************************************************************

// ***** Private Slots *****************************************************************************
void Client::onStartedThread()
{
    connectToServer();
}

void Client::onGetTCPStream()
{
    QString content = socket->readAll();

    if(content == "connected")
    {
        qDebug() << "Comm: connected to Host";
        //emit changedStateConnection(true);
    }
    // else if(content == "started")
    // {
    //     emit addLog("Comm: conference started");
    //     emit changedStateConference(true);
    // }
    // else if(content == "removed")
    // {
    //     emit addLog("Comm: removed from Host");
    //     emit changedStateConference(false);
    //     emit changedStateConnection(false);
    //     m_tcpSocketHost->disconnectFromHost();
    // }
    // else if(content == "closed")
    // {
    //     emit addLog("Comm: conference closed");
    //     emit changedStateConference(false);
    // }
    // else if(content == "disconnected")
    // {
    //     addLog("Comm: disconnected from Host");
    //     emit changedStateConnection(false);
    //     m_tcpSocketHost->disconnectFromHost();
    // }
    // else addLog("Comm: unknown Stream");
}

void Client::onDisconnected()
{
    qDebug() << "Connection closed";
    socket->deleteLater();
}
// *************************************************************************************************
