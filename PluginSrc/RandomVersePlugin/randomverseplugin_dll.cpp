//---------------------------------------------------------------------------

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

#include <vector>
#include <string>
using namespace std;

//for rand() and time()
#include <stdlib.h>
#include <time.h>


#include "Bible.h"
using namespace TearsForNations;

const int mnuGET_RANDOM_BIBLE_VERSE = 0;

#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
        srand ( time(NULL) );
        return 1;
}
//---------------------------------------------------------------------------
extern "C"
{
   char* __declspec(dllexport) Plugin_GetName()
   {
      return "Random Verse Functionality";
   }

   char* __declspec(dllexport) Plugin_GetVersion()
   {
      return "0.01";
   }

   int __declspec(dllexport) Plugin_GetMenuItemCount(){ return 1; }

   char* __declspec(dllexport) Plugin_GetMenuItemName(int index)
   {
     switch(index)
     {
       case mnuGET_RANDOM_BIBLE_VERSE: return "Random Verse";
       default:
       return NULL;
     }
   }

   char* __declspec(dllexport) Plugin_GetMenuItemHotKey(int index)
   {
     switch(index)
     {
       case mnuGET_RANDOM_BIBLE_VERSE: return "Ctrl+R";
       default:
       return NULL;
     }
   }

   bool __declspec(dllexport) Plugin_OnMenuItemExecute(int itemIndex,
        const vector<Bible*>& bibles, unsigned int selectedBibleIndex,
        const char* searchBoxText, char*& newResultText)
   {
     switch(itemIndex)
     {
       case mnuGET_RANDOM_BIBLE_VERSE:
         bibles[selectedBibleIndex]->getScripture(rand() % 31103 + 1);
       break;
       default:
       return false;
     }
   }
}
//---------------------------------------------------------------------------

