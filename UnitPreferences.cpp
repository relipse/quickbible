//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitPreferences.h"
#include "CallDLLInterfaceFunction.h"
#include "Unit1.h"
#include "LowLevelDLLFunction.h"
using namespace CallDLLInterface;
#include "SqliteBible.h"
using namespace TearsForNations;

#include "UnitUtils.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "NxColumnClasses"
#pragma link "NxColumns"
#pragma link "NxCustomGrid"
#pragma link "NxCustomGridControl"
#pragma link "NxGrid"
#pragma link "NxScrollControl"
#pragma link "NxThemesSupport"
#pragma resource "*.dfm"
TFrmPreferences *FrmPreferences;
//---------------------------------------------------------------------------
__fastcall TFrmPreferences::TFrmPreferences(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmPreferences::OnLoad(TObject *Sender)
{
    //go through command line parameters and set options if neccessary
    PluginsPath = ExtractFilePath(Application->ExeName) + "Plugins\\";
    BiblesPath =  ExtractFilePath(Application->ExeName) + "Bibles\\";
    int paramCount = ParamCount();
    if (paramCount > 0)
    {
    for (int i = 1; i <= paramCount; ++i)
    {
       if (ParamStr(i) == "-PluginsPath")
       {
          PluginsPath = ParamStr(++i);
          //add last slash if does not exist
          if (PluginsPath[PluginsPath.Length()] != '\\')
          {
             PluginsPath += '\\';
          }
       }
       else if (ParamStr(i) == "-BiblesPath")
       {
          BiblesPath = ParamStr(++i);
          if (BiblesPath[BiblesPath.Length()] != '\\')
          {
             BiblesPath += '\\';
          }
       }
       else if (ParamStr(i) == "-Debug"){
          ShowMessage("BiblesPath: " + BiblesPath + "\n"
                      "PluginsPath: " + PluginsPath);
       }
    }


    }

  PopulateBiblesControl();
  PopulatePluginsControl();
  TreeView1->OnClick(NULL);
  this->OnPaint = NULL;
}
//---------------------------------------------------------------------------
Bible* __fastcall TFrmPreferences::AddBible(AnsiString Path)
{
          SqliteBible* bible = new SqliteBible(ExtractFileName(Path).c_str(), Path.c_str());

          //remember to free this memory sometime

          BiblesGrid->AddRow(1);

          BiblesGrid->Row[BiblesGrid->LastAddedRow]->Data = bible;

          int Col = 0;


          BiblesGrid->Cell[Col++][BiblesGrid->LastAddedRow]->AsBoolean = true;
          BiblesGrid->Cell[Col++][BiblesGrid->LastAddedRow]->AsString = bible->getAbbreviation().c_str();
          BiblesGrid->Cell[Col++][BiblesGrid->LastAddedRow]->AsString = bible->getDescription().c_str();
          BiblesGrid->Cell[Col++][BiblesGrid->LastAddedRow]->AsString = bible->getFormat().c_str();
          BiblesGrid->Cell[Col++][BiblesGrid->LastAddedRow]->AsString = Path;
          return bible;
}
//---------------------------------------------------------------------------
void __fastcall TFrmPreferences::PopulateBiblesControl()
{
    //first free all the memory
    if (frmQuickBible->vctBibles.size() > 0)
    {
       for (unsigned int i = frmQuickBible->vctBibles.size() - 1; i >= 0; --i)
       {
          delete frmQuickBible->vctBibles[i];
       }
    }

    BiblesGrid->NoSelection();
    BiblesGrid->ClearRows();

    TStringList* BibleFiles;

    BibleFiles = ListFilesInDirectory(BiblesPath + "*.bible.sqlite3");
    for(int i = 0; i < BibleFiles->Count; ++i)
    {
       String Path = BiblesPath + BibleFiles->Strings[i];
       AddBible(Path);
    }
   delete BibleFiles;
   BiblesGrid->BestFitColumns(bfCells);
}


void __fastcall TFrmPreferences::CleanPluginNonEvents(TStringList* PluginFunctions)
{
   for (int i = PluginFunctions->Count-1; i >= 0; --i)
   {
      if (PluginFunctions->Strings[i].Pos("_Plugin_On") != 1)
      {
         PluginFunctions->Delete(i);
      }
   }
}

bool __fastcall TFrmPreferences::AddPlugin(AnsiString Path, bool startInstalled)
{
          String pluginName = GlobalAppCallDLL.Str_Function(Path.c_str(),
                              "_Plugin_GetName");
          String pluginVersion = GlobalAppCallDLL.Str_Function(Path.c_str(),
                              "_Plugin_GetVersion");

          PluginsGrid->AddRow(1);
          int Row = PluginsGrid->LastAddedRow;

          int Col = 0;

          //is this plugin enabled or disabled? well, by default it will be enabled,
          //but if the user unchecks it (manually disables it), then we must make sure not
          //to populate the hashedprocesses for the specific plugin dll
          PluginsGrid->Cell[Col++][Row]->AsBoolean = startInstalled;
          PluginsGrid->Cell[Col++][Row]->AsString = ExtractFileName(Path);
          PluginsGrid->Cell[Col++][Row]->AsString = pluginName;
          PluginsGrid->Cell[Col++][Row]->AsString = pluginVersion;
          PluginsGrid->Cell[Col++][Row]->AsString = ExtractFilePath(Path);


          if (startInstalled)
          {
             if (!InstallPlugin(Path))
             {
                //plugin failed to install correctly
                PluginsGrid->Cell[0][Row]->AsBoolean = false;
                return false;
             }
          }

          return true;

}

//install or re-install a plugin (it should already exist in the PluginsGrid
bool __fastcall TFrmPreferences::InstallPlugin(AnsiString Path)
{
          //for this current DLL, store all the functions from that DLL, that way we can easily
          //call a function with the same name in EACH DLL,
          //and also we have a list of all functions available for each DLL
          TStringList* PluginFunctions = new TStringList();
          try
          {
             ListDLLFunctions(Path, PluginFunctions);
          }
          catch(...)
          {
             //lets just try to call a bunch of functions that we think
             //might be in the DLL
             HANDLE DLLHandle = GlobalAppCallDLL.HashedLibs.LoadHashedLibrary(Path.c_str());
             if (DLLHandle)
             {
               vector<string> validPluginFunctions(11);
               validPluginFunctions.push_back("_Plugin_GetName");   //-- required
               validPluginFunctions.push_back("_Plugin_GetVersion"); //-- required
               validPluginFunctions.push_back("_Plugin_OnParseVerseResult");
               validPluginFunctions.push_back("_Plugin_OnRTFParseVerseResult");
               validPluginFunctions.push_back("_Plugin_OnHTMLParseVerseResult");
               validPluginFunctions.push_back("_Plugin_OnTEXTParseVerseResult");
               validPluginFunctions.push_back("_Plugin_OnAfterBibleVerseLoad");

               //for a menu item
               validPluginFunctions.push_back("_Plugin_GetMenuItemCount");
               validPluginFunctions.push_back("_Plugin_GetMenuItemName");
               validPluginFunctions.push_back("_Plugin_GetMenuItemHotKey");
               validPluginFunctions.push_back("_Plugin_OnMenuItemExecute");

               for (unsigned int i = 0; i < validPluginFunctions.size(); ++i)
               {
                  bool exists = AddIfDLLFunctionExists(DLLHandle,
                      const_cast<char*>(validPluginFunctions[i].c_str()), PluginFunctions);
                  if (!exists && (i == 0 || i == 1))
                  {
                     //either one of the required plugin functions do not exist
                     delete PluginFunctions;
                     return false;
                  }
               }
             }
             else
             {
               delete PluginFunctions;
               return false;
             }
          }
          //this is a menu item plugin
          int temp;
          if (PluginFunctions->Find("_Plugin_OnMenuItemExecute", temp) &&
              PluginFunctions->Find("_Plugin_GetMenuItemCount", temp) )
          {
              int numItems = GlobalAppCallDLL.Int_Function(Path.c_str(),
                              "_Plugin_GetMenuItemCount");

              for (int i = 0; i < numItems; ++i)
              {
                 TMenuItem* nwItem = new TMenuItem(frmQuickBible->PopupMenu1);
                 nwItem->OnClick = frmQuickBible->mniPluginItemClick;
                 nwItem->Caption = GlobalAppCallDLL.Str_Function_Int(Path.c_str(),
                              "_Plugin_GetMenuItemName", i);
                 nwItem->ShortCut = TextToShortCut(GlobalAppCallDLL.Str_Function_Int(Path.c_str(),
                              "_Plugin_GetMenuItemHotKey", i));
                 LowLevelDLLFunction llDLLfunc;

                 FARPROC f = GlobalAppCallDLL.GetFunction(Path.c_str(), "_Plugin_OnMenuItemExecute", &llDLLfunc);
                 if (f != NULL)
                 {
                    PluginMenuItem* pPluginMenuItem = new PluginMenuItem();
                    pPluginMenuItem->fullDLLfunc.path = Path.c_str();
                    pPluginMenuItem->fullDLLfunc.function = "_Plugin_OnMenuItemExecute";
                    pPluginMenuItem->fullDLLfunc.handle = llDLLfunc.handle;
                    pPluginMenuItem->fullDLLfunc.farproc = f;
                    pPluginMenuItem->index = i;
                    nwItem->Tag = (int)pPluginMenuItem;
                    //TODO: free this memory somewhere
                 }
                 frmQuickBible->PopupMenu1->Items->Add(nwItem);
              }
          }
          CleanPluginNonEvents(PluginFunctions);
          PopulateHashedProcs(Path, PluginFunctions, GlobalAppCallDLL.HashedDLLFuncs);
          delete PluginFunctions;
          return true;
}


void __fastcall TFrmPreferences::PopulatePluginsControl()
{
    PluginsGrid->NoSelection();
    PluginsGrid->ClearRows();
    delete Plugins;

    Plugins = ListFilesInDirectory(PluginsPath + "*.dll");
    for(int i = 0; i < Plugins->Count; ++i)
    {
       String Path = PluginsPath + Plugins->Strings[i];

       try
       {
            AddPlugin(Path, true);
       }
       catch(...)
       {
         //do nothing, dll did not contain this funciton
       }
    }
    PluginsGrid->BestFitColumns(bfCells);
}

//---------------------------------------------------------------------------
void __fastcall TFrmPreferences::PluginsGridCellClick(TObject *Sender,
      int ACol, int ARow)
{
  std::string plugin_path = ("" + PluginsGrid->Cell[4][ARow]->AsString + PluginsGrid->Cell[1][ARow]->AsString).c_str();

  bool disabled_selected_plugin = false;
  if (ACol == 0) //Enabled checkbox
  {
    PluginsGrid->Cell[ACol][ARow]->AsBoolean = !PluginsGrid->Cell[ACol][ARow]->AsBoolean;
    //actively disable the plugin, some may require a restart, since a few functions
    //have already been called! (_Plugin_GetName())
    if (PluginsGrid->Cell[ACol][ARow]->AsBoolean == false)
    {
       disabled_selected_plugin = true;
       plugin_disabled[plugin_path] = true;
    }
    else  //enabling plugin!
    {
       plugin_disabled[plugin_path] = false; //not disabled any more, lets erase it too
       plugin_disabled.erase(plugin_disabled.find(plugin_path));

       if (!InstallPlugin(plugin_path.c_str()))
       {
          //plugin could not be installed for some reason
          PluginsGrid->Cell[ACol][ARow]->AsBoolean = false;
       }
    }
  }

  //
  //  TStringList* PluginFunctions = new TStringList();
  //  ListDLLFunctions("Plugins\\" + PluginsGrid->Cell[1][ARow]->AsString, PluginFunctions);
    PluginsInfoMemo->Clear();
    PluginsInfoMemo->Lines->Add(PluginsGrid->Cell[1][ARow]->AsString + " events: ");

    //we want an alias
    map<string, map<string, LowLevelDLLFunction> > & DLLFuncs =
       GlobalAppCallDLL.HashedDLLFuncs.loaded_dll_to_funcs;

   if (DLLFuncs.find(plugin_path) == DLLFuncs.end())
   {
      ShowMessage("This DLL is not loaded.");//was not loaded into HashedDLLFuncs");
      return;
   }

   int total_functions_detached = 0;

    //this is how we iterate across functions
    //if we are disabling this plugin, we have to disable each function!
    map<string, LowLevelDLLFunction>::iterator i;
    map<string, LowLevelDLLFunction>::iterator end;
    for (i = DLLFuncs[plugin_path].begin(), end = DLLFuncs[plugin_path].end(); i != end; ++i)
    {
         //if we are disabling the plugin, now lets erase each lowleveldllfunction associated
         //with this plugin DLL
         std::string & func_name = i->first;
         LowLevelDLLFunction& lowDLLfunc = i->second;
         if (disabled_selected_plugin)
         {
             total_functions_detached += DetachFunctionInDLL(plugin_path, lowDLLfunc);
             for (int j = frmQuickBible->PopupMenu1->Items->Count-1; j >= 0; --j)
             {
                TMenuItem* mni = frmQuickBible->PopupMenu1->Items->Items[j];
                if (mni->Tag > 0) //assume menu items with tags > 0 are plugin menu items --kinda dangerous
                {
                   PluginMenuItem* pmi = (PluginMenuItem*)(mni->Tag);
                   if (pmi->fullDLLfunc.path == plugin_path)
                   {
                      delete pmi; 
                      delete mni;
                   }
                }
             }
         }

         if (func_name.find("_Plugin_On") == 0)
         {
           PluginsInfoMemo->Lines->Add( func_name.c_str() );
         }
    }

    if (disabled_selected_plugin)
    {
       //we actually unload the dll now so it should be uncallable completely
       GlobalAppCallDLL.HashedLibs.FreeHashedLibrary(plugin_path);
       PluginsInfoMemo->Lines->Add(IntToStr(total_functions_detached) + String(" events unloaded."));
       PluginsInfoMemo->Lines->Add("This plugin has been disabled.");
    }

    /**
    for (int i = 0; i < PluginFunctions->Count; i++)
    {
        if (PluginFunctions->Strings[i].Pos("_Plugin_On") == 1)
        {
           PluginsInfoMemo->Lines->Add(PluginFunctions->Strings[i].SubString(9,99));
        }
    }
    
    delete PluginFunctions;
    **/
}
//---------------------------------------------------------------------------
int TFrmPreferences::DetachFunctionInDLL(const std::string& DLL, const CallDLLInterface::LowLevelDLLFunction& lowDLLfunc)
{
    int detached_count = 0;
    map<string, list<LowLevelDLLFunction> > & FuncsToDLLs =
       GlobalAppCallDLL.HashedDLLFuncs.loaded_func_to_dlls;

    
    map<string, list<LowLevelDLLFunction> >::iterator iter;
    map<string, list<LowLevelDLLFunction> >::iterator iter_end;

    //iterate through all callable functions, removing the ones which are pointing to the given DLL
    for (iter = FuncsToDLLs.begin(), iter_end = FuncsToDLLs.end(); iter != iter_end; ++iter)
    {
      list<LowLevelDLLFunction>::iterator i;

      //erase all found elements
      list<LowLevelDLLFunction>::iterator rit;
      for ( i=iter->second.begin() ; i != iter->second.end(); ++i )
      {
         //does the DLL match???
         if (i->handle == lowDLLfunc.handle)
         {
            //does the FUNCTION match too?
            if (i->farproc == lowDLLfunc.farproc)
            {
               //go ahead and erase it!!
               i = iter->second.erase(i);
               detached_count++;
            }
         }
      }
   }

   return detached_count;

}

//---------------------------------------------------------------------------
void __fastcall TFrmPreferences::TreeView1Click(TObject *Sender)
{
   if (TreeView1->Selected)
   {
      TTreeNode* ItemSelected = TreeView1->Selected;
      TPanel* AssociatedPanel = dynamic_cast<TPanel*>((TPanel*)ItemSelected->Data);
      if (!AssociatedPanel)
      {
         //no associated panel in the data, lets try to use the name
         //ie PnlPrefPlugins or PnlPrefBibles
         AssociatedPanel = dynamic_cast<TPanel*>(
            FindComponent("PnlPref" + ItemSelected->Text));
      }
      //no associated panel, just use the PleaseChoosePanel
      if (!AssociatedPanel) AssociatedPanel = PleaseChoosePanel;

      if (AssociatedPanel)
      {
            AssociatedPanel->Show();
            AssociatedPanel->BringToFront();
      }
   }        
}
//---------------------------------------------------------------------------
void __fastcall TFrmPreferences::btnPluginsRefreshClick(TObject *Sender)
{
   PopulatePluginsControl();
}
//---------------------------------------------------------------------------

void __fastcall TFrmPreferences::BiblesGridCellClick(TObject *Sender,
      int ACol, int ARow)
{
  if (ACol == 0) //Enabled checkbox
  {
    BiblesGrid->Cell[ACol][ARow]->AsBoolean = !BiblesGrid->Cell[ACol][ARow]->AsBoolean;
  }

  SqliteBible* bible = dynamic_cast<SqliteBible*>((SqliteBible*)BiblesGrid->Row[ARow]->Data);
  if (bible)
  {
     String rtf_start = "{\\rtf1";
     bool use_rtf = false;
     String comments = bible->getComments().c_str();
     if (comments.Pos("\\par") != 0)
     {
        use_rtf = true;
     }
     comments = StringReplace(comments, "\"", "", TReplaceFlags());
     if (comments[comments.Length()] == '\"')
     {
       comments.Delete(comments.Length(), 1);
       //comments[comments.Length()] = '\0';
     }
     String rtf_end = "}";

     TStringStream* stream;
     //embed rtf if contains rtf
     if (use_rtf)
     {
       BiblesInfoMemo->PlainText = false;
       stream = new TStringStream(rtf_start + comments + rtf_end);
       stream->Seek(0, soFromBeginning);
       BiblesInfoMemo->Lines->LoadFromStream(stream);
       delete stream;

     }
     else
     {
       BiblesInfoMemo->SelAttributes->Style  =
          BiblesInfoMemo->SelAttributes->Style.Clear();
       BiblesInfoMemo->PlainText = true;
       BiblesInfoMemo->Clear();
       BiblesInfoMemo->Text = comments;
     }


  }
}
//---------------------------------------------------------------------------
void __fastcall TFrmPreferences::Button1Click(TObject *Sender)
{
  //apply changes
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmPreferences::BiblesGridAfterRowMove(TObject *Sender,
      int FromPos, int ToPos)
{
   frmQuickBible->UpdateBibleComboBox();
}
//---------------------------------------------------------------------------


void __fastcall TFrmPreferences::AppearanceCbxStayOnTopClick(
      TObject *Sender)
{
   FormStyle = (AppearanceCbxStayOnTop->Checked) ? fsStayOnTop : fsNormal;
   frmQuickBible->FormStyle = FormStyle;
}
//---------------------------------------------------------------------------

void __fastcall TFrmPreferences::FormCreate(TObject *Sender)
{
   DragAcceptFiles(Handle, true);
}
//---------------------------------------------------------------------------
 void __fastcall TFrmPreferences::WmDropFiles(TWMDropFiles& Message)
 {
   char buff[MAX_PATH];
   HDROP hDrop = (HDROP)Message.Drop;
   int numFiles = 
      DragQueryFile(hDrop, -1, NULL, NULL);
   for (int i=0;i < numFiles;i++) {
      DragQueryFile(hDrop, i, buff, sizeof(buff));
      AnsiString Path(buff);
      if (TreeView1->Selected->Text == "Bibles")
      {
          //try to install another bible
          if (ExtractFileExt(Path).UpperCase() == ".SQLITE3")
          {
             //install the bible!
             Bible* bibleJustDropped = AddBible(Path);
             frmQuickBible->UpdateBibleComboBox();
          }
      }
      else if (TreeView1->Selected->Text == "Plugins")
      {
          //try to install another plugin
          if (ExtractFileExt(Path).UpperCase() == ".DLL")
          {
             //add the plugin, do not install yet
             if (AddPlugin(Path, true))
             {
                ShowMessage("Plugin Installed");
             }
          }
          ShowMessage("Plugin tried to install");
      }
   }
   DragFinish(hDrop);
 }
void __fastcall TFrmPreferences::AdvancedBtnClearCacheClick(
      TObject *Sender)
{
   frmQuickBible->mapSearchCache.erase(
         frmQuickBible->mapSearchCache.begin(),
         frmQuickBible->mapSearchCache.end()
   );
   dynamic_cast<TButton*>(Sender)->Enabled = frmQuickBible->mapSearchCache.size() > 0;
}
//---------------------------------------------------------------------------


void __fastcall TFrmPreferences::cbxAppearanceShowSearchOptionsClick(
      TObject *Sender)
{
   frmQuickBible->pnlSearchOptions->Visible =
     cbxAppearanceShowSearchOptions->Checked;
   frmQuickBible->ShowHideSearchBar1Click(cbxAppearanceShowSearchOptions);
}
//---------------------------------------------------------------------------

