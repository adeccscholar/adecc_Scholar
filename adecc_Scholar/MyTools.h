/** \file
\brief   Hilsklasse zum Arbeiten mit std::string
\details Umsetzung von einfachen Operationen mit std::string mit Hilfe der
         C++ Standard- Bibliothek. Die Umsetzung erfolgt es Header- Only
         Bibliothek in einer Tool- Class mit statischen Funktionen.
<hr>
$LastChangedDate: $
$Revision: $
$HeadURL: $
<hr>
\author Volker Hillmann
\date    05.03.2022
\version 1.0
\since   Version 1.0
\copyright Copyright ©2022 adecc Systemhaus GmbH, Siehe Lizenzbedingungen.
*/
#ifndef MyToolsH
#define MyToolsH

#include <string>
#include <map>
#include <array>
#include <sstream>
#include <algorithm>
#include <regex>
#include <charconv>
#include <system_error>

using namespace std::literals::string_literals;

class TMyGuard {
   private:
      std::string strMessage;
      bool boActive = false;
   public:
      TMyGuard(void) = delete;
      TMyGuard(TMyGuard const&) = delete;
      TMyGuard(TMyGuard&&) = delete;
      
      TMyGuard(std::string const& msg) : strMessage(msg) { }
      virtual ~TMyGuard() { }

      void               Active() { boActive = true; }

      std::string const& Message() const { return strMessage; }
      bool               IsActive() const { return boActive; }
};

class TMyToggle : TMyGuard {
   private:
      bool &boRunning;
   public:
      TMyToggle(void) = delete;
      TMyToggle(TMyToggle const&) = delete;
      TMyToggle(TMyToggle&&) = delete;
      TMyToggle(std::string const& msg, bool& boRef) : TMyGuard(msg), boRunning(boRef) { 
         if(boRunning) throw std::runtime_error(Message() + ", value is true");
         Active();
         }
      ~TMyToggle() {
         if(IsActive()) boRunning = false;
         }
   };



/** \class
\brief Tool- Klasse mit Hilfsfunktionen für std::string
\details Tool- Klasse mit Hilfsfunktionen zum Bearbeiten von std::string. Neben
         Methoden zum Trimmen von Zeichenketten, zum Umwandeln in Klein- oder
         Großbuchstaben, gehören von Konvertierungen dazu.
\date    05.03.2022
\version 1.0
\since   Version 1.0
*/
class TMyTools {
     template<typename ty>
     struct dependent_false : std::false_type {};
   public:
      //------------------------------------------------------------------------
      /** \name Methoden zum Trimmen von std::string
         \{
      */

      //------------------------------------------------------------------------
      static std::string trim_left(std::string && strVal, std::string const& strDelimiter = " "s) {
         strVal.erase(0, strVal.find_first_not_of(strDelimiter));
         return strVal;
         }

      //------------------------------------------------------------------------
      static std::string trim_right(std::string && strVal, std::string const& strDelimiter = " "s) {
         strVal.erase(strVal.find_last_not_of(strDelimiter) + 1, strVal.length());
         return strVal;
         }

      //------------------------------------------------------------------------
      static std::string trim(std::string && strVal, std::string const& strDelimiter = " "s) {
         strVal.erase(0, strVal.find_first_not_of(strDelimiter));
         strVal.erase(strVal.find_last_not_of(strDelimiter) + 1, strVal.length());
         return strVal;
         }

      static std::string trim_left(std::string const& strText, std::string const& strDelimiter = " "s) {
         std::string strVal(strText);
		 return trim_left(std::forward<std::string>(strVal), strDelimiter);
         }

      //------------------------------------------------------------------------
      static std::string trim_right(std::string const& strText, std::string const& strDelimiter = " "s) {
         std::string strVal(strText);
         return trim_right(std::forward<std::string>(strVal), strDelimiter);
         }

      //------------------------------------------------------------------------
      static std::string trim(std::string const& strText, std::string const& strDelimiter = " "s) {
         std::string strVal(strText);
         return trim(std::forward<std::string>(strVal), strDelimiter);
         }

