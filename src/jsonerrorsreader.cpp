#include "jsonerrorsreader.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

static bool DeviceErrorFromJson(DeviceError& result, const QJsonValue& value) {
  const auto obj = value.toObject();
  if (obj["code"].isUndefined() || obj["name"].isUndefined()) return false;
  bool success;
  const auto code = obj["code"].toString().toUInt(&success, 16);
  if (!success) return false;
  const auto name = obj["name"].toString();
  const auto description = obj["description"].toString("");
  result = DeviceError(code, name, description);
  return true;
}

QList<DeviceError> JsonErrorsReader::ReadErrorsList(const QByteArray& data) {
  QList<DeviceError> errors;
  auto doc = QJsonDocument::fromJson(data);
  if (!doc.isNull() && !doc.isEmpty()) {
    for (const auto& value : doc.array()) {
      DeviceError error(0, "", "");
      if (DeviceErrorFromJson(error, value)) errors.append(error);
    }
  }
  return errors;
}

QList<DeviceError> JsonErrorsReader::ReadErrorsList(const QString& path) {
  QFile file(path);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return QList<DeviceError>();
  return ReadErrorsList(file.readAll());
}
