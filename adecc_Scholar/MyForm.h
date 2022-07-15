#ifndef xMyFormH
#define xMyFormH

#include "MyType_Traits.h"
#include "MyStream.h"
#include "MyTools.h"
#include "MyStdTypes.h"
#include "MyFramework_Selection.h"

#include <string>
#include <stdexcept>
#include <tuple>
#include <map>
#include <vector>
#include <memory>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <typeinfo>

#if defined BUILD_WITH_VCL
class TMyWait {
   private:
      TCursor old_cursor;
   public:
      TMyWait(void) {
         old_cursor = Screen->Cursor;
         Screen->Cursor = crHourGlass;
         }

      TMyWait(TMyWait const&) = delete;
      ~TMyWait(void) {
         Screen->Cursor = old_cursor;
         }
   };


#elif defined BUILD_WITH_FMX
class TMyWait {
   private:
      TCursor old_cursor;
      _di_IFMXCursorService cs;

   public:
      TMyWait(void) {
         if(TPlatformServices::Current->SupportsPlatformService(__uuidof(IFMXCursorService), (void*)&cs)) {
            old_cursor = cs->GetCursor();
            cs->SetCursor(crHourGlass);
            }
         }

      TMyWait(TMyWait const&) = delete;
      ~TMyWait(void) {
         cs->SetCursor(old_cursor);
         }
   };
#elif defined BUILD_WITH_QT
class TMyWait {
   private:
      QWidget* currentWidget = nullptr;
   public:
      TMyWait(void) {
         currentWidget = QApplication::activeWindow();
         if (currentWidget) {
            currentWidget->setCursor(Qt::WaitCursor);
            }
         }
      TMyWait(TMyWait const&) = delete;
      ~TMyWait(void) {
         if(currentWidget) currentWidget->unsetCursor();
         }
   };

#else
  #error Fehlende Klasse TMyWait in diesem Framework
#endif



class TMyForm {
   friend void swap(TMyForm& lhs, TMyForm& rhs) {
             std::swap(lhs.form, rhs.form);
             std::swap(lhs.boOwner, rhs.boOwner);
             std::swap(lhs.cbsrep, rhs.cbsrep);
             }
   private:
     fw_Form*     form    = nullptr;
     bool         boOwner = false;
     mapRepositories cbsrep;

      #if defined BUILD_WITH_VCL
      static const inline std::map<EMyAlignmentType, TAlignment> align_type_conv = {
                               { EMyAlignmentType::left,  taLeftJustify },
                               { EMyAlignmentType::right, taRightJustify },
                               { EMyAlignmentType::center, taCenter }
                               };
      #elif defined BUILD_WITH_FMX
      static const inline std::map<EMyAlignmentType, TTextAlign> align_type_conv = {
                               { EMyAlignmentType::left,    TTextAlign::Leading },
                               { EMyAlignmentType::right,   TTextAlign::Trailing },
                               { EMyAlignmentType::center,  TTextAlign::Center },
                               { EMyAlignmentType::unknown, TTextAlign::Leading }
                               };
      #elif defined BUILD_WITH_QT
      static const inline std::map<EMyAlignmentType, Qt::Alignment> align_type_conv = {
                               { EMyAlignmentType::left,    Qt::AlignVCenter | Qt::AlignLeft },
                               { EMyAlignmentType::right,   Qt::AlignVCenter | Qt::AlignRight },
                               { EMyAlignmentType::center,  Qt::AlignVCenter | Qt::AlignCenter },
                               { EMyAlignmentType::unknown, Qt::AlignVCenter | Qt::AlignLeft }
                               };
      #else
         #error fehlende definition von align_type_conv in diesem Framework
      #endif

   public:
      TMyForm(fw_Form* frm = nullptr, bool owner = false) {
         form    = frm;
       boOwner = owner;
       }

      TMyForm(TMyForm const&) = delete;

      TMyForm(TMyForm&& ref) noexcept {
         std::swap(form, ref.form);
         std::swap(boOwner, ref.boOwner);
         std::swap(cbsrep, ref.cbsrep);
       }

      ~TMyForm(void) {
         if(boOwner) delete form;
         form = nullptr;
         }

      void Set(fw_Form* frm = nullptr, bool owner = false) {
         if(form != nullptr && boOwner == true) delete form;
         form    = frm;
         boOwner = owner;
         }

      std::string GetCaption(void) {
         #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
            return AnsiString(Form()->Caption).c_str();
         #elif defined BUILD_WITH_QT
            return Form()->windowTitle().toStdString();
         #else
            #error fehlende Implementierung für SetCaption() in diesem Framework
         #endif
         }

      template <typename ty>
      void SetCaption(ty const& strCaption)  {
         #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
            auto SetFunc = [this](fw_String const& val) { this->Form()->Caption = val; };
         #elif defined BUILD_WITH_QT
            auto SetFunc = [this](fw_String const& val) { this->Form()->setWindowTitle(val); };
         #else
            #error fehlende Implementierung für SetCaption() in diesem Framework
         #endif
       SetFunction(SetFunc, strCaption);
       }


     std::string FormName(void) const {
        #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
           return AnsiString(Form()->Name).c_str();   ///< todo AnsiString eleminating
        #elif defined BUILD_WITH_QT
           return Form()->objectName().toStdString();
        #else
          #error fehlende Implementierung für FormName() in diesem Framework
        #endif
       }

      //------------------------------------------------------------------------
      template<typename ty_base, EMyFrameworkType ft>
      void GetAsStream(TStreamWrapper<ty_base>& wrapper, std::string const& strName) {
         if constexpr (ft == EMyFrameworkType::memo)
            wrapper.Activate(Find<fw_Memo>(strName));
         else if constexpr (ft == EMyFrameworkType::listbox)
            wrapper.Activate(Find<fw_Listbox>(strName));
         else if constexpr (ft == EMyFrameworkType::combobox)
            wrapper.Activate(Find<fw_Combobox>(strName));
         else if constexpr (ft == EMyFrameworkType::statusbar)
            #if defined BUILD_WITH_VCL
            wrapper.Activate(Find<fw_Statusbar>(strName));
            #elif defined BUILD_WITH_FMX || defined BUILD_WITH_QT
            wrapper.Activate(Find<fw_Label>(strName));
            #endif
         // Aktivierung einer Liste nicht mehr möglich, da Überschriften benötigt werden
         // Deshal neue Funktion
         else
            static_assert_no_supported();
         }

