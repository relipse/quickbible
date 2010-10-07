//---------------------------------------------------------------------------


#pragma hdrstop

#include "CallDLLInterfaceFunction.h"
CallDLLInterface::CallDLL GlobalAppCallDLL;

#include <imagehlp.hpp>

bool CallDLLInterface::ListDLLFunctions(AnsiString sADllName, TStringList *slListOfDllFunctions)
{
   bool Return = true;

   DWORD *dNameRVAs = (DWORD *)calloc(1048575, sizeof(DWORD));
   _IMAGE_EXPORT_DIRECTORY *ImageExportDirectory;
   _IMAGE_SECTION_HEADER   *ImageSectionHeader;
   unsigned long           cDirSize;
   _LOADED_IMAGE           LoadedImage;
   AnsiString              sName;

   slListOfDllFunctions->Clear();
   if (MapAndLoad(sADllName.c_str(), NULL, &LoadedImage, True, True))
   {
      try{
         ImageExportDirectory = (_IMAGE_EXPORT_DIRECTORY *)ImageDirectoryEntryToData(LoadedImage.MappedAddress,
                                                                                     false, IMAGE_DIRECTORY_ENTRY_EXPORT, &cDirSize);
         if (ImageExportDirectory != NULL)
         {
            dNameRVAs = (DWORD *)ImageRvaToVa(LoadedImage.FileHeader, LoadedImage.MappedAddress,
                                              DWORD(ImageExportDirectory->AddressOfNames), &ImageSectionHeader);
            for (int i = 0; i < ImageExportDirectory->NumberOfNames; i++)
            {
               sName = (char *)ImageRvaToVa(LoadedImage.FileHeader, LoadedImage.MappedAddress,
                                            dNameRVAs[i], &ImageSectionHeader);
               slListOfDllFunctions->Add(sName);
            }
         }
      }
      __finally {
         UnMapAndLoad(&LoadedImage);
      }
   }
   //did any functions get added?
   Return = (slListOfDllFunctions->Count > 0);
   return Return;
}



AnsiString __fastcall CallDLLInterface::CallDLL::Str_Function_Str(char* DLL, char* Function, char* arg1)
{
   String Result;
   HANDLE DLLHandle = HashedLibs.LoadHashedLibrary(DLL); // Define the path string

   if (DLLHandle != NULL)
   {

      typedef char* (*aDLLInterfaceFunction)(char*); // This is a function pointer
      aDLLInterfaceFunction DLLInterfaceFunction =
         (aDLLInterfaceFunction)GetProcAddress(DLLHandle, Function);
            // Leading underscore required by DLL and caller compiler settings
      char* cs_result;

      if (DLLInterfaceFunction != NULL)
      {
         try
         {
           cs_result = DLLInterfaceFunction(arg1);
           Result = String(cs_result);
         }
         catch(...)
         {

         }
      }
      else //function does not exist in this plugin
      {
         throw "Function does not exist in plugin";
      }
      //FreeLibrary(DLLHandle);
   };

   return Result;
}

AnsiString __fastcall CallDLLInterface::CallDLL::Str_Function_Int(char* DLL, char* Function, int arg1)
{
   String Result;
   HANDLE DLLHandle = HashedLibs.LoadHashedLibrary(DLL); // Define the path string

   if (DLLHandle != NULL)
   {

      typedef char* (*aDLLInterfaceFunction)(int); // This is a function pointer
      aDLLInterfaceFunction DLLInterfaceFunction =
         (aDLLInterfaceFunction)GetProcAddress(DLLHandle, Function);
            // Leading underscore required by DLL and caller compiler settings
      char* cs_result;

      if (DLLInterfaceFunction != NULL)
      {
         try
         {
           cs_result = DLLInterfaceFunction(arg1);
           Result = String(cs_result);
         }
         catch(...)
         {

         }
      }
      else //function does not exist in this plugin
      {
         throw "Function does not exist in plugin";
      }
      //FreeLibrary(DLLHandle);
   };

   return Result;
}

int __fastcall CallDLLInterface::CallDLL::Int_Function(char* DLL, char* Function)
{
   HANDLE DLLHandle = HashedLibs.LoadHashedLibrary(DLL); // Define the path string
   int result;
   if (DLLHandle != NULL)
   {

      typedef int (*aDLLInterfaceFunction)(void); // This is a function pointer
      aDLLInterfaceFunction DLLInterfaceFunction =
         (aDLLInterfaceFunction)GetProcAddress(DLLHandle, Function);
            // Leading underscore required by DLL and caller compiler settings
      

      if (DLLInterfaceFunction != NULL)
      {
         try
         {
           result = DLLInterfaceFunction();
         }
         catch(...)
         {

         }
      }
      else //function does not exist in this plugin
      {
         throw "Function does not exist in plugin";
      }
      //FreeLibrary(DLLHandle);
   };

   return result;
}

bool CallDLLInterface::AddIfDLLFunctionExists(HANDLE DLLHandle, char* Function, TStringList* slListOfDllFunctions)
{
   LowLevelDLLFunction lowDLLfunc(DLLHandle, Function);
   if (lowDLLfunc.farproc != NULL)
   {
      slListOfDllFunctions->Add(Function);
      return true;
   }
   else return false;
}

