//---------------------------------------------------------------------------

#ifndef formDllExportTesterH
#define formDllExportTesterH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "CodeEdit.hpp"
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmDllExportTester : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TLabel *Label3;
        TEdit *EdtDllPath;
        TPanel *Panel2;
        TPanel *Panel3;
        TListBox *LbxDllFuncs;
        TButton *Button1;
        TGroupBox *GroupBox1;
        TLabel *Label2;
        TLabel *Label4;
        TButton *btnCallDllFunction;
        TEdit *EdtArg1;
        TEdit *EdtArg2;
        TRadioButton *rdoReturnCharPtr;
        TRadioButton *rdoReturnInt;
        TRadioButton *rdoReturnVoid;
        TMemo *MmoFunc;
        TLabel *Label1;
        TButton *btnShowCppExample;
        TButton *btnLoadLibrary;
        TButton *btnFreeLibrary;
        TCodeEdit *mmoCallExample;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall btnCallDllFunctionClick(TObject *Sender);
        void __fastcall LbxDllFuncsClick(TObject *Sender);
        void __fastcall EdtDllPathKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall ShowDLLFuncs(TObject *Sender);
        void __fastcall btnLoadLibraryClick(TObject *Sender);
        void __fastcall btnFreeLibraryClick(TObject *Sender);
private:	// User declarations
   void __fastcall WmDropFiles(TWMDropFiles& Message);
public:		// User declarations
        void __fastcall GenerateCppExample(String dll, String function, TStringList* args);
 BEGIN_MESSAGE_MAP
   MESSAGE_HANDLER(WM_DROPFILES, TWMDropFiles, WmDropFiles)
 END_MESSAGE_MAP(TForm)
        __fastcall TFrmDllExportTester(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmDllExportTester *FrmDllExportTester;
//---------------------------------------------------------------------------
#endif
