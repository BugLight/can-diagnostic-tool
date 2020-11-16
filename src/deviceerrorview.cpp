#include "deviceerrorview.h"

DeviceErrorView::DeviceErrorView(const DeviceError& error, QWidget* parent)
    : QWidget(parent), error_(error) {
  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

  layout = new QVBoxLayout(this);

  groupBox = new QGroupBox(error.GetName(), this);
  layout->addWidget(groupBox);

  groupBoxLayout = new QVBoxLayout(groupBox);

  labelCode =
      new QLabel(u8"Код ошибки: " + QString::number(error.GetCode(), 16), groupBox);
  groupBoxLayout->addWidget(labelCode);

  labelDescription = new QLabel(u8"Описание: " + error.GetDescription());
  labelDescription->setWordWrap(true);
  groupBoxLayout->addWidget(labelDescription);
}
