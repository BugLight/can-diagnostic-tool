#include "diagnosticprotocol.h"

DiagnosticProtocol::DiagnosticProtocol(
    std::shared_ptr<QCanBusDevice> canBusDevicePtr,
    const DiagnosticProtocolOptions& options,
    const QList<DeviceError>& errors) noexcept
    : canBusDevicePtr_(canBusDevicePtr), options_(options), errors_(errors) {
  SetCanFilter();
}

void DiagnosticProtocol::SetCanFilter() {
  if (!canBusDevicePtr_) return;
  QList<QCanBusDevice::Filter> filterList;
  QCanBusDevice::Filter filter;
  filter.frameId = options_.GetResponseCanId();
  filter.frameIdMask = 0x1fffffffu;
  filter.format = QCanBusDevice::Filter::MatchBaseAndExtendedFormat;
  filter.type = QCanBusFrame::DataFrame;
  filterList.append(filter);
  canBusDevicePtr_->setConfigurationParameter(QCanBusDevice::RawFilterKey,
                                              QVariant::fromValue(filterList));
}

QByteArray DiagnosticProtocol::SendRequest(DiagnosticRequestCode code) const {
  if (!canBusDevicePtr_) throw DiagnosticProtocolException(u8"CAN не выбран.");

  if (canBusDevicePtr_->state() != QCanBusDevice::ConnectedState)
    throw DiagnosticProtocolException(u8"CAN не подключен.");

  auto retries = 0;
  do {
    auto currentCode = static_cast<char>((code << 2) | (retries & 0b11));
    canBusDevicePtr_->writeFrame(
        QCanBusFrame(options_.GetRequestCanId(), QByteArray(1, currentCode)));
    if (!canBusDevicePtr_->waitForFramesReceived(options_.GetTimeout())) break;
    while (canBusDevicePtr_->framesAvailable() > 0) {
      auto frame = canBusDevicePtr_->readFrame();
      if (frame.frameId() == options_.GetResponseCanId()) {
        auto payload = frame.payload();
        if (payload.startsWith(currentCode)) return payload.remove(0, 1);
      }
    }
    ++retries;
  } while (retries <= options_.GetMaxRetries());
  throw DiagnosticProtocolException(
      u8"ѕревышено врем€ ожидани€ или количество попыток.");
}

QList<DeviceError> DiagnosticProtocol::ErrorsWordToList(
    quint32 errorsWord) const {
  QList<DeviceError> confirmedErrors;
  for (const auto& error : errors_) {
    if ((error.GetCode() & errorsWord) != 0) confirmedErrors.append(error);
  }
  return confirmedErrors;
}

DeviceInfo DiagnosticProtocol::RequestDeviceInfo() const {
  auto response = SendRequest(DiagnosticRequestCode::Info);
  QDataStream stream(response);
  stream.setByteOrder(QDataStream::LittleEndian);
  quint32 id;
  quint16 version;
  stream >> id;
  stream >> version;
  return DeviceInfo(id, version);
}

QList<DeviceError> DiagnosticProtocol::ReadErrors() const {
  auto response = SendRequest(DiagnosticRequestCode::ReadErrors);
  QDataStream stream(response);
  stream.setByteOrder(QDataStream::LittleEndian);
  quint32 errorsWord;
  stream >> errorsWord;
  return ErrorsWordToList(errorsWord);
}

void DiagnosticProtocol::ResetErrors() const {
  SendRequest(DiagnosticRequestCode::ResetErrors);
}
