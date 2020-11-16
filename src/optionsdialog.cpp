#include "optionsdialog.h"

OptionsDialog::OptionsDialog(QWidget* parent) : QDialog(parent) {
  setFont(QFont("Arial", 12));
  setLocale(QLocale(QLocale::Russian, QLocale::Russia));
  setWindowTitle(u8"Настройки");

  layout = new QVBoxLayout(this);

  form = new QWidget(this);
  layout->addWidget(form);

  formLayout = new QFormLayout(form);

  hexValidator = new HexValidator(this);

  editRequestId = new QLineEdit(this);
  editRequestId->setValidator(hexValidator);
  formLayout->addRow(u8"CAN ID запроса:", editRequestId);

  editResponseId = new QLineEdit(this);
  editResponseId->setValidator(hexValidator);
  formLayout->addRow(u8"CAN ID ответа:", editResponseId);

  spinTimeout = new QSpinBox(this);
  spinTimeout->setMinimum(100);
  spinTimeout->setMaximum(10000);
  formLayout->addRow(u8"Таймаут:", spinTimeout);

  spinMaxRetries = new QSpinBox(this);
  spinMaxRetries->setMinimum(1);
  spinMaxRetries->setMaximum(20);
  formLayout->addRow(u8"Число повторных попыток:", spinMaxRetries);

  buttonCancel = new QPushButton(u8"Отмена", this);
  layout->addWidget(buttonCancel);

  buttonOk = new QPushButton(u8"Сохранить", this);
  buttonOk->setFocus();
  layout->addWidget(buttonOk);

  connect(buttonCancel, &QPushButton::clicked, this, &OptionsDialog::reject);
  connect(buttonOk, &QPushButton::clicked, this, &OptionsDialog::accept);
}

void OptionsDialog::SetOptions(
    const DiagnosticProtocolOptions& options) noexcept {
  options_ = options;
  editRequestId->setText(QString::number(options_.GetRequestCanId(), 16));
  editResponseId->setText(QString::number(options_.GetResponseCanId(), 16));
  spinTimeout->setValue(options_.GetTimeout());
  spinMaxRetries->setValue(options_.GetMaxRetries());
}

void OptionsDialog::accept() {
  options_.SetRequestCanId(editRequestId->text().toUInt(nullptr, 16));
  options_.SetResponseCanId(editResponseId->text().toUInt(nullptr, 16));
  options_.SetTimeout(spinTimeout->value());
  options_.SetMaxRetries(spinMaxRetries->value());
  emit OptionsConfigured(options_);
  QDialog::accept();
}
