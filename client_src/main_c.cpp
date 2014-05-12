

#include <QApplication>

#include <QImage>
#include <iostream>

#include "Client.hpp"


int	main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  FenClient fenetre;
  fenetre.show();

  return app.exec();
}

