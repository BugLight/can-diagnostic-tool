#pragma once

#include <QCanBus>
#include <QList>
#include <exception>
#include <memory>

#include "deviceerror.h"
#include "deviceinfo.h"

class DiagnosticProtocolOptions {
 public:
  quint32 GetRequestCanId() const noexcept { return requestCanId_; }

  void SetRequestCanId(quint32 requestCanId) noexcept {
    requestCanId_ = requestCanId;
  }

  quint32 GetResponseCanId() const noexcept { return responseCanId_; }

  void SetResponseCanId(quint32 responseCanId) noexcept {
    responseCanId_ = responseCanId;
  }

  int GetTimeout() const noexcept { return timeout_; }

  void SetTimeout(int timeout) noexcept { timeout_ = timeout; }

  int GetMaxRetries() const noexcept { return maxRetries_; }

  void SetMaxRetries(int maxRetries) noexcept { maxRetries_ = maxRetries; }

 private:
  quint32 requestCanId_;
  quint32 responseCanId_;
  int timeout_;
  int maxRetries_;
};

enum DiagnosticRequestCode : quint8 {
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
  DiagnosticProtocolException() noexcept : std::exception() {}

  explicit DiagnosticProtocolException(const char* msg) noexcept
      : std::exception(msg) {}
};

class DiagnosticProtocol {
 public:
  DiagnosticProtocol(std::shared_ptr<QCanBusDevice> canBusDevicePtr,
                     const DiagnosticProtocolOptions& options,
                     const QList<DeviceError>& errors) noexcept;

  DiagnosticProtocol() noexcept {}

  DeviceInfo RequestDeviceInfo() const;

  QList<DeviceError> ReadErrors() const;

  void ResetErrors() const;

  void SetCanBusDevice(std::shared_ptr<QCanBusDevice> canBusDevicePtr) {
    canBusDevicePtr_ = canBusDevicePtr;
    SetCanFilter();
  }

  void SetOptions(const DiagnosticProtocolOptions& options) {
    options_ = options;
    SetCanFilter();
  }

  void SetErrorsList(const QList<DeviceError>& errors) noexcept {
    errors_ = errors;
  }

 private:
  void SetCanFilter();

  QByteArray SendRequest(DiagnosticRequestCode code) const;

  QList<DeviceError> ErrorsWordToList(quint32 errorsWord) const;

  std::shared_ptr<QCanBusDevice> canBusDevicePtr_ = nullptr;
  DiagnosticProtocolOptions options_{};
  QList<DeviceError> errors_{};
};
