#pragma once

#include "MyStdTypes.h"

#if defined BUILD_WITH_VCL
   EMyFramework inline currentFramework = EMyFramework::vcl;

   #include <System.Classes.hpp>
   #include <Vcl.Forms.hpp>
   #include <Vcl.StdCtrls.hpp>
   #include <Vcl.ComCtrls.hpp>

   struct is_delpi_compatible : std::true_type {};
   struct is_vcl_compile : std::true_type {};
   struct is_fmx_compile : std::false_type {};
   struct is_qt_compile : std::false_type {};

   using TForm        = Vcl::Forms::TForm;
   using TGroupBox    = Vcl::Stdctrls::TGroupBox;
   using TEdit        = Vcl::Stdctrls::TEdit;
   using TLabel       = Vcl::Stdctrls::TLabel;
   using TButton      = Vcl::Stdctrls::TButton;
   using TCheckBox    = Vcl::Stdctrls::TCheckBox;
   using TListBox     = Vcl::Stdctrls::TListBox;
   using TComboBox    = Vcl::Stdctrls::TComboBox;
   using TMemo        = Vcl::Stdctrls::TMemo;
   using TStatusBar   = Vcl::Comctrls::TStatusBar;
   using TListView    = Vcl::Comctrls::TListView;
   using TPageControl = Vcl::Comctrls::TPageControl;

   using fw_String    = UnicodeString;
   using fw_Form      = TForm;
   using fw_Groupbox  = TGroupBox;
   using fw_Edit      = TEdit;
   using fw_Memo      = TMemo;
   using fw_Label     = TLabel;
   using fw_Combobox  = TComboBox;
   using fw_Listbox   = TListBox;
   using fw_Checkbox  = TCheckBox;
   using fw_Button    = TButton;
   using fw_Table     = TListView;
   using fw_Statusbar = TStatusBar;

#elif defined BUILD_WITH_FMX
   EMyFramework inline currentFramework = EMyFramework::fmx;

   #include <System.Classes.hpp>
   #include <Fmx.Dialogs.hpp>
   #include <System.UITypes.hpp>
   #include <Fmx.Forms.hpp>
   #include <Fmx.StdCtrls.hpp>
   #include <Fmx.Edit.hpp>
   #include <Fmx.ListBox.hpp>
   #include <Fmx.Grid.hpp>
   #include <Fmx.Grid.Style.hpp>
   #include <Fmx.TabControl.hpp>

   struct is_delpi_compatible : std::true_type {};
   struct is_vcl_compile : std::false_type {};
   struct is_fmx_compile : std::true_type {};
   struct is_qt_compile : std::false_type {};

   using TForm       = Fmx::Forms::TForm;
   using TGroupBox   = Fmx::Stdctrls::TGroupBox;
   using TEdit       = Fmx::Edit::TEdit;
   using TLabel      = Fmx::Stdctrls::TLabel;
   using TButton     = Fmx::Stdctrls::TButton;
   using TCheckBox   = Fmx::Stdctrls::TCheckBox;
   using TComboBox   = Fmx::Listbox::TComboBox;
   using TListBox    = Fmx::Listbox::TListBox;
   using TComboBox   = Fmx::Listbox::TComboBox;
   using TMemo       = Fmx::Memo::TMemo;
   using TStringGrid = Fmx::Grid::TStringGrid;
   using TTabControl = Fmx::Tabcontrol::TTabControl;

   using fw_String    = UnicodeString;
   using fw_Form      = TForm;
   using fw_Groupbox  = TGroupBox;
   using fw_Edit      = TEdit;
   using fw_Memo      = TMemo;
   using fw_Label     = TLabel;
   using fw_Combobox  = TComboBox;
   using fw_Listbox   = TListBox;
   using fw_Checkbox  = TCheckBox;
   using fw_Button    = TButton;
   using fw_Table     = TStringGrid;
   using fw_Statusbar = TStatusBar;

#elif defined BUILD_WITH_QT
   EMyFramework inline currentFramework = EMyFramework::qt;

   #include <QApplication>
   #include <QMessageBox>
   #include <QWidget>
   #include <QDialog>
   #include <QGroupBox>
   #include <QLineEdit>
   #include <QTextEdit>
   #include <QPushButton>
   #include <QComboBox>
   #include <QListWidget>
   #include <QCheckBox>
   #include <QTableWidget>
   #include <QString>
   #include <QStatusBar>


   struct is_delpi_compatible : std::false_type {};
   struct is_vcl_compile : std::false_type {};
   struct is_fmx_compile : std::false_type {};
   struct is_qt_compile : std::true_type {};

   using fw_String    = QString;
   using fw_Form      = QWidget;
   using fw_Groupbox  = QGroupBox;
   using fw_Edit      = QLineEdit;
   using fw_Memo      = QTextEdit;
   using fw_Label     = QLabel;
   using fw_Combobox  = QComboBox;
   using fw_Listbox   = QListWidget;
   using fw_Checkbox  = QCheckBox;
   using fw_Button    = QPushButton;
   using fw_Table     = QTableWidget;
   using fw_Statusbar = QStatusBar;

#else
  EMyFramework inline currentFramework = EMyFramework::unknown;
  #error Ein Framework muss gewählt sein, um diese Bibliothek zu nutzen
#endif

template <EMyFrameworkType ft> struct MyFrameworkSelect { using type = void; };
template <> struct MyFrameworkSelect<EMyFrameworkType::edit> { using type = fw_Edit;  };
template <> struct MyFrameworkSelect<EMyFrameworkType::label> { using type = fw_Label; };
template <> struct MyFrameworkSelect<EMyFrameworkType::groupbox> { using type = fw_Groupbox; };
template <> struct MyFrameworkSelect<EMyFrameworkType::button> { using type = fw_Button; };
template <> struct MyFrameworkSelect<EMyFrameworkType::checkbox> { using type = fw_Checkbox; };
template <> struct MyFrameworkSelect<EMyFrameworkType::combobox> { using type = fw_Combobox; };
template <> struct MyFrameworkSelect<EMyFrameworkType::memo> { using type = fw_Memo; };
template <> struct MyFrameworkSelect<EMyFrameworkType::statusbar> { using type = fw_Statusbar; };
template <> struct MyFrameworkSelect<EMyFrameworkType::listbox> { using type = fw_Listbox; };
template <> struct MyFrameworkSelect<EMyFrameworkType::listview> { using type = fw_Table; };
