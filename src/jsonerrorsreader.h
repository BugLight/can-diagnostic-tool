#pragma once

#include <QByteArray>
#include <QList>
#include <QString>

#include "deviceerror.h"

class JsonErrorsReader {
 public:
  QList<DeviceError> ReadErrorsList(const QByteArray& data);

  QList<DeviceError> ReadErrorsList(const QString& path);
};
