#pragma once

#include <QString>
#include <QValidator>

class HexValidator final : public QValidator {
 public:
  explicit HexValidator(QObject* parent) : QValidator(parent) {}

  QValidator::State validate(QString& input, int& pos) const override;
};
