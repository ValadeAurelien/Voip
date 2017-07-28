#include "application.h"
#include <QApplication>
#include <type_traits>
#include <cassert>
#include <iostream>

int main(int argc, char *argv[])
{
  QApplication Qapp (argc, argv);
  Identity aurelien;
  aurelien.setName("aurélien");
  aurelien.setAddress(QHostAddress(QString(argv[1]))); 
  aurelien.setPort(atoi(argv[2]));
  Application* app = new Application(aurelien);
  assert(sizeof(DatagramHD)==DATAGRAMSIZE);
  app->showMainWindow();

  return Qapp.exec();
}
  
