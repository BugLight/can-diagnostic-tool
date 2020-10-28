#pragma once

#include <QCanBus>
#include <QList>

#include <exception>
#include <memory>

#include "deviceerror.h"
#include "deviceinfo.h"

class DiagnosticProtocolOptions {
 public:
  std::uint32_t GetRequestCanId() const noexcept { return requestCanId_; }

  void SetRequestCanId(std::uint32_t requestCanId) noexcept {
    requestCanId_ = requestCanId;
  }

  std::uint32_t GetResponseCanId() const noexcept { return responseCanId_; }

  void SetResponseCanId(std::uint32_t responseCanId) noexcept {
    responseCanId_ = responseCanId;
  }

  int GetTimeout() const noexcept { return timeout_; }

  void SetTimeout(int timeout) noexcept { timeout_ = timeout; }

  int GetMaxRetries() const noexcept { return maxRetries_; }

  void SetMaxRetries(int maxRetries) { maxRetries_ = maxRetries; }

 private:
  std::uint32_t requestCanId_;
  std::uint32_t responseCanId_;
  int timeout_;
  int maxRetries_;
};

enum DiagnosticRequestCode {
  BeginDiagnostics = 0x0,
  EndDiagnostics = 0x1,
  Info = 0x2,
  ReadErrors = 0x4,
  ResetErrors = 0x6,
  TestDisplay = 0x3,
  TestMemory = 0x5,
  TestSensors = 0x7
};

class DiagnosticProtocolException : public std::exception {
 public:
  DiagnosticProtocolException() : std::exception() {}

  explicit DiagnosticProtocolException(const char* msg) : std::exception(msg) {}
};

class DiagnosticProtocol {
 public:
  DiagnosticProtocol(std::shared_ptr<QCanBusDevice> canBusDevicePtr,
                     const DiagnosticProtocolOptions& options,
                     const QList<DeviceError>& errors) noexcept;

  DeviceInfo RequestDeviceInfo() const;

  QList<DeviceError> ReadErrors() const;

  void ResetErrors() const;

  void SetCanBusDevice(
      std::shared_ptr<QCanBusDevice> canBusDevicePtr) noexcept {
    canBusDevicePtr_ = canBusDevicePtr;
  }

  void SetOptions(const DiagnosticProtocolOptions& options) noexcept {
    options_ = options;
  }

  void SetErrors(const QList<DeviceError>& errors) noexcept {
    errors_ = errors;
  }

 private:
  QByteArray SendRequest(DiagnosticRequestCode code) const;

  std::shared_ptr<QCanBusDevice> canBusDevicePtr_;
  DiagnosticProtocolOptions options_;
  QList<DeviceError> errors_;
};