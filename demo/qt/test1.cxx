
#ifdef __CINT__

#include <qtcint.dll>

#else

#include <qapplication.h>
#include <qpushbutton.h>

#endif

int main(int argc, char **argv) {
  QApplication a(argc,argv);
  QPushButton hello("Hello world!",0);
  hello.resize(100,30);
  a.setMainWidget(&hello);
  hello.show();
  return (a.exec());
}