      /// \}


      static int tokenize(std::string const& strInput, std::string const& strDelimiter, std::vector<std::string>& vecData, int iSize = -1) {
         int iCount = 0;
         vecData.clear();
         if (iSize >= 0) vecData.reserve(iSize);
         size_t iPos = 0, iEnd;
         do {
            iEnd = strInput.find(strDelimiter, iPos);
            std::string strPart = strInput.substr(iPos, iEnd - iPos);
            vecData.emplace_back(std::forward<std::string>(strPart));
            ++iCount;
            iPos = iEnd + strDelimiter.length();
         } while (iEnd != std::string::npos);
         return iCount;
      }

      static int tokenize(std::string const& strInput, std::vector<int> const& grouping, std::vector<std::string>& vecData, int iSize = -1) {
         auto it = grouping.cbegin();
         int iStart = 0;
         int iCount = 0;
         vecData.clear();
         if (iSize >= 0) vecData.reserve(iSize);
         while (it != grouping.end()) {
            std::string strPart = strInput.substr(iStart, *it);
            vecData.emplace_back(std::forward<std::string>(strPart));
            iStart += *it;
            ++iCount;
            ++it;
         }
         return iCount;
      }


      //------------------------------------------------------------------------
      static std::string clean(std::string const& strText, char cDelimiter = ' ') {
         std::string src(strText);
         for(auto it = src.find(cDelimiter); it != std::string::npos; it = src.find(cDelimiter))
            src.erase(it, 1);
         return src;
         }

      //------------------------------------------------------------------------
      static bool contain(std::string const& strFile, std::string const& strPlaceholder) {
         return strFile.find_first_of(strPlaceholder) != std::string::npos ? true : false;
         }

      //------------------------------------------------------------------------
      static bool contain_only(std::string const& strFile, std::string const& strPlaceholder) {
         return strFile.find_first_not_of(strPlaceholder) != std::string::npos ? false : true;
         }

      //------------------------------------------------------------------------
      static std::string lower(std::string const& strInput) {
         std::string src(strInput);
         std::transform(src.begin(), src.end(), src.begin(), [](auto val) {  return tolower(val); } );
	      return src;
         }

      //------------------------------------------------------------------------
      static std::string upper(std::string const& strInput) {
         std::string src(strInput);
         std::transform(src.begin(), src.end(), src.begin(), [](auto val) { return toupper(val); });
	      return src;
         }


      //------------------------------------------------------------------------
      /** \name Methoden zum Konvertieren von double to std::string und umgekehrt
         \{
      */
      static std::string double_to_string(double const& value, int iPrec) {
	     std::array<char, 25> tgt;
	     auto [ptr, ec] = std::to_chars(tgt.data(), tgt.data() + tgt.size(), value, std::chars_format::fixed, iPrec);
         check_or_exception(ec);
	     return std::string(tgt.data(), ptr);
         }

      //------------------------------------------------------------------------
      static std::string double_to_string_fmt(double const& value, int iPrec) {
	     std::array<char, 25> tgt;
	     auto [ptr, ec] = std::to_chars(tgt.data(), tgt.data() + tgt.size(), value, std::chars_format::fixed, iPrec);
         check_or_exception(ec);
	     auto itPoint = std::find(tgt.begin(), tgt.end(), '.');
	     if(itPoint != tgt.end()) *itPoint = ',';
	     auto iLen = itPoint - tgt.begin();
	     auto d = std::div((int)iLen, 3);
	     if (d.quot > 0) {
		    std::string src(tgt.data(), ptr);
		    auto start = d.rem == 0 ? 3 : d.rem;
		    for (auto a = start == 3 ? 1 : 0; a < d.quot; ++a) {
		       src.insert(src.begin() + start, '.');
		       start += 4;
		       }
		    return src;
	        }
	     return std::string ( tgt.data(), ptr );
         }

