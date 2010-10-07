//---------------------------------------------------------------------------

#ifndef autocomplete_plugin_dropdown_formH
#define autocomplete_plugin_dropdown_formH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "fnHtmlListBox.h"
//---------------------------------------------------------------------------
class TFrmAutoComplete : public TForm
{
__published:	// IDE-managed Components
        TComboBox *cmbDropDown;
        TfnHtmlListBox *fnHtmlListBox1;
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall cmbDropDownCloseUp(TObject *Sender);
        void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
        void __fastcall cmbDropDownExit(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmAutoComplete(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmAutoComplete *FrmAutoComplete;
//---------------------------------------------------------------------------
#endif
