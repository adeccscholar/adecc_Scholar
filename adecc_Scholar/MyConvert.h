#ifndef MyConvertH
#define MyConvertH

#include <string>
#include <cstdlib>
#include <cwchar>

#if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
#include <System.Classes.hpp>
#elif defined BUILD_WITH_QT
#include <QString>
#endif

/*
std::wstring ws(s.size(), L' '); // Overestimate number of code points.
ws.resize(::mbstowcs_s(&ws[0], ws.size(), s.c_str(), s.size())); // Shrink to fit
*/

class TMyConvert {
   public:
      static void Convert(wchar_t const* source, std::string& target) {
         size_t origsize  = wcslen(source);
         target.resize(origsize);

         for(size_t converted = 0; converted < origsize; ++converted) {
	         char szBuffer[50] = "";
	         int iCnt = wctomb(szBuffer, source[converted]);
	         if(iCnt != 1) {
		         // spezifische Codierung notwendig
		         target[converted] = '?';
		         }
	         else
		      target[converted] = szBuffer[0];
	         }
         }

      static void Convert(std::wstring const& source, std::string& target) {
         Convert(source.c_str(), target);
         }


	  #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX

	  static void Convert(std::wstring const& source, AnsiString& target) {
		 std::string s("");
		 Convert(source, s);
		 target = s.c_str();
		 }

      static void Convert(std::wstring const& source, String& target) {
		 target = source.c_str();
		 }

	  static void Convert(String const& source, std::string& target) {
		 Convert(source.c_str(), target);
		 }

      static void Convert(String const& source, AnsiString& target) {
		  std::string s("");
		  target = s.c_str();
		  }

      #elif defined BUILD_WITH_QT

      static void Convert(std::wstring const& source, QString& target);
      static void Convert(std::string const& source,  QString& target);
      static void Convert(QString const& source, std::wstring& target);
      static void Convert(QString const& source, std::string& target);
	  #endif



};

#endif