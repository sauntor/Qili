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

#include "Utility.h"

namespace Utility {

QString Speakable::username(const QString &text)
    {
        QString speakable;
        for (const auto &c : std::as_const(text)) {
            if (c.isDigit()) {
                int digit = c.digitValue();
                switch(digit) {
                case 0:
                    speakable += Speakable::tr("0", "speakable-username");
                    break;
                case 1:
                    speakable += Speakable::tr("1", "speakable-username");
                    break;
                case 2:
                    speakable += Speakable::tr("2", "speakable-username");
                    break;
                case 3:
                    speakable += Speakable::tr("3", "speakable-username");
                    break;
                case 4:
                    speakable += Speakable::tr("4", "speakable-username");
                    break;
                case 5:
                    speakable += Speakable::tr("5", "speakable-username");
                    break;
                case 6:
                    speakable += Speakable::tr("6", "speakable-username");
                    break;
                case 7:
                    speakable += Speakable::tr("7", "speakable-username");
                    break;
                case 8:
                    speakable += Speakable::tr("8", "speakable-username");
                    break;
                case 9:
                    speakable += Speakable::tr("9", "speakable-username");
                    break;
                }
            } else {
                speakable += c;
            }
        }
        return speakable;
    }

}

namespace Utility::JSON {

    bool strongTyped()
    {
        if (!SpecializationEnabled) {
            return StrongTyped;
        }
        return specialized.hasLocalData() ? specialized.localData() : StrongTyped;
    };

    QJsonObject operator>>(const QByteArray &bytes, JValueType<QJsonObject>)
    {
        return QJsonDocument::fromJson(bytes).object();
    }

    QByteArray operator>>(const QJsonObject &json, JValueType<QByteArray>)
    {
        return QJsonDocument(json).toJson(QJsonDocument::Compact);
    }

    QString operator>>(const QJsonObject &json, JValueType<QString>)
    {
        return QJsonDocument(json).toJson(QJsonDocument::Compact);
    }

    QJsonValue JValueVisitor::value() const
    {
        const QJsonObject *current = &jobject;
        for (auto iterator = jpaths.begin(), last = jpaths.end() - 1; iterator < jpaths.end(); iterator++) {
            if (iterator == last) {
                return current->value(*iterator);
            }
            QJsonObject value = current->value(*iterator).toObject();
            current = &value;
        }
        return QJsonValue::Undefined;
    }

    template<>
    QStringList operator>><QStringList>(const QJsonValue &value, JValueType<QStringList>)
    {
        if (value.isNull() || value.isUndefined()) {
            return QStringList();
        }

        if (!value.isArray()) {
            if (strongTyped()) {
                throw JTypeError(value.type());
            }
            return QStringList();
        }

        QStringList list;
        for (const auto &item : value.toArray()) {
            if (!item.isString() && strongTyped()) {
                throw JTypeError(item.type());
            }
            list << item.toString();
        }

        return list;
    }

    JValueVisitor& operator/(JValueVisitor&& visitor, const QString path)
    {
        visitor.jpaths << path;
        return visitor;
    }
    JValueVisitor& operator/(JValueVisitor& visitor, const QString path)
    {
        visitor.jpaths << path;
        return visitor;
    }

    JValueVisitor operator/(const QJsonObject &object, const QString path)
    {
        return JValueVisitor(object, path);
    }

    JValueVisitor operator/(const QJsonValueRef &object, const QString path)
    {
        return JValueVisitor(object.toObject(), path);
    }

    JValueVisitor operator/(const QJsonValueRef &&object, const QString path)
    {
        return JValueVisitor(object.toObject(), path);
    }
}
