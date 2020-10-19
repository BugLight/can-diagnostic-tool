#pragma once

#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QtWidgets/QMainWindow>

class CanDiagnosticTool : public QMainWindow {
  Q_OBJECT

 public:
  CanDiagnosticTool(QWidget* parent = Q_NULLPTR);

 private:
  QComboBox* comboBoxAdapter;
  QGridLayout* mainLayout;
  QLabel* labelAdapterStatus;
  QLabel* labelDeviceId;
  QLabel* labelDeviceVersion;
  QLabel* labelStatus;
  QScrollArea* errorsArea;
  QVBoxLayout* errorsAreaLayout;
  QWidget* errorsAreaContents;
  QWidget* central;
};
