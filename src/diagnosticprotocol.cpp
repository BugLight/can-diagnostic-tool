#include "diagnosticprotocol.h"

DiagnosticProtocol::DiagnosticProtocol(
    std::shared_ptr<QCanBusDevice> canBusDevicePtr,
    const DiagnosticProtocolOptions& options,
    const QList<DeviceError>& errors) noexcept
    : canBusDevicePtr_(canBusDevicePtr), options_(options), errors_(errors) {
  // Setup filter to receive only protocol frames
  QCanBusDevice::Filter filter;
  filter.frameId = options.GetResponseCanId();
  filter.frameIdMask = 0x7fffffff;
  filter.format = QCanBusDevice::Filter::MatchBaseFormat;
  filter.type = QCanBusFrame::DataFrame;
  canBusDevicePtr->setConfigurationParameter(
      QCanBusDevice::RawFilterKey,
      QVariant::fromValue(QList<QCanBusDevice::Filter>{filter}));
}

QByteArray DiagnosticProtocol::SendRequest(DiagnosticRequestCode code) const {
  if (canBusDevicePtr_->state() != QCanBusDevice::ConnectedState)
    throw DiagnosticProtocolException(u8"CAN не подключен.");

  auto retries = 0;
  do {
    auto currentCode = (code << 2) | (retries & 0b11);
    canBusDevicePtr_->writeFrame(
        QCanBusFrame(options_.GetRequestCanId(), QByteArray(1, currentCode)));
    if (!canBusDevicePtr_->waitForFramesReceived(options_.GetTimeout())) break;
    while (canBusDevicePtr_->framesAvailable() > 0) {
      auto payload = canBusDevicePtr_->readFrame().payload();
      if (payload.startsWith(currentCode)) return payload.remove(0, 1);
    }
    ++retries;
  } while (retries <= options_.GetMaxRetries());
  throw DiagnosticProtocolException(u8"Превышено время ожидания или количество попыток.");
}

DeviceInfo DiagnosticProtocol::RequestDeviceInfo() const {
  auto response = SendRequest(DiagnosticRequestCode::Info);
  QDataStream stream(response);
  std::uint32_t id;
  std::uint16_t version;
  stream >> id;
  stream >> version;
  return DeviceInfo(id, version);
}

QList<DeviceError> DiagnosticProtocol::ReadErrors() const {
  auto response = SendRequest(DiagnosticRequestCode::ReadErrors);
  QDataStream stream(response);
  std::uint32_t errorsWord;
  stream >> errorsWord;
  QList<DeviceError> confirmedErrors;
  for (const auto& error : errors_) {
    if ((error.GetCode() & errorsWord) != 0) confirmedErrors.append(error);
  }
  return confirmedErrors;
}

void DiagnosticProtocol::ResetErrors() const {
  SendRequest(DiagnosticRequestCode::ResetErrors);
}
