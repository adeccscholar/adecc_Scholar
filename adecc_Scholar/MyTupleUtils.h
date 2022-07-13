#pragma once

#include <iostream>
#include <string>
#include <tuple>
#include <chrono>

template <typename char_type>
struct myTupleHlp {
   template <size_t Index, class... Args>
   static void Output(char_type::stream_type& out, TMyDelimiter<char_type> const& delimiters, std::tuple<Args...> const& value) {
      if constexpr (Index == sizeof...(Args)) { out << delimiters.trailing; }
      else {
         out << std::get<Index>(value);
         if constexpr (Index + 1 != sizeof...(Args)) { out << delimiters.center; }
         Output<Index + 1>(out, delimiters, value);
      }
   }

   template <class... Args>
   static void Output(char_type::stream_type& out, TMyDelimiter<char_type> const& delimiters, std::tuple<Args...> const& value) {
      out << delimiters.leading;
      Output<0>(out, delimiters, value);
   }
};


template <size_t Index, class... Args>
inline size_t mySum(std::tuple<Args...> const& value) {
   if constexpr (Index == sizeof...(Args)) { return 0u; }
   else { return std::get<Index>(value) + mySum<Index + 1>(value); }
   }

template <class... Args>
inline size_t mySum(std::tuple<Args...> const& value) {
    return mySum<0>(value);
   }


template <typename tty, typename fty, typename... Args>
inline auto Call(tty& time, fty function, Args... args) {
   auto func_start = std::chrono::high_resolution_clock::now();
   auto ret = function(std::forward<Args>(args)...);
   auto func_ende = std::chrono::high_resolution_clock::now();
   time = std::chrono::duration_cast<tty>(func_ende - func_start);
   return ret;
   }