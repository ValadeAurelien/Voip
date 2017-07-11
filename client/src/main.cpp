#include <QApplication>
#include <application.h>

int main (int argc, char **argv)
{
  QApplication Qapp(argc, argv);
  Application * App = new Application();
  App->showMainWindow();
  
  return Qapp.exec();
}

