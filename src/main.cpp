#include <QApplication>
#include <mainwindow.h>

int main (int argc, char **argv)
{
  QApplication Qapp(argc, argv);
  MainWindow* MW = new MainWindow();
  MW->show();
  
  return Qapp.exec();
}

