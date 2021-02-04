#pragma once

#include <memory>

#include <QCanBus>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

class CanBusView : public QWidget {
  Q_OBJECT

 public:
  CanBusView(QWidget* parent = Q_NULLPTR);

  std::shared_ptr<QCanBusDevice> GetCanBusDevice() const noexcept { return canBusDevice; }

 signals:
  void BusConnected(std::shared_ptr<QCanBusDevice>);

 private slots:
  void Connect();

  void RefreshDevicesList();

  void ChangeCurrentDevice(int index);

  void SetStatus(QCanBusDevice::CanBusDeviceState state);

 private:
  QComboBox* comboBoxDevices;
  QGridLayout* layout;
  QLabel* labelStatus;
  QLabel* labelTitle;
  QPushButton* buttonRefresh;
  QPushButton* buttonConnect;

  std::shared_ptr<QCanBusDevice> canBusDevice;
};