      template<typename ty_base, EMyFrameworkType ft>
      auto* GetAsStreamBuff(std::string const& strName) {
         if constexpr (ft == EMyFrameworkType::memo)          return new MemoStreamBuf<ty_base>(Find<fw_Memo>(strName));
         else if constexpr (ft == EMyFrameworkType::listbox)  return new ListBoxStreamBuf<ty_base>(Find<fw_Listbox>(strName));
         else if constexpr (ft == EMyFrameworkType::combobox) return new ComboBoxStreamBuf<ty_base>(Find<fw_Combobox>(strName));
         else
            static_assert_no_supported();
      }


      //------------------------------------------------------------------------
      template<typename ty_base, EMyFrameworkType ft>
      void GetAsStream(TStreamWrapper<ty_base>& wrapper, std::string const& strName, std::vector<tplList<ty_base>> const& caps) {
         if constexpr (ft == EMyFrameworkType::listview)
            wrapper.Activate(Find<fw_Table>(strName), caps);
         else
            static_assert_no_supported();
         }


      //-----------------------------------------------------------------------
      //  Hier sind keine Probleme zu erwarten, da es nur C++ ist
      // eventuell runtime_error wenn boOpt == false und Feld leer
      template <typename ty, bool boOpt = false>
      auto GetEditAs(std::string const& strField) {
         std::optional<ty> value;
         GetEdit(strField, value);
         if constexpr (boOpt == true) return value;
         else return value ? *value : ty();
          }

      template <typename ty, bool boOpt = false>
      auto GetCheckBoxAs(std::string const& strField) {
         std::optional<ty> value;
         GetCheckBox(strField, value);
         if constexpr (boOpt == true) return value;
         else return value ? *value : ty();
         }


      //----------------------------------------------------------------------------------------
      template <typename ty, bool boOpt = false>
      auto GetComboBoxAs(std::string const& strField) {
         std::optional<ty> value;
         GetComboBox(strField, value);
         if constexpr (boOpt == true) return value;
         else return value ? *value : ty();
         }



      template <EMyFrameworkType ft, typename ty>
      auto Get(std::string const& strField) {
         if constexpr (ft == EMyFrameworkType::edit) return GetEditAs<ty, true>(strField);
         else if constexpr (ft == EMyFrameworkType::checkbox) return GetCheckBoxAs<ty, true>(strField);
         else if constexpr (ft == EMyFrameworkType::combobox) return GetComboBoxAs<ty, true>(strField);
         else if constexpr (ft == EMyFrameworkType::listbox && std::is_same < std::string, ty>::value) return std::make_optional(GetListBox(strField));
         else                                            static_assert_no_match();
         }

      //----------------------------------------------------------------------------------------
      template <EMyFrameworkType ft>
      void Alignment(std::string const& strField, EMyAlignmentType align) {
         #if defined BUILD_WITH_VCL
           auto set = [this](auto fld, TAlignment align_val) { fld->Alignment = align_val; };
         #elif defined BUILD_WITH_FMX
           auto set = [this](auto fld, TTextAlign align_val) { fld->TextSettings->HorzAlign = align_val; };
         #elif defined BUILD_WITH_QT
           auto set =[this](auto fld, Qt::Alignment align_val) { fld->setAlignment(align_val); };
         #else
           #error fehlende Implementierung von Alignment() in diesem Framework
         #endif

         if(auto myret = align_type_conv.find(align); myret != align_type_conv.end()) {
            if constexpr      (ft == EMyFrameworkType::edit)       set(Find<fw_Edit>(strField), myret->second);
            else if constexpr (ft == EMyFrameworkType::label)      set(Find<fw_Label>(strField), myret->second);
            else if constexpr (ft == EMyFrameworkType::groupbox) return;
            else if constexpr (ft == EMyFrameworkType::button) return;
            else if constexpr (ft == EMyFrameworkType::listbox) return;
            else if constexpr (ft == EMyFrameworkType::checkbox) return;
            else if constexpr (ft == EMyFrameworkType::combobox) return;
            else if constexpr (ft == EMyFrameworkType::memo) return;
            else if constexpr (ft == EMyFrameworkType::statusbar) return;
            else if constexpr (ft == EMyFrameworkType::listview) return;
            else static_assert_no_match();
            }
         else {
            throw std::runtime_error("unexpexted align for Alignment in Form");
            }
         }

      //----------------------------------------------------------------------------------------
      template <EMyFrameworkType ft>
      void ReadOnly(std::string const& strField, bool boSet) {
         #if defined BUILD_WITH_VCL
           auto set = [this](auto fld, bool boSet) { fld->ReadOnly = boSet; };
         #elif defined BUILD_WITH_FMX
           auto set = [this](auto fld, bool boSet) { fld->ReadOnly = boSet; };
         #elif defined BUILD_WITH_QT
           auto set =[this](auto fld, bool boSet) { fld->setReadOnly(boSet); };
         #else
           #error fehlende Implementierung von Alignment() in diesem Framework
         #endif

         if constexpr (ft == EMyFrameworkType::edit)          set(Find<fw_Edit>(strField), boSet);
         else if constexpr (ft == EMyFrameworkType::memo)     set(Find<fw_Memo>(strField), boSet);
         else if constexpr (ft == EMyFrameworkType::listbox)  set(Find<fw_Listbox>(strField), boSet);
         else if constexpr (ft == EMyFrameworkType::combobox) set(Find<fw_Combobox>(strField), boSet);
         else                                            static_assert_no_match();
         }

