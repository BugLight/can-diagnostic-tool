#pragma once

#include <cstdint>

#include <QString>

class DeviceError {
 public:
  DeviceError(std::uint32_t code, const QString& name,
              const QString& description)
      : code_(code), name_(name), description_(description) {}

  std::uint32_t GetCode() const noexcept { return code_; }

  QString GetName() const noexcept { return name_; }

  QString GetDescription() const noexcept { return description_; }

 private:
  std::uint32_t code_;
  QString name_;
  QString description_;
};
