C++ Code Problem (to be run under Windows OS)

Implement a command console for changing settings on a particular object. The command console should allow you to enter a string and will return the response (very similar to a terminal session).

The commands are as follows:
SET propertyname=newvalue 

will change the target object’s member named “propertyname” to have a value equal to “newvalue”. 
If the input value is incompatible (i.e. an int being set to a string), print out an appropriate error message.

GET propertyname will print out the current value of the target object’s member named “propertyname”.

GET * will print out a list of all target object members and their current values.

The system should be extensible for future commands and should accept an arbitrary object, such that another developer could insert another object into the system and rely on the command console to get and set the properties correctly.
