#pragma once

#include <QLabel>
#include <QWidget>

class DeviceErrorView : public QWidget {
  Q_OBJECT

 public:
  DeviceErrorView(QWidget* parent = Q_NULLPTR);

 private:
  QLabel* labelErrorName;
};
