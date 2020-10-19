#include "deviceerrorview.h"

DeviceErrorView::DeviceErrorView(QWidget *parent) : QWidget(parent) {
  labelErrorName = new QLabel("Error name", this);
  labelErrorName->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}