      //----------------------------------------------------------------------------------------
      template <EMyFrameworkType ft>
      void Visible(std::string const& strField, bool boVisible) {
         #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
           auto set = [this](auto fld, bool boSet) { fld->Visible = boSet; };
         #elif defined BUILD_WITH_QT
           auto set = [](auto fld, bool boSet) { fld->setVisible(boSet); };
         #else
            #error fehlende Implementierung von Visible() in diesem Framework
         #endif
         if constexpr      (ft == EMyFrameworkType::edit)     set(Find<fw_Edit>(strField), boVisible);
         else if constexpr (ft == EMyFrameworkType::memo)     set(Find<fw_Memo>(strField), boVisible);
         else if constexpr (ft == EMyFrameworkType::label)    set(Find<fw_Label>(strField), boVisible);
         else if constexpr (ft == EMyFrameworkType::groupbox) set(Find<fw_Groupbox>(strField), boVisible);
         else if constexpr (ft == EMyFrameworkType::combobox) set(Find<fw_Combobox>(strField), boVisible);
         else if constexpr (ft == EMyFrameworkType::checkbox) set(Find<fw_Checkbox>(strField), boVisible);
         else if constexpr (ft == EMyFrameworkType::button)   set(Find<fw_Button>(strField), boVisible);
         else                                                 static_assert_no_match();
         }

      //----------------------------------------------------------------------------------------
      template <EMyFrameworkType ft>
      void Enable(std::string const& strField, bool boEnabled) {
         #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
           auto set = [this](auto fld, bool boSet) { fld->Enabled = boSet; };
         #elif defined BUILD_WITH_QT
           auto set = [](auto fld, bool boSet) { fld->setEnabled(boSet); };
         #else
           #error fehlende Implementierung von Enable() in diesem Framework
         #endif
         if constexpr      (ft == EMyFrameworkType::edit) 	  set(Find<fw_Edit>(strField), boEnabled);
         else if constexpr (ft == EMyFrameworkType::memo)     set(Find<fw_Memo>(strField), boEnabled);
         else if constexpr (ft == EMyFrameworkType::label)    set(Find<fw_Label>(strField), boEnabled);
         else if constexpr (ft == EMyFrameworkType::groupbox) set(Find<fw_Groupbox>(strField), boEnabled);
         else if constexpr (ft == EMyFrameworkType::combobox) set(Find<fw_Combobox>(strField), boEnabled);
         else if constexpr (ft == EMyFrameworkType::listbox)  set(Find<fw_Listbox>(strField), boEnabled);
         else if constexpr (ft == EMyFrameworkType::checkbox) set(Find<fw_Checkbox>(strField), boEnabled);
         else if constexpr (ft == EMyFrameworkType::button)   set(Find<fw_Button>(strField), boEnabled);
         else                                                 static_assert_no_match();
         }



      //----------------------------------------------------------------------------------------
      // Listbox fehlt
      template <EMyFrameworkType ft, typename ty>
       void Set(std::string const& strField, ty const& value, int iLen = -1, int iScale = -1) {
          //---------------------------------------------------------------------------------------------
          // Set<EMyFrameworkType::edit>
          //---------------------------------------------------------------------------------------------
        if constexpr      (ft == EMyFrameworkType::edit) {
           #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
             auto SetFunc = [this, strField](fw_String const& val) { this->Find<fw_Edit>(strField)->Text = val; };
           #elif defined BUILD_WITH_QT
             auto SetFunc = [this, strField](fw_String const& val) { this->Find<fw_Edit>(strField)->setText(val); };
           #endif
           SetFunction(SetFunc, value, iLen, iScale);
             }
          //---------------------------------------------------------------------------------------------
          // Set<EMyFrameworkType::memo>
          //---------------------------------------------------------------------------------------------
        else if constexpr (ft == EMyFrameworkType::memo) {
           #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
             auto SetFunc = [this, strField](fw_String const& val) { this->Find<fw_Memo>(strField)->Text = val; };
           #elif defined BUILD_WITH_QT
             auto SetFunc = [this, strField](fw_String const& val) { this->Find<fw_Memo>(strField)->setText(val); };
           #endif
           SetFunction(SetFunc, value, iLen, iScale);
             }
          //---------------------------------------------------------------------------------------------
          // Set<EMyFrameworkType::label>
          //---------------------------------------------------------------------------------------------
        else if constexpr (ft == EMyFrameworkType::label) {
             #if defined BUILD_WITH_VCL
                auto SetFunc = [this, strField](fw_String const& val) { this->Find<fw_Label>(strField)->Caption = val; };
             #elif defined BUILD_WITH_FMX
                auto SetFunc = [this, strField](fw_String const& val) { this->Find<fw_Label>(strField)->Text = val; };
             #elif defined BUILD_WITH_QT
               auto SetFunc = [this, strField](fw_String const& val) { this->Find<fw_Label>(strField)->setText(val); };
             #endif
           SetFunction(SetFunc, value, iLen, iScale);
             }
          //---------------------------------------------------------------------------------------------
          // Set<EMyFrameworkType::groupbox>
          //---------------------------------------------------------------------------------------------
        else if constexpr (ft == EMyFrameworkType::groupbox) {
             #if defined BUILD_WITH_VCL
                auto SetFunc = [this, strField](fw_String const& val) { this->Find<fw_Groupbox>(strField)->Caption = val; };
             #elif defined BUILD_WITH_FMX
                auto SetFunc = [this, strField](fw_String const& val) { this->Find<fw_Groupbox>(strField)->Text = val; };
             #elif defined BUILD_WITH_QT
               auto SetFunc = [this, strField](fw_String const& val) { this->Find<fw_Groupbox>(strField)->setTitle(val); };
             #endif
           SetFunction(SetFunc, value);
             }
          //---------------------------------------------------------------------------------------------
          // Set<EMyFrameworkType::combobox>
          //---------------------------------------------------------------------------------------------
        else if constexpr (ft == EMyFrameworkType::combobox) {
             auto field = Find<fw_Combobox>(strField);
             try {
                if (auto rep = cbsrep.find(strField); rep != cbsrep.end()) SetCombobox(field, value, rep->second);
                else                                                       SetCombobox(field, value);
                }
             catch(std::exception& ex) {
                std::stringstream os;
                os << "error in formular \"" << FormName() << "\", field \"" << strField << "\": " << ex.what();
                throw std::runtime_error(os.str());
                }
             }
           else if constexpr (ft == EMyFrameworkType::listbox)  {
             /// !!!!!!
             }
          //---------------------------------------------------------------------------------------------
          // Set<EMyFrameworkType::checkbox>
          //---------------------------------------------------------------------------------------------
           else if constexpr (ft == EMyFrameworkType::checkbox) {
             #if defined BUILD_WITH_VCL
                auto SetFunc = [this, strField](fw_String const& val) { this->Find<fw_Checkbox>(strField)->Caption = val; };
              auto SetBool = [this, strField](bool val) { this->Find<fw_Checkbox>(strField)->Checked = val; };
             #elif defined BUILD_WITH_FMX
                auto SetFunc = [this, strField](fw_String const& val) { this->Find<fw_Checkbox>(strField)->Text = val; };
                auto SetBool = [this, strField](bool val) { this->Find<fw_Checkbox>(strField)->IsChecked = val; };
             #elif defined BUILD_WITH_QT
                auto SetFunc = [this, strField](fw_String const& val) { this->Find<fw_Checkbox>(strField)->setText ( val ) ; };
                auto SetBool = [this, strField](bool val) { this->Find<fw_Checkbox>(strField)->setCheckState( val ? Qt::Checked : Qt::Unchecked); };
             #endif
             if constexpr (is_bool_param<ty>::value)         SetBool(value);
             else if constexpr (is_number_param<ty>::value)  SetBool(value != 0);
             else                                            SetFunction(SetFunc, value);;
             }
          //---------------------------------------------------------------------------------------------
          // Set<EMyFrameworkType::button>
          //---------------------------------------------------------------------------------------------
        else if constexpr (ft == EMyFrameworkType::button)  {
           #if defined BUILD_WITH_VCL
              auto SetFunc = [this, strField](fw_String const& val) { this->Find<fw_Button>(strField)->Caption = val; };
           #elif defined BUILD_WITH_FMX
              auto SetFunc = [this, strField](fw_String const& val) { this->Find<fw_Button>(strField)->Text = val; };
           #elif defined BUILD_WITH_QT
              auto SetFunc = [this, strField](fw_String const& val) { this->Find<fw_Button>(strField)->setText(val); };
           #endif
           SetFunction(SetFunc, value);
             }
          #if defined BUILD_WITH_VCL
          else if constexpr (ft == EMyFrameworkType::statusbar) {
             auto SetFunc = [this, strField](fw_String const& val) { this->Find<TStatusBar>(strField)->SimpleText = val; };
             SetFunction(SetFunc, value);
             }
          #endif

          else if constexpr (ft == EMyFrameworkType::statusbar) return;
          else if constexpr (ft == EMyFrameworkType::listview) return;

        else                                                 static_assert_no_match();
      }