      static std::string replace_all(std::string const& strSource, std::string const& strFrom, std::string const& strTo) {
         std::string strText = strSource;
         size_t start = 0;
         while ((start = strText.find(strFrom, start)) != std::string::npos) {
            strText.replace(start, strFrom.length(), strTo);
            start += strTo.length(); 
         }
         return strText;
      }

      // Optimierung von replace
      static std::string&& replace_all(std::string &&strSource, std::string const& strFrom, std::string const& strTo) {
         size_t start = 0;
         while ((start = strSource.find(strFrom, start)) != std::string::npos) {
            strSource.replace(start, strFrom.length(), strTo);
            start += strTo.length();
         }
         return std::move(strSource);
      }

      static bool wildcard_matching(std::string const& text, std::string const& pattern, bool boCaseSensitive = false) {
         std::string strTemp = replace_all(replace_all(replace_all("^"s + pattern + "$"s, "."s, "\\."s), "?"s, "."s), "*"s, ".*?"s);
         if(!boCaseSensitive) {
            if (std::regex_match(text, std::regex { strTemp, std::regex_constants::icase })) return true;
            }
         else {
            if (std::regex_match(text, std::regex { strTemp } )) return true;
            }
         return false;
         }

      //------------------------------------------------------------------------
      static double string_to_double(std::string&& strSrc) {
         static const std::regex parser("^[\\+|-]?(0|([1-9][0-9]*))(\\.[0-9]+)?$");
         std::string src = trim(std::forward<std::string>(strSrc));
         if(!std::regex_match(src, parser)) {
            throw std::runtime_error(src + " can't converted to a double value");
            }
         size_t pos;
         double flRet = std::stod(src, &pos);
         if(pos < src.length()) throw std::runtime_error(src + " can't converted to a double value");
         return flRet;
         }

      //------------------------------------------------------------------------
      static double string_to_double(std::string const& strSrc) {
         std::string src = strSrc;
         return string_to_double(std::forward<std::string>(src));
         }

      //------------------------------------------------------------------------
      static double string_to_double_fmt(std::string const& strSrc) {
         std::string src(strSrc);
         for(auto it = src.find(cThousand); it != std::string::npos; it = src.find(cThousand)) src.erase(it, 1);
         if(auto it = src.find(cScale); it != std::string::npos) src[it] = '.';
         return string_to_double(std::forward<std::string>(src));
         }
      /// \}
      //------------------------------------------------------------------------
      /** \name Methoden zum Konvertieren von Ganzzahlen to std::string und umgekehrt
         \{
      */
      template <typename ty>
      static std::string integral_to_string(ty const& value) {
	     std::array<char, 25> tgt;
         if constexpr (std::is_integral<ty>::value && !std::is_same<ty, bool>::value) {
	        auto [ptr, ec] = std::to_chars(tgt.data(), tgt.data() + tgt.size(), value);
            check_or_exception(ec);
	        return std::string(tgt.data(), ptr);
            }
         else {
            static_assert(dependent_false<ty>::value, "integral to string for this type not implemented.!");
            }
         }

      //------------------------------------------------------------------------
      template <typename ty>
      static std::string integral_to_string_fmt(ty const& value) {
         std::string ret = integral_to_string<ty>(value);
	     auto iLen = ret.length();
	     auto d = std::div(iLen, 3);
	     if (d.quot > 0) {
		    auto start = d.rem == 0 ? 3 : d.rem;
		    for (auto a = start == 3 ? 1 : 0; a < d.quot; ++a) {
		       ret.insert(ret.begin() + start, '.');
		       start += 4;
		       }
		    return ret;
	        }
	     return ret;
         }
      /// \}
      //------------------------------------------------------------------------
      /** \name Hilfsmethoden zum eingeben und ausgeben von Tausender- Trennen
         \{
      */
      static std::string&& integral_double_clean_Thousand(std::string && src) {
         for(auto it = src.find(cThousand); it != std::string::npos; it = src.find(cThousand)) src.erase(it, 1);
         return std::move(src);
         }

