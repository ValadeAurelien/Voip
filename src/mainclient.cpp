#include "application.h"
#include <QApplication>



int main(int argc, char *argv[])
{
  QApplication Qapp (argc, argv);
  Application* app = new Application();
  app->showMainWindow();

  return Qapp.exec();
}
  