      //------------------------------------------------------------------------------------------------------------------------
      template <typename ty>
      void GetEdit(std::string const& strField, ty& value) {
          #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
          auto get_len = [](auto fld) -> int { return fld->Text.Length(); };
          auto get_txt = [](auto fld) -> fw_String { return fld->Text; };
          #elif defined BUILD_WITH_QT
          auto get_len = [](auto fld) -> int { return fld->text().length(); };
          auto get_txt = [](auto fld) -> fw_String { return fld->text(); };
          #else
            #error Fehlende Implementierung für GetEdit in diesem Framework
          #endif
          auto field = Find<fw_Edit>(strField);
          if constexpr (is_optional<ty>::value) {
            using used_type  = typename std::remove_reference<typename std::remove_cv<decltype(*value)>::type>::type;
            if(get_len(field) == 0) value = std::nullopt;
             else {
                value = GetText<used_type>(get_txt(field));
                }
             }
          else {
             using used_type  = typename std::remove_reference<typename std::remove_cv<ty>::type>::type;
             value = GetText<used_type>(get_txt(field));
             }
          }

      void InitListBox(std::string const& strField, std::vector<std::string> const& values) {
         auto field = Find<fw_Listbox>(strField);
         #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
           field->Items->Clear();
           std::for_each(values.cbegin(), values.cend(), [field](std::string const& value) { field->Items->Add(value.c_str()); });
         #elif defined BUILD_WITH_QT
           field->clear();
           std::for_each(values.cbegin(), values.cend(), [field](std::string const& value) { field->addItem(QString::fromStdString(value)); });
         #else
           #error Fehlende Implementierung für InitListBox für dieses Framework
         #endif
         }

      void AddListBox(std::string const& strField, std::string const& value) {
         auto field = Find<fw_Listbox>(strField);
         #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
           field->Items->Add(value.c_str());
         #elif defined BUILD_WITH_QT
           field->addItem(QString::fromStdString(value));
         #else
           #error Fehlende Implementierung für AddListBox für dieses Framework
         #endif
         }

      void SetFirstListBox(std::string const& strField) {
         auto field = Find<fw_Listbox>(strField);
         #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
           if (field->Items->Count > 0) field->ItemIndex = 0;
         #elif defined BUILD_WITH_QT
           if(field->count() > 0) field->setCurrentRow(0);
         #else
           #error Fehlende Implementierung für SetFirstListbox für dieses Framework
         #endif
         }

      void SetListBox(std::string const& strField, std::string const& value, bool boCaseSensitive = false) {
         auto field = Find<fw_Listbox>(strField);
         #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
           if (field->Items->Count > 0) {
              fw_String strSeek = boCaseSensitive ? value.c_str() : TMyTools::lower(value).c_str();
              int i = 0;
              for (; i < field->Items->Count; ++i) {
                 if (strSeek == (boCaseSensitive ? field->Items->Strings[i] : LowerCase(field->Items->Strings[i]))) break;
                 }
              if (i < field->Items->Count) field->ItemIndex = i;  // eventuell else mit Fehlermeldung
              }
         #elif defined BUILD_WITH_QT
           std::unique_ptr<QListWidgetItem> itemSeek = std::make_unique<QListWidgetItem>(QString::fromStdString(boCaseSensitive ? value : TMyTools::lower(value)));
           //field->setCurrentItem(itemSeek.get());
           int i = 0;
           for (; i < field->count(); ++i) {
              QListWidgetItem* item = field->item(i);
              if (itemSeek->text() == item->text()) break;
              }
           if (i < field->count()) field->setCurrentRow(0);
         #else
           #error Fehlende Implementierung für SetListbox für dieses Framework
         #endif
         }

