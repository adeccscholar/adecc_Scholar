#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <chrono>
#include <ctime>
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 202002L) || __cplusplus >= 202002L)
#include <format>
#endif

std::string TimeStamp(void) {
   #if ((defined(_MSVC_LANG) && _MSVC_LANG >= 202002L) || __cplusplus >= 202002L)
   auto const time = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
   return std::format("{:%d.%m.%Y %X}", time);
   #else
   std::ostringstream os;
   const std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
   os << std::put_time(std::localtime(&time), "%d.%m.%Y %X");
   return os.str();
   #endif
   }

class TMyLogger {
   private:
      static inline std::string strLogFile = "mylog.txt";
      std::ostringstream ostr;
      std::string strTime;
      std::string strFunc;
      std::string strFile;
      int         iRow;
   public:
      TMyLogger() = delete;
      TMyLogger(std::string const& func, std::string const& file, int r) : strFunc(func), strFile(file), iRow(r) {
         strTime = TimeStamp();
         }

      TMyLogger(TMyLogger const&) = delete;
      ~TMyLogger() = default;

      static void LogFile(std::string const& file) { strLogFile = file; }

      void set(std::string const& func, std::string const& file, int r) {
         strTime = TimeStamp();
         strFunc = func;
         strFile = file;
         iRow    = r;
         }

      std::ostream& stream() { return ostr; }

      void Write(std::ostream& out) {
         out << "[" << strTime << "] " << ostr.str()
             << " [ Function: \"" << strFunc << "\" in file \"" << strFile
             << "\" in line " << iRow << "]" << std::endl;
         }

      void Write(void) {
         ofstream of(strLogFile, std::ios::app);
         Write(of);
         }

      void except() {
         std::ostringstream os;
         os << "error at " << strTime << std::endl
            << ostr.str() << std::endl
            << "[ Function: \"" << strFunc << "\" in file \"" << strFile
            << "\" in line " << iRow << "]";
         throw std::runtime_error(os.str());
         }

   };

