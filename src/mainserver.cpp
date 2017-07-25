#include "serverapp.h"
#include <QApplication>



int main(int argc, char *argv[])
{
  QApplication Qapp (argc, argv);
  Identity server;
  server.setName("Server");
  server.setAddress(QHostAddress(QString(argv[1]))); 
  server.setPort(atoi(argv[2]));
  ServerApp* serverapp = new ServerApp(server);

  return Qapp.exec();
}
  