      std::string GetListBox(std::string const& strField) {
         auto field = Find<fw_Listbox>(strField);
         #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
            fw_String strText = field->Items->Strings[field->ItemIndex];
            return AnsiString(strText).c_str();
         #elif defined BUILD_WITH_QT
            return field->currentItem()->text().toStdString();
         #else
           #error Fehlende Implementierung für SetListbox für dieses Framework
         #endif
         }

      void SetFirstComboBox(std::string const& strField) {
         auto field = Find<fw_Combobox>(strField);
         #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
            if (field->Items->Count > 0) field->ItemIndex = 0;
         #elif defined BUILD_WITH_QT
            if (field->count() > 0) field->setCurrentIndex(0);
         #else
           #error Fehlende Implementierung für SetFirstListbox für dieses Framework
         #endif
         }

      //---------------------------------------------------------------------------
     template <typename ty>
      void GetCheckBox(std::string const& strField, ty& theVal) {
         fw_Checkbox* box = Find<fw_Checkbox>(strField);
         std::optional<bool> chosen;
         #if defined BUILD_WITH_VCL
         switch(box->State) {
            case cbGrayed:
               chosen = std::nullopt;
               break;
            case cbChecked:
               chosen = std::make_optional(true);
               break;
            case cbUnchecked:
               chosen = std::make_optional(false);
               break;
            default:
                {
              std::ostringstream os;
                os << "unexpected Option in GetCheckBox";
              throw std::runtime_error(os.str());
                }
            }
         #elif defined BUILD_WITH_FMX
            chosen = std::make_optional(box->IsChecked);
         #elif defined BUILD_WITH_QT
           chosen = std::make_optional(box->checkState() == Qt::Checked); // partially possible, attention
         #else
            #error todo, define for new framework
         #endif

         if constexpr (is_optional<ty>::value) {
            using used_type = decltype(*theVal);
            if constexpr (std::is_same<used_type, bool>::value || std::is_same<used_type, bool&>::value) {
               theVal = chosen;
               }
            else if constexpr (std::is_integral<used_type>::value) {
               if(!chosen) theVal = { };
               else        theVal = std::make_optional<ty>(*chosen ? 1 : 0);
               }
            else {
               // for strings add variant with boolalpha
               used_type x = *theVal;
               chosen = std::nullopt;
               static_assert(dependent_false<ty>::value, "no valid type for GetCheckBox");
               }
            }
         else {
            if constexpr (std::is_same<ty, bool>::value) {
               if(!chosen || *chosen == false) theVal = false;
               else theVal = true;
               }
            else if constexpr (std::is_integral<ty>::value) {
               if(!chosen || *chosen == false) theVal = 0;
               else theVal = 1;
               }
            else {
               // for strings add variant with boolalpha
               static_assert(dependent_false<ty>::value, "no valid type for GetCheckBox");
               }
            }
         return;
         }

      // -----------------------------------------------------------------------
      void InitCombobox(std::string const& strField, std::vector<std::string> const& values) {
         auto field = Find<fw_Combobox>(strField);
         #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
           field->Items->Clear();
           std::for_each(values.cbegin(), values.cend(), [field](std::string const& value) { field->Items->Add(value.c_str()); });
         #elif defined BUILD_WITH_QT
           field->clear();
           QStringList list;
           std::for_each(values.cbegin(), values.cend(), [&list](std::string const& value) { 
              QString txt = QString::fromStdString(value);
              txt.remove(QChar::Null);
              list.append(txt); 
              });
           field->addItems(list);
         #else
           #error Fehlende Implementierung für InitListBox für dieses Framework
         #endif
         }


      void InitCombobox(std::string const& strField, vecRepVals const& values) {
         auto field = Find<fw_Combobox>(strField);

         #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
            auto clear_items = [this, &field]() { field->Items->Clear(); };
            auto set_index = [this, &field](int iVal) { field->ItemIndex = iVal;  };
         #elif defined BUILD_WITH_QT
         auto clear_items = [this, &field]() { field->clear(); };
            auto set_index = [&field](int iVal) { field->setCurrentIndex(iVal); };
         #else
           #error
         #endif
         clear_items();
         std::ostream cos ( GetAsStreamBuff<Narrow, EMyFrameworkType::combobox>(strField) );
         if(auto it = cbsrep.find(strField); it != cbsrep.end())  cbsrep.erase(it);
         if(values.empty()) {
            set_index(-1);
            }
         else {
            std::for_each(values.cbegin(), values.cend(), [&cos, &strField, this](auto const& value) {
                  cos << std::get<0>(value) << std::endl;
                  this->cbsrep[strField].insert( { std::get<0>(value), std::get<1>(value) } );
                  });
            set_index(0);
            #if defined BUILD_WITH_VCL
              field->Style = csDropDownList;
            #elif defined BUILD_WITH_QT
              field->setInsertPolicy(QComboBox::NoInsert);
            #endif
            }
         }


