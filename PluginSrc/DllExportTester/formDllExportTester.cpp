//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "formDllExportTester.h"
#include "CallDLLInterfaceFunction.h"
#include "LowLevelDLLFunction.h"
#include "listdllfunctions.hpp"
using namespace CallDLLInterface;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CodeEdit"
#pragma resource "*.dfm"
TFrmDllExportTester *FrmDllExportTester;
//---------------------------------------------------------------------------
__fastcall TFrmDllExportTester::TFrmDllExportTester(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmDllExportTester::FormCreate(TObject *Sender)
{
  DragAcceptFiles(Handle, true);
  if (ParamCount() > 0)
  {
     EdtDllPath->Text = ParamStr(1);
     ShowDLLFuncs(NULL);
  }
}
//---------------------------------------------------------------------------
 void __fastcall TFrmDllExportTester::WmDropFiles(TWMDropFiles& Message)
 {
   char buff[MAX_PATH];
   HDROP hDrop = (HDROP)Message.Drop;
   int numFiles = 
      DragQueryFile(hDrop, -1, NULL, NULL);
   for (int i=0;i < numFiles;i++) {
      DragQueryFile(hDrop, i, buff, sizeof(buff));
      AnsiString Path(buff);
      if (ExtractFileExt(Path).UpperCase() == ".DLL")
      {
        EdtDllPath->Text = Path;
        ShowDLLFuncs(NULL);
      }
   }
   DragFinish(hDrop);
 }
//---------------------------------------------------------------------------
void __fastcall TFrmDllExportTester::GenerateCppExample(String dll, String function, TStringList* args)
{
       mmoCallExample->Clear();
       mmoCallExample->Lines->Add("HINSTANCE hDLL = LoadLibrary(\"" + dll + "\");");
       mmoCallExample->Lines->Add("typedef char* (CALLBACK* p_cstr_function_cstr)(char*);");
       mmoCallExample->Lines->Add("if (hDLL){");
       mmoCallExample->Lines->Add("   p_cstr_function_cstr func;");
       mmoCallExample->Lines->Add("   char* return_value;");
       mmoCallExample->Lines->Add("   func = (p_cstr_function_cstr)GetProcAddress(hDLL,\"" + function + "\");");
       mmoCallExample->Lines->Add("   if (func){");
       mmoCallExample->Lines->Add("      return_value = func(\""+EdtArg1->Text+"\");");
       mmoCallExample->Lines->Add("   }");
       mmoCallExample->Lines->Add("   FreeLibrary(hDLL);");
       mmoCallExample->Lines->Add("}");
}
//---------------------------------------------------------------------------
void __fastcall TFrmDllExportTester::btnCallDllFunctionClick(TObject *Sender)
{
  if (MmoFunc->Lines->Count == 0){
     ShowMessage("Cannot call empty function!");
     MmoFunc->SetFocus();
     return;
  }
  String dll = EdtDllPath->Text.c_str();
  String function = MmoFunc->Lines->Strings[0].c_str();
  String Result = "<no return value>";
  if (rdoReturnCharPtr->Checked)
  {

  if (EdtArg1->Text.Length() > 0)
  {
     if (Sender == btnShowCppExample)
     {
       mmoCallExample->Clear();
       mmoCallExample->Lines->Add("HINSTANCE hDLL = LoadLibrary(\"" + dll + "\");");
       mmoCallExample->Lines->Add("typedef char* (CALLBACK* p_cstr_function_cstr)(char*);");
       mmoCallExample->Lines->Add("if (hDLL){");
       mmoCallExample->Lines->Add("   p_cstr_function_cstr func;");
       mmoCallExample->Lines->Add("   char* return_value;");
       mmoCallExample->Lines->Add("   func = (p_cstr_function_cstr)GetProcAddress(hDLL,\"" + function + "\");");
       mmoCallExample->Lines->Add("   if (func){");
       mmoCallExample->Lines->Add("      return_value = func(\""+EdtArg1->Text+"\");");
       mmoCallExample->Lines->Add("   }");
       mmoCallExample->Lines->Add("   FreeLibrary(hDLL);");
       mmoCallExample->Lines->Add("}");
     }
     else
     {
       Result = GlobalAppCallDLL.Str_Function_Str(dll.c_str(), function.c_str(), EdtArg1->Text.c_str());
     }
  }
  else
  {
     if (Sender == btnShowCppExample)
     {
       mmoCallExample->Clear();
       mmoCallExample->Lines->Add("HINSTANCE hDLL = LoadLibrary(\"" + dll + "\");");
       mmoCallExample->Lines->Add("typedef char* (CALLBACK* p_cstr_function_void)(void);");
       mmoCallExample->Lines->Add("if (hDLL){");
       mmoCallExample->Lines->Add("   p_cstr_function_void func;");
       mmoCallExample->Lines->Add("   char* return_value;");
       mmoCallExample->Lines->Add("   func = (p_cstr_function_void)GetProcAddress(hDLL,\"" + function + "\");");
       mmoCallExample->Lines->Add("   if (func){");
       mmoCallExample->Lines->Add("      return_value = func(\""+EdtArg1->Text+"\");");
       mmoCallExample->Lines->Add("   }");
       mmoCallExample->Lines->Add("   FreeLibrary(hDLL);");
       mmoCallExample->Lines->Add("}");
     }
     else
     {
       Result = GlobalAppCallDLL.Str_Function(dll.c_str(), function.c_str());
     }
  }

  }
  else if (rdoReturnInt->Checked)
  {
    if (EdtArg1->Text.Length() > 0)
    {
       if (Sender == btnShowCppExample)
       {
         mmoCallExample->Clear();
         mmoCallExample->Lines->Add("HINSTANCE hDLL = LoadLibrary(\"" + dll + "\");");
         mmoCallExample->Lines->Add("typedef char* (CALLBACK* p_cstr_function_int)(int);");
         mmoCallExample->Lines->Add("if (hDLL){");
         mmoCallExample->Lines->Add("   p_cstr_function_int func;");
         mmoCallExample->Lines->Add("   char* return_value;");
         mmoCallExample->Lines->Add("   func = (p_cstr_function_int)GetProcAddress(hDLL,\"" + function + "\");");
         mmoCallExample->Lines->Add("   if (func){");
         mmoCallExample->Lines->Add("      return_value = func("+String(EdtArg1->Text.ToIntDef(0))+");");
         mmoCallExample->Lines->Add("   }");
         mmoCallExample->Lines->Add("   FreeLibrary(hDLL);");
         mmoCallExample->Lines->Add("}");
       }
       else
       {
          Result = GlobalAppCallDLL.Str_Function_Int(dll.c_str(), function.c_str(), EdtArg1->Text.ToIntDef(0) );
       }
    }
    else
    {
       if (Sender == btnShowCppExample)
       {
         mmoCallExample->Clear();
         mmoCallExample->Lines->Add("HINSTANCE hDLL = LoadLibrary(\"" + dll + "\");");
         mmoCallExample->Lines->Add("typedef char* (CALLBACK* p_int_function_void)(void);");
         mmoCallExample->Lines->Add("if (hDLL){");
         mmoCallExample->Lines->Add("   p_int_function_void func;");
         mmoCallExample->Lines->Add("   int return_value;");
         mmoCallExample->Lines->Add("   func = (p_int_function_void)GetProcAddress(hDLL,\"" + function + "\");");
         mmoCallExample->Lines->Add("   if (func){");
         mmoCallExample->Lines->Add("      return_value = func();");
         mmoCallExample->Lines->Add("   }");
         mmoCallExample->Lines->Add("   FreeLibrary(hDLL);");
         mmoCallExample->Lines->Add("}");
       }
       else
       {
          Result = String( GlobalAppCallDLL.Int_Function(dll.c_str(), function.c_str()) );
       }
    }
  }
  else if (rdoReturnVoid->Checked)
  {
     if (Sender == btnShowCppExample)
     {
       mmoCallExample->Clear();
       mmoCallExample->Lines->Add("HINSTANCE hDLL = LoadLibrary(\"" + dll + "\");");
       mmoCallExample->Lines->Add("typedef void (CALLBACK* p_void_function_void)(void);");
       mmoCallExample->Lines->Add("if (hDLL){");
       mmoCallExample->Lines->Add("   p_void_function_void func;");
       mmoCallExample->Lines->Add("   char* return_value;");
       mmoCallExample->Lines->Add("   func = (p_void_function_void)GetProcAddress(hDLL,\"" + function + "\");");
       mmoCallExample->Lines->Add("   if (func){");
       mmoCallExample->Lines->Add("      func();");
       mmoCallExample->Lines->Add("   }");
       mmoCallExample->Lines->Add("   FreeLibrary(hDLL);");
       mmoCallExample->Lines->Add("}");
     }
     else
     {
        GlobalAppCallDLL.GetFunction(dll.c_str(), function.c_str(), NULL)();
     }
  }
  if (Sender != btnShowCppExample) ShowMessage(Result);
  else {
    mmoCallExample->Language = lsCPlusPlus;
    mmoCallExample->Repaint();
  }
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllExportTester::LbxDllFuncsClick(TObject *Sender)
{
   if (LbxDllFuncs->ItemIndex >= 0)
   {
     MmoFunc->Text = LbxDllFuncs->Items->Strings[LbxDllFuncs->ItemIndex];
     btnShowCppExample->Click();
   }
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllExportTester::EdtDllPathKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
   if (Key == VK_RETURN)
   {
      ShowDLLFuncs(NULL);
   }
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllExportTester::ShowDLLFuncs(TObject *Sender)
{
        LbxDllFuncs->Clear();
        TStringList* dll_functions = new TStringList();
        String Path = EdtDllPath->Text;
        
        if (!ListDLLFunctions(Path, dll_functions))
        {
           PAS_ListDLLFunctions(Path, dll_functions);
        }


        if (dll_functions->Count > 0)
        {
           for (int i = 0; i < dll_functions->Count; ++i)
           {
              LbxDllFuncs->Items->Add(dll_functions->Strings[i]);
           }
        }
        else
        {
           ShowMessage("Error Listing Exported Functions");
        }
        delete dll_functions;
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllExportTester::btnLoadLibraryClick(TObject *Sender)
{
   GlobalAppCallDLL.HashedLibs.LoadHashedLibrary(EdtDllPath->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TFrmDllExportTester::btnFreeLibraryClick(TObject *Sender)
{
  GlobalAppCallDLL.HashedLibs.FreeHashedLibrary(EdtDllPath->Text.c_str());
}
//---------------------------------------------------------------------------

