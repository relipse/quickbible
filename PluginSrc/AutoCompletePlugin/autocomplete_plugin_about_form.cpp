//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "autocomplete_plugin_about_form.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmAutoCompleteQuickBiblePluginAbout *FrmAutoCompleteQuickBiblePluginAbout;
//---------------------------------------------------------------------------
__fastcall TFrmAutoCompleteQuickBiblePluginAbout::TFrmAutoCompleteQuickBiblePluginAbout(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFrmAutoCompleteQuickBiblePluginAbout::Label4Click(TObject *Sender)
{
   ShellExecute(Handle, "open",
      Label4->Caption.c_str(),
      "", "", SW_SHOWNORMAL    );

}
//---------------------------------------------------------------------------
 