      template <typename ty>
      void GetComboBox(std::string const& strField, ty& value) {
         auto field = Find<fw_Combobox>(strField);

         #if defined BUILD_WITH_VCL 
            auto get_index = [this, &field]() { return field->ItemIndex;  };
            auto get_text = [this, &field]() { return field->Text; };
            static constexpr auto convert_text = [](fw_String const& val) { return std::string(AnsiString(val).c_str()); };
         #elif defined BUILD_WITH_FMX
            auto get_index = [this, &field]() { return field->ItemIndex;  };
            auto get_text = [this, &field]() { return field->Items->Strings[field->ItemIndex]; };
            static constexpr auto convert_text = [] (fw_String const& val) -> std::string { return AnsiString(val).c_str(); };
         #elif defined BUILD_WITH_QT
            auto get_index = [&field]() { return field->currentIndex(); };
            auto get_text = [&field]() { return field->currentText(); };
            static constexpr auto convert_text = [](fw_String const& val) -> std::string { return val.toStdString(); };
         #else
           #error
         #endif

         if constexpr (is_optional<ty>::value) {
            if(get_index() == -1) value = std::nullopt;
            else {
               using used_type = decltype(*value);
               fw_String strText = get_text();
               if constexpr (is_number_param<used_type>::value) {
                  // seeking in repositiory
                  if(auto rep = cbsrep.find(strField); rep  != cbsrep.end()) {
                     std::string strVal = convert_text(strText);
                     if(auto it = rep->second.find(strVal); it != rep->second.end()) {
                        value = std::make_optional(it->second);
                        }
                     else {
                        std::ostringstream os;
                        os << "For the field \"" << strField << "\" in formular \"" << FormName() << "\"isn't a value for "
                           << strVal << " in the repository.";
                        throw std::runtime_error(os.str());
                        }
                     }
                  else {
                     std::ostringstream os;
                     os << "No repository for field field \"" << strField << "\" in formular \"" << FormName() << "\"";
                     throw std::runtime_error(os.str());
                     }
                  }
               else if constexpr (std::is_same<std::string, used_type>::value || std::is_same<std::string&, used_type>::value)
                  value = std::make_optional<std::string>(convert_text(strText));
               #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
               else if constexpr (is_delphi_string<used_type>::value)
                  value = strText;
               else if constexpr (std::is_same<std::wstring, used_type>::value || std::is_same<std::wstring&, used_type>::value)
                  value = std::make_optional<std::wstring>(strText.c_str());
               #elif defined BUILD_WITH_QT
               else if constexpr (is_qt_string<used_type>::value)
                  value = strText;
               else if constexpr (std::is_same<std::wstring, used_type>::value || std::is_same<std::wstring&, used_type>::value)
                  value = std::make_optional<std::wstring>(strText.toStdWString());
               #endif
               else
                  static_assert(dependent_false<ty>::value, "no valid type for GetCombobox");
               }
            }
         else { // no optional
            fw_String strText = get_text();
            if constexpr (is_number_param<ty>::value) {
               if(auto rep = cbsrep.find(strField); rep  != cbsrep.end()) {
                  std::string strVal = convert_text(strText);
                  if(auto it = rep->second.find(strVal); it != rep->second.end()) {
                     value = std::make_optional(it->second);
                     }
                  else {
                     std::ostringstream os;
                     os << "For the field \"" << strField << "\" in formular \"" << FormName() << "\"isn't a value for "
                        << strVal << " in the repository.";
                     throw std::runtime_error(os.str());
                     }
                  }
               else {
                  std::ostringstream os;
                  os << "No repository for field field \"" << strField << "\" in formular \"" << FormName() << "\"";
                  throw std::runtime_error(os.str());
                  }
               }
            else if constexpr (std::is_same<std::string, ty>::value || std::is_same<std::string&, ty>::value)
               value = convert_text(strText);
            #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
            else if constexpr (is_delphi_string<ty>::value)
               value = strText;
            else if constexpr (std::is_same<std::wstring, ty>::value || std::is_same<std::wstring&, ty>::value)
               value = strText.c_str();
            #elif defined BUILD_WITH_QT
            else if constexpr (is_qt_string<ty>::value)
               value = strText;
            else if constexpr (std::is_same<std::wstring, ty>::value || std::is_same<std::wstring&, ty>::value)
               value = std::make_optional<std::wstring>(strText.toStdWString());
            #endif
            else
               static_assert(dependent_false<ty>::value, "no valid type for GetCombobox");
            }
         }


      //-----
      template <EMyFrameworkType ft, typename ty>
      ty Value_in_list(std::string const& strField, size_t index) {
         #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
            auto GetFunc = [](auto* field, size_t index) { return field->Items->Strings[index]; };
         #elif defined BUILD_WITH_QT
            auto GetFunc = [](auto* field, size_t index) { return field->item(index)->text(); };
         #else
            #error Fehlende Implementierung für Count_in_list für dieses Framework
         #endif
         // GetText<ty>(
         ty ret;
         if constexpr (ft == EMyFrameworkType::combobox) {
            ret = GetText<ty>(GetFunc(Find<fw_Combobox>(strField), index));
            }
         if constexpr (ft == EMyFrameworkType::listbox) {
            ret = GetText<ty>(GetFunc(Find<fw_Listbox>(strField), index));
            }
         else static_assert_no_match();
         return ret;
         }

      template <EMyFrameworkType ft>
      size_t Count_in_list(std::string const& strField) {
         #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
            auto CountFunc = [](auto* field) { return field->Items->Count; };
         #elif defined BUILD_WITH_QT
            auto CountFunc = [](auto* field) { return field->count(); };
         #else
            #error Fehlende Implementierung für Count_in_list für dieses Framework
         #endif

         size_t ret;

         if constexpr (ft == EMyFrameworkType::combobox) {
            ret = CountFunc(Find<fw_Combobox>(strField));
            }
         if constexpr (ft == EMyFrameworkType::listbox) {
            ret = CountFunc(Find<fw_Listbox>(strField));
            }
         else static_assert_no_match();
         return ret;
         }


      //------------------------------------------------------------------------
      EMyRetResults ShowModal(void) {
         #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
           auto call = [](fw_Form* form) { return form->ShowModal(); };
           static const std::map<int, EMyRetResults> RetVals = {
               { mrOk,     EMyRetResults::ok },
               { mrYes,    EMyRetResults::yes },
               { mrNo,     EMyRetResults::no },
               { mrCancel, EMyRetResults::cancel }
              };
         #elif defined BUILD_WITH_QT
           auto call = [](fw_Form* form) {
              QDialog* dlg = dynamic_cast<QDialog*>(form);
              if (dlg == nullptr) throw std::runtime_error("keine Umwandlung für QDialog möglich");
              return dlg->exec(); 
              };
           static const std::map<int, EMyRetResults> RetVals = {
               { QDialog::Accepted,     EMyRetResults::ok },
               { 2,                     EMyRetResults::yes },
               { 3,                     EMyRetResults::no },
               { QDialog::Rejected,     EMyRetResults::cancel }
              };
         #else
           #error fehlende Implementierung für ShowModal()
         #endif

         if(auto myret = RetVals.find(call(Form())); myret != RetVals.end()) {
            return myret->second;
            }
         else {
            return EMyRetResults::unknown;
            }
         }

