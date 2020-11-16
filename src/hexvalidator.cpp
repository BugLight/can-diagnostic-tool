#include "hexvalidator.h"

QValidator::State HexValidator::validate(QString& input, int& pos) const {
  bool success;
  input.toUInt(&success, 16);
  if (success)
    return QValidator::Acceptable;
  if (input.isEmpty())
    return QValidator::Intermediate;
  return QValidator::Invalid;
}
