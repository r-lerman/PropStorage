C++ Code Problem (to be run under Windows OS)

Implement a command console for changing settings on a particular object. The command console should allow you to enter a string and will return the response (very similar to a terminal session).

The commands are as follows:
SET propertyname=newvalue 

will change the target object’s member named “propertyname” to have a value equal to “newvalue”. 
If the input value is incompatible (i.e. an int being set to a string), print out an appropriate error message.

GET propertyname will print out the current value of the target object’s member named “propertyname”.

GET * will print out a list of all target object members and their current values.

The system should be extensible for future commands and should accept an arbitrary object, such that another developer could insert another object into the system and rely on the command console to get and set the properties correctly.

------------------------------------------------------------------------------------------------------------------------------------------------------

Please use "Microsoft Visual C++ 2019".

    // Note 1: All commands are case sensitive
    //         (since SPEC does not contain explicit instructions for it)

    // Note 2: In console I allow to define new properties in the storage if the property is not yet defined.
    //         I will assign property type based on the value (using the best assumption) in this case.

    // Note 3: I implemented "DELETE properyName" command in addition to "SET properyName=value", "GET properyName" and "GET *".
    //         Use "EXIT" command for closing the console.
    //         New commands can be easily added in "Console::ProcessCommand" function.

    // Note 4: Steps for adding new property type:
    //         1) Add new type to Storage::PropertyType enum class.
    //         2) Define setDefaultValue() and toString() functions for PropertyValue template class for a new type.
    //         3) Add new type to "PropertyStorage::CreateProperty" function.
    //         4) Create "class NEWTYPEProperty : public Property, public PropertyValue<NEWTYPE>"
    //            and overload "setFromString" and "copy" functions. 
