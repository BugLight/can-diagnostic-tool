#include "diagnosticview.h"

#include <QMessageBox>

#include <deviceerrorview.h>
#include <jsonerrorsreader.h>

DiagnosticView::DiagnosticView(QWidget* parent) : QWidget(parent) {
  layout = new QGridLayout(this);

  labelDeviceId = new QLabel(u8"Устройства не обнаружены", this);
  labelDeviceId->setWordWrap(true);
  layout->addWidget(labelDeviceId, 0, 0);

  labelDeviceVersion = new QLabel(this);
  layout->addWidget(labelDeviceVersion, 1, 0);

  labelErrors = new QLabel(u8"Обнаруженные ошибки:", this);
  layout->addWidget(labelErrors, 2, 0);

  errorsArea = new QScrollArea(this);
  errorsArea->setMinimumSize(600, 600);
  errorsArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  errorsArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  errorsArea->setWidgetResizable(true);
  layout->addWidget(errorsArea, 3, 0);

  errorsAreaContents = new QWidget(errorsArea);
  errorsArea->setWidget(errorsAreaContents);

  errorsAreaLayout = new QVBoxLayout(errorsAreaContents);
  errorsAreaLayout->setAlignment(Qt::Alignment(Qt::AlignTop));
  errorsAreaContents->setLayout(errorsAreaLayout);

  DiagnosticProtocolOptions options;
  options.SetRequestCanId(0x0);
  options.SetResponseCanId(0x1);
  options.SetMaxRetries(5);
  options.SetTimeout(1000);
  protocol.SetOptions(options);
  protocol.SetErrorsList(JsonErrorsReader().ReadErrorsList(QStringLiteral("device_errors.json")));
}

void DiagnosticView::SetCanBusDevice(
  std::shared_ptr<QCanBusDevice> canBusDevicePtr) {
  protocol.SetCanBusDevice(canBusDevicePtr);
  SearchDevice();
  ReadErrors();
}

void DiagnosticView::SearchDevice() {
  try {
    auto info = protocol.RequestDeviceInfo();
    labelDeviceId->setText(u8"Идентификатор устройства: " +
                           QString::number(info.GetId()));
    labelDeviceVersion->setText(u8"Версия ПО: " + info.GetVersionString());
  } catch (DiagnosticProtocolException& e) {
    labelDeviceId->setText(u8"Устройства не обнаружены: " + QString(e.what()));
    labelDeviceVersion->setText("");
  }
}

void DiagnosticView::ReadErrors() {
  try {
    auto errors = protocol.ReadErrors();
    ClearErrorsArea();
    for (const auto& error : errors) {
      errorsAreaLayout->addWidget(
          new DeviceErrorView(error, errorsAreaContents));
    }
  } catch (DiagnosticProtocolException& e) {
    QMessageBox::critical(this, u8"Ошибка протокола", e.what());
  }
}

void DiagnosticView::ResetErrors() {
  try {
    protocol.ResetErrors();
    ClearErrorsArea();
  } catch (DiagnosticProtocolException& e) {
    QMessageBox::critical(this, u8"Ошибка протокола", e.what());
  }
}

void DiagnosticView::ClearErrorsArea() {
  while (errorsAreaLayout->count() > 0) {
    auto item = errorsAreaLayout->takeAt(0);
    delete item->widget();
    delete item;
  }
}
