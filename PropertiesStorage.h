#pragma once

#include <sstream>
#include <iostream>
#include <map>
#include <typeinfo>

namespace Storage
{
    typedef signed __int64 Int64;
    typedef signed __int32 Int32;
    typedef std::string    String;
    typedef double         Double;

    enum class PropertyType
    {
        Type_Unknown, 
        Type_String,
        Type_Int32,
        Type_Int64,
        Type_Double,
    };

    template<class T> class VariantValue
    {
    public:

        VariantValue() { setDefaultValue(); }
        VariantValue(const T& val) : m_value(val) { }

        void setDefaultValue();
        void operator= (const T& rVal) { set(rVal); }
        void set(const T& val) { m_value = val; }
        T get() const { return m_value; }
        std::string toString() const;

    private:

        T m_value;
    };

    template<class T> inline std::ostream& operator<<(std::ostream& out, const VariantValue<T>& v)
    {
        return out << v.toString();
    }

    // --------------------------------------------------------------------------------------------

    class Property
    {
    public:

        virtual PropertyType getType() const = 0;
        virtual std::string getAsString() const = 0;
        virtual bool fromString(const std::string& value) = 0;
        virtual bool copy(const Property* p) = 0;
        virtual bool operator== (const Property* p) = 0;

        std::string getLastError() const { return m_lastError; }

    protected:

        mutable std::string m_lastError;
    };

    inline std::ostream& operator<<(std::ostream& out, const Property *p)
    {
        return out << (p ? p->getAsString() : std::string());
    }

    // --------------------------------------------------------------------------------------------

    template<class T> class PropValue : public Property, public VariantValue<T>
    {
    public:

        PropValue() { }
        PropValue(const T& val) : VariantValue<T>(val) { }
        std::string getAsString() const { return VariantValue<T>::toString(); }
        T get() const { return VariantValue<T>::get(); }
        void set(const T& val) { VariantValue<T>::set(val); }
        bool fromString(const std::string& value);

        PropertyType getType() const 
        { 
            if (typeid(get()) == typeid(String))
                return PropertyType::Type_String;
            else if (typeid(get()) == typeid(Int32))
                return PropertyType::Type_Int32;
            else if (typeid(get()) == typeid(Int64))
                return PropertyType::Type_Int64;
            else if (typeid(get()) == typeid(Double))
                return PropertyType::Type_Double;

            return PropertyType::Type_Unknown;
        }

        bool copy(const Property* p)
        {
            m_lastError.clear();
            if (p && p->getType() == getType())
            {
                VariantValue<T>::set( dynamic_cast<const Storage::PropValue<T>*>(p)->get() );
                return true;
            }
            m_lastError = "Illigal operation";
            return false;
        }

        bool operator== (const Property* p)
        {
            return p && p->getType() == getType() &&
                   dynamic_cast<const Storage::PropValue<T>*>(p)->get() == get();
        }
    };

    // --------------------------------------------------------------------------------------------

    using PropertyMap = std::map<std::string, Property*>;

    class PropertyStorage
    {
        friend inline std::ostream& operator<<(std::ostream& out, const PropertyStorage& p);

    public:
        PropertyStorage() { }
        PropertyStorage(const std::string &name) : m_storageName(name) { }
        ~PropertyStorage() 
        { 
            for (auto& it : m_propStorage)
            {
                delete it.second;
                it.second = nullptr;
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

        static Property* createProperty(PropertyType prop_type);
        static Property* createProperty(const std::string &value);

        // Helper methods for convinience (if you sure about the type of the property and you know that propery is defined)

        bool getProp(const std::string& prop_name, String &val) const;
        bool getProp(const std::string& prop_name, Int32& val) const;
        bool getProp(const std::string& prop_name, Int64& val) const;
        bool getProp(const std::string& prop_name, Double& val) const;

        String getString(const std::string& prop_name) const;
        Int32 getInt32(const std::string& prop_name) const;
        Int64 getInt64(const std::string& prop_name) const;
        Double getDouble(const std::string& prop_name) const;

        bool setProp(const std::string& prop_name, const String& val);
        bool setProp(const std::string& prop_name, const Int32& val);
        bool setProp(const std::string& prop_name, const Int64& val);
        bool setProp(const std::string& prop_name, const Double& val);

    private:
        std::string m_storageName;
        PropertyMap m_propStorage;
        mutable std::string m_lastError;
    };
    
    inline std::ostream& operator<<(std::ostream& out, const PropertyStorage &p)
    {
        for (const auto& it : p.m_propStorage)
            out << it.first << " = " << it.second << std::endl;

        return out;
    }

}
