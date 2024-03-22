#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <tuple>

struct Narrow {
   using stream_type  = std::ostream;
   using string_type  = std::string;
   using streambuf    = std::streambuf;
   using stringstream = std::ostringstream;

   static inline string_type                  strEmpty = "";
   static inline const stringstream::int_type cNL  = '\n';
   static inline const stringstream::int_type cTab = '\t';
   };

struct Latin {
   using stream_type = std::ostream;
   using string_type = std::string;
   using streambuf = std::streambuf;
   using stringstream = std::ostringstream;

   static inline string_type                  strEmpty = "";
   static inline const stringstream::int_type cNL = '\n';
   static inline const stringstream::int_type cTab = '\t';
};


struct Wide {
   using stream_type  = std::wostream;
   using string_type  = std::wstring;
   using streambuf    = std::wstreambuf;
   using stringstream = std::wostringstream;

   static inline string_type                  strEmpty = L"";
   static inline const stringstream::int_type cNL  = L'\n';
   static inline const stringstream::int_type cTab = L'\t';
   };

enum EMyFramework : int { vcl, fmx, qt, unknown };

template <typename char_type>
struct TMyDelimiter {
  typename char_type::string_type leading;
  typename char_type::string_type center;
  typename char_type::string_type trailing;
};


// ab C++20 Umbau auf source_location möglich
class my_source_position {
   friend std::ostream& operator << (std::ostream& out, my_source_position const& pos) {
      return out << "[function \"" << pos.strFunction << "\" in " << pos.strFile << " at line " << pos.iLine << "]";
   }

private:
   std::string strFunction = "";
   std::string strFile = "";
   size_t      iLine = 0;

public:
   //source_position() = default;
   my_source_position(std::string const& pFunc, std::string const& pFile, size_t pLine) :
      strFunction(pFunc), strFile(pFile), iLine(pLine) { }
   my_source_position(my_source_position const&) = delete;

   my_source_position(my_source_position&& rref) noexcept {
      std::swap(strFunction, rref.strFunction);
      std::swap(strFile, rref.strFile);
      std::swap(iLine, rref.iLine);
   }

   ~my_source_position(void) = default;

   my_source_position& operator = (my_source_position&& rref) noexcept {
      std::swap(strFunction, rref.strFunction);
      std::swap(strFile, rref.strFile);
      std::swap(iLine, rref.iLine);
      return *this;
   }

   std::string const& Function() const { return strFunction; }
   std::string const& File() const { return strFile; }
   size_t      Line() const { return iLine; }

};


/// Enumeration with values for the alignment of a field independent of the framework
enum class EMyAlignmentType : int {
   left = 1,
   right = 2,
   center = 3,
   unknown = 100
};

enum class EMyMessageType : int {
   information = 1,
   warning = 2,
   error = 3,
   question = 4,
   unknown = 100
};

template <typename char_type>
using tplList = std::tuple<typename char_type::string_type, int, EMyAlignmentType>;

template <typename char_type>
using vecList = std::vector<tplList<char_type>>;


/// Return values for modal dialogs and windows
enum class EMyRetResults { 
	                  none,          ///< no modal result for the form
						   ok,            ///< modal value ok returned
						   yes,           ///< modal value yes returned
						   no,            ///< modal value no returned
						   cancel,        ///< modal value cancel returned
                     error,         ///< error inside modal window
						   unknown  =100  ///< returned modal value is unknown
						   };

/// Enumeration to specify a type for an element of the framework
enum class EMyFrameworkType : int { 
	                  edit = 0,      ///< single-line input field in the framework, value is 0
							label = 1,     ///< single-line labeling field in the framework, value is 1
							groupbox = 2,  ///< container for grouping other display and input fields, value is 2
							button = 3,    ///< button to start an action, value is 3
                     listbox = 4,   ///< input field with selectable items in list form, value is 4
							checkbox = 5,  ///< input field for bool values, value is 5
							combobox = 6,  ///< control that allows to make a selection from given options or alternatively make an input, value is 6
						   memo = 7,      ///< multiline input field, value is 7
							statusbar = 8, ///< display field at the bottom of the window, value is 8
							listview = 9   ///< tableview / list as display field for values in table form, value is 9
                     };

template <typename char_type>
using mapLists = std::map<std::string, vecList<char_type>>;

// Repository for all comboboxes in a form
using tplRepVal     = std::tuple<std::string, int>;
using vecRepVals    = std::vector<tplRepVal>;
using mapRepVals    = std::map<std::string, int>;
using mapRepositories = std::map<std::string, mapRepVals>;

/// Type for memory for reading information about the combobox repositories from XML
using mapReposAll = std::map<std::string, vecRepVals>;

/// type for memory for reading formular information from XML
// 
// Identifier, Type, Align, Caption, Hint, ID
using tplFormElements = std::tuple<std::string, EMyFrameworkType, EMyAlignmentType, std::string, std::string, int>;
using vecFormElements = std::vector<tplFormElements>;
using tplFormInformation = std::tuple<std::string, int>;
using tplFormData = std::tuple<tplFormInformation, vecFormElements>;
using mapFormsDictionary = std::map<std::string, tplFormData>;



