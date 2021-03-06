#include "Console.h"

int main()
{
    Storage::PropertyStorage st("alfa");
    
    // It is not nesessary but we can predefine some properties
    st.defineProperty("str", Storage::PropertyType::Type_String);
    st.defineProperty("number", Storage::PropertyType::Type_Int32);
    st.defineProperty("very_long", Storage::PropertyType::Type_Int64);
    st.defineProperty("dbl", Storage::PropertyType::Type_Double);

    // Note 1: All commands are case sensitive
    //         (since SPEC does not contain explicit instructions for it)

    // Note 2: In console I allow to define new properties in the storage if the property is not yet defined.
    //         I will assign "property type" based on the value (using the best assumption) in this case.

    // Note 3: I implemented "DELETE properyName" command in addition to "SET properyName=value", "GET properyName" and "GET *".
    //         Use "EXIT" command for closing the console.
    //         New commands can be easily added in "Console::ProcessCommand" function.

    // Note 4: Steps for adding new property type:
    //         1) Add new type to Storage::PropertyType enum class.
    //         2) Define "setDefaultValue", "toString" and "fromString" functions for VariantValue template class for a new type.
    //         3) Add new type to "PropertyStorage::createProperty" function.

    Console con(st);
    con.Run(std::cin, std::cout);

    return 0;
}

