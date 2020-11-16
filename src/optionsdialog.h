#pragma once

#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

#include <hexvalidator.h>
#include <diagnosticprotocol.h>

class OptionsDialog : public QDialog {
  Q_OBJECT

 public:
  OptionsDialog(QWidget *parent = Q_NULLPTR);

  void SetOptions(const DiagnosticProtocolOptions& options) noexcept;

  DiagnosticProtocolOptions GetOptions() const noexcept { return options_; }

  void accept() override;

 signals:
  void OptionsConfigured(const DiagnosticProtocolOptions&);

 private:
  DiagnosticProtocolOptions options_;
  HexValidator* hexValidator;
  QFormLayout* formLayout;
  QLineEdit* editRequestId;
  QLineEdit* editResponseId;
  QPushButton* buttonCancel;
  QPushButton* buttonOk;
  QSpinBox* spinMaxRetries;
  QSpinBox* spinTimeout;
  QVBoxLayout* layout;
  QWidget* form;
};
