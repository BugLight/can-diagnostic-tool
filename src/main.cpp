#include "candiagnostictool.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  CanDiagnosticTool w;
  w.show();
  return a.exec();
}
