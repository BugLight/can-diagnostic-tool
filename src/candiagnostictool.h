#pragma once

#include <QGridLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QWidget>

#include <canbusview.h>
#include <diagnosticview.h>

class CanDiagnosticTool : public QMainWindow {
  Q_OBJECT

 public:
  CanDiagnosticTool(QWidget* parent = Q_NULLPTR);

 private:
  CanBusView* canBusView;
  DiagnosticView* diagnosticView;
  QGridLayout* mainLayout;
  QPushButton* buttonRead;
  QPushButton* buttonReset;
  QPushButton* buttonSearch;
  QVBoxLayout* leftColumnLayout;
  QWidget* central;
  QWidget* leftColumn;
};
