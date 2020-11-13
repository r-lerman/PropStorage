#include <iomanip>
#include "PropertiesStorage.h"
#include "Auxiliary.h"

namespace Storage
{
    // String type template functions ------------------------------------------------

    void PropertyValue<String>::setDefaultValue()
    {
        m_value.clear();
    }

    template<> std::string PropertyValue<String>::toString() const
    {
        return "\"" + m_value + "\"";
    }

    // Int32 type template functions ------------------------------------------------

    void PropertyValue<Int32>::setDefaultValue()
    {
        m_value = 0;
    }

    template<> std::string PropertyValue<Int32>::toString() const
    {
        std::ostringstream oss;
        oss << m_value;
        return oss.str();
    }

    // Int64 type template functions ------------------------------------------------

    void PropertyValue<Int64>::setDefaultValue()
    {
        m_value = 0;
    }

    template<> std::string PropertyValue<Int64>::toString() const
    {
        std::ostringstream oss;
        oss << m_value;
        return oss.str();
    }

    // Double type template functions ------------------------------------------------

    void PropertyValue<Double>::setDefaultValue()
    {
        m_value = 0.0;
    }

    template<> std::string PropertyValue<Double>::toString() const
    {
        std::ostringstream oss;
        oss << std::setprecision(5) << std::setiosflags(std::ios::fixed) << m_value;
        return oss.str();
    }

    // PropertyStorage functions -----------------------------------------------------------------------------------

    inline String PropertyStorage::GetStringProp(const std::string& prop_name) const
    {
        const Property* p = getProperty(prop_name);
        return (p && p->getType() == PropertyType::Type_String) ? dynamic_cast<const Storage::StringProperty*>(p)->get() : String();
    }

    inline Int32 PropertyStorage::GetInt32Prop(const std::string& prop_name) const
    {
        const Property* p = getProperty(prop_name);
        return (p && p->getType() == PropertyType::Type_Int32) ? dynamic_cast<const Storage::Int32Property*>(p)->get() : 0;
    }

    inline Int64 PropertyStorage::GetInt64Prop(const std::string& prop_name) const
    {
        const Property* p = getProperty(prop_name);
        return (p && p->getType() == PropertyType::Type_Int64) ? dynamic_cast<const Storage::Int64Property*>(p)->get() : 0;
    }

    inline Double PropertyStorage::GetDoubleProp(const std::string& prop_name) const
    {
        const Property* p = getProperty(prop_name);
        return (p && p->getType() == PropertyType::Type_Double) ? dynamic_cast<const Storage::DoubleProperty*>(p)->get() : 0.0;
    }

    Property* PropertyStorage::CreateProperty(PropertyType prop_type)
    {
        if (prop_type == PropertyType::Type_String)
            return new StringProperty();
        else if (prop_type == PropertyType::Type_Int32)
            return new Int32Property();
        else if (prop_type == PropertyType::Type_Int64)
            return new Int64Property();
        else if (prop_type == PropertyType::Type_Double)
            return new DoubleProperty();

        return nullptr;
    }

    Property* PropertyStorage::CreateProperty(const std::string& value)
    {
        if (isNumber(value))
        {
            std::string::size_type idx = value.find_first_of(".");
            if (idx != std::string::npos)
                return new DoubleProperty();
            else
            {
                try
                {
                    Int32 n = std::stol(value);
                    return new Int32Property();
                }
                catch (const std::out_of_range)
                {
                    return new Int64Property();
                }
            }
        }
        else
            return new StringProperty();

        return nullptr;
    }

    bool PropertyStorage::defineProperty(const std::string &prop_name, PropertyType prop_type)
    {
        m_lastError.clear();
        if (prop_name.empty())
        {
            m_lastError = "Empty property name";
            return false;
        }

        PropertyMap::const_iterator it = m_propStorage.find(prop_name);
        if (it != m_propStorage.end())
        {
            m_lastError = "Attempt property redefinition";
            return false;
        }

        Property *p = CreateProperty(prop_type);
        if (!p)
        {
            m_lastError = "Wrong property type";
            return false;
        }

        m_propStorage[prop_name] = p;
        return true;
    }

    const Property* PropertyStorage::getProperty(const std::string &prop_name) const
    {
        m_lastError.clear();
        PropertyMap::const_iterator it = m_propStorage.find(prop_name);
        if (it != m_propStorage.end())
            return it->second;

        m_lastError = "Property not defined";
        return nullptr;
    }

    bool PropertyStorage::deleteProperty(const std::string& prop_name)
    {
        m_lastError.clear();
        PropertyMap::const_iterator it = m_propStorage.find(prop_name);
        if (it != m_propStorage.end())
        {
            m_propStorage.erase(it);
            return true;
        }

        m_lastError = "Property not defined";
        return false;
    }

    bool PropertyStorage::setProperty(const std::string &prop_name, const Property* p)
    {
        m_lastError.clear();
        if (prop_name.empty())
        {
            m_lastError = "Empty property name";
            return false;
        }

        PropertyMap::const_iterator it = m_propStorage.find(prop_name);
        if (it == m_propStorage.end())
        {
            m_lastError = "Property not defined";
            return false;
        }

        if (!p)
        {
            m_lastError = "Wrong property value";
            return false;
        }

        if(p->getType() != it->second->getType())
        {
            m_lastError = "Attempt property type redefinition";
            return false;
        }

        if (!it->second->copy(p))
        {
            m_lastError = it->second->getLastError();
            return false;
        }

        return true;
    }

    void PropertyStorage::operator= (const PropertyStorage& rVal)
    {
        for (PropertyMap::iterator it = m_propStorage.begin(); it != m_propStorage.end(); it++)
        {
            delete it->second;
            it->second = nullptr;
        }

        m_propStorage.clear();
        m_storageName = rVal.m_storageName;
        m_lastError.clear();

        for (PropertyMap::const_iterator it = rVal.m_propStorage.begin(); it != rVal.m_propStorage.end(); it++)
        {
            if (defineProperty(it->first, it->second->getType()))
                setProperty(it->first, it->second);
        }
    }

    bool PropertyStorage::saveStorage() const
    {
        // Save storage to file or DB

        // TO DO
        // It was out of my task scope 

        return true;
    }

    bool PropertyStorage::loadStorage()
    {
        // Load storage from file or DB

        // TO DO
        // It was out of my task scope 

        return true;
    }
}
