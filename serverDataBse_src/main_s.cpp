

#include "Server.hpp"


int main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  Server fenetre;
  fenetre.show();

  return app.exec();
}

