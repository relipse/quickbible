//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "autocomplete_plugin_dropdown_form.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "fnHtmlListBox"
#pragma resource "*.dfm"
TFrmAutoComplete *FrmAutoComplete;
//---------------------------------------------------------------------------
__fastcall TFrmAutoComplete::TFrmAutoComplete(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmAutoComplete::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if (Key == VK_ESCAPE){ Close(); }        
}
//---------------------------------------------------------------------------
void __fastcall TFrmAutoComplete::cmbDropDownCloseUp(TObject *Sender)
{
   //do something?
   Close();
}
//---------------------------------------------------------------------------
void __fastcall TFrmAutoComplete::FormCloseQuery(TObject *Sender,
      bool &CanClose)
{
   Hide();        
}
//---------------------------------------------------------------------------
void __fastcall TFrmAutoComplete::cmbDropDownExit(TObject *Sender)
{
   Close();        
}
//---------------------------------------------------------------------------

