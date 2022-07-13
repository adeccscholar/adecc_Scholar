#ifndef MyStdTypesH
#define MyStdTypesH

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

template <typename char_type>
struct TMyDelimiter {
  typename char_type::string_type leading;
  typename char_type::string_type center;
  typename char_type::string_type trailing;
};


/// Return values for modal dialogs and windows
enum class EMyRetResults { 
	                  none,          ///< no modal result for the form
						   ok,            ///< modal value ok returned
						   yes,           ///< modal value yes returned
						   no,            ///< modal value no returned
						   cancel,        ///< modal value cancel returned
						   unknown        ///< returned modal value is unknown
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

/// Enumeration with values for the alignment of a field independent of the framework
enum class EMyAlignmentType : int { 
	                  left = 1, 
							right = 2, 
							center = 3, 
							unknown = 100 
                     };

using tplList = std::tuple<std::string, int, EMyAlignmentType>;
using vecList = std::vector<tplList>;

using mapLists = std::map<std::string, vecList>;

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


#endif

