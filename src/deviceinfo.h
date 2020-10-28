#pragma once

#include <cstdint>

#include <QString>

class DeviceInfo {
 public:
  DeviceInfo(std::uint32_t id, std::uint16_t version)
      : id_(id), version_(version) {}

  std::uint32_t GetId() const noexcept { return id_; }

  std::uint16_t GetVersion() const noexcept { return version_; }

  QString GetVersionString() const noexcept {
    return QString::number(version_ >> 11) + "." +
           QString::number((version_ >> 4) & 0x3f) + "-" +
           QString::number(version_ & 0xf);
  }

 private:
  std::uint32_t id_;
  std::uint16_t version_;
};
