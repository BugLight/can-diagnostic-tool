#include "candiagnostictool.h"

#include "deviceerrorview.h"

CanDiagnosticTool::CanDiagnosticTool(QWidget* parent) : QMainWindow(parent) {
  setFont(QFont("Arial", 12));
  setLocale(QLocale(QLocale::Russian, QLocale::Russia));

  central = new QWidget(this);
  setCentralWidget(central);

  mainLayout = new QGridLayout(central);

  leftColumn = new QWidget(central);
  mainLayout->addWidget(leftColumn, 0, 0);

  leftColumnLayout = new QVBoxLayout(leftColumn);

  canBusView = new CanBusView(leftColumn);
  leftColumnLayout->addWidget(canBusView);

  buttonSearch = new QPushButton(u8"Обнаружить устройство", leftColumn);
  leftColumnLayout->addWidget(buttonSearch);

  buttonRead = new QPushButton(u8"Прочитать ошибки", leftColumn);
  leftColumnLayout->addWidget(buttonRead);

  buttonReset = new QPushButton(u8"Очистить ошибки", leftColumn);
  leftColumnLayout->addWidget(buttonReset);

  diagnosticView = new DiagnosticView(central);
  mainLayout->addWidget(diagnosticView, 0, 1);

  connect(canBusView, &CanBusView::BusConnected, diagnosticView,
          &DiagnosticView::SetCanBusDevice);
  connect(buttonSearch, &QPushButton::clicked, diagnosticView,
          &DiagnosticView::SearchDevice);
  connect(buttonRead, &QPushButton::clicked, diagnosticView,
          &DiagnosticView::ReadErrors);
  connect(buttonReset, &QPushButton::clicked, diagnosticView,
          &DiagnosticView::ResetErrors);
}
