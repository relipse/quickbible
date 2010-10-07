//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "RunSilent.h"
#include "CallDLLInterfaceFunction.h"
#include "UnitPreferences.h"
#include "UnitUtils.h"
#include "BibleVerseParse.h"
#include "Win32LoadLibHelper.h"
using namespace CallDLLInterface;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "NxEdit"
#pragma link "fnHtmlListBox"
#pragma resource "*.dfm"
TfrmQuickBible *frmQuickBible;
//---------------------------------------------------------------------------
__fastcall TfrmQuickBible::TfrmQuickBible(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmQuickBible::edtLookupButtonClick(TObject *Sender)
{
  BibleVectorSearch();
}
//---------------------------------------------------------------------------
String __fastcall TfrmQuickBible::GetSettingsIniPath()
{
   static String ini_location_stored = "";
   if (ini_location_stored != ""){ return ini_location_stored; }

   String ini_location;
   if (Win32LoadLibHelper::GetLocalAppDataPath(ini_location))
   {
      if (!DirectoryExists(ini_location + "\\QuickBible"))
      {
         CreateDir(ini_location+"\\QuickBible");
      }
      ini_location += "\\QuickBible\\QuickBible.ini";
   }
   else
   {
      ini_location = ChangeFileExt( Application->ExeName, ".ini" );
   }
   ini_location_stored = ini_location;
}
//---------------------------------------------------------------------------
void __fastcall TfrmQuickBible::SaveSettings()
{
   try
   {

       TIniFile *ini;
       ini = new TIniFile( GetSettingsIniPath() );
       if (FormStyle == fsStayOnTop)
       {
          ini->WriteString("Appearance","FormStyle","StayOnTop");
       }
       else if (FormStyle == fsNormal)
       {
          ini->WriteString("Appearance","FormStyle","Normal");
       }
       ini->WriteBool("Appearance","ShowSearchOptions", pnlSearchOptions->Visible);


       ini->WriteString("Settings", "Bible", cmbBibleVersions->Text);
       ini->WriteString("Settings","SearchText", edtLookup->Text);



       if (NewTestament1->Checked && OldTestament1->Checked)
         ini->WriteString("Settings","SearchWhere", "BOTH" );
       else if (NewTestament1->Checked)
         ini->WriteString("Settings","SearchWhere", "NT" );
       else if (OldTestament1->Checked)
         ini->WriteString("Settings","SearchWhere", "OT" );

       ini->WriteBool("Settings","IgnoreCase", IgnoreCase1->Checked );

       ini->WriteString("Settings","Results", StringReplace(rchVerseResults->Text, "\r\n", "\\r\\n", TReplaceFlags() << rfReplaceAll));
       delete ini;
   }
   catch(...)
   {
      //do nothing, don't save
      MessageDlg("Can not save settings.", mtError, TMsgDlgButtons() << mbOK, 0);
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmQuickBible::LoadSettingsFromParameters()
{
    int paramCount = ParamCount();
    if (paramCount > 0)
    {
      bool searchNow = false;
      bool calcDebug = false;
      String debug;
      for (int i = 1; i <= paramCount; ++i)
      {
         if (ParamStr(i) == "-Debug")
         {
           calcDebug = true;
         }
         else if (ParamStr(i) == "-IgnoreCase")
         {
           IgnoreCase1->Checked = true;
           cbxIgnoreCaseClick(IgnoreCase1);
         }
         else if (ParamStr(i) == "-LeftPos")
         {
           String strLeft = ParamStr(++i);
           //ShowMessage(strLeft);
           int left = StrToIntDef(strLeft,-99999);
           if (left != -99999)
             this->Left = left;
         }
         else if (ParamStr(i) == "-TopPos")
         {
           String strTop = ParamStr(++i);
           //ShowMessage(strTop);
           int top = StrToIntDef(strTop,-99999);
           if (top != -99999)
             this->Top = top;
         }
         else if (ParamStr(i) == "-ShowSearchOpts")
         {
           FrmPreferences->cbxAppearanceShowSearchOptions->Checked =
              true;
           ShowHideSearchBar1Click(FrmPreferences->cbxAppearanceShowSearchOptions);
         }
         else if (ParamStr(i) == "-SearchWhere")
         {
             String searchWhere = ParamStr(++i);
             if (searchWhere == "BOTH")
             {
                rdoSearchAll->Checked = true;
                rdoSearchOTNTBothClick(rdoSearchAll);
             }
             else if (searchWhere == "OT")
             {
                rdoSearchOT->Checked = true;
                rdoSearchOTNTBothClick(rdoSearchOT);
             }
             else if (searchWhere == "NT")
             {
                rdoSearchNT->Checked = true;
                rdoSearchOTNTBothClick(rdoSearchNT);
             }
         }
         else if (ParamStr(i) == "-Bible")
         {
           String bibleSelected = ParamStr(++i);
           for (int i = 0; i < cmbBibleVersions->Items->Count; ++i)
           {
              if (cmbBibleVersions->Items->Strings[i] == bibleSelected){
                 cmbBibleVersions->ItemIndex = i;
                 break;
              }
           }
         }
         //this should be the last flag in the parameters
         else if (ParamStr(i) == "-SearchText")
         {
            String searchText = ParamStr(++i);
            for(int j = i+1;j<paramCount;++j)
            {
              String jstr = ParamStr(j);
              searchText += " " + jstr;
            }
            edtLookup->Text = searchText;
            searchNow = true;
            edtLookupButtonClick(NULL);
         }
      }//end foreach parameter
      if (debug.Length() > 0){
        ShowMessage(debug);
      }
    }
}

void __fastcall TfrmQuickBible::LoadSettings()
{
   TIniFile *ini;
   ini = new TIniFile( GetSettingsIniPath() );
   String formStyle = ini->ReadString("Appearance","FormStyle","Normal");
   if (formStyle == "StayOnTop")
      FormStyle = fsStayOnTop;
   else
      FormStyle = fsNormal;
   FrmPreferences->AppearanceCbxStayOnTop->Checked = (FormStyle == fsStayOnTop);

   FrmPreferences->cbxAppearanceShowSearchOptions->Checked =
      ini->ReadBool("Appearance","ShowSearchOpts", false);
   ShowHideSearchBar1Click(FrmPreferences->cbxAppearanceShowSearchOptions);

   String bibleSelected = ini->ReadString("Settings", "Bible", "NKJV");
   edtLookup->Text = ini->ReadString("Settings","SearchText", "Jesus*truth");
   String searchWhere = ini->ReadString("Settings","SearchWhere", "BOTH" );
   if (searchWhere == "BOTH")
   {
      rdoSearchAll->Checked = true;
      rdoSearchOTNTBothClick(rdoSearchAll);
   }
   else if (searchWhere == "OT")
   {
      rdoSearchOT->Checked = true;
      rdoSearchOTNTBothClick(rdoSearchOT);
   }
   else if (searchWhere == "NT")
   {
      rdoSearchNT->Checked = true;
      rdoSearchOTNTBothClick(rdoSearchNT);
   }

   IgnoreCase1->Checked = ini->ReadBool("Settings","IgnoreCase", true);
   cbxIgnoreCaseClick(IgnoreCase1);

   rchVerseResults->Text = StringReplace(ini->ReadString("Settings","Results", "C++Builder Version"), "\\r\\n", "\r\n", TReplaceFlags() << rfReplaceAll);
   //select bible from the ini file
   for (int i = 0; i < cmbBibleVersions->Items->Count; ++i)
   {
      if (cmbBibleVersions->Items->Strings[i] == bibleSelected){
         cmbBibleVersions->ItemIndex = i;
         break;
      }
   }
   delete ini;
   //edtLookupButtonClick(NULL);

}

//---------------------------------------------------------------------------
void __fastcall TfrmQuickBible::FormDestroy(TObject *Sender)
{
  for (unsigned int i = 0; i < vctBibles.size(); ++i)
  {
    delete vctBibles[i];
  }
}
//---------------------------------------------------------------------------

String __fastcall TfrmQuickBible::OnParseVerseResult(Bible* curBible)
{
    AnsiString format = AnsiString(curBible->getFormat().c_str());
    //can be RTF, HTML, or TEXT


    std::string functionToCall = std::string("_Plugin_On") + format.UpperCase().c_str() + "ParseVerseResult";
    AnsiString VerseAfterParsed = curBible->getLastFoundScripture().c_str();


    map<string, list<LowLevelDLLFunction> > & FuncsToDLLs =
       GlobalAppCallDLL.HashedDLLFuncs.loaded_func_to_dlls;



   //**WARNING** ORDER MATTERS!!!
   if (FuncsToDLLs.find(functionToCall) != FuncsToDLLs.end())
   {
      list<LowLevelDLLFunction>::iterator i;
      list<LowLevelDLLFunction>::iterator end;

      for (i = FuncsToDLLs[functionToCall].begin(), end = FuncsToDLLs[functionToCall].end(); i != end; ++i)
      {
         if (i->farproc != NULL)
         {
             typedef char* (*fptChar_DLLFunc_Char)(char*);
             VerseAfterParsed = ((fptChar_DLLFunc_Char)i->farproc)(VerseAfterParsed.c_str());
         }
      }
   }

   //set title to full verse

   
   String nwCaption = VerseAfterParsed;
   Application->Title = nwCaption;
   Caption = nwCaption;


   return VerseAfterParsed;
}

//return the # of plugins loaded and executed
int __fastcall TfrmQuickBible::OnAfterBibleVerseLoad(Bible* bible)
{
//dunno ..
}
//---------------------------------------------------------------------------
void __fastcall TfrmQuickBible::edtLookupKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
   //fire the plugins to handle change of the search box
   //auto-complete, etc.

   /********* disable these plugins for now ****************
   std::string functionToCall = std::string("_Plugin_OnLookupBoxKeyDown");

   map<string, list<LowLevelDLLFunction> > & FuncsToDLLs =
       GlobalAppCallDLL.HashedDLLFuncs.loaded_func_to_dlls;

   //**WARNING** ORDER MATTERS!!!
   if (FuncsToDLLs.find(functionToCall) != FuncsToDLLs.end())
   {
      list<LowLevelDLLFunction>::iterator i;
      list<LowLevelDLLFunction>::iterator end;

      for (i = FuncsToDLLs[functionToCall].begin(), end = FuncsToDLLs[functionToCall].end(); i != end; ++i)
      {
         if (i->farproc != NULL)
         {
             typedef char* (*fptChar_DLLFunc_Char)(WORD&, TShiftState);
             char* nwText = ((fptChar_DLLFunc_Char)i->farproc)(Key, Shift);
             if (nwText != NULL)
             {
                edtLookup->Text = nwText;
             }
         }
      }
   }
   **********/
   /**
   if (LbxAutoComplete->Visible && LbxAutoComplete->Items->Count > 0)
   {
     if (Key == VK_TAB){
       Key = VK_DOWN;
      //ChooseAutoCompleteItem(NULL);
     }

     else if (Key == VK_DOWN)
     {
       LbxAutoComplete->SetFocus();
       LbxAutoComplete->ItemIndex++;
     }
     else if (Key == VK_UP)
     {
        LbxAutoComplete->ItemIndex = LbxAutoComplete->Items->Count-1;
        LbxAutoComplete->SetFocus();
     }
   }
   **/
   if (Key == VK_RETURN)
   {
      BibleVectorSearch();
   }
}

void __fastcall TfrmQuickBible::ClearResults()
{
  VersesFoundCounter = 0;
  rchVerseResults->Clear();
  StatusBar1->SimpleText = "";
}

void __fastcall TfrmQuickBible::StartSearch(char* search_text)
{
   CachedSearchResult & srch = LastSearch;
   srch.startTime = GetTickCount();
   edtLookup->ButtonCaption = "X";
   gagProgress->Visible = true;
   srch.search_text = search_text;
}
void __fastcall TfrmQuickBible::FinishSearch()
{
   CachedSearchResult & srch = LastSearch;
   srch.endTime = GetTickCount();
   StatusBar1->SimpleText = String(VersesFoundCounter) + " verses found.";
   edtLookup->ButtonCaption = "Go";
   gagProgress->Visible = false;
   long totaltime = srch.endTime - srch.startTime;
   StatusBar1->SimpleText = StatusBar1->SimpleText + " " + String(totaltime / 1000.0) + " seconds.";
}
//---------------------------------------------------------------------------
void __fastcall TfrmQuickBible::BibleVectorSearch()
{
     VersesFoundCounter = 0;
     //LbxAutoComplete->Visible = false;
     if (gagProgress->Visible){
        CancelLookup = true;
        return;
     }
     
     if (vctBibles.size() == 0)
     {
         //hmm, no bibles exist or none are selected?
         //TODO: prompt to install a bible?
         return;
     }
     if (cmbBibleVersions->ItemIndex == -1)
     {
         //no bible is selected (umm this should never happen)
         return;
     }
      String nwCaption = edtLookup->Text + String(" (") + vctBibles[cmbBibleVersions->ItemIndex]->getAbbreviation().c_str() + ")";
      Application->Title = nwCaption;
      Caption = nwCaption;
      
      CachedSearchResult & srch = LastSearch;

      srch.startTime = GetTickCount();

      StatusBar1->SimpleText = "Vector Search...";

      //Do a vector search instead of edtLookupButtonClick(Sender);
      edtLookup->ButtonCaption = "X";
      gagProgress->Visible = true;

      srch.bible = vctBibles[cmbBibleVersions->ItemIndex];

      srch.startTime = GetTickCount();
      if (IgnoreCase1->Checked)
      {
        //case-insensitive uses LIKE instead of GLOB, therefore replace * with %
        srch.search_text = StringReplace(edtLookup->Text,"*","%", TReplaceFlags() << rfReplaceAll).c_str();
      }
      else
        srch.search_text = edtLookup->Text.c_str();

      bool usedCache = false;

      //did we search for this before??? CACHE IS DISABLED NOW
      std::multimap<std::string, CachedSearchResult>::iterator found_in_cache =
         mapSearchCache.end();
          //super_find(srch.search_text, srch);
          
      if (found_in_cache != mapSearchCache.end())
      {
             usedCache = true;
             VersesFoundCounter = found_in_cache->second.verseCount;
             rchVerseResults->Text = found_in_cache->second.ResultRichText;
             srch.endTime = GetTickCount();
      }
      else //normal search, not in cache
      {
          rchVerseResults->Clear();

          BibleVerseTokens tokens;
          int bk;

          if (VerseRangeTokenize(srch.search_text.c_str(), tokens) &&
              (bk = srch.bible->AbbrToBook(const_cast<char*>(tokens.book.c_str()))))
          {
             srch.bible->getScriptureVerseRange(bk, tokens.ch, tokens.vs_min, tokens.vs_max);
          }
          else
          {
             //this could take a while -- we populate the results via callback event
             srch.searchRange.ignore_case = IgnoreCase1->Checked;

             //determine verse minimum and maximums
             if (OldTestament1->Checked){srch.searchRange.min_abs_vs = 1;}
             else
              if (NewTestament1->Checked){srch.searchRange.min_abs_vs = 23146;}

             if (NewTestament1->Checked){srch.searchRange.max_abs_vs = 31103;}
             else
              if (OldTestament1->Checked){srch.searchRange.max_abs_vs = 23145;}

             srch.bible->findAll(srch.search_text, &srch.searchRange);
          }
          //rchVerseResults->Lines->Delete(0); //delete first line

          //rchVerseResults->SelAttributes->Height -= 2;
          rchVerseResults->Lines->Add(String()+ srch.bible->getAbbreviation().c_str() + " - " + srch.bible->getDescription().c_str());
          //rchVerseResults->SelAttributes->Height += 2;
          //rchVerseResults->SelStart = 0;

          srch.verseCount = VersesFoundCounter;
          srch.endTime = GetTickCount();


          //if the lookup was NOT cancelled, store search results in cache
          if (!CancelLookup) //SUCCESS!!!
          {
             //by now the RichEdit is done being filled with the results
             srch.ResultRichText = rchVerseResults->Text;
             mapSearchCache.insert(pair<string, CachedSearchResult>(srch.search_text, srch));
             FrmPreferences->AdvancedCacheGrid->AddRow(1);
             int row = FrmPreferences->AdvancedCacheGrid->LastAddedRow;
             FrmPreferences->AdvancedCacheGrid->Cell[0][row]->AsString = srch.search_text.c_str();
             FrmPreferences->AdvancedCacheGrid->Cell[1][row]->AsString = srch.bible->getAbbreviation().c_str();
             FrmPreferences->AdvancedCacheGrid->Cell[2][row]->AsDateTime = Now();
             FrmPreferences->AdvancedCacheGrid->Cell[3][row]->AsInteger = srch.verseCount;
             FrmPreferences->AdvancedCacheGrid->BestFitColumns(bfBoth);
          }
      }
      //Did the search get cancelled? Don't store the results into a cache
      if (CancelLookup)
      {
         CancelLookup = false;
      }
      else
      {
         StatusBar1->SimpleText = String(VersesFoundCounter) + " verses found.";
      }


      edtLookup->ButtonCaption = "Go";
      gagProgress->Visible = false;


      long totaltime = srch.endTime - srch.startTime;

      StatusBar1->SimpleText = StatusBar1->SimpleText + " " + String(totaltime / 1000.0) + " seconds.";
      if (usedCache)
         StatusBar1->SimpleText = StatusBar1->SimpleText + " Used cache.";
      if (rdoSearchOT->Checked){
         StatusBar1->SimpleText = StatusBar1->SimpleText + " (Old Testament only)";
      }else if (rdoSearchNT->Checked){
         StatusBar1->SimpleText = StatusBar1->SimpleText + " (New Testament only)";
      }else if (rdoSearchAll->Checked){
         StatusBar1->SimpleText = StatusBar1->SimpleText + " (Full Bible)";
      }
}

void __fastcall TfrmQuickBible::BibleFindSearch()
{

  if (gagProgress->Visible){
     CancelLookup = true;
     return;
  }
  edtLookup->ButtonCaption = "X";
  gagProgress->Visible = true;
  Bible* bible = vctBibles[cmbBibleVersions->ItemIndex];
  StatusBar1->SimpleText = "Find Next Search...";
      long starttime = GetTickCount();
      long endtime;

  int bk = 0, ch = 1, vs = 1;
  bool verse_ref = false;
  //probably a bible reference (ie. Mat 1)
  if (edtLookup->Text.Pos(" ") == 4)
  {
    bk = Bible::AbbrToBook(edtLookup->Text.c_str());
    if (bk)
    {
       int posColon = edtLookup->Text.Pos(":");
       if (posColon == 0){ posColon = edtLookup->Text.Length(); }

       ch = edtLookup->Text.SubString(5, posColon -5).ToIntDef(1);
       vs = edtLookup->Text.SubString(posColon + 1, 99).ToIntDef(1);
    }
  }
  std::string verse;
  int count = 0;
  if (bk == 0)
  {
     verse = bible->find(edtLookup->Text.c_str());
  }
  else //get verse 1
  {
     verse_ref = true;
     verse = bible->getScripture(bk, ch, vs);
     if (verse == "")
     {
       verse = ((SqliteBible*)bible)->getSqlite3DB()->getErrorMsg();
     }
  }
  rchVerseResults->Clear();
  while (verse != "" && !CancelLookup)
  {
     StatusBar1->SimpleText = String(++count) + " verses found.";
     Application->ProcessMessages();

     gagProgress->Position = bible->getLastFoundAbsoluteVerse();

     AnsiString VerseAfterParsed = OnParseVerseResult(bible);

     rchVerseResults->Lines->Add(VerseAfterParsed + " - " + String(Bible::BookToAbbr(bible->getLastFoundVerse().bk).c_str()) +
        " " + String(bible->getLastFoundVerse().ch) + ":" + String(bible->getLastFoundVerse().vs) +
        + " (" + (bible->getVersion().c_str()) + ")" );
     if (bk > 0){ break; }
     rchVerseResults->Lines->Add("");
     verse = bible->findNext(edtLookup->Text.c_str());
  }

  gagProgress->Visible = false;
  gagProgress->Position = 0;
  if (CancelLookup)
  {
    CancelLookup = false;
    StatusBar1->SimpleText = StatusBar1->SimpleText + " - Cancelled search.";
  }
  edtLookup->ButtonCaption = "Go";

      endtime = GetTickCount();
      long totaltime = endtime - starttime;

      StatusBar1->SimpleText = StatusBar1->SimpleText + " " + String(totaltime / 1000.0) + " seconds.";

  OnAfterBibleVerseLoad(bible);
}

void __fastcall TfrmQuickBible::mniPreferencesClick(TObject *Sender)
{
   FrmPreferences->Show();        
}
//---------------------------------------------------------------------------
void __fastcall TfrmQuickBible::UpdateBibleComboBox()
{
  vctBibles.erase(vctBibles.begin(), vctBibles.end());
  cmbBibleVersions->Clear();
  for (int i = 0; i < FrmPreferences->BiblesGrid->RowCount; i++)
  {
     Bible* curBible = static_cast<SqliteBible*>(FrmPreferences->BiblesGrid->Row[i]->Data);
     //add events
     curBible->setEventOnFoundBibleVerse(OnFoundBibleVerse);
     vctBibles.push_back(curBible);
  }

  for (unsigned int i = 0; i < vctBibles.size(); ++i)
  {
    cmbBibleVersions->Items->Add(((SqliteBible*)vctBibles[i])->getAbbreviation().c_str());
  }
  cmbBibleVersions->ItemIndex = 0;
}

void __fastcall TfrmQuickBible::FormPaint(TObject *Sender)
{
  //must do this
  this->OnPaint = NULL;

  FrmPreferences->OnLoad(this);
  UpdateBibleComboBox();

  //parameters should override ini file
  LoadSettings();
  LoadSettingsFromParameters();
}
//---------------------------------------------------------------------------

void __fastcall TfrmQuickBible::mniAboutClick(TObject *Sender)
{
   ShowMessage(
      "Quick Bible 0.915 beta - Feb 2010\n\n"
      "As the bible being greatest book ever written, \n"
      "we have a right to copy and give it to the whole world. \n"
      "And the gospel must first be published among all nations. \n-Mar 13:10\n"
      "Thanks also to Berg Component Suite \n"
   );
}
//---------------------------------------------------------------------------

void __fastcall TfrmQuickBible::mniRefreshClick(TObject *Sender)
{
  //this basically deletes the cache before continuing
  multimap<std::string, CachedSearchResult>::
    iterator found = super_find(edtLookup->Text.c_str(), LastSearch);
  if (found != mapSearchCache.end())
  {
    mapSearchCache.erase(found);
  }
  edtLookupButtonClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmQuickBible::mniPluginItemClick(TObject *Sender)
{
   PluginMenuItem* pmi = (PluginMenuItem*)dynamic_cast<TMenuItem*>(Sender)->Tag;
   if (pmi)
   {
      ClearResults();
      StartSearch(edtLookup->Text.c_str());
      typedef bool (*Plugin_OnMenuItemExecute)(int, const vector<Bible*>&, unsigned int, const char*, char*&);
      char* ptr_str = NULL;
      ((Plugin_OnMenuItemExecute)pmi->fullDLLfunc.farproc)(pmi->index, vctBibles,
         cmbBibleVersions->ItemIndex, edtLookup->Text.c_str(), ptr_str);
      if (ptr_str != NULL)
      {
        rchVerseResults->Text = ptr_str; //OnParseVerseResult(vctBibles[cmbBibleVersions->ItemIndex]);
      }
      FinishSearch();
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmQuickBible::rchVerseResultsMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if (Shift.Contains(ssCtrl))
   {
      //if no selection, select 2 words
      if (rchVerseResults->SelLength == 0)
      {
         return;
         
         long result, original;
         original = rchVerseResults->SelStart;

         //result = SendMessage(rchVerseResults->Handle, EM_CHARFROMPOS,
         //                      0, MAKELPARAM(X,Y));

         //result = SendMessage(rchVerseResults->Handle, EM_FINDWORDBREAK,
         //                     WB_MOVEWORDLEFT, 0);

         rchVerseResults->SelStart = result;

         result = SendMessage(rchVerseResults->Handle, EM_FINDWORDBREAK,
                              WB_MOVEWORDRIGHT, original - result);

         rchVerseResults->SelLength = result - rchVerseResults->SelStart;

      }
      if (Shift.Contains(ssAlt))  //open in new window
      {
        //set left and top to make it cascade when opening
        String paramStr = "-LeftPos " + IntToStr(this->Left + 20);
        paramStr += " -TopPos " + IntToStr(this->Top + 20);

        paramStr += " -Bible " + cmbBibleVersions->Text + " ";

        if (cbxIgnoreCase->Checked)
          paramStr += "-IgnoreCase ";



        if (pnlSearchOptions->Visible)
          paramStr += "-ShowSearchOpts ";

        if (rdoSearchAll->Checked)
          paramStr += "-SearchWhere BOTH ";
        else if (rdoSearchNT->Checked)
          paramStr += "-SearchWhere NT ";
        else if (rdoSearchOT->Checked)
          paramStr += "-SearchWhere OT ";

        paramStr += "-SearchText " + rchVerseResults->SelText;

        OpeningNewInstance = true;
        StatusBar1->SimpleText = "Spawning another QuickBible instance...";
        ShellExecute(
            NULL,	// handle to parent window
            "open",	// pointer to string that specifies operation to perform
            Application->ExeName.c_str(),	// pointer to filename or folder name string
            paramStr.c_str(),	// pointer to string that specifies executable-file parameters
            ExtractFilePath(Application->ExeName).c_str(),	// pointer to string that specifies default directory
            SW_SHOWNORMAL 	// whether file is shown when opened
           );
        StatusBar1->SimpleText = "Done.";
      }
      else //open in same window
      {
       edtLookup->Text = rchVerseResults->SelText;
       edtLookupButtonClick(NULL);
      }

   }        
}
//---------------------------------------------------------------------------



























//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void OnFoundBibleVerse(Bible* bible, FullBibleVerse* verse, bool& abort)
{
     frmQuickBible->StatusBar1->SimpleText = String(++frmQuickBible->VersesFoundCounter) + " verses found.";
     Application->ProcessMessages();

     frmQuickBible->gagProgress->Position = bible->getLastFoundAbsoluteVerse();

     AnsiString VerseAfterParsed = frmQuickBible->OnParseVerseResult(bible);

     frmQuickBible->rchVerseResults->Lines->Add(
     VerseAfterParsed + " - " + String(Bible::BookToAbbr(bible->getLastFoundVerse().bk).c_str()) +
        " " + String(bible->getLastFoundVerse().ch) + ":" + String(bible->getLastFoundVerse().vs) +
        " (" + String(bible->getVersion().c_str()) + ")"
     //other format
     /**String(Bible::BookToAbbr(bible->getLastFoundVerse().bk).c_str()) +
        " " + String(bible->getLastFoundVerse().ch) + ":" + String(bible->getLastFoundVerse().vs) +
        " - " + VerseAfterParsed**/

     );

     frmQuickBible->rchVerseResults->Lines->Add("");

     
    if (frmQuickBible->CancelLookup){
       abort = true;
       frmQuickBible->StatusBar1->SimpleText = frmQuickBible->StatusBar1->SimpleText + " Cancelled.";
    }
}
void __fastcall TfrmQuickBible::edtLookupChange(TObject *Sender)
{
   //fire the plugins to handle change of the search box
   //auto-complete, etc.
   BibleVerseTokens tokens;
   int bk;

   Bible* bible = vctBibles[cmbBibleVersions->ItemIndex];

   if (VerseRangeTokenize(edtLookup->Text.c_str(), tokens) &&
              (bk = bible->AbbrToBook(const_cast<char*>(tokens.book.c_str()))))
   {
     String sBook(bk);
     String sCh(tokens.ch);
     nsSQLITE3::quick_results qres = ((SqliteBible*)bible)->getSqlite3DB()->
        quick_exe("SElECT COUNT(*) FROM bible_verses WHERE b = " +
        std::string( sBook.c_str() )  + " AND c = " + sCh.c_str());

     String num_verses = qres.result[qres.ncol];
     if (qres.rc == SQLITE_OK)
     {
       String num_verses = qres.result[qres.ncol];
       if (num_verses != "0")
       {
         /**
         LbxAutoComplete->Items->Clear();
         LbxAutoComplete->Items->Add(num_verses + " verses. <b>" + String(bible->BookToAbbr(bk).c_str()) + " " + sCh + ":1-" + num_verses + "</b>");
         LbxAutoComplete->ItemIndex = 0;
         LbxAutoComplete->Height = LbxAutoComplete->Items->Count * 17 + 3;
         LbxAutoComplete->Width = 20*10;//LbxAutoComplete->
         LbxAutoComplete->Show();
         **/
       }
     }
     ((SqliteBible*)bible)->getSqlite3DB()->free(qres.result);
   }
   else //performing a search?
   {
         //LbxAutoComplete->Items->Clear();
         //LbxAutoComplete->Items->Add("<font color=clWhite>Search for <b>"+edtLookup->Text + "</b>");
   }

   TStringList* partialMatches = new TStringList();
   GetPartialMatches(edtLookup->Text, partialMatches, false);
   for (int i = 0; i < partialMatches->Count; ++i)
   {
      //LbxAutoComplete->Items->Add(partialMatches->Strings[i]);
   }

   delete partialMatches;

   /**
   LbxAutoComplete->ItemIndex = 0;
   LbxAutoComplete->Height = LbxAutoComplete->Items->Count * 17 + 3;
   LbxAutoComplete->Width = 20*10;//LbxAutoComplete->
   LbxAutoComplete->Show();
   **/
   /***** disable these plugins for now ********
   std::string functionToCall = std::string("_Plugin_OnLookupBoxChange");

   map<string, list<LowLevelDLLFunction> > & FuncsToDLLs =
       GlobalAppCallDLL.HashedDLLFuncs.loaded_func_to_dlls;

   //**WARNING** ORDER MATTERS!!!
   if (FuncsToDLLs.find(functionToCall) != FuncsToDLLs.end())
   {
      list<LowLevelDLLFunction>::iterator i;
      list<LowLevelDLLFunction>::iterator end;

      for (i = FuncsToDLLs[functionToCall].begin(), end = FuncsToDLLs[functionToCall].end(); i != end; ++i)
      {
         if (i->farproc != NULL)
         {
             typedef char* (*fptChar_DLLFunc_Char)(const char*, Bible*);
             char* nwText = ((fptChar_DLLFunc_Char)i->farproc)(edtLookup->Text.c_str(), vctBibles[cmbBibleVersions->ItemIndex]);
             if (nwText != NULL)
             {
                edtLookup->Text = nwText;
             }
         }
      }
   }
   *******************/
}
//---------------------------------------------------------------------------

void __fastcall TfrmQuickBible::ChooseAutoCompleteItem(TObject *Sender)
{
   //LbxAutoComplete->innerText[LbxAutoComplete->ItemIndex];
   //if (LbxAutoComplete->ItemIndex < 0){ LbxAutoComplete->ItemIndex = 0; }
   //int i = LbxAutoComplete->ItemIndex;
   //edtLookup->Text = LbxAutoComplete->TokenText[i][LbxAutoComplete->LastTokenIndex[i]];
   //LbxAutoComplete->ItemIndex = -1;
   //LbxAutoComplete->Hide();

   edtLookup->SetFocus();
   edtLookup->SelStart = edtLookup->Text.Length();
   edtLookup->SelLength = 0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmQuickBible::LbxAutoCompleteKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
  if (Key == VK_RETURN)
  {
     ChooseAutoCompleteItem(NULL);
  }
  else if (Key == VK_ESCAPE)
  {
     edtLookup->SetFocus();
     edtLookup->SelStart = edtLookup->Text.Length();
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmQuickBible::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  SaveSettings();
}
//---------------------------------------------------------------------------


void __fastcall TfrmQuickBible::btnFindClick(TObject *Sender)
{
  static int LastFoundAt = 0;
  static bool StopRecursion = false;

  int FoundAt, StartPos, ToEnd;
  // begin the search after the current selection 
  // if there is one 
  // otherwise, begin at the start of the text 
  if (rchVerseResults->SelLength)
    StartPos = rchVerseResults->SelStart + rchVerseResults->SelLength;
  else
    StartPos = 0;

  // ToEnd is the length from StartPos 
  // to the end of the text in the rich edit control

  ToEnd = rchVerseResults->Text.Length() - StartPos;

  FoundAt = rchVerseResults->FindText(edtFind->Text, StartPos, ToEnd, TSearchTypes());
  LastFoundAt = FoundAt;
  if (FoundAt != -1)
  {
    //rchVerseResults->SetFocus();
    rchVerseResults->SelStart = FoundAt;
    rchVerseResults->SelLength = edtFind->Text.Length();
    StatusBar1->SimpleText = "Found at " + IntToStr(FoundAt);
  }
  else //not found
  {
    rchVerseResults->SelStart = 0;
    StatusBar1->SimpleText = "Not found";
    if (!StopRecursion)
    {
      StopRecursion = true;
      btnFindClick(NULL);
    }
    else
    {
      StopRecursion = false;
    }
  }
}
//---------------------------------------------------------------------------
//show find panel if not showing, otherwise find next
void __fastcall TfrmQuickBible::Find1Click(TObject *Sender)
{
  if (!pnlFind->Visible)
  {
    pnlFind->Visible = true;
    edtFind->SetFocus();
    edtFind->SelectAll();
  }
  else
  {
     edtFind->SetFocus();
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmQuickBible::edtFindKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if (Key == VK_ESCAPE)
  {
     edtFind->Text = "";
     pnlFind->Hide();
  }
  else if (Key != VK_RETURN)
  {
     rchVerseResults->SelStart = 0;
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmQuickBible::edtFindKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
      if (Shift.Contains(ssCtrl) && Key == VK_BACK && edtFind->Text.Length() > 0)
      {
         String text = edtFind->Text;
         //delete backwards from cursor position until we reach a non-alpha
         bool start = true;
         for (int i = edtFind->SelStart; i >= 1; --i)
         {
            if (text[i] == ''){ text.Delete(i,1); continue; }
            
            if (start){
               text.Delete(i, 1);
            }else if (isalnum(text[i]))
            {
               text.Delete(i, 1);
            }
            else //not an alpha or numeric and not at start, just exit
            {
               break;
            }

             if (start == true){ start = false; }
         }
         edtFind->Text = text;
         edtFind->SelStart = text.Length();
         Key = 0;// gets generated
      }
      
  btnFindClick(NULL);
}
//---------------------------------------------------------------------------


void __fastcall TfrmQuickBible::rchVerseResultsKeyPress(TObject *Sender,
      char &Key)
{
  if (GetAsyncKeyState(VK_CONTROL) & 0x8000 ||
      GetAsyncKeyState(VK_ESCAPE) & 0x8000 ||
      GetAsyncKeyState(VK_BACK) & 0x8000 ||
      //GetAsyncKeyState(VK_SHIFT) & 0x8000 ||
      GetAsyncKeyState(VK_MENU) & 0x8000)
  {
    return;
  }

  //if (!pnlFind->Visible)
  {
    pnlFind->Visible = true;
    edtFind->SetFocus();
    edtFind->Text = edtFind->Text + String(Key);
    edtFind->SelStart = edtFind->Text.Length();
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmQuickBible::edtFindExit(TObject *Sender)
{
   edtFind->Clear();
   pnlFind->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmQuickBible::Copy1Click(TObject *Sender)
{
  rchVerseResults->CopyToClipboard();
}
//---------------------------------------------------------------------------

void __fastcall TfrmQuickBible::OldNewTestament1Click(TObject *Sender)
{
  if (!OldTestament1->Checked && !NewTestament1->Checked)
  {
     if (Sender == OldTestament1)
     {
        NewTestament1->Checked = true;
        rdoSearchNT->Checked = true;
     }
     else //clicking New Testament
     {
        OldTestament1->Checked = true;
        rdoSearchOT->Checked = true;
     }
  }

  if (OldTestament1->Checked && NewTestament1->Checked)
  {
    rdoSearchAll->Checked = true;
  }
  else if (OldTestament1->Checked)
  {
    rdoSearchOT->Checked = true;
  }
  else if (NewTestament1->Checked)
  {
    rdoSearchNT->Checked = true;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmQuickBible::rdoSearchOTNTBothClick(TObject *Sender)
{
   if (Sender == rdoSearchNT)
   {
      NewTestament1->Checked = true;
      OldTestament1->Checked = false;
   }
   else if (Sender == rdoSearchOT)
   {
      NewTestament1->Checked = false;
      OldTestament1->Checked = true;
   }
   else if (Sender == rdoSearchAll) //both OT and NT
   {
      NewTestament1->Checked = true;
      OldTestament1->Checked = true;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmQuickBible::ShowHideSearchBar1Click(TObject *Sender)
{
   if (Sender == FrmPreferences->cbxAppearanceShowSearchOptions)
   {
     pnlSearchOptions->Visible = FrmPreferences->cbxAppearanceShowSearchOptions->Checked;
   }
   else //right click menu
   {
     pnlSearchOptions->Visible = !pnlSearchOptions->Visible;
     FrmPreferences->cbxAppearanceShowSearchOptions->Checked = pnlSearchOptions->Visible;
   }
   

}
//---------------------------------------------------------------------------

void __fastcall TfrmQuickBible::cbxIgnoreCaseClick(TObject *Sender)
{
  if (Sender == cbxIgnoreCase)
    IgnoreCase1->Checked = cbxIgnoreCase->Checked;
  else if (Sender == IgnoreCase1)
    cbxIgnoreCase->Checked = IgnoreCase1->Checked;
}
//---------------------------------------------------------------------------





void __fastcall TfrmQuickBible::rchVerseResultsMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  if (Shift.Contains(ssCtrl) && rchVerseResults->SelLength > 0)
  {
     if (Shift.Contains(ssAlt)){
       StatusBar1->SimpleText = "Click to search for " + rchVerseResults->SelText + " (new instance)";
     }else{
       StatusBar1->SimpleText = "Click to search for " + rchVerseResults->SelText;
     }
  }

}
//---------------------------------------------------------------------------


void __fastcall TfrmQuickBible::edtFindKeyPress(TObject *Sender, char &Key)
{
 if (Key == ''){ Key = 0; }        
}
//---------------------------------------------------------------------------

