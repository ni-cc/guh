/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Copyright (C) 2015 Simon Stürz <simon.stuerz@guh.guru>                 *
 *  Copyright (C) 2014 Michael Zanetti <michael_zanetti@gmx.net>           *
 *                                                                         *
 *  This file is part of guh.                                              *
 *                                                                         *
 *  Guh is free software: you can redistribute it and/or modify            *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, version 2 of the License.                *
 *                                                                         *
 *  Guh is distributed in the hope that it will be useful,                 *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with guh. If not, see <http://www.gnu.org/licenses/>.            *
 *                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*!
    \class ParamType
    \brief Describes a certain ParamType.

    \ingroup guh-types
    \inmodule libguh

    \sa Device, Param, ParamDescriptor
*/


#include "paramtype.h"

/*! Constructs a ParamType object with the given \a name, \a type and \a defaultValue. */
ParamType::ParamType(const QString &name, const QVariant::Type type, const QVariant &defaultValue):
    m_name(name),
    m_index(0),
    m_type(type),
    m_defaultValue(defaultValue),
    m_inputType(Types::InputTypeNone),
    m_unit(Types::UnitNone),
    m_readOnly(false)
{

}

/*! Returns the name of this ParamType. */
QString ParamType::name() const
{
    return m_name;
}

/*! Sets the name of this ParamType to the given \a name. */
void ParamType::setName(const QString &name)
{
    m_name = name;
}

/*! Returns the index of this \l{ParamType}. The index of an \l{ParamType} indicates the order in the corresponding Type. */
int ParamType::index() const
{
    return m_index;
}

/*! Set the \a index of this \l{ParamType}. */
void ParamType::setIndex(const int &index)
{
    m_index = index;
}

/*! Returns the type of this ParamType. */
QVariant::Type ParamType::type() const
{
    return m_type;
}

/*! Sets the type of this ParamType to the given \a type. */
void ParamType::setType(QVariant::Type type)
{
    m_type = type;
}

/*! Returns the default value of this ParamType. */
QVariant ParamType::defaultValue() const
{
    return m_defaultValue;
}

/*! Sets the default value of this ParamType to the given \a defaultValue. */
void ParamType::setDefaultValue(const QVariant &defaultValue)
{
    m_defaultValue = defaultValue;
}

/*! Returns the minimum value of this ParamType. */
QVariant ParamType::minValue() const
{
    return m_minValue;
}

/*! Sets the minimum value of this ParamType to the given \a minValue. */
void ParamType::setMinValue(const QVariant &minValue)
{
    m_minValue = minValue;
}

/*! Returns the maximum value of this ParamType. */
QVariant ParamType::maxValue() const
{
    return m_maxValue;
}

/*! Sets the maximum value of this ParamType to the given \a maxValue. */
void ParamType::setMaxValue(const QVariant &maxValue)
{
    m_maxValue = maxValue;
}

/*! Returns the input type of this ParamType. */
Types::InputType ParamType::inputType() const
{
    return m_inputType;
}

/*! Sets the input value of this ParamType to the given \a inputType. */
void ParamType::setInputType(const Types::InputType &inputType)
{
    m_inputType = inputType;
}

/*! Returns the unit of this ParamType. */
Types::Unit ParamType::unit() const
{
    return m_unit;
}

/*! Sets the unit of this ParamType to the given \a unit. */
void ParamType::setUnit(const Types::Unit &unit)
{
    m_unit = unit;
}

/*! Returns the limits of this ParamType. limits(minValue, maxValue). */
QPair<QVariant, QVariant> ParamType::limits() const
{
    return qMakePair<QVariant, QVariant>(m_minValue, m_maxValue);
}

/*! Sets the limits of this ParamType. limits(\a min, \a max). */
void ParamType::setLimits(const QVariant &min, const QVariant &max)
{
    m_minValue = min;
    m_maxValue = max;
}

/*! Returns the list of the allowed values of this ParamType. */
QList<QVariant> ParamType::allowedValues() const
{
    return m_allowedValues;
}

/*! Sets the list of the allowed values of this ParamType to the given List of \a allowedValues. */
void ParamType::setAllowedValues(const QList<QVariant> &allowedValues)
{
    m_allowedValues = allowedValues;
}

/*! Returns false if this ParamType is writable by the user. By default a ParamType is always writable. */
bool ParamType::readOnly() const
{
    return m_readOnly;
}

/*! Sets this ParamType \a readOnly. By default a ParamType is always writable. */
void ParamType::setReadOnly(const bool &readOnly)
{
    m_readOnly = readOnly;
}

/*! Writes the name, type defaultValue, min value, max value and readOnly of the given \a paramType to \a dbg. */
QDebug operator<<(QDebug dbg, const ParamType &paramType)
{
    dbg.nospace() << "ParamType(Name: " << paramType.name()
                  << ", Type:" << QVariant::typeToName(paramType.type())
                  << ", Default:" << paramType.defaultValue()
                  << ", Min:" << paramType.minValue()
                  << ", Max:" << paramType.maxValue()
                  << ", Allowed values:" << paramType.allowedValues()
                  << ", ReadOnly:" << paramType.readOnly()
                  << ")";

    return dbg.space();
}

/*! Writes the name, type defaultValue, min and max value of each \a paramTypes to \a dbg. */
QDebug operator<<(QDebug dbg, const QList<ParamType> &paramTypes)
{
    dbg.nospace() << "ParamTypeList (count:" << paramTypes.count() << ")" << endl;
    for (int i = 0; i < paramTypes.count(); i++ ) {
        dbg.nospace() << "     " << i << ": " << paramTypes.at(i) << endl;
    }

    return dbg.space();
}
