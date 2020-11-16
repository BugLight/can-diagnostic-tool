﻿#include "candiagnostictool.h"

#include <QMenuBar>

#include <deviceerrorview.h>

CanDiagnosticTool::CanDiagnosticTool(QWidget* parent) : QMainWindow(parent) {
  setFont(QFont("Arial", 12));
  setLocale(QLocale(QLocale::Russian, QLocale::Russia));

  searchAction = new QAction(u8"Обнаружить устройство", this);
  readAction = new QAction(u8"Прочитать ошибки", this);
  resetAction = new QAction(u8"Очистить ошибки", this);

  fileMenu = menuBar()->addMenu(u8"Файл");

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

  diagnosticView = new DiagnosticView(central);
  mainLayout->addWidget(diagnosticView, 0, 1);

  connect(canBusView, &CanBusView::BusConnected, diagnosticView,
          &DiagnosticView::SetCanBusDevice);
  connect(searchAction, &QAction::triggered, diagnosticView,
          &DiagnosticView::SearchDevice);
  connect(readAction, &QAction::triggered, diagnosticView,
          &DiagnosticView::ReadErrors);
  connect(resetAction, &QAction::triggered, diagnosticView,
          &DiagnosticView::ResetErrors);
}
