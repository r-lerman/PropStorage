#pragma once

#include <sstream>
#include <iostream>
#include <map>

namespace Storage
{
    typedef signed __int64 Int64;
    typedef signed __int32 Int32;
    typedef std::string    String;
    typedef double         Double;

    enum class PropertyType
    {
        Type_String,
        Type_Int32,
        Type_Int64,
        Type_Double
    };

    template<class T> class PropertyValue
    {
    public:

        PropertyValue() { setDefaultValue(); }
        PropertyValue(const T& val) : m_value(val) { }

        void setDefaultValue();
        void operator= (const T& rVal) { set(rVal); }
        void set(const T& val) { m_value = val; }
        T get() const { return m_value; }
        std::string toString() const;

    private:

        T m_value;
    };

    template<class T> inline std::ostream& operator<<(std::ostream& out, const PropertyValue<T>& v)
    {
        return out << v.toString();
    }

    // --------------------------------------------------------------------------------------------

    class Property
    {
    public:

        virtual PropertyType getType() const = 0;
        virtual std::string getAsString() const = 0;
        virtual bool setFromString(const std::string& value) = 0;
        virtual bool copy(const Property* p) = 0;

        std::string getLastError() const { return m_lastError; }

    protected:

        mutable std::string m_lastError;
    };

    inline std::ostream& operator<<(std::ostream& out, const Property *p)
    {
        return out << (p ? p->getAsString() : std::string());
    }

    // --------------------------------------------------------------------------------------------

    class StringProperty : public Property, public PropertyValue<String>
    {
    public:

        StringProperty() { }
        StringProperty(const String& s) : PropertyValue<String>(s) { }

        std::string getAsString() const { return toString(); }
        PropertyType getType() const { return PropertyType::Type_String; }
        bool setFromString(const std::string& value) { set(value); return true; }

        bool copy(const Property* p)
        { 
            m_lastError.clear();
            if (p && p->getType() == getType())
            {
                set(dynamic_cast<const StringProperty*>(p)->get());
                return true;
            }
            m_lastError = "Illigal operation";
            return false;
        }
    };

    class Int32Property : public Property, public PropertyValue<Int32>
    {
    public:

        Int32Property() { }
        Int32Property(Int32 n) : PropertyValue<Int32>(n) { }

        std::string getAsString() const { return toString(); }
        PropertyType getType() const { return PropertyType::Type_Int32; }
        bool setFromString(const std::string& value) 
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

        bool copy(const Property* p)
        {
            m_lastError.clear();
            if (p && p->getType() == getType())
            {
                set(dynamic_cast<const Int32Property*>(p)->get());
                return true;
            }
            m_lastError = "Illigal operation";
            return false;
        }
    };

    class Int64Property : public Property, public PropertyValue<Int64>
    {
    public:

        Int64Property() { }
        Int64Property(Int64 n) : PropertyValue<Int64>(n) { }

        std::string getAsString() const { return toString(); }
        PropertyType getType() const { return PropertyType::Type_Int64; }
        bool setFromString(const std::string& value)
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

        bool copy(const Property* p)
        {
            m_lastError.clear();
            if (p && p->getType() == getType())
            {
                set(dynamic_cast<const Int64Property*>(p)->get());
                return true;
            }
            m_lastError = "Illigal operation";
            return false;
        }
    };

    class DoubleProperty : public Property, public PropertyValue<Double>
    {
    public:

        DoubleProperty() { }
        DoubleProperty(Double n) : PropertyValue<Double>(n) { }

        std::string getAsString() const { return toString(); }
        PropertyType getType() const { return PropertyType::Type_Double; }
        bool setFromString(const std::string& value)
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

        bool copy(const Property* p)
        {
            m_lastError.clear();
            if (p && p->getType() == getType())
            {
                set(dynamic_cast<const DoubleProperty*>(p)->get());
                return true;
            }
            m_lastError = "Illigal operation";
            return false;
        }
    };

    // --------------------------------------------------------------------------------------------

    typedef std::map<std::string, Property*> PropertyMap;

    class PropertyStorage
    {
        friend inline std::ostream& operator<<(std::ostream& out, const PropertyStorage& p);

    public:
        PropertyStorage() { }
        PropertyStorage(const std::string &name) : m_storageName(name) { }
        ~PropertyStorage() 
        { 
            for (PropertyMap::iterator it = m_propStorage.begin(); it != m_propStorage.end(); it++)
            {
                delete it->second;
                it->second = nullptr;
            }

            m_propStorage.clear(); 
        }
        void operator= (const PropertyStorage& rVal);

        bool defineProperty(const std::string &prop_name, PropertyType prop_type);
        bool isProperyDefined(const std::string& prop_name) { return m_propStorage.find(prop_name) != m_propStorage.end(); }

        const Property* getProperty(const std::string &prop_name) const;
        bool setProperty(const std::string &prop_name, const Property* p);
        bool deleteProperty(const std::string& prop_name);

        size_t propCount() const { return m_propStorage.size(); }
        bool saveStorage() const;
        bool loadStorage();

        void setName(const std::string& name) { m_storageName = name; }
        std::string getName() const { return m_storageName; }
        
        std::string getLastError() const { return m_lastError; }

        static Property* CreateProperty(PropertyType prop_type);
        static Property* CreateProperty(const std::string &value);

        // Helper methods for convinience (if you shure about the type of the property and you know that propery is defined)

        String GetStringProp(const std::string& prop_name) const;
        Int32 GetInt32Prop(const std::string& prop_name) const;
        Int64 GetInt64Prop(const std::string& prop_name) const;
        Double GetDoubleProp(const std::string& prop_name) const;

    private:
        std::string m_storageName;
        PropertyMap m_propStorage;
        mutable std::string m_lastError;
    };
    
    inline std::ostream& operator<<(std::ostream& out, const PropertyStorage &p)
    {
        for (PropertyMap::const_iterator it = p.m_propStorage.begin(); it != p.m_propStorage.end(); it++)
            out << it->first << " = " << it->second << std::endl;

        return out;
    }

}
