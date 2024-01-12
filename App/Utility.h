/* Copyright (C) 2024  Sauntor <sauntor@live.com>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef UTILITY_H
#define UTILITY_H

#include "QiliAppGlobal.h"

#include <QString>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QThread>
#include <QThreadStorage>
#include <QCoreApplication>


namespace Utility {

class Speakable {
    Q_DECLARE_TR_FUNCTIONS(Text)

public:
    static QString username(const QString &text);
};

namespace JSON {

    static bool StrongTyped = false;
    static bool SpecializationEnabled = false;
    static QThreadStorage<bool> specialized;

    class JTypeError
    {
    public:
        JTypeError(QJsonValue::Type type): actualType(type) {};
        const QJsonValue::Type actualType;
    };

    enum JSpecializationError {
        SpecializationDisabled
    };

    // Marker for QJsonValue/QJsonObject <=> `Qt Native Type` converting
    template<typename ToQt>
    class JValueType {};


    static const JValueType<QJsonValue> JValue{};
    static const JValueType<QJsonObject> JObject{};
    static const JValueType<QJsonArray> JArray{};
    static const JValueType<QByteArray> JBinary{};


    static const JValueType<bool> JBool{};
    static const JValueType<QList<bool>> JBools{};

    static const JValueType<int> JInt{};
    static const JValueType<QList<int>> JInts{};

    static const JValueType<double> JDouble{};
    static const JValueType<QList<double>> JDoubles{};

    static const JValueType<QString> JString{};
    static const JValueType<QStringList> JStrings{};

    bool strongTyped();

    QJsonObject operator>>(const QByteArray &bytes, JValueType<QJsonObject>);
    QByteArray operator>>(const QJsonObject &json, JValueType<QByteArray>);

    QString operator>>(const QJsonObject &json, JValueType<QString>);

    // templated converter for QJsonValue => `Qt Native Type`
    template<typename ToQt, template<typename> typename J = JValueType>
    ToQt operator>>(const QJsonValue &value, const J<ToQt>);

    // QJsonValue => QJsonValue
    template<>
    inline QJsonValue operator>><QJsonValue>(const QJsonValue &value, JValueType<QJsonValue>)
    {
        return value;
    }

    // QJsonValue => QJsonArray
    template<>
    inline QJsonArray operator>><QJsonArray>(const QJsonValue &value, JValueType<QJsonArray>)
    {
        return value.toArray();
    }


    // QJsonValue => QJsonObject
    template<>
    inline QJsonObject operator>><QJsonObject>(const QJsonValue &value, JValueType<QJsonObject>)
    {
        if (value.isNull()) {
            return value.toObject();
        }
        if (value.isUndefined()) {
            return value.toObject();
        }

        if (!value.isObject() && strongTyped()) {
            throw JTypeError(value.type());
        }

        return value.toObject();
    }

    // QJsonValue => QStringList
    template<>
    QStringList operator>><QStringList>(const QJsonValue &value, JValueType<QStringList>);

    template<>
    inline bool operator>><bool>(const QJsonValue &value, JValueType<bool>)
    {
        if (!value.isBool() && strongTyped()) {
            throw JTypeError(value.type());
        }
        return value.toBool();
    }

    // QJsonValue => int
    template<>
    inline int operator>><int>(const QJsonValue &value, JValueType<int>)
    {
        if (!value.isDouble() && strongTyped()) {
            throw JTypeError(value.type());
        }
        return value.toInt();
    }

    // QJsonValue => double
    template<>
    inline double operator>><double>(const QJsonValue &value, JValueType<double>)
    {
        if (!value.isDouble() && strongTyped()) {
            throw JTypeError(value.type());
        }
        return value.toDouble();
    }

    // QJsonValue => QString
    template<>
    inline QString operator>><QString>(const QJsonValue &value, JValueType<QString>)
    {
        if (!value.isString() && strongTyped()) {
            throw JTypeError(value.type());
        }
        return value.toString();
    }

    // A wrapper for json tree traversing
    class JValueVisitor
    {
        friend JValueVisitor& operator/(JValueVisitor&& visitor, const QString path);
        friend JValueVisitor& operator/(JValueVisitor& visitor, const QString path);

    public:
        JValueVisitor(const QJsonObject &object, const QString &path): jobject(object), jpaths(path) {};

        QJsonValue value() const;

    private:
        QStringList jpaths;
        const QJsonObject &jobject;
    };

    // Json Tree Visitor
    JValueVisitor operator/(const QJsonObject &object, const QString path);
    JValueVisitor& operator/(JValueVisitor &&visitor, const QString path);
    JValueVisitor& operator/(JValueVisitor &visitor, const QString path);
    JValueVisitor operator/(const QJsonValueRef &object, const QString path);

    // Json Tree Visitor => Qt Native Type
    template<typename ToQt, template<typename> typename J = JValueType>
    ToQt operator>>(const JValueVisitor &visitor, const J<ToQt> type)
    {
        return visitor.value() >> type;
    }

    // Fo spicialized code block which not using global type checking settings
    template<typename T>
    T withoutChecking(T (*getter)())
    {
        if (!StrongTyped) {
            return getter();
        }
        if (!SpecializationEnabled) {
            throw JSpecializationError::SpecializationDisabled;
        }
        specialized.setLocalData(false);
        T &value = getter();
        specialized.setLocalData(StrongTyped);
        return value;
    }

    template<typename T>
    T withChecking(T (*getter)())
    {
        if (StrongTyped) {
            return getter();
        }
        if (!SpecializationEnabled) {
            throw JSpecializationError::SpecializationDisabled;
        }
        specialized.setLocalData(true);
        T &value = getter();
        specialized.setLocalData(StrongTyped);
        return value;
    }
}; // JSON
}; // Utility

#endif // UTILITY_H
