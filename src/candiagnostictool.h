#pragma once

#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QtWidgets/QMainWindow>

#include "canbusview.h"

class CanDiagnosticTool : public QMainWindow {
  Q_OBJECT

 public:
  CanDiagnosticTool(QWidget* parent = Q_NULLPTR);

 private:
  CanBusView* canBusView;
  QGridLayout* mainLayout;
  QLabel* labelDeviceId;
  QLabel* labelDeviceVersion;
  QScrollArea* errorsArea;
  QVBoxLayout* errorsAreaLayout;
  QWidget* errorsAreaContents;
  QWidget* central;
};
