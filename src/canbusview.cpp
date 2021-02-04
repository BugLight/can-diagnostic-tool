#include "canbusview.h"

CanBusView::CanBusView(QWidget* parent) : QWidget(parent) {
  layout = new QGridLayout(this);
  layout->setAlignment(Qt::Alignment(Qt::AlignTop));

  auto plugins = QCanBus::instance()->plugins();

  labelTitle = new QLabel(u8"Шина CAN:", this);
  layout->addWidget(labelTitle, 0, 0, 1, 2);

  comboBoxDevices = new QComboBox(this);
  connect(comboBoxDevices, QOverload<int>::of(&QComboBox::currentIndexChanged),
          this, &CanBusView::ChangeCurrentDevice);
  layout->addWidget(comboBoxDevices, 1, 0);

  buttonRefresh = new QPushButton(this);
  buttonRefresh->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
  connect(buttonRefresh, &QPushButton::clicked, this,
          &CanBusView::RefreshDevicesList);
  layout->addWidget(buttonRefresh, 1, 1);

  buttonConnect = new QPushButton(u8"Подключить", this);
  connect(buttonConnect, &QPushButton::clicked, this, &CanBusView::Connect);
  layout->addWidget(buttonConnect, 2, 0, 1, 2);

  labelStatus = new QLabel(this);
  labelStatus->setFixedWidth(240);
  labelStatus->setWordWrap(true);
  layout->addWidget(labelStatus, 3, 0);

  RefreshDevicesList();
}

void CanBusView::Connect() {
  if (canBusDevice->state() == QCanBusDevice::UnconnectedState) {
    if (!canBusDevice->connectDevice())
      labelStatus->setText(u8"Ошибка подключения");
  } else if (canBusDevice->state() == QCanBusDevice::ConnectedState) {
    canBusDevice->disconnectDevice();
  }
}

void CanBusView::RefreshDevicesList() {
  auto plugins = QCanBus::instance()->plugins();
  comboBoxDevices->clear();
  for (const auto& plugin : plugins) {
    for (const auto& device : QCanBus::instance()->availableDevices(plugin)) {
      auto description = device.description();
      if (description.isEmpty()) description = u8"Описание отсутствует";
      comboBoxDevices->addItem(device.name() + ": " + description,
                               QStringList{plugin, device.name()});
    }
  }
}

void CanBusView::ChangeCurrentDevice(int index) {
  if (canBusDevice) canBusDevice->disconnectDevice();
  if (index < 0) {
    canBusDevice.reset();
  } else {
    auto data = comboBoxDevices->itemData(index).toStringList();
    auto plugin = data[0];
    auto deviceName = data[1];
    QString errorMsg;
    canBusDevice.reset(
        QCanBus::instance()->createDevice(plugin, deviceName, &errorMsg));
    if (canBusDevice) {
      connect(canBusDevice.get(), &QCanBusDevice::stateChanged, this,
              &CanBusView::SetStatus);
      canBusDevice->setConfigurationParameter(QCanBusDevice::BitRateKey,
                                              250000);
    } else {
      labelStatus->setText(u8"Ошибка: " + errorMsg);
    }
  }
}

void CanBusView::SetStatus(QCanBusDevice::CanBusDeviceState state) {
  switch (state) {
    case QCanBusDevice::UnconnectedState: {
      labelStatus->setText(u8"Не подключено");
      buttonConnect->setText(u8"Подключить");
      buttonConnect->setDisabled(false);
      break;
    }
    case QCanBusDevice::ConnectingState: {
      labelStatus->setText(u8"Подключение...");
      buttonConnect->setDisabled(true);
      break;
    }
    case QCanBusDevice::ConnectedState: {
      labelStatus->setText(u8"Подключено");
      buttonConnect->setText(u8"Отключить");
      buttonConnect->setDisabled(false);
      emit BusConnected(canBusDevice);
      break;
    }
    case QCanBusDevice::ClosingState: {
      labelStatus->setText(u8"Отключение...");
      buttonConnect->setDisabled(true);
      break;
    }
  }
}
