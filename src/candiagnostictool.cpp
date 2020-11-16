#include "candiagnostictool.h"

#include <deviceerrorview.h>
#include <optionsdialog.h>

#include <QMenuBar>

CanDiagnosticTool::CanDiagnosticTool(QWidget* parent) : QMainWindow(parent) {
  setFont(QFont("Arial", 12));
  setLocale(QLocale(QLocale::Russian, QLocale::Russia));

  searchAction = new QAction(u8"Обнаружить устройство", this);
  readAction = new QAction(u8"Прочитать ошибки", this);
  resetAction = new QAction(u8"Очистить ошибки", this);
  optionsAction = new QAction(u8"Настройки", this);

  fileMenu = menuBar()->addMenu(u8"Файл");
  fileMenu->addAction(optionsAction);

  diagnosticMenu = menuBar()->addMenu(u8"Диагностика");
  diagnosticMenu->addAction(searchAction);
  diagnosticMenu->addAction(readAction);
  diagnosticMenu->addAction(resetAction);

  central = new QWidget(this);
  setCentralWidget(central);

  mainLayout = new QGridLayout(central);

  leftColumn = new QWidget(central);
  mainLayout->addWidget(leftColumn, 0, 0);

  leftColumnLayout = new QVBoxLayout(leftColumn);

  canBusView = new CanBusView(leftColumn);
  leftColumnLayout->addWidget(canBusView);

  options.SetRequestCanId(0);
  options.SetResponseCanId(1);
  options.SetTimeout(1000);
  options.SetMaxRetries(5);

  diagnosticView = new DiagnosticView(central);
  diagnosticView->SetProtocolOptions(options);
  mainLayout->addWidget(diagnosticView, 0, 1);

  connect(canBusView, &CanBusView::BusConnected, diagnosticView,
          &DiagnosticView::SetCanBusDevice);
  connect(searchAction, &QAction::triggered, diagnosticView,
          &DiagnosticView::SearchDevice);
  connect(readAction, &QAction::triggered, diagnosticView,
          &DiagnosticView::ReadErrors);
  connect(resetAction, &QAction::triggered, diagnosticView,
          &DiagnosticView::ResetErrors);
  connect(optionsAction, &QAction::triggered, this,
          &CanDiagnosticTool::ExecOptionsDialog);
}

void CanDiagnosticTool::ExecOptionsDialog() {
  OptionsDialog dialog(this);
  dialog.SetOptions(options);
  connect(&dialog, &OptionsDialog::OptionsConfigured, diagnosticView,
          &DiagnosticView::SetProtocolOptions);
  dialog.exec();
  options = dialog.GetOptions();
}
