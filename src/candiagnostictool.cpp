#include "candiagnostictool.h"

#include "deviceerrorview.h"

CanDiagnosticTool::CanDiagnosticTool(QWidget* parent) : QMainWindow(parent) {
  setFont(QFont("Arial", 12));
  setLocale(QLocale(QLocale::Russian, QLocale::Russia));

  central = new QWidget(this);
  setCentralWidget(central);
  mainLayout = new QGridLayout(central);
  central->setLayout(mainLayout);

  comboBoxAdapter = new QComboBox(central);
  mainLayout->addWidget(comboBoxAdapter, 0, 0, 1, 2);

  labelStatus = new QLabel(u8"Статус:", central);
  mainLayout->addWidget(labelStatus, 1, 0);

  labelAdapterStatus = new QLabel(u8"не подключено", central);
  mainLayout->addWidget(labelAdapterStatus, 1, 1);

  labelDeviceId = new QLabel(u8"Устройства не обнаружены", central);
  mainLayout->addWidget(labelDeviceId, 0, 2);

  labelDeviceVersion = new QLabel("", central);
  mainLayout->addWidget(labelDeviceVersion, 1, 2);

  errorsArea = new QScrollArea(central);
  errorsArea->setMinimumSize(600, 600);
  errorsArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  errorsArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  errorsArea->setWidgetResizable(true);
  mainLayout->addWidget(errorsArea, 2, 2);

  errorsAreaContents = new QWidget(errorsArea);
  errorsArea->setWidget(errorsAreaContents);

  errorsAreaLayout = new QVBoxLayout(errorsAreaContents);
  errorsAreaContents->setLayout(errorsAreaLayout);

  auto error = new DeviceErrorView(errorsAreaContents);
  errorsAreaLayout->addWidget(error);
}
