#pragma once

#include <QCanBus>
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

#include <memory>

#include <diagnosticprotocol.h>

class DiagnosticView : public QWidget {
  Q_OBJECT

 public:
  DiagnosticView(QWidget* parent = Q_NULLPTR);

 public slots:
  void SetCanBusDevice(std::shared_ptr<QCanBusDevice>);

  void SearchDevice();

  void ReadErrors();

  void ResetErrors();

 private:
  void ClearErrorsArea();

  QGridLayout* layout;
  QLabel* labelDeviceId;
  QLabel* labelDeviceVersion;
  QLabel* labelErrors;
  QScrollArea* errorsArea;
  QVBoxLayout* errorsAreaLayout;
  QWidget* errorsAreaContents;

  DiagnosticProtocol protocol;
};
