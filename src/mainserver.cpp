#include "serverapp.h"
#include <QApplication>



int main(int argc, char *argv[])
{
  QApplication Qapp (argc, argv);
  ServerApp* serverapp = new ServerApp();

  return Qapp.exec();
}
  
