#include "serverapp.h"
#include <QString>
#include <iostream>
#include <string>


ServerApp::ServerApp() : QObject()
{
  udpsocket.bind(QHostAddress::LocalHost, port);
  connect(&udpsocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(bindingFailed(QAbstractSocket::SocketError)));
}

void ServerApp::listenAndCout()
{
  connect(&udpsocket, SIGNAL(readyRead()), this, SLOT(coutMessage()));
}

void ServerApp::bindingFailed(QAbstractSocket::SocketError serr)
{
  reportError(QString("Socket error : ") + socketerrorname(serr));
}

void ServerApp::coutMessage()
{
  host1dginsize = udpsocket.pendingDatagramSize();
  if (host1dginsize > ba_host1messagein.capacity()) ba_host1messagein.resize(host1dginsize);
  udpsocket.readDatagram(ba_host1messagein.data(), host1dginsize, &host1address, &host1port);
  std::cout << "Message from : " << host1address.toString().toStdString() << ":" << host1port 
            << " (" << ba_host1messagein.size() << " bytes) > " 
            << QString(ba_host1messagein).toStdString() << std::endl;
}

void ServerApp::reportError(QString err)
{
  std::cout << err.data() << std::endl;
}
