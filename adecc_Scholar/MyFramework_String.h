#pragma once

#include "MyTools.h"
#include "MyType_Traits.h"

#include <string>
#include <optional>
#include <iostream>
#include <iomanip>

#if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
   #include <System.Classes.hpp>
   using fw_String = UnicodeString;
#elif defined BUILD_WITH_QT
   #include <QString>
   using fw_String = QString;
#endif


class TMy_FW_String {
public:
   template <typename ty>
   static ty GetText(fw_String const& value) {
      #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
         if constexpr (std::is_same<std::wstring, ty>::value)                   return value.c_str();
         else if constexpr (is_delphi_string<ty>::value)                        return value;
      #elif defined BUILD_WITH_QT
         if constexpr (std::is_same<std::wstring, ty>::value)                   return value.toStdWString();
         else if constexpr (is_qt_string<ty>::value)                            return value;
      #else
         #error fehlende Implementierung for GetText in diesem Framwork
      #endif
         else {
      #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
         std::string strValue = AnsiString(value).c_str();
      #elif defined BUILD_WITH_QT
         std::string strValue = value.toStdString();
      #else
         #error Missing implementation for function TMyForm::GetText() for the chosen framework
      #endif
      if constexpr (std::is_integral<ty>::value && !std::is_same<ty, bool>::value) return TMyTools::string_to_integral_fmt<ty>(strValue);
      else if constexpr (std::is_floating_point<ty>::value)                        return TMyTools::string_to_double_fmt(strValue);
      else if constexpr (std::is_same<std::string, ty>::value)                     return strValue;
      else {
         ty ret;
         std::istringstream ins(strValue);
         ins >> ret;
         return ret;
         }
      }
   }

   template <typename ty>
   static fw_String SetText(ty const& value, int iLen = -1, int iScale = -1) {
      fw_String retVal("");
      #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
         auto convert_string = [](std::string const& strValue) { return fw_String(strValue.c_str());  };
         auto convert_wstring = [](std::wstring const& strValue) { return fw_String(strValue.c_str()); };
      #elif defined BUILD_WITH_QT
         auto convert_string = [](std::string const& strValue) { return QString::fromStdString(strValue); };
         auto convert_wstring = [](std::wstring const& strValue) { return QString::fromStdWString(strValue); };
      #else
         #error Missing implementation for function TMyForm::SetFunction() for the chosen framework
      #endif

      if constexpr (is_optional<ty>::value) {
         using used_type = typename std::remove_reference<typename std::remove_cv<decltype(*value)>::type>::type;
         if (!value.has_value())  retVal = "";
         else {
            if constexpr (is_cpp_narrow_string<used_type>::value)          retVal = convert_string(*value);
            else if constexpr (is_cpp_wide_string<used_type>::value)       retVal = convert_wstring(*value);
            else if constexpr (is_delphi_string<used_type>::value)         retVal = *value;
            else if constexpr (is_qt_string<used_type>::value)             retVal = *value;
            else if constexpr (is_wchar_param<used_type>::value)           retVal = *value;
            else if constexpr (is_char_param<used_type>::value)            retVal = *value;
            else if constexpr (std::is_integral<used_type>::value && !std::is_same<used_type, bool>::value) {
               retVal = convert_string(TMyTools::integral_to_string_fmt<used_type>(*value));
            }
            else if constexpr (std::is_floating_point<used_type>::value) {
               retVal = convert_string(TMyTools::double_to_string_fmt(*value, iScale));
            }
            else {
               std::ostringstream os;
               if (iLen >= 0)   os << std::setw(iLen);
               if (iScale >= 0) {
                  os.setf(std::ios::showpoint);
                  os.setf(std::ios::fixed);
                  os << std::setprecision(iScale);
               }
               os << *value;
               retVal = convert_string(os.str());
            }
         }
      }
      else {
         using used_type = typename std::remove_reference<typename std::remove_cv<ty>::type>::type;;
         if constexpr (is_cpp_narrow_string<used_type>::value)                 retVal = convert_string(value);
         else if constexpr (is_cpp_wide_string<used_type>::value)              retVal = convert_wstring(value);
         else if constexpr (is_delphi_string<used_type>::value)                retVal = value;
         else if constexpr (is_qt_string<used_type>::value)                    retVal = value;
         else if constexpr (is_wchar_param<used_type>::value)                  retVal = value;
         else if constexpr (is_char_param<used_type>::value)                   retVal = value;
         else if constexpr (std::is_integral<used_type>::value && !std::is_same<used_type, bool>::value) {
            retVal = convert_string(TMyTools::integral_to_string_fmt<ty>(value));
         }
         else if constexpr (std::is_floating_point<used_type>::value) {
            retVal = convert_string(TMyTools::double_to_string_fmt(value, iScale));
         }
         else {
            std::ostringstream os;
            if (iLen >= 0)   os << std::setw(iLen);
            if (iScale >= 0) {
               os.setf(std::ios::showpoint);
               os.setf(std::ios::fixed);
               os << std::setprecision(iScale);
            }
            os << value;
            retVal = convert_string(os.str());
         }
      }
      return retVal;
   }


};