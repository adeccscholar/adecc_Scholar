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

#if defined BUILD_WITH_QT
#include <QProcess>
#endif


class TMyLogger {
   private:
      static inline std::string strLogFile = "mylog.txt";
      std::ostringstream ostr;
      std::chrono::time_point<std::chrono::system_clock> time_p;
      std::string strFunc;
      std::string strFile;
      std::string strUser;
      int         iRow;
   public:
      TMyLogger() = delete;
      TMyLogger(std::string const& func, std::string const& file, int r) : strFunc(func), strFile(file), iRow(r) {
         #if defined BUILD_WITH_QT
         QProcess process;
         process.setProgram("whoami");
         process.start();
         while (process.state() != QProcess::NotRunning)
            qApp->processEvents();
         strUser = process.readAll().toStdString();
         #else
         strUser = "";
         #endif
         time_p = std::chrono::system_clock::now();
         }

      TMyLogger(TMyLogger const&) = delete;
      ~TMyLogger() = default;

      static void LogFile(std::string const& file) { strLogFile = file; }

      void set(std::string const& func, std::string const& file, int r) {
         time_p = std::chrono::system_clock::now();
         strFunc = func;
         strFile = file;
         iRow    = r;
         }

      std::ostream& stream() { return ostr; }

      void Write(std::ostream& out) {
         out << "[" << TimeStamp(time_p) << "] " << ostr.str()
             << " [ Function: \"" << strFunc << "\" in file \"" << strFile
             << "\" in line " << iRow << "]" << std::endl;
         }

      void Write(void) {
         std::ofstream of(strLogFile, std::ios::app);
         Write(of);
         }

      
      void except() {
         std::ostringstream os;
         os << "error at " << TimeStamp(time_p) << std::endl
            << ostr.str() << std::endl
            << "[ Function: \"" << strFunc << "\" in file \"" << strFile
            << "\" in line " << iRow << "]" << std::endl << strUser;
         throw std::runtime_error(os.str());
         }

      static std::string TimeStamp(std::chrono::time_point<std::chrono::system_clock> const& now) {
         const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
         #if ((defined(_MSVC_LANG) && _MSVC_LANG >= 202002L) || __cplusplus >= 202002L)
         auto const timew = std::chrono::current_zone()->to_local(now);
         return std::format(L"{:%d.%m.%Y %X},{:03d}", timew, millis.count());
         #else
         std::ostringstream os;
         const std::time_t timew = std::chrono::system_clock::to_time_t(now);
         os << std::put_time(std::localtime(&timew), "%d.%m.%Y %X");
         os << "," << std::setfill('0') << std::setw(3) << millis.count();
         return os.str();
         #endif
      }

   };

