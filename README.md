@mainpage

NXtpandedLib
============

A library for nxtOSEK C++

----
Why?

There is a library for C++ in nxtosek called ECRobot C++
The problem is that this library is only built on the C API and it does not really offer more functionality than the C API.
I started programming my own library which uses all the good stuff from C++ to create an easy to use API for efficent
programming with NXTosek.
For example you can now use ostream(cout), vectors, strings... and for nxt designs lcd objects like widgets, 
rectangles, circles, advanced timer, motorcontrollers ...

State:

Most of the classes are working fine and should be tested.
Next steps are:
- Making documentation ready.
- Make examples.
- Make first release package.
- Make tutorial on new virtual mashine from installation to first program.

After that I have much ideas for new classes.

Contact me if you want to work with me on the project!

-----------------------------------------------------------------------------------------

The NXtpandedLib project is free software: you can redistribute it and/or modify 
it under the terms of the GNU General Public License as published by the Free Software 
Foundation, either version 3 of the License, or (at your option) any later version. 

The NXtpandedLib project is distributed in the hope that it will be useful, 
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. 

--------------------------------------------------------------------------------------------
http://www.gnu.org/licenses/

Author Website:
<http://www.roboticsaumair.jimdo.com/>

@author Martin Aumair
@date 2.March.2013

\defgroup NxtIO NxtIO
\brief Includes functionality for nxt inputs and outputs. (actuators, sensors, buttons)
\defgroup System System
\brief Things which extend the functionality of the operating system.
\defgroup Utilities Utilities
\brief Useful classes/functions which have no particular affiliation.
\defgroup Communication Communication
\brief Communicate with the outside world.
\defgroup NxtLcd NxtLcd
\brief All things make working with the lcd easier.
