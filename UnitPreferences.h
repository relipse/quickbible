//---------------------------------------------------------------------------

#ifndef UnitPreferencesH
#define UnitPreferencesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "NxColumnClasses.hpp"
#include "NxColumns.hpp"
#include "NxCustomGrid.hpp"
#include "NxCustomGridControl.hpp"
#include "NxGrid.hpp"
#include "NxScrollControl.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>

#include "SqliteBible.h"
#include "CallDLLInterfaceFunction.h"
#include "NxThemesSupport.hpp"
//---------------------------------------------------------------------------
struct PluginMenuItem
{
   CallDLLInterface::FullDLLFunction fullDLLfunc;
   int index;
};
//---------------------------------------------------------------------------
class TFrmPreferences : public TForm
{
__published:	// IDE-managed Components
        TTreeView *TreeView1;
        TSplitter *Splitter1;
        TPanel *PnlPrefPlugins;
        TNextGrid *PluginsGrid;
        TNxCheckBoxColumn *nxCbxEnabled;
        TNxTextColumn *nxTxtName;
        TNxTextColumn *nxTxtVersion;
        TPanel *PleaseChoosePanel;
        TNxTextColumn *nxTxtPlugin;
        TButton *btnPluginsRefresh;
        TMemo *PluginsInfoMemo;
        TPanel *PnlPrefBibles;
        TNextGrid *BiblesGrid;
        TNxCheckBoxColumn *nxCbxInstalled;
        TNxTextColumn *nxTxtAbbreviation;
        TNxTextColumn *nxTxtDescription;
        TNxTextColumn *nxTxtFormat;
        TPanel *Panel1;
        TButton *Button1;
        TPanel *PnlPrefAppearance;
        TCheckBox *AppearanceCbxStayOnTop;
        TNxTextColumn *nxTxtBibleFolder;
        TNxTextColumn *nxTxtPluginFolder;
        TPanel *PnlPrefAdvanced;
        TButton *AdvancedBtnClearCache;
        TNextGrid *AdvancedCacheGrid;
        TNxTextColumn *NxTextColumn1;
        TNxMemoColumn *NxMemoColumn1;
        TNxTimeColumn *NxTimeColumn1;
        TNxTextColumn *NxTextColumn2;
        TRichEdit *BiblesInfoMemo;
        TCheckBox *cbxAppearanceShowSearchOptions;
        void __fastcall OnLoad(TObject *Sender);
        void __fastcall PluginsGridCellClick(TObject *Sender, int ACol,
          int ARow);
        void __fastcall TreeView1Click(TObject *Sender);
        void __fastcall btnPluginsRefreshClick(TObject *Sender);
        void __fastcall BiblesGridCellClick(TObject *Sender, int ACol,
          int ARow);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall BiblesGridAfterRowMove(TObject *Sender,
          int FromPos, int ToPos);
        void __fastcall AppearanceCbxStayOnTopClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall AdvancedBtnClearCacheClick(TObject *Sender);
        void __fastcall cbxAppearanceShowSearchOptionsClick(
          TObject *Sender);
private:	// User declarations
   void __fastcall WmDropFiles(TWMDropFiles& Message);
public:		// User declarations
 BEGIN_MESSAGE_MAP
   MESSAGE_HANDLER(WM_DROPFILES, TWMDropFiles, WmDropFiles)
 END_MESSAGE_MAP(TForm)

        void __fastcall CleanPluginNonEvents(TStringList* PluginFunctions);
        bool __fastcall InstallPlugin(AnsiString Path);
        bool __fastcall AddPlugin(AnsiString Path, bool startInstalled = true);
        void __fastcall PopulatePluginsControl();

        TStringList* Plugins;
        std::map<std::string, bool> plugin_disabled;

        String PluginsPath;
        String BiblesPath;


        //return the number of iterations
        int DetachFunctionInDLL(const std::string& DLL, const CallDLLInterface::LowLevelDLLFunction& lowDLLfunc);

        void __fastcall InstallBible(AnsiString Path);
        TearsForNations::Bible* __fastcall AddBible(AnsiString Path);
        void __fastcall PopulateBiblesControl();

        __fastcall TFrmPreferences(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmPreferences *FrmPreferences;
//---------------------------------------------------------------------------
#endif
