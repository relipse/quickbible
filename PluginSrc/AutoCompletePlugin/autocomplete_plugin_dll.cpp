//---------------------------------------------------------------------------
#include <vcl.h>
#include <windows.h>
//---------------------------------------------------------------------------
//   Important note about DLL memory management when your DLL uses the
//   static version of the RunTime Library:
//
//   If your DLL exports any functions that pass String objects (or structs/
//   classes containing nested Strings) as parameter or function results,
//   you will need to add the library MEMMGR.LIB to both the DLL project and
//   any other projects that use the DLL.  You will also need to use MEMMGR.LIB
//   if any other projects which use the DLL will be performing new or delete
//   operations on any non-TObject-derived classes which are exported from the
//   DLL. Adding MEMMGR.LIB to your project will change the DLL and its calling
//   EXE's to use the BORLNDMM.DLL as their memory manager.  In these cases,
//   the file BORLNDMM.DLL should be deployed along with your DLL.
//
//   To avoid using BORLNDMM.DLL, pass string information using "char *" or
//   ShortString parameters.
//
//   If your DLL uses the dynamic version of the RTL, you do not need to
//   explicitly add MEMMGR.LIB as this will be done implicitly for you
//---------------------------------------------------------------------------

#include <string>
using namespace std;

#include "Bible.h"
using namespace TearsForNations;

#include "autocomplete_plugin_dropdown_form.h"
#include "autocomplete_plugin_about_form.h"
char buffer[100];

#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
       if (reason == DLL_PROCESS_ATTACH)
       {
          FrmAutoComplete = new TFrmAutoComplete(NULL);
       }
       else if (reason == DLL_PROCESS_DETACH)
       {
          delete FrmAutoComplete;
       }
       return 1;
}
//---------------------------------------------------------------------------
extern "C"
{
   char* __declspec(dllexport) Plugin_GetName()
   {
      return "Auto-Complete Plugin";
   }

   char* __declspec(dllexport) Plugin_GetVersion()
   {
      return "0.01";
   }

   char* __declspec(dllexport) Plugin_OnLookupBoxKeyDown(WORD &Key, TShiftState Shift)
   {
      if (Key == VK_RETURN && Shift.Contains(ssCtrl))
      {
        FrmAutoComplete->Show();
        Key = 0;
      }
      if (Key == VK_ESCAPE)
      {
         //unshow the form
         FrmAutoComplete->Close();
      }
      return NULL;
   }

   char* pbuffer = buffer;
   int last_len = 0;

   char* __declspec(dllexport) Plugin_OnLookupBoxChange(const char* searchBoxText, Bible* selectedBible)
   {
     //cool little about box
     if (strcmp(searchBoxText, "aboutac ") == 0){
        FrmAutoCompleteQuickBiblePluginAbout = new TFrmAutoCompleteQuickBiblePluginAbout(NULL);
        FrmAutoCompleteQuickBiblePluginAbout->ShowModal();
        delete FrmAutoCompleteQuickBiblePluginAbout;
        buffer[0] = '\0';
        return pbuffer;
     }


     //add items based on text in box
     AnsiString text(searchBoxText);

     int len = strlen(searchBoxText);
     if (len < 2 || len < last_len || ' ' != searchBoxText[len-1]){
        FrmAutoComplete->Hide();
        last_len = len;
        return NULL;
     }
     else if (!FrmAutoComplete->Visible && len > 0)
     {
        //FrmAutoComplete->Show();
        //not showing
        FrmAutoComplete->cmbDropDown->Items->Clear();
        FrmAutoComplete->fnHtmlListBox1->Items->Clear();
        FrmAutoComplete->fnHtmlListBox1->Items->Add("Search for <b>" + text + "</b>");
        
     }
     //int verses_found = selectedBible->findAllCount(text.SubString(1, text.Length()-1).c_str() );


     //FrmAutoComplete->cmbDropDown->Items->Add(String(verses_found) +" verses found");


     //if (text.Pos("") == 0)
     //{

       FrmAutoComplete->cmbDropDown->ItemIndex = 0;
     //}


     if (FrmAutoComplete->cmbDropDown->Items->Count > 0)
     {
       FrmAutoComplete->Show();
       FrmAutoComplete->cmbDropDown->DroppedDown = true;
     }








     last_len = len;
     return NULL;
   }
}
//---------------------------------------------------------------------------

