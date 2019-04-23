/*********************************************************************
Copyright 2019, Inria, CNRS, University of Lille

This file is part of runSofa2

runSofa2 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

runSofa2 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with sofaqtquick. If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/
/********************************************************************
 Contributors:
    - damien.marchal@univ-lille.fr
********************************************************************/
#include <sofa/helper/logging/Message.h>
using sofa::helper::logging::Message;

#include <SofaQtQuickGUI/Bindings/SofaBase.h>

#include <SofaQtQuickGUI/Bindings/SofaData.h>
using sofaqtquick::bindings::SofaData;

namespace sofaqtquick::bindings::_sofabase_
{

SofaBase::SofaBase()
{
}

SofaBase::SofaBase(Base::SPtr self)
{
    m_self = self;
}

QString SofaBase::getName() const
{
    return QString::fromStdString(m_self->getName());
}

QString SofaBase::getClassName() const
{
    return QString::fromStdString(m_self->getClassName());
}

QString SofaBase::getTemplateName() const
{
    return QString::fromStdString(m_self->getTemplateName());
}

QObject* SofaBase::getData(const QString& name) const
{
    BaseData* data = m_self->findData(name.toStdString());
    if(!data)
        return nullptr;
    return new SofaData(data);
}

QStringList SofaBase::getDataFields() const
{
    const sofa::helper::vector<BaseData*>& datafields = m_self->getDataFields();
    QStringList list;
    for (auto& field : datafields)
        list.append(QString(field->getName().c_str()));
    return list;
}

bool SofaBase::hasLocations() const
{
    const Base* base = m_self.get();
    if(base)
    {
        return base->findData("Defined in") != nullptr ;
    }
    return false;
}

QString SofaBase::getSourceLocation() const
{
    const Base* base = m_self.get();
    if(base)
    {
        BaseData* data = base->findData("Defined in") ;
        if(data)
            return QString::fromStdString(data->getValueString());
    }
    return "('',0)";
}

QString SofaBase::getCreationLocation() const
{
    const Base* base = m_self.get();
    if(base)
    {
        BaseData* data = base->findData("Instantiated in") ;
        if(data)
            return QString::fromStdString(data->getValueString());
    }
    return "('',0)";
}

QString SofaBase::output() const
{
    const Base* base = m_self.get();
    if(base)
        return QString::fromStdString(base->getLoggedMessagesAsString({Message::Info, Message::Advice}));

    return QString();
}

QString SofaBase::warning() const
{
    const Base* base = m_self.get();
    if(base)
        return QString::fromStdString(base->getLoggedMessagesAsString({Message::Deprecated,
                                                                       Message::Warning,
                                                                       Message::Error,
                                                                       Message::Fatal
                                                                      }));

return QString();
}

void SofaBase::clearOutput() const
{
    Base* base = m_self.get();
    if(base)
    {
        base->clearOutputs();
    }
}

void SofaBase::clearWarning() const
{
    Base* base = m_self.get();
    if(base)
    {
        base->clearWarnings();
    }
}

} /// sofaqtquick