      static std::string integral_double_clean_Thousand(std::string const& strSrc) {
         std::string src(strSrc);
         return integral_double_clean_Thousand(std::forward<std::string>(src));
         }

      static std::string&& integral_double_add_Thousand(std::string && src) {
         src = integral_double_clean_Thousand(std::forward<std::string>(src));

         auto iLen = src.find(cThousand);
	     if(iLen == std::string::npos) iLen = src.length();
	     auto d = std::div(iLen, 3);
	     if (d.quot > 0) {
		    auto start = d.rem == 0 ? 3 : d.rem;
		    for (auto a = start == 3 ? 1 : 0; a < d.quot; ++a) {
		       src.insert(src.begin() + start, '.');
		       start += 4;
		       }
		    }
	     return std::move(src);
         }

      static std::string integral_double_add_Thousand(std::string const& strSrc) {
         std::string src(strSrc);
         return integral_double_add_Thousand(std::forward<std::string>(src));
         }

      //------------------------------------------------------------------------
      template <typename ty>
      static ty string_to_integral(std::string && strSrc) {
         static const std::regex parser("^[\\+|-]?(0|([1-9][0-9]*))$");
         std::string src = trim(std::forward<std::string>(strSrc));
         if(!std::regex_match(src, parser)) {
            throw std::runtime_error(src + " can't converted to an integral value");
            }
         size_t pos;
         using used_type  = typename std::remove_reference<typename std::remove_cv<ty>::type>::type;

         used_type return_value;
         if constexpr (std::is_same<int, used_type>::value)                    return_value = std::stoi(src, &pos);
         else if constexpr(std::is_same<unsigned int, used_type>::value)       return_value = (unsigned int)std::stol(src, &pos);
         else if constexpr(std::is_same<short, used_type>::value)              return_value = (short)std::stoi(src, &pos);
         else if constexpr(std::is_same<unsigned short, used_type>::value)     return_value = (unsigned short)std::stoi(src, &pos);
         else if constexpr(std::is_same<long, used_type>::value)               return_value = std::stol(src, &pos);
         else if constexpr(std::is_same<unsigned long, used_type>::value)      return_value = std::stoul(src, &pos);
         else if constexpr(std::is_same<long long, used_type>::value)          return_value = std::stoll(src, &pos);
         else if constexpr(std::is_same<unsigned long long, used_type>::value) return_value = std::stoull(src, &pos);
         else {
            static_assert(dependent_false<ty>::value, "string_to_int for this type not implemented.!");
            }

         if(pos < src.length()) {
            std::ostringstream os;
            os << "Fehler beim Umwandeln zu einer Ganzzahl." << std::endl
               << "String: " << strSrc << ", an Position " << pos << " ungültig.";
            throw std::runtime_error(os.str());
            }
         return return_value;
         }


      //------------------------------------------------------------------------
      template <typename ty>
      static ty string_to_integral(std::string const& strSrc) {
         std::string src = strSrc;
         return string_to_integral<ty>(std::forward<std::string>(src));
         }

      //------------------------------------------------------------------------
      template <typename ty>
      static ty string_to_integral_fmt(std::string const& strSrc) {
         // add a static_assert(), type_traits for integral
         std::string src = strSrc;
         for(auto it = src.find(cThousand); it != std::string::npos; it = src.find(cThousand)) src.erase(it, 1);
         return string_to_integral<ty>(std::forward<std::string>(src));
         }

      /// \}

      //------------------------------------------------------------------------
      static void check_or_exception(std::errc ec) {
         if(ec == std::errc()) return;
         std::ostringstream os;
         os << "error with code " << std::make_error_code(ec) << ", ";
         if(auto it = errc_messages.find(ec); it != errc_messages.end())
            os << "(POSIX: " << it->second.first << "):" << it->second.second;
         else
            os << "unexpected error";
         throw std::runtime_error(os.str());
         }

