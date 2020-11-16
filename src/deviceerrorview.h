#pragma once

#include <QGroupBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

#include <deviceerror.h>

class DeviceErrorView : public QWidget {
  Q_OBJECT

 public:
  DeviceErrorView(const DeviceError& error, QWidget* parent = Q_NULLPTR);

 private:
  DeviceError error_;

  QGroupBox* groupBox;
  QVBoxLayout* groupBoxLayout;
  QVBoxLayout* layout;
  QLabel* labelCode;
  QLabel* labelDescription;
};
