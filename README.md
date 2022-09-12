# adecc_Scholar

In this archive the sources from the course are shared on the channel "adecc Scholar". I have put them under MIT license, so the sources can be used by everyone without any restrictions. It should be noted that these are educational examples, so we do not provide any warranty. The goal was not the safe productive use, but to impart knowledge.

Some of the classes are not complete, but only hint at the learning content. 

To use the headeronly library from the semester break, one of the following constants must be defined
- BUILD_WITH_VCL, C++Builder or RAD Studio 10.3 or higher with Windows application (VCL)
- BUILD_WITH_FMX, C++Builder or RAD Studio 10.3 or higher with platform independent application (FMX)
- BUILD_WITH_QT - for Visual Studio with Qt6 and with the Qt Creator

Questions and suggestions please via the Discord server for adecc Scholar

# From the wish list to the independent program

At events and trainings we often claimed that it is easy to start with a wish list and to design a platform independent program in C++ from it. It should be possible to start immediately and it should be possible to integrate it into every project in an uncomplicated way, in order not to have to start again and again on a green field. We had already shown this in the beginning with the two libraries of Embarcadero, the VCL on the one hand and Firemonkey (FMX) on the other hand. Now these two libraries are based on the same, in Delphi written basis, and are structurally similarly developed. So is it at all possible to generalize?

In the semester break between our lectures on the adecc Scholar platform, we repeated this from the beginning in a semester project and finally extended it to the independent user interface library Qt and Visual Studio 2019 as a development platform.

This already answers the question, of course it is possible to start with an interface, let's just call it a wish list now, and create an independent solution for standard C++ from it. The C++ language contains all necessary abstractions, data types and containers. The wish list should be as short as possible, we should not orient ourselves on what would be possible with the respective frameworks, but always only on what we really need. This can be extended step by step later.

Thereby the various possibilities of the new standards C++11, C++14, C++17 and C++20 help of course considerably to write such a library. By the use of the Embarcadero C++Builder the use of C++20 is unfortunately not yet possible. Concepts would help to make the implementation more compact and robust. Fortunately, however, with the type-treats of C++11 and the possibilities of the compiletime commands of C++17, there are precursors for these that help us with the implementation.

The project was not about an extensive and complex user interface, but about the use of a few components and features. It was to be shown, how this can be modeled and finally the implementation is possible. Beside the direct access to the components (reading and writing) also C++ Streams were used, in order to arrange expenditures independently of the framework. With these not only text fields (for the VCL Memofelder) can be described, are also tables to be filled. The ListView component was used for the VCL, a StringGrid for FMX and a TableWidget for Qt. 



Copyright 2022 adecc Systemhaus GmbH
Author Volker Hillmann

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
