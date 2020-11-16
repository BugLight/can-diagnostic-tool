#pragma once

#include <QAction>
#include <QGridLayout>
#include <QMenu>
#include <QVBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QWidget>

#include <canbusview.h>
#include <diagnosticview.h>

class CanDiagnosticTool : public QMainWindow {
  Q_OBJECT

 public:
  CanDiagnosticTool(QWidget* parent = Q_NULLPTR);

 private slots:
  void ExecOptionsDialog();

 private:
  QAction* readAction;
  QAction* resetAction;
  QAction* searchAction;
  QAction* optionsAction;
  CanBusView* canBusView;
  DiagnosticView* diagnosticView;
  QGridLayout* mainLayout;
  QMenu* fileMenu;
  QMenu* diagnosticMenu;
  QVBoxLayout* leftColumnLayout;
  QWidget* central;
  QWidget* leftColumn;
};
