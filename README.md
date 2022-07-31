# adecc_Scholar

In this archive the sources from the course are shared on the channel "adecc Scholar". I have put them under MIT license, so the sources can be used by everyone without any restrictions. It should be noted that these are educational examples, so we do not provide any warranty. The goal was not the safe productive use, but to impart knowledge.

Some of the classes are not complete, but only hint at the learning content. 

To use the headeronly library from the semester break, one of the following constants must be defined
- BUILD_WITH_VCL, C++Builder or RAD Studio 10.3 or higher with Windows application (VCL)
- BUILD_WITH_FMX, C++Builder or RAD Studio 10.3 or higher with platform independent application (FMX)
- BUILD_WITH_QT - for Visual Studio with Qt6 and with the Qt Creator

Translated with www.DeepL.com/Translator (free version)

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




In diesem Archiv werden die Quellen aus der Lehrveranstaltung auf dem Channel "adecc Scholar" geteilt. Ich habe diese unter MIT Lizenz gestellt, so sind die Quellen ohne Einschränkungen von jedem nutzbar. Dabei sollte bachtet werden, dass es sich um Schulungsbeispiele handelt, wir also keine Garantie übernehmen. Das Ziel war nicht die sichere produktive Nutzung, sondern das vermitteln von Wissen.

Einige der Klassen sind nicht vollständig, sondern deuten nur die Lerninhalte an. 

Um die Headeronly Bibliothek aus der Semesterpause zu nutzen, muss einer der folgenden Konstante definiert werden
- BUILD_WITH_VCL, C++Builder oder RAD Studio 10.3 oder höher mit Windows- Anwendung (VCL)
- BUILD_WITH_FMX, C++Builder oder RAD Studio 10.3 oder höher mit plattformunabhängiger Anwendung (FMX)
- BUILD_WITH_QT - für Visual Studio mit Qt6 und mit dem Qt Creator

Fragen und Anregungen bitte über den Discord Server für adecc Scholar

# Von der Wunschliste zum unabhängiggen Programm

Auf Veranstaltungen und Schulungen haben wir oft behauptet, dass es einfach möglich ist, mit einer Wunschliste zu beginnen und daraus ein plattformunabhängiges Programm in C++ zu entwerfen. Dabei sollte der Beginn sofort möglich sein und in jedes Projekt unkompliziert integrierbar sein, um nicht immer wieder auf einer grünen Wiese beginnen zu müssen. Dieses hatten wir im Ansatz mit den beiden Bibliotheken von Embarcadero, der VCL auf der einen und Firemonkey (FMX) auf der anderen Seite auch schon gezeigt. Nun basieren diese beiden Bibliotheken auf derselben, in Delphi geschriebenen Basis, und sind strukturell ähnlich aufgebaut. Kann man das also überhaupt  verallgemeinern?

In der Semesterpause zwischen unseren Vorlesungen auf der Plattform adecc Scholar haben wir in einem Semesterprojekt dieses von Beginn an wiederholt und schließlich auf die unabhängige Oberflächenbibliothek Qt und Visual Studio 2019 als Entwicklungsplattform erweitert.

Damit ist die Frage auch schon beantworten, natürlich ist es möglich mit einer Schnittstelle, nennen wir sie jetzt einfach mal Wunschliste, zu beginnen, und daraus eine unabhängige Lösung für Standard C++ zu schaffen. Die Sprache C++ enthält alle notwendigen Abstraktionen, Datentypen und Container. Dabei sollte die Wunschliste möglichst kurz sein, wir sollten uns nicht an dem orientieren, was mit den jeweiligen Frameworks möglich wäre, sondern immer nur an dem, was wir wirklich brauchen. Dieses kann später schrittweise erweitert werden.

Dabei helfen die vielfältigen Möglichkeiten der neuen Standards C++11, C++14, C++17 und C++20 natürlich erheblich, so eine Bibliothek zu schreiben. Durch die Verwendung des Embarcadero C++Builder ist die Nutzung von C++20 aber leider noch nicht möglich. Dabei würden Concepts helfen, die Umsetzung kompakter und robuster zu gestalten. Glücklicherweise gibt es aber mit den Type-treats von C++11 und den Möglichkeiten der compiletime Befehle von C++17 Vorstufen für diese, die uns bei der Umsetzung helfen.

Bei dem Projekt ging es nicht um eine umfangreiche und komplexe Oberfläche, sondern um die Nutzung weniger Komponenten und Eigenschaften. Dabei war sollte gezeigt werden, wie dieses modelliert werden kann und letztendlich die Implementierung möglich ist. Neben dem direkten Zugriff auf die Komponenten (Lesen und Schreiben) wurden auch C++ Streams verwendet, um Ausgaben vom Framework unabhängig zu gestalten. Mit diesen können nicht nur Textfelder (für die VCL Memofelder) beschrieben werden, sollen auch Tabellen gefüllt werden. Dabei wurde für die VCL die ListView- Komponente, für FMX ein StringGrid und für Qt ein TableWidget verwendet. 