   private:
      static inline std::string::value_type cScale    = ',';  ///< user defined scale for decimal numbers
      static inline std::string::value_type cThousand = '.';  ///< user defined thousand separator for decimal numbers
      /// informations about errors with key = std::errc and pair of strings with informations for this error
      static inline std::map<std::errc, std::pair<std::string, std::string>> errc_messages = {
            { std::errc::address_family_not_supported,       { "EAFNOSUPPORT"s,    "address family not supported"s } },
            { std::errc::address_in_use,                     { "EADDRINUSE"s,      "address in use"s } },
            { std::errc::address_not_available,              { "EADDRNOTAVAIL"s,   "address not available"s } },
            { std::errc::already_connected,                  { "EISCONN"s,         "already connected"s } },
            { std::errc::argument_list_too_long,             { "E2BIG"s,           "argument list too long"s } },
            { std::errc::argument_out_of_domain,             { "EDOM"s,            "argument out of domain"s } },
            { std::errc::bad_address,                        { "EFAULT"s,          "bad address"s } },
            { std::errc::bad_file_descriptor,                { "EBADF"s,           "bad file descriptor"s } },
            { std::errc::bad_message,                        { "EBADMSG"s,         "bad message"s } },
            { std::errc::broken_pipe,                        { "EPIPE"s,           "broken pipe"s } },
            { std::errc::connection_aborted,                 { "ECONNABORTED"s,    "connection aborted"s } },
            { std::errc::connection_already_in_progress,     { "EALREADY"s,        "connection already in progress"s } },
            { std::errc::connection_refused,                 { "ECONNREFUSED"s,    "connection refused"s } },
            { std::errc::connection_reset,                   { "ECONNRESET"s,      "connection reset"s } },
            { std::errc::cross_device_link,                  { "EXDEV"s,           "cross device link"s } },
            { std::errc::destination_address_required,       { "EDESTADDRREQ"s,    "destination address required"s } },
            { std::errc::device_or_resource_busy,            { "EBUSY"s,           "device or resource busy"s } },
            { std::errc::directory_not_empty,                { "ENOTEMPTY"s,       "directory not empty"s } },
            { std::errc::executable_format_error,            { "ENOEXEC"s,         "executable format error"s } },
            { std::errc::file_exists,                        { "EEXIST"s,          "file exists"s } },
            { std::errc::file_too_large,                     { "EFBIG"s,           "file too large"s } },
            { std::errc::filename_too_long,                  { "ENAMETOOLONG"s,    "filename too long"s } },
            { std::errc::function_not_supported,             { "ENOSYS"s,          "function not supported"s } },
            { std::errc::host_unreachable,                   { "EHOSTUNREACH"s,    "host unreachable"s } },
            { std::errc::identifier_removed,                 { "EIDRM"s,           "identifier removed"s } },
            { std::errc::illegal_byte_sequence,              { "EILSEQ"s,          "illegal byte sequence"s } },
            { std::errc::inappropriate_io_control_operation, { "ENOTTY"s,          "inappropriate io control operation"s } },
            { std::errc::interrupted,                        { "EINTR"s,           "interrupted"s } },
            { std::errc::invalid_argument,                   { "EINVAL"s,          "invalid argument"s } },
            { std::errc::invalid_seek,                       { "ESPIPE"s,          "invalid seek"s } },
            { std::errc::io_error,                           { "EIO"s,             "io error"s } },
            { std::errc::is_a_directory,                     { "EISDIR"s,          "is a directory"s } },
            { std::errc::message_size,                       { "EMSGSIZE"s,        "message size"s } },
            { std::errc::network_down,                       { "ENETDOWN"s,        "network down"s } },
            { std::errc::network_reset,                      { "ENETRESET"s,       "network reset"s } },
            { std::errc::network_unreachable,                { "ENETUNREACH"s,     "network unreachable"s } },
            { std::errc::no_buffer_space,                    { "ENOBUFS"s,         "nobufferspace"s } },
            { std::errc::no_child_process,                   { "ECHILD"s,          "no child process"s } },
            { std::errc::no_link,                            { "ENOLINK"s,         "no link"s } },
            { std::errc::no_lock_available,                  { "ENOLCK"s,          "no lock available"s } },
            { std::errc::no_message_available,               { "ENODATA"s,         "no message available"s } },
            { std::errc::no_message,                         { "ENOMSG"s,          "no message"s } },
            { std::errc::no_protocol_option,                 { "ENOPROTOOPT"s,     "no protocol option"s } },
            { std::errc::no_space_on_device,                 { "ENOSPC"s,          "no space on device"s } },
            { std::errc::no_stream_resources,                { "ENOSR"s,           "no stream_resources"s } },
            { std::errc::no_such_device_or_address,          { "ENXIO"s,           "no suc device or address"s } },
            { std::errc::no_such_device,                     { "ENODEV"s,          "no such device"s } },
            { std::errc::no_such_file_or_directory,          { "ENOENT"s,          "no such file or directory"s } },
            { std::errc::no_such_process,                    { "ESRCH"s,           "no such process"s } },
            { std::errc::not_a_directory,                    { "ENOTDIR"s,         "not a directory"s } },
            { std::errc::not_a_socket,                       { "ENOTSOCK"s,        "not a socket"s } },
            { std::errc::not_a_stream,                       { "ENOSTR"s,          "not a stream"s } },
            { std::errc::not_connected,                      { "ENOTCONN"s,        "not connected"s } },
            { std::errc::not_enough_memory,                  { "ENOMEM"s,          "not enough memory"s } },
            { std::errc::not_supported,                      { "ENOTSUP"s,         "not supported"s } },
            { std::errc::operation_canceled,                 { "ECANCELED"s,       "operation canceled"s } },
            { std::errc::operation_in_progress,              { "EINPROGRESS"s,     "operation in progress"s } },
            { std::errc::operation_not_permitted,            { "EPERM"s,           "operation not permitted"s } },
            { std::errc::operation_not_supported,            { "EOPNOTSUPP"s,      "operation not supported"s } },
            { std::errc::operation_would_block,              { "EWOULDBLOCK"s,     "operation would block"s } },
            { std::errc::owner_dead,                         { "EOWNERDEAD"s,      "owner dead"s } },
            { std::errc::permission_denied,                  { "EACCES"s,          "permission denied"s } },
            { std::errc::protocol_error,                     { "EPROTO"s,          "protocol error"s } },
            { std::errc::protocol_not_supported,             { "EPROTONOSUPPORT"s, "protocol not supported"s } },
            { std::errc::read_only_file_system,              { "EROFS"s,           "read only file system"s } },
            { std::errc::resource_deadlock_would_occur,      { "EDEADLK"s,         "resource deadlock would occur"s } },
            { std::errc::resource_unavailable_try_again,     { "EAGAIN"s,          "resource unavailable try again"s } },
            { std::errc::result_out_of_range,                { "ERANGE"s,          "result out of range"s } },
            { std::errc::state_not_recoverable,              { "ENOTRECOVERABLE"s, "state not recoverable"s } },
            { std::errc::stream_timeout,                     { "ETIME"s,           "stream timeout"s } },
            { std::errc::text_file_busy,                     { "ETXTBSY"s,         "text file busy"s } },
            { std::errc::timed_out,                          { "ETIMEDOUT"s,       "timed out"s } },
            { std::errc::too_many_files_open_in_system,      { "ENFILE"s,          "too many files open in system"s } },
            { std::errc::too_many_files_open,                { "EMFILE"s,          "too many files open"s } },
            { std::errc::too_many_links,                     { "EMLINK"s,          "too many links"s } },
            { std::errc::too_many_symbolic_link_levels,      { "ELOOP"s,           "too many symbolic link levels"s } },
            { std::errc::value_too_large,                    { "EOVERFLOW"s,       "value too large"s } },
            { std::errc::wrong_protocol_type,                { "EPROTOTYPE"s,      "wrong protocol type"s } } };
   };

#endif


