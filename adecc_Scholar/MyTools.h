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
      static std::string trim_left(std::string && strVal, std::string const& strDelimiter = " ") {
         strVal.erase(0, strVal.find_first_not_of(strDelimiter));
         return strVal;
         }

      //------------------------------------------------------------------------
      static std::string trim_right(std::string && strVal, std::string const& strDelimiter = " ") {
         strVal.erase(strVal.find_last_not_of(strDelimiter), strVal.length());
         return strVal;
         }

      //------------------------------------------------------------------------
      static std::string trim(std::string && strVal, std::string const& strDelimiter = " ") {
         strVal.erase(0, strVal.find_first_not_of(strDelimiter));
         strVal.erase(strVal.find_last_not_of(strDelimiter) + 1, strVal.length());
         return strVal;
         }

      static std::string trim_left(std::string const& strText, std::string const& strDelimiter = " ") {
         std::string strVal(strText);
		 return trim_left(std::forward<std::string>(strVal))
         }

      //------------------------------------------------------------------------
      static std::string trim_right(std::string const& strText, std::string const& strDelimiter = " ") {
         std::string strVal(strText);
         strVal.erase(strVal.find_last_not_of(strDelimiter), strVal.length());
         return trim_right(std::forward<std::string>(strVal))
         }

      //------------------------------------------------------------------------
      static std::string trim(std::string const& strText, std::string const& strDelimiter = " ") {
         std::string strVal(strText);
         return trim(std::forward<std::string>(strVal))
         }

      /// \}

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

      static bool wildcard_matching(std::string const& text, std::string const& pattern) {
         std::string strTemp = "^" + pattern + "$";
         replace_all(strTemp, "?", ".");
         replace_all(strTemp, "*", ".*?");
         std::regex parser(strTemp);
         if (std::regex_match(text, parser)) return true;
         else return false;
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

      /// \}

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
         std::string src = strSrc;
         for(auto it = src.find(cThousand); it != std::string::npos; it = src.find(cThousand)) src.erase(it, 1);
         return string_to_integral<ty>(std::forward<std::string>(src));
         }

      /// \}

      //------------------------------------------------------------------------
      static void check_or_exception(std::errc ec) {
         if(ec == std::errc()) return;
         if(auto it = errc_messages.find(ec); it != errc_messages.end())
            throw std::runtime_error(it->second);
         else
            throw std::runtime_error("unexpected error");
         }

   private:
      static inline std::string::value_type cScale    = ',';
      static inline std::string::value_type cThousand = '.';
      static inline std::map<std::errc, std::string> errc_messages = {
            { std::errc::address_family_not_supported,        "address_family_not_supported" },
            { std::errc::address_in_use,                      "address_in_use" },
            { std::errc::address_not_available,               "address_not_available" },
            { std::errc::already_connected,                   "already_connected" },
            { std::errc::argument_list_too_long,              "argument_list_too_long" },
            { std::errc::argument_out_of_domain,              "argument_out_of_domain" },
            { std::errc::bad_address,                         "bad_address" },
            { std::errc::bad_file_descriptor,                 "bad_file_descriptor" },
            { std::errc::bad_message,                         "bad_message" },
            { std::errc::broken_pipe,                         "broken_pipe" },
            { std::errc::connection_aborted,                  "connection_aborted" },
            { std::errc::connection_already_in_progress,      "connection_already_in_progress" },
            { std::errc::connection_refused,                  "connection_refused" },
            { std::errc::connection_reset,                    "connection_reset" },
            { std::errc::cross_device_link,                   "cross_device_link" },
            { std::errc::destination_address_required,        "destination_address_required" },
            { std::errc::device_or_resource_busy,             "device_or_resource_busy" },
            { std::errc::directory_not_empty,                 "directory_not_empty" },
            { std::errc::executable_format_error,             "executable_format_error" },
            { std::errc::file_exists,                         "file_exists" },
            { std::errc::file_too_large,                      "file_too_large" },
            { std::errc::filename_too_long,                   "filename_too_long" },
            { std::errc::function_not_supported,              "function_not_supported" },
            { std::errc::host_unreachable,                    "host_unreachable" },
            { std::errc::identifier_removed,                  "identifier_removed" },
            { std::errc::illegal_byte_sequence,               "illegal_byte_sequence" },
            { std::errc::inappropriate_io_control_operation,  "inappropriate_io_control_operation" },
            { std::errc::interrupted,                         "interrupted" },
            { std::errc::invalid_argument,                    "invalid_argument" },
            { std::errc::invalid_seek,                        "invalid_seek" },
            { std::errc::io_error,                            "io_error" },
            { std::errc::is_a_directory,                      "is_a_directory" },
            { std::errc::message_size,                        "message_size" },
            { std::errc::network_down,                        "network_down" },
            { std::errc::network_reset,                       "network_reset" },
            { std::errc::network_unreachable,                 "network_unreachable" },
            { std::errc::no_buffer_space,                     "no_buffer_space" },
            { std::errc::no_child_process,                    "no_child_process" },
            { std::errc::no_link,                             "no_link" },
            { std::errc::no_lock_available,                   "no_lock_available" },
            { std::errc::no_message_available,                "no_message_available" },
            { std::errc::no_message,                          "no_message" },
            { std::errc::no_protocol_option,                  "no_protocol_option" },
            { std::errc::no_space_on_device,                  "no_space_on_device" },
            { std::errc::no_stream_resources,                 "no_stream_resources" },
            { std::errc::no_such_device_or_address,           "no_such_device_or_address" },
            { std::errc::no_such_device,                      "no_such_device" },
            { std::errc::no_such_file_or_directory,           "no_such_file_or_directory" },
            { std::errc::no_such_process,                     "no_such_process" },
            { std::errc::not_a_directory,                     "not_a_directory" },
            { std::errc::not_a_socket,                        "not_a_socket" },
            { std::errc::not_a_stream,                        "not_a_stream" },
            { std::errc::not_connected,                       "not_connected" },
            { std::errc::not_enough_memory,                   "not_enough_memory" },
            { std::errc::not_supported,                       "not_supported" },
            { std::errc::operation_canceled,                  "operation_canceled" },
            { std::errc::operation_in_progress,               "operation_in_progress" },
            { std::errc::operation_not_permitted,             "operation_not_permitted" },
            { std::errc::operation_not_supported,             "operation_not_supported" },
            { std::errc::operation_would_block,               "operation_would_block" },
            { std::errc::owner_dead,                          "owner_dead" },
            { std::errc::permission_denied,                   "permission_denied" },
            { std::errc::protocol_error,                      "protocol_error" },
            { std::errc::protocol_not_supported,              "protocol_not_supported" },
            { std::errc::read_only_file_system,               "read_only_file_system" },
            { std::errc::resource_deadlock_would_occur,       "resource_deadlock_would_occur" },
            { std::errc::resource_unavailable_try_again,      "resource_unavailable_try_again" },
            { std::errc::result_out_of_range,                 "result_out_of_range" },
            { std::errc::state_not_recoverable,               "state_not_recoverable" },
            { std::errc::stream_timeout,                      "stream_timeout" },
            { std::errc::text_file_busy,                      "text_file_busy" },
            { std::errc::timed_out,                           "timed_out" },
            { std::errc::too_many_files_open_in_system,       "too_many_files_open_in_system" },
            { std::errc::too_many_files_open,                 "too_many_files_open" },
            { std::errc::too_many_links,                      "too_many_links" },
            { std::errc::too_many_symbolic_link_levels,       "too_many_symbolic_link_levels" },
            { std::errc::value_too_large,                     "value_too_large" },
            { std::errc::wrong_protocol_type,                 "wrong_protocol_type" } };

   };


#endif


