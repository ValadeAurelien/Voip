#include "auxwindows.h"


ConnectionWindow::ConnectionWindow : QWdiget()
{
  setWindowTitle("Manage Connection");
  QLabel *ask_for_ha = new QLabel("Enter host address (IPv4, IPv6, URL) : ");
  QLineEdit *le_host_adress = new QLineEdit;
  QPushButton *button_cancel = new QPushButton,
              *button_confirm = new QPushButton;
  QGridLayout *wc_grid = QGridLayout(this);
  wc_grid->addWidget(ask_for_ha, 0, 0, 1, 2);
  wc_grid->addWidget(le_host_adress, 1, 0, 1, 2);
  wc_grid->addWidget(button_cancel, 2, 0, 1, 1); wc_grid->addWidget(button_confirm, 2, 1, 1, 1);

  connect(button_cancel, SIGNAL(clicked()), this, SLOT(close()));
  connect(button_confirm, SIGNAL(clicked()), this, SLOT(send_ha()));
}

