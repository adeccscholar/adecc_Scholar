#pragma once

#if defined BUILD_WITH_VCL || BUILD_WITH_FMX
#include <System.Classes.hpp>
#elif defined BUILD_WITH_QT
#include <QString>
#endif

#include <type_traits>
#include <string>
#include <set>
#include <vector>
#include <optional>

template<typename ty>
struct dependent_false : std::false_type {};


template<bool flag = false>
void static_assert_no_match() {
   static_assert(flag, "no match for if constexpr found");
   }

template<bool flag = false>
void static_assert_no_supported() {
   static_assert(flag, "this type isn't supported");
   }


template <typename ty, template <class> typename... Args>
constexpr bool correspond_to_any = std::disjunction<Args<ty>...>::value;


template <typename ty>
struct is_wchar_param {
   constexpr static bool value = std::is_same<wchar_t*, ty>::value ||
                                 std::is_same<const wchar_t*, ty>::value ||
                                 std::is_same<wchar_t* const, ty>::value ||
                                 std::is_same<const wchar_t* const, ty>::value ||
                                 std::is_same<wchar_t [], ty>::value ||
                                 std::is_same<const wchar_t [], ty>::value ||
                                 std::is_same<wchar_t const [], ty>::value;
   };

template<typename ty>
struct is_char_param {
   constexpr static bool value = std::is_same<char, ty>::value ||
                                 std::is_same<char*, ty>::value ||
                                 std::is_same<const char*, ty>::value ||
                                 std::is_same<char* const, ty>::value ||
                                 std::is_same<const char* const, ty>::value ||
                                 std::is_same<char [], ty>::value ||
                                 std::is_same<const char [], ty>::value ||
                                 std::is_same<char const [], ty>::value;

   };

template<typename ty>
struct is_wchar_or_char_param {
   constexpr static bool value = is_wchar_param<ty>::value || is_char_param<ty>::value;
   };

#if defined BUILD_WITH_VCL || BUILD_WITH_FMX
template <typename ty>
struct is_delphi_narrow_string {
   constexpr static bool value = std::is_same<AnsiString, ty>::value ||
								 std::is_same<AnsiString&, ty>::value ||
								 std::is_same<const AnsiString&, ty>::value;
   };

template <typename ty>
struct is_delphi_wide_string {
   constexpr static bool value = std::is_same<UnicodeString, ty>::value ||
								 std::is_same<UnicodeString&, ty>::value ||
								 std::is_same<const UnicodeString&, ty>::value;
   };

template <typename ty>
struct is_delphi_string {
   constexpr static bool value = is_delphi_narrow_string<ty>::value ||
								 is_delphi_wide_string<ty>::value;
   };

template<typename ty>
struct is_qt_string : std::false_type {};

#elif defined BUILD_WITH_QT

template <typename ty>
struct is_qt_string {
   constexpr static bool value = std::is_same<QString, ty>::value ||
								 std::is_same<QString&, ty>::value ||
								 std::is_same<const QString&, ty>::value;
   };

template <typename ty>
struct is_delphi_narrow_string : std::false_type {} ;

template <typename ty>
struct is_delphi_wide_string : std::false_type {};

template <typename ty>
struct is_delphi_string : std::false_type {};

#endif


template <typename ty>
struct is_cpp_narrow_string {
   constexpr static bool value = std::is_same<std::string, ty>::value ||
								 std::is_same<std::string&, ty>::value ||
								 std::is_same<const std::string&, ty>::value;
   };

template <typename ty>
struct is_cpp_wide_string {
   constexpr static bool value = std::is_same<std::wstring, ty>::value ||
								 std::is_same<std::wstring&, ty>::value ||
								 std::is_same<const std::wstring&, ty>::value;
   };

template <typename ty>
struct is_cpp_string {
   constexpr static bool value = is_cpp_narrow_string<ty>::value ||
								 is_cpp_wide_string<ty>::value;
   };

template <typename ty>
struct is_bool_param {
   constexpr static bool value = std::is_same<bool, ty>::value      || std::is_same<bool&, ty>::value      || std::is_same<const bool&, ty>::value;
   }
;
template <typename ty>
struct is_number_param {
   constexpr static bool value = std::is_same<char, ty>::value      || std::is_same<char&, ty>::value      || std::is_same<const char&, ty>::value ||
                                 std::is_same<int, ty>::value       || std::is_same<int&, ty>::value       || std::is_same<const int&, ty>::value ||
                                 std::is_same<short, ty>::value     || std::is_same<short&, ty>::value     || std::is_same<const short&, ty>::value ||
                                 std::is_same<long, ty>::value      || std::is_same<long&, ty>::value      || std::is_same<const long&, ty>::value ||
                                 std::is_same<long long, ty>::value || std::is_same<long long&, ty>::value || std::is_same<const long long&, ty>::value;
   };

template <typename ty>
struct is_optional : std::false_type {};

template <typename ty>
struct is_optional<std::optional<ty>> : std::true_type {};


template <typename T, typename = void>
struct is_container : std::false_type {};


template <typename T>
struct is_container<T
   , std::void_t<typename T::iterator,
                 typename T::const_iterator,
                 decltype(std::declval<T>().begin()),
                 decltype(std::declval<T>().end()),
                 decltype(std::declval<T>().size()),
                 decltype(std::declval<T>().begin()),
                 decltype(std::declval<T>().cend()) >> : std::true_type {};


template <typename ty, typename _ = void>
struct is_vector {
  static constexpr bool value = false;
};

template <typename ty>
struct is_vector<ty, typename std::enable_if<std::is_same<ty, std::vector<typename ty::value_type,
                                             typename ty::allocator_type>>::value>::type> {
  static constexpr bool value = true;
};




template <typename ty, typename _ = void>
struct is_set {
  static constexpr bool value = false;
};

template <typename ty>
struct is_set<ty, typename std::enable_if<std::is_same<ty,std::set<typename ty::key_type,
                                          typename ty::key_compare, typename ty::allocator_type>>::value>::type> {
  static constexpr bool value = true;
};


