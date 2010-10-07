//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "NxEdit.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
#include "SqliteBible.h"
#include <Menus.hpp>
#include "fnHtmlListBox.h"
#include <Buttons.hpp>

#include <list>
using namespace TearsForNations;

struct CachedSearchResult
{
    std::string search_text;
    BibleSearchRange searchRange;
    Bible* bible;

    AnsiString ResultRichText;
    std::list<std::string> plugins_enabled;
    int verseCount;
    long startTime;
    long endTime;
};


class TfrmQuickBible : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TRichEdit *rchVerseResults;
        TPanel *Panel2;
        TNxButtonEdit *edtLookup;
        TComboBox *cmbBibleVersions;
        TStatusBar *StatusBar1;
        TProgressBar *gagProgress;
        TPopupMenu *PopupMenu1;
        TMenuItem *mniPreferences;
        TMenuItem *mniAbout;
        TMenuItem *mniRefresh;
        TMenuItem *N1;
        TMenuItem *Find1;
        TfnHtmlListBox *LbxAutoComplete;
        TMenuItem *Copy1;
        TMenuItem *N2;
        TMenuItem *SearchOptions1;
        TMenuItem *IgnoreCase1;
        TMenuItem *NewTestament1;
        TMenuItem *OldTestament1;
        TMenuItem *N3;
        TMenuItem *N4;
        TPanel *pnlSearchOptions;
        TRadioButton *rdoSearchAll;
        TRadioButton *rdoSearchNT;
        TRadioButton *rdoSearchOT;
        TMenuItem *ShowHideSearchBar1;
        TCheckBox *cbxIgnoreCase;
        TPanel *pnlFind;
        TSpeedButton *btnFind;
        TEdit *edtFind;
        TCheckBox *cbxFindWholeWord;
        TCheckBox *cbxMatchCase;
        void __fastcall edtLookupButtonClick(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall edtLookupKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall mniPreferencesClick(TObject *Sender);
        void __fastcall FormPaint(TObject *Sender);
        void __fastcall mniAboutClick(TObject *Sender);
        void __fastcall mniRefreshClick(TObject *Sender);
        void __fastcall mniPluginItemClick(TObject *Sender);
        void __fastcall rchVerseResultsMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall edtLookupChange(TObject *Sender);
        void __fastcall ChooseAutoCompleteItem(TObject *Sender);
        void __fastcall LbxAutoCompleteKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall btnFindClick(TObject *Sender);
        void __fastcall Find1Click(TObject *Sender);
        void __fastcall edtFindKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall edtFindKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall rchVerseResultsKeyPress(TObject *Sender,
          char &Key);
        void __fastcall edtFindExit(TObject *Sender);
        void __fastcall Copy1Click(TObject *Sender);
        void __fastcall OldNewTestament1Click(TObject *Sender);
        void __fastcall rdoSearchOTNTBothClick(TObject *Sender);
        void __fastcall ShowHideSearchBar1Click(TObject *Sender);
        void __fastcall cbxIgnoreCaseClick(TObject *Sender);
        void __fastcall rchVerseResultsMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall edtFindKeyPress(TObject *Sender, char &Key);
public:	// User declarations

        void __fastcall ClearResults();
        void __fastcall StartSearch(char* search_text);
        void __fastcall FinishSearch();

        std::vector<Bible*> vctBibles;
        bool CancelLookup;

        void __fastcall BibleVectorSearch();

        std::multimap< std::string, CachedSearchResult > mapSearchCache;

        multimap<std::string, CachedSearchResult>::iterator
            super_find(const std::string& key, const CachedSearchResult& srch)
        {
          multimap<std::string, CachedSearchResult>::iterator itr = mapSearchCache.find(key);
          if (itr == mapSearchCache.end()){ return itr; }

          multimap<std::string, CachedSearchResult>::iterator lastElement;
          lastElement = mapSearchCache.upper_bound(key);
          for ( ; itr != lastElement; ++itr)
          {
                //search text is same, now check bible and search range
		if (itr->second.bible == srch.bible &&
                    itr->second.searchRange == srch.searchRange)
                {
                   return itr;
                }
          }
          return mapSearchCache.end(); //not found
        }

        void __fastcall GetPartialMatches(String searchStr, TStringList* slPartialMatches, bool AllowDuplicates = false)
        {
           multimap<std::string, CachedSearchResult>::iterator itr;
           slPartialMatches->Clear();
           for (itr = mapSearchCache.begin(); itr != mapSearchCache.end(); ++itr)
           {
              if (itr->first.find(searchStr.c_str()) == 0)
              {
                if (AllowDuplicates ||
                  //duplicate not found
                   slPartialMatches->IndexOf(itr->first.c_str()) == -1)
                {
                   slPartialMatches->Add(itr->first.c_str());
                }

              }
           }

        }

        void __fastcall BibleFindSearch();

        //some events that will be useful for plugins
        String __fastcall OnParseVerseResult(Bible* curBible);

        int __fastcall OnAfterBibleVerseLoad(Bible* bible);

        void __fastcall UpdateBibleComboBox();

        int VersesFoundCounter;
        
        CachedSearchResult LastSearch;

        bool OpeningNewInstance;

        void __fastcall SaveSettings();
        String __fastcall GetSettingsIniPath();

        void __fastcall LoadSettings();
        void __fastcall LoadSettingsFromParameters();

public:		// User declarations
        __fastcall TfrmQuickBible(TComponent* Owner);
};

void OnFoundBibleVerse(Bible* bible, FullBibleVerse* verse, bool& abort);
//---------------------------------------------------------------------------
extern PACKAGE TfrmQuickBible *frmQuickBible;
//---------------------------------------------------------------------------


#endif