void CallDLLInterface::PopulateHashedProcs(AnsiString sADllName,
   TStringList* slListOfDllFunctions, HashedProcAdddresses& HashedDLLFuncsToPopulate)
{
  //define an alias
  std::map< std::string ,std::map<std::string, LowLevelDLLFunction> > & loadedFuncs =
     HashedDLLFuncsToPopulate.loaded_dll_to_funcs;

    for (int i = 0; i < slListOfDllFunctions->Count; i++)
    {
       std::string functionName = slListOfDllFunctions->Strings[i].c_str();
       loadedFuncs[sADllName.c_str()][functionName] = LowLevelDLLFunction(
          GlobalAppCallDLL.HashedLibs.LoadHashedLibrary(sADllName.c_str()),
          functionName.c_str());
       //now go ahead add to the function -> dll list map
       HashedDLLFuncsToPopulate.loaded_func_to_dlls[functionName].push_back(
          loadedFuncs[sADllName.c_str()][functionName]);
    }
}


//---------------------------------------------------------------------------
//get a DLL function pointer , try to load it if it doesnt exist in memory,
// return NULL if either the DLL fails to load, or the Function does not exist in the DLL
FARPROC CallDLLInterface::CallDLL::GetFunction(const char* DLL, const char* Function, LowLevelDLLFunction* lowDLLfunc)
{
  //define an alias
  std::map< std::string ,std::map<std::string, LowLevelDLLFunction> > & loadedFuncs =
    HashedDLLFuncs.loaded_dll_to_funcs;

  if (loadedFuncs.find(DLL) != loadedFuncs.end())
  {
     //the DLL exists, now is the function a part of the dll?
     if (loadedFuncs[DLL].find(Function) != loadedFuncs[DLL].end()
        /*&& loadedFuncs[DLL][Function].farproc != NULL*/ )
     {
        if (lowDLLfunc){ *lowDLLfunc = LowLevelDLLFunction(loadedFuncs[DLL][Function]); }
        //the function is in this DLL (or so it says it is)
        return  loadedFuncs[DLL][Function].farproc;
     }
     //DLL is stored, but the function cannot be found!
     //Warning* the programmer may not have populated the loaded functions!
     else return NULL;
  }
  else //dll does not exist, try to load it
  {
     HANDLE DLLHandle = HashedLibs.LoadHashedLibrary(DLL);
     if (DLLHandle)
     {
        loadedFuncs[DLL] = map< std::string , LowLevelDLLFunction>();
        FARPROC func = GetProcAddress(DLLHandle, Function);
        if (func != NULL)
        {
          //function actually exists in the DLL! yay!
          loadedFuncs[DLL][Function].handle = DLLHandle;
          loadedFuncs[DLL][Function].farproc = func;
          if (lowDLLfunc){ *lowDLLfunc = LowLevelDLLFunction(loadedFuncs[DLL][Function]); }

          //go ahead and populate the func_to_dlls list also with this info!
          HashedDLLFuncs.loaded_func_to_dlls[Function].push_back( loadedFuncs[DLL][Function] );
        }
        return func;
     }
     else return NULL; //could not load DLL!!
  }
}
//---------------------------------------------------------------------------
AnsiString __fastcall CallDLLInterface::CallDLL::Str_Function(char* DLL, char* Function)
{
   String Result;

   HANDLE DLLHandle = HashedLibs.LoadHashedLibrary(DLL); // Define the path string

   if (DLLHandle)
   {
      typedef char* (*aDLLInterfaceFunction)(void); // This is a function pointer
      aDLLInterfaceFunction DLLInterfaceFunction =
         (aDLLInterfaceFunction)GetProcAddress(DLLHandle, Function);
            // Leading underscore required by DLL and caller compiler settings
      char* cs_result;

      if (DLLInterfaceFunction != NULL)
      {
         try
         {
           cs_result = DLLInterfaceFunction();
           Result = String(cs_result);
         }
         catch(...)
         {

         }
      }
      else //function does not exist in this plugin
      {
         throw "Function does not exist in plugin";
      }
      //FreeLibrary(DLLHandle);
   }

   return Result;
}
//--------------------------------------------
HANDLE CallDLLInterface::HashedLibraries::LoadHashedLibrary(std::string DLL)
{
   if (loaded_libs.find(DLL) == loaded_libs.end() || loaded_libs[DLL] == NULL)
   {
      loaded_libs[DLL] = LoadLibrary(DLL.c_str());
   }
   return loaded_libs[DLL];
}

bool CallDLLInterface::HashedLibraries::FreeHashedLibrary(std::string DLL)
{
   if (loaded_libs.find(DLL) != loaded_libs.end() && loaded_libs[DLL] != NULL)
   {
      FreeLibrary(loaded_libs[DLL]);
      loaded_libs.erase(loaded_libs.find(DLL));
      return true;
   }
   return false; //does not exist in map!
}

//do this before program exits, or if we want to refresh the loaded libraries
void CallDLLInterface::HashedLibraries::FreeHashedLibraries()
{
    map<string, HANDLE>::iterator i;
    map<string, HANDLE>::iterator end;
    for (i = loaded_libs.begin(), end = loaded_libs.end(); i != end; ++i)
    {
      if (i->second != NULL)
      {
         FreeLibrary(i->second);
         i->second = NULL;
      }
    }
    loaded_libs.erase(loaded_libs.begin(), loaded_libs.end());
}
