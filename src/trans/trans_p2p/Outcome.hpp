#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include <QThread>
#include <QTcpSocket>
#include <QFile>
#include <string>
#include "../../view/IView.hpp"

class Outcome : public QThread
{
    Q_OBJECT

public:
    Outcome(IViewPtr viewPtr, std::string addr, unsigned int port, std::string fileName);
    ~Outcome();

protected:
    void run();

private:
    bool connectToServer();
    bool sendFile();

    IViewPtr m_viewPtr;
    std::string m_addr;
    unsigned int m_port;
    std::string m_fileName;
    std::string m_logIdent;

    QTcpSocket *m_socket;
    QFile m_file;

private slots:
    void onDisconnected();
};