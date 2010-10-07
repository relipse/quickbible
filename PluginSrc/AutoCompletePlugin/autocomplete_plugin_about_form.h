//---------------------------------------------------------------------------

#ifndef autocomplete_plugin_about_formH
#define autocomplete_plugin_about_formH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFrmAutoCompleteQuickBiblePluginAbout : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TButton *Button1;
        void __fastcall Label4Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmAutoCompleteQuickBiblePluginAbout(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmAutoCompleteQuickBiblePluginAbout *FrmAutoCompleteQuickBiblePluginAbout;
//---------------------------------------------------------------------------
#endif
