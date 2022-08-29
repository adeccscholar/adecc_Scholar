#pragma once

#include <iostream>
#include <string>
#include <tuple>
#include <chrono>
#include <thread>

template <typename char_type>
struct myTupleHlp {
   template <size_t Index, class... Args>
   static void Output(typename char_type::stream_type& out, TMyDelimiter<char_type> const& delimiters, std::tuple<Args...> const& value) {
      if constexpr (Index == sizeof...(Args)) { out << delimiters.trailing; }
      else {
         out << std::get<Index>(value);
         if constexpr (Index + 1 != sizeof...(Args)) { out << delimiters.center; }
         Output<Index + 1>(out, delimiters, value);
      }
   }

   template <class... Args>
   static void Output(typename char_type::stream_type& out, TMyDelimiter<char_type> const& delimiters, std::tuple<Args...> const& value) {
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


class TMyTimer {
    private:
       std::atomic<bool> boActive = false;
      // std::atomic<std::vector<std::function<void ()>>> tasks;
    public:
       TMyTimer() = default;
       TMyTimer(TMyTimer const&) = delete;

       template <typename func_type, typename... arguments>
       void add_task(unsigned int interval, func_type func, arguments&&... args) {
          std::function<typename std::result_of<func_type(arguments...)>::type()> localtask(std::bind(std::forward<func_type>(func), std::forward<arguments>(args)...));
          //std::function<void ()> task(std::bind(std::forward<func_type>(func), std::forward<arguments>(args)...));
          std::thread([this, interval, localtask]() {
             while(this->boActive == true) {
                std::this_thread::sleep_for(std::chrono::milliseconds(interval));
                if(this->boActive) localtask();
                }
             }).detach();
          }

    void start() { boActive = true; }
    void stop() { boActive = false; }
    };



template <typename tty, typename fty, typename... Args>
inline auto Call(tty& time, fty function, Args... args) {
   auto func_start = std::chrono::high_resolution_clock::now();
   auto ret = function(std::forward<Args>(args)...);
   auto func_ende = std::chrono::high_resolution_clock::now();
   time = std::chrono::duration_cast<tty>(func_ende - func_start);
   return ret;
   }