   private:
      fw_Form* Form(void) {
         if(form == nullptr) throw std::runtime_error("Critical error, member form in TMyForm is nullptr");
         return form;
         }

      fw_Form const* Form(void) const {
         if(form == nullptr) throw std::runtime_error("Critical error, member form in TMyForm is nullptr");
         return form;
         }

      //------------------------------------------------------------------------------
      template <typename ty>
      ty* Find(std::string const& strField) {
         #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
           auto *comp = Form()->FindComponent(strField.c_str());
         #elif defined BUILD_WITH_QT
           auto* comp = Form()->findChild<QObject* >(QString::fromStdString(strField));
         #else
           #error keine Umsetzung von Find für dieses Framework
         #endif
         if(!comp) {
            std::ostringstream os;
            os << "A field with the name \"" << strField << "\" don\'t exist in the formular \""
               << FormName() << "\".";
            throw std::runtime_error(os.str().c_str());
            }
         ty* field = dynamic_cast<ty*>(comp);
         if(!field) {
            std::ostringstream os;
            // type name for gcc and clanf mangled
            // #include <boost/core/demangle.hpp>
            // boost::core::demangle(...);
            os << "A field \"" << strField << "\" exist in the formular \"" << FormName()
               << "\", but can\'t cast to requested type " << typeid(ty).name() <<".";
            throw std::runtime_error(os.str().c_str());
            }
         return field;
         }


      // -----------------------------------------------------------------------
      template <typename ty>
      void SetFunction(std::function<void (fw_String const& val)> func, ty const& value, int iLen = -1, int iScale = -1) {
         #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
            auto convert_string  = [](std::string const& strValue) { return fw_String(strValue.c_str());  };
            auto convert_wstring = [](std::wstring const& strValue) { return fw_String(strValue.c_str()); };
         #elif defined BUILD_WITH_QT
            auto convert_string  = [](std::string const& strValue) { return QString::fromStdString(strValue); };
            auto convert_wstring = [](std::wstring const& strValue) { return QString::fromStdWString(strValue); };
         #else
           #error keine Definition von SetFunction im gewählten Framework
         #endif
         if constexpr (is_optional<ty>::value) {
            using used_type  = typename std::remove_reference<typename std::remove_cv<decltype(*value)>::type>::type;
            if(!value.has_value())  func("");
            else {
               if constexpr      (is_cpp_narrow_string<used_type>::value)   func(convert_string(*value));
               if constexpr      (is_cpp_wide_string<used_type>::value)     func(convert_wstring(*value));
               #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
               else if constexpr (is_delphi_string<used_type>::value)       func(*value);
               #elif defined BUILD_WITH_QT
               else if constexpr (is_qt_string<used_type>::value)           func(*value);
               #endif
               else if constexpr (is_wchar_param<used_type>::value)         func(*value);
               else if constexpr (is_char_param<used_type>::value)          func(*value);
               else if constexpr (std::is_integral<used_type>::value && !std::is_same<used_type, bool>::value) {
                  func(convert_string(TMyTools::integral_to_string_fmt<used_type>(*value)));
                  }
               else if constexpr (std::is_floating_point<used_type>::value) {
                  func(convert_string(TMyTools::double_to_string_fmt(*value, iScale)));
                  }
               else {
                  std::ostringstream os;
                  if(iLen >= 0)   os << std::setw(iLen);
                  if(iScale >= 0) {
                     os.setf(std::ios::showpoint);
                     os.setf(std::ios::fixed);
                     os << std::setprecision(iScale);
                     }
                  os << *value;
                  func(convert_string(os.str()));
                  }
               }
            }
         else {
            //using used_type = typename std::remove_all_extents<typename std::remove_const<typename std::remove_reference<typename std::remove_cv<ty>::type>::type>::type>::type;
            using used_type = ty;
            if constexpr      (is_cpp_narrow_string<ty>::value)                   func(convert_string(value));
            if constexpr      (is_cpp_wide_string<ty>::value)                     func(convert_wstring(value));
            #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
            else if constexpr (is_delphi_string<used_type>::value)                func(value);
            #elif defined BUILD_WITH_QT
            else if constexpr (is_qt_string<used_type>::value)                    func(value);
            #endif
            else if constexpr (is_wchar_param<used_type>::value)                  func(value);
            else if constexpr (is_char_param<used_type>::value)                   func(value);
            else if constexpr (std::is_integral<used_type>::value && !std::is_same<used_type, bool>::value) {
               func(convert_string(TMyTools::integral_to_string_fmt<ty>(value)));
               }
            else if constexpr (std::is_floating_point<used_type>::value) {
               func(convert_string(TMyTools::double_to_string_fmt(value, iScale)));
               }
            else {
               std::ostringstream os;
               if(iLen >= 0)   os << std::setw(iLen);
               if(iScale >= 0) {
                  os.setf(std::ios::showpoint);
                  os.setf(std::ios::fixed);
                  os << std::setprecision(iScale);
                  }
               os << value;
               func(convert_string(os.str()));
               }
            }
         return;
         }

