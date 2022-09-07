#pragma once


struct my_control_for_find { };

template <typename ty>
typename ty::mapped_type& my_find(ty& container, typename ty::key_type const& value) {
   auto it = container.find(value);
   if (it == container.end()) throw my_control_for_find();
   else return it->second;
   }
   
   
 //----------------------------------------------------------------------------
template <typename Iterator, typename Pred, typename Operation>
void for_each_if(Iterator begin, Iterator end, Pred pred, Operation op) {
   if(begin < end) {
      for(; begin != end; ++begin) {
         if(pred(*begin)) op(*begin);
         }
      }
   }

