#include "diagnosticview.h"

#include <QMessageBox>

#include <deviceerrorview.h>
#include <jsonerrorsreader.h>

DiagnosticView::DiagnosticView(QWidget* parent) : QWidget(parent) {
  layout = new QGridLayout(this);

  labelDeviceId = new QLabel(u8"���������� �� ����������", this);
  labelDeviceId->setWordWrap(true);
  layout->addWidget(labelDeviceId, 0, 0);

  labelDeviceVersion = new QLabel(this);
  layout->addWidget(labelDeviceVersion, 1, 0);

  labelErrors = new QLabel(u8"������������ ������:", this);
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

  protocol.SetErrorsList(JsonErrorsReader().ReadErrorsList(QStringLiteral("device_errors.json")));
}

void DiagnosticView::SetCanBusDevice(
  std::shared_ptr<QCanBusDevice> canBusDevicePtr) {
  protocol.SetCanBusDevice(canBusDevicePtr);
  SearchDevice();
  ReadErrors();
}

void DiagnosticView::SetProtocolOptions(const DiagnosticProtocolOptions& options) {
  protocol.SetOptions(options);
}

void DiagnosticView::SearchDevice() {
  try {
    auto info = protocol.RequestDeviceInfo();
    labelDeviceId->setText(u8"������������� ����������: " +
                           QString::number(info.GetId()));
    labelDeviceVersion->setText(u8"������ ��: " + info.GetVersionString());
  } catch (DiagnosticProtocolException& e) {
    labelDeviceId->setText(u8"���������� �� ����������: " + QString(e.what()));
    labelDeviceVersion->setText("");
  }
}

void DiagnosticView::ReadErrors() {
  try {
    ClearErrorsArea();
    auto errors = protocol.ReadErrors();
    for (const auto& error : errors) {
      errorsAreaLayout->addWidget(
          new DeviceErrorView(error, errorsAreaContents));
    }
  } catch (DiagnosticProtocolException& e) {
    QMessageBox::critical(this, u8"������ ���������", e.what());
  }
}

void DiagnosticView::ResetErrors() {
  try {
    protocol.ResetErrors();
    ClearErrorsArea();
  } catch (DiagnosticProtocolException& e) {
    QMessageBox::critical(this, u8"������ ���������", e.what());
  }
}

void DiagnosticView::ClearErrorsArea() {
  while (errorsAreaLayout->count() > 0) {
    auto item = errorsAreaLayout->takeAt(0);
    delete item->widget();
    delete item;
  }
}
