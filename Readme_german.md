
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