      template <typename ty>
      ty GetText(fw_String const& value) {
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
              #error fehlende Implementierung for GetText in diesem Framwork
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
      void SetCombobox(fw_Combobox* field, ty const& value, mapRepVals const& rep = mapRepVals()) {
         #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
         auto get_index = [this, &field]() { return field->ItemIndex;  };
         auto set_index = [this, &field](int val) { return field->ItemIndex = val;  };
         auto set_to = [this, &field](fw_String const& strSeek) {
            field->ItemIndex = -1;
            for (int i = 0; i < field->Items->Count; ++i) {
               if (strSeek == field->Items->Strings[i]) {
                  field->ItemIndex = i;
                  break;
                  }
               }
            if (field->ItemIndex == -1) {
               std::stringstream os;
               os << "Element \"" << AnsiString(strSeek).c_str() << "\" not found!";
               throw std::runtime_error(os.str().c_str());
               }
            };
         #elif defined BUILD_WITH_QT
           auto get_index = [&field]() { return field->currentIndex(); };
           auto set_index = [&field](int val) { return field->setCurrentIndex(val); };
           auto set_to = [&field](fw_String const& strSeek) { 
              field->setCurrentText(strSeek);
              };
         #else
           #error Fehlende Implementierung für interne SetCombobox Funktion in diesem Framework
         #endif
         if constexpr (is_optional<ty>::value) {
            if (!value) {
               set_index( -1);
               }
            else {
               using used_type = decltype(*value);
               #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
               if constexpr (is_delphi_string<used_type>::value)            set_to(*value);
               else if constexpr (is_cpp_string<used_type>::value)          set_to(value->c_str());
               else if constexpr (is_wchar_or_char_param<used_type>::value) set_to(*value);
               #elif defined BUILD_WITH_QT
               if constexpr (is_qt_string<used_type>::value)                set_to(*value);
               else if constexpr (is_cpp_narrow_string<used_type>::value) {
                  auto txt = QString::fromStdString(*value);
                  txt.remove(QChar::Null);
                  set_to(txt);
                  }
               else if constexpr (is_cpp_wide_string<used_type>::value)     set_to(QString::fromStdWString(*value));
               else if constexpr (is_wchar_or_char_param<used_type>::value) set_to(QString(*value));
               #endif
               else if constexpr (is_number_param<used_type>::value) {
                  int iVal = static_cast<int>(*value);
                  auto it = std::find_if(rep.begin(), rep.end(), [iVal](auto const& val) {
                     return iVal == val.second;
                     });
                  if (it != rep.end()) {
                     #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
                     set_to(it->first.c_str());
                     #elif defined BUILD_WITH_QT
                     set_to(QString::fromStdString(it->first()));
                     #endif
                     }
                  else {
                     std::stringstream os;
                     os << "Element with ID = " << iVal << " not found!";
                     throw std::runtime_error(os.str().c_str());
                     }
                  }
               else  static_assert_no_match();
               }
            }
         else {
            #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
            if constexpr (is_delphi_string<ty>::value)                set_to(value);
            else if constexpr (is_cpp_string<ty>::value)              set_to(value.c_str());
            else if constexpr (is_wchar_or_char_param<ty>::value)     set_to(value);
            #elif defined BUILD_WITH_QT
            if constexpr (is_qt_string<ty>::value)                set_to(value);
            else if constexpr (is_cpp_narrow_string<ty>::value) {
               QString txt = QString::fromStdString(value);
               txt.remove(QChar::Null);
               set_to(txt);
               }
            else if constexpr (is_cpp_wide_string<ty>::value)     set_to(QString::fromStdWString(value));
            else if constexpr (is_wchar_or_char_param<ty>::value) set_to(QString(value));
            #endif
            else if constexpr (is_number_param<ty>::value) {
               int iVal = static_cast<int>(value);
               auto it = std::find_if(rep.begin(), rep.end(), [iVal](auto const& val) {
                  return iVal == val.second;
                  });
               if (it != rep.end()) {
                  #if defined BUILD_WITH_VCL || defined BUILD_WITH_FMX
                  set_to(it->first.c_str());
                  #elif defined BUILD_WITH_QT
                  set_to(QString::fromStdString(it->first()));
                  #endif
                  }
               else {
                  std::stringstream os;
                  os << "Element with ID = " << iVal << " not found!";
                  throw std::runtime_error(os.str().c_str());
                  }
               }
            else   static_assert_no_match();
            }
      }

   };



template <EMyFrameworkType ft, typename ty>
struct my_formlist_iterator {
   using iterator_category = std::input_iterator_tag;
   using value_type = ty;
   using difference_type = std::ptrdiff_t;
   using reference = value_type;
   using pointer = const value_type*;

   my_formlist_iterator() = default;
   my_formlist_iterator(TMyForm* frm, std::string const& fld) : 
      form(frm), strField(fld) {
      ++* this;
   }


   my_formlist_iterator& operator = (std::pair<TMyForm*, std::string> para) {
      form     = para.first;
	  strField = para.second;
      start_pos = 0;
      return *this;
      }
 
   my_formlist_iterator& operator = (my_formlist_iterator const& ref) {
      form      = ref.form;
      strField  = ref.strField;
      start_pos = ref.start_pos;
      return *this;
      }

   reference operator*() const { if (!form) throw std::runtime_error("xxx"); return form->Value_in_list<ft, ty>(strField, start_pos - 1); }
   //pointer operator->() const { return &theLine; }

 
   my_formlist_iterator& operator++() {
      if (form) {
         if(start_pos < form->Count_in_list<ft>(strField)) start_pos++;
         else {
            form  = nullptr;
			   strField = "";
            }
         }
      return *this;
      }

   my_formlist_iterator operator++(int) {
      auto copy(*this);
      ++* this;
      return copy;
   }
   
   friend bool operator==(const my_formlist_iterator& x, const my_formlist_iterator& y) {
     if (x.form == nullptr && y.form == nullptr) return true;
	  if(x.form == nullptr || y.form == nullptr) return false;
	  if(x.form == y.form) {
         return x.strField == y.strField;
	     }
      else return false;
      }

   friend bool operator!=(const my_formlist_iterator& x, const my_formlist_iterator& y) {
      return !(x == y);
   }

private:
   TMyForm *form = nullptr;
   std::string strField = "";
   size_t start_pos = 0;

};


template <EMyFrameworkType ft, typename ty>
struct my_formlist {
   my_formlist(TMyForm* frm, std::string const& fld) : form(frm), strField(fld) { }
   my_formlist(my_formlist<ft, ty> const& ref) : form(ref.form), strField(ref.strField) { }
   my_formlist_iterator<ft, ty> begin() const { return my_formlist_iterator<ft, ty>(form, strField); }
   my_formlist_iterator<ft, ty> end() const { return my_formlist_iterator<ft, ty>(); }

private:
   TMyForm *form;
   std::string strField;
};



#endif

