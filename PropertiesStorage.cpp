#include <iomanip>
#include "PropertiesStorage.h"
#include "Auxiliary.h"

namespace Storage
{
    // String type template functions ------------------------------------------------

    void VariantValue<String>::setDefaultValue()
    {
        m_value.clear();
    }

    template<> std::string VariantValue<String>::toString() const
    {
        return "\"" + m_value + "\"";
    }

    template<> bool PropValue<String>::fromString(const std::string& value)
    {
        set(value); return true;
    }

    // Int32 type template functions ------------------------------------------------

    void VariantValue<Int32>::setDefaultValue()
    {
        m_value = 0;
    }

    template<> std::string VariantValue<Int32>::toString() const
    {
        std::ostringstream oss;
        oss << m_value;
        return oss.str();
    }

    template<> bool PropValue<Int32>::fromString(const std::string& value)
    {
        m_lastError.clear();
        try
        {
            Int32 n = std::stol(value);
            set(n);
            return true;
        }
        catch (const std::invalid_argument)
        {
            m_lastError = "Invalid property value";
        }
        catch (const std::out_of_range)
        {
            m_lastError = "Out of Range error";
        }
        return false;
    }

    // Int64 type template functions ------------------------------------------------

    void VariantValue<Int64>::setDefaultValue()
    {
        m_value = 0;
    }

    template<> std::string VariantValue<Int64>::toString() const
    {
        std::ostringstream oss;
        oss << m_value;
        return oss.str();
    }

    template<> bool PropValue<Int64>::fromString(const std::string& value)
    {
        m_lastError.clear();
        try
        {
            Int64 n = std::stoll(value);
            set(n);
            return true;
        }
        catch (const std::invalid_argument)
        {
            m_lastError = "Invalid property value";
        }
        catch (const std::out_of_range)
        {
            m_lastError = "Out of Range error";
        }
        return false;
    }

    // Double type template functions ------------------------------------------------

    void VariantValue<Double>::setDefaultValue()
    {
        m_value = 0.0;
    }

    template<> std::string VariantValue<Double>::toString() const
    {
        std::ostringstream oss;
        oss << std::setprecision(5) << std::setiosflags(std::ios::fixed) << m_value;
        return oss.str();
    }

    template<> bool PropValue<Double>::fromString(const std::string& value)
    {
        m_lastError.clear();
        try
        {
            Double d = std::stold(value);
            set(d);
            return true;
        }
        catch (const std::invalid_argument)
        {
            m_lastError = "Invalid property value";
        }
        catch (const std::out_of_range)
        {
            m_lastError = "Out of Range error";
        }
        return false;
    }

    // PropertyStorage functions -----------------------------------------------------------------------------------

#define GET_PROP(f) \
    try { val = f(prop_name); return true; } \
    catch (std::string err) { m_lastError = err; } \
    return false;

    bool PropertyStorage::getProp(const std::string& prop_name, String& val) const
    {
        GET_PROP(getString)
    }

    bool PropertyStorage::getProp(const std::string& prop_name, Int32& val) const
    {
        GET_PROP(getInt32)
    }

    bool PropertyStorage::getProp(const std::string& prop_name, Int64& val) const
    {
        GET_PROP(getInt64)
    }

    bool PropertyStorage::getProp(const std::string& prop_name, Double& val) const
    {
        GET_PROP(getDouble)
    }

#define GET_PROP_BY_TYPE(type_name, type) \
    const Property* p = getProperty(prop_name); \
    if (!p) throw "Property not defined"; \
    if (p->getType() != PropertyType::type_name) throw "Type mismatch"; \
    return dynamic_cast<const Storage::PropValue<type>*>(p)->get();

    inline String PropertyStorage::getString(const std::string& prop_name) const
    {
        GET_PROP_BY_TYPE(Type_String, String)
    }

    inline Int32 PropertyStorage::getInt32(const std::string& prop_name) const
    {
        GET_PROP_BY_TYPE(Type_Int32, Int32)
    }

    inline Int64 PropertyStorage::getInt64(const std::string& prop_name) const
    {
        GET_PROP_BY_TYPE(Type_Int64, Int64)
    }

    inline Double PropertyStorage::getDouble(const std::string& prop_name) const
    {
        GET_PROP_BY_TYPE(Type_Double, Double)
    }

    // -------------------------------------------------------------------------------------------------------

#define SET_PROP(type, class_name) \
    m_lastError.clear(); \
    PropertyMap::const_iterator it = m_propStorage.find(prop_name); \
    if (it == m_propStorage.end()) { m_lastError = "Property not defined"; return false; } \
    if (it->second->getType() != PropertyType::type) { m_lastError = "Type mismatch"; return false; } \
    dynamic_cast<Storage::PropValue<class_name>*>(it->second)->set(val); \
    return true;

    bool PropertyStorage::setProp(const std::string& prop_name, const String& val) 
    {
        SET_PROP(Type_String, String)
    }

    bool PropertyStorage::setProp(const std::string& prop_name, const Int32& val)
    {
        SET_PROP(Type_Int32, Int32)
    }

    bool PropertyStorage::setProp(const std::string& prop_name, const Int64& val)
    {
        SET_PROP(Type_Int64, Int64)
    }

    bool PropertyStorage::setProp(const std::string& prop_name, const Double& val)
    {
        SET_PROP(Type_Double, Double)
    }

    // -------------------------------------------------------------------------------------------------------

    Property* PropertyStorage::createProperty(PropertyType prop_type)
    {
        try
        {
            if (prop_type == PropertyType::Type_String)
                return new PropValue<String>();
            else if (prop_type == PropertyType::Type_Int32)
                return new PropValue<Int32>();
            else if (prop_type == PropertyType::Type_Int64)
                return new PropValue<Int64>();
            else if (prop_type == PropertyType::Type_Double)
                return new PropValue<Double>();
        }
        catch(...) { }

        return nullptr;
    }

    Property* PropertyStorage::createProperty(const std::string& value)
    {
        try
        {
            if (isNumber(value))
            {
                std::string::size_type idx = value.find_first_of(".");
                if (idx != std::string::npos)
                    return new PropValue<Double>();
                else
                {
                    try
                    {
                        Int32 n = std::stol(value);
                        return new PropValue<Int32>();
                    }
                    catch (const std::out_of_range)
                    {
                        return new PropValue<Int64>();
                    }
                }
            }
            else
                return new PropValue<String>();
        }
        catch (...) {}

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

        Property *p = createProperty(prop_type);
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
