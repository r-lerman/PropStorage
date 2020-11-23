#include "Console.h"
#include "Auxiliary.h"

void Console::Run(std::istream& in, std::ostream& out)
{
    if (!storage.getName().empty())
        out << "Console for storage: [" << storage.getName() << "]" << std::endl << std::endl;

    std::string input, cmn_name, cmn_text;
    do
    {
        out << ">";
        std::getline(in, input);
        ltrim(input);

        SplitText(input, cmn_name, cmn_text, " ");
        ProcessCommand(out, cmn_name, cmn_text);
        out << std::endl;
    } 
    while (cmn_name != "EXIT");
}

void Console::ProcessCommand(std::ostream& out, std::string cmn_name, std::string cmn_text)
{
    if (cmn_name == "GET")
    {
        cmn_text = trim(cmn_text);
        if (cmn_text.empty())
            out << "Wrong syntax." << std::endl;
        else if (cmn_text == "*")
        {
            if (storage.propCount() > 0)
                out << storage;
            else
                out << "No properties defined in the storage." << std::endl;
        }
        else
        {
            if (storage.isProperyDefined(cmn_text))
                out << storage.getProperty(cmn_text) << std::endl;
            else
                out << "Property not defined." << std::endl;
        }
    }
    else if (cmn_name == "SET")
    {
        std::string prop_name, prop_value;
        SplitText(cmn_text, prop_name, prop_value, "=");

        if (prop_name.empty() || prop_value.empty())
            out << "Wrong syntax." << std::endl;
        else
        {
            if (storage.isProperyDefined(prop_name))
            {
                Storage::Property *prop = Storage::PropertyStorage::createProperty(storage.getProperty(prop_name)->getType());
                
                if (prop->fromString(prop_value))
                {
                    if (!storage.setProperty(prop_name, prop))
                        out << storage.getLastError() << std::endl;
                }
                else
                    out << prop->getLastError() << std::endl;

                delete prop;
            }
            else
            {
                // Define a new property.
                // Find property type based on the value using the best assumption.

                Storage::Property* prop = Storage::PropertyStorage::createProperty(prop_value);

                if (!prop)
                    out << "Cannot determine type for a new (not defined) property." << std::endl;
                else
                {
                    if (prop->fromString(prop_value))
                    {
                        if (!storage.defineProperty(prop_name, prop->getType()) || !storage.setProperty(prop_name, prop))
                            out << storage.getLastError() << std::endl;
                        else
                            out << "New property was added to the storage." << std::endl;
                    }
                    else
                        out << prop->getLastError() << std::endl;

                    delete prop;
                }
            }
        }
    }
    else if (cmn_name == "DELETE")
    {
        cmn_text = trim(cmn_text);
        if (cmn_text.empty())
            out << "Wrong syntax." << std::endl;
        else
        {
            if (storage.deleteProperty(cmn_text))
                out << "Property was deleted." << std::endl;
            else                
                out << storage.getLastError() << std::endl;
        }
    }
    else
    {
        if (cmn_name != "EXIT") 
            out << "Unknown command." << std::endl;
    }
}
