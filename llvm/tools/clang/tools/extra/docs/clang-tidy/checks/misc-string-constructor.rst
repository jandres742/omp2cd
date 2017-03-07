.. title:: clang-tidy - misc-string-constructor

misc-string-constructor
=======================

Finds string constructors that are suspicious and probably errors.

A common mistake is to swap parameters to the 'fill' string-constructor.

Examples:

.. code:: c++

  std::string('x', 50) str; // should be std::string(50, 'x') 


Calling the string-literal constructor with a length bigger than the literal is
suspicious and adds extra random characters to the string.

Examples:

.. code:: c++

  std::string("test", 200);   // Will include random characters after "test".


Creating an empty string from constructors with parameters is considered
suspicious. The programmer should use the empty constructor instead.

Examples:

.. code:: c++

  std::string("test", 0);   // Creation of an empty string.

