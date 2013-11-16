NXtpandedLib
============

A library for nxtOSEK C++

----
Why?

There is a library for C++ in nxtosek called ECRobot C++
The problem is that this library is only built on the C API and it does not really offer more functionality than the C API.
I started programming my own library which uses all the good stuff from C++ to create an easy to use API for efficent
programming with NXTosek.
For example you can now use cout, vectors, strings... and for nxt designs lcd objects like labels, numeric indicators,
lines, rectangles, circles, advanced timer, motorcontrollers ...


Actual state

Most of the classes are working fine but i am busy to rewrite everything because i always get new ideas.
For example separate NWidget into a widget class and a shape class. Then make show, erease, invert virtual...

I have much ideas for new classes. :)

In a later state of the project you should be able to form additional classes from existing classes. 
For example make a triangle class by inheriting from NShape and using 3 NLines.
