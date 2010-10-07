//---------------------------------------------------------------------------

#ifndef CallDLLInterfaceFunctionH
#define CallDLLInterfaceFunctionH

#include <vcl.h>
#include <map>
#include <string>
#include <list>
using namespace std;


#include "HighLevelDLLFunction.h"
#include "LowLevelDLLFunction.h"


namespace CallDLLInterface
{
  //std class
  class HashedLibraries
  {
    public:
     map <std::string, HANDLE> loaded_libs;

     HANDLE LoadHashedLibrary(std::string DLL);
     bool FreeHashedLibrary(std::string DLL);
     void FreeHashedLibraries();

     ~HashedLibraries(){ FreeHashedLibraries(); }
  };
  class HighLevelDLLFunction;
  class LowLevelDLLFunction;


  struct FullDLLFunction
  {
     std::string path;
     std::string function;
     HANDLE handle;
     FARPROC farproc;
  };

  /**
   * This class holds a bunch of loaded DLL functions (using LoadLibrary() GetProcAddress())
   * and can be called if casted correctly. the order of the vector can seriously matter
   */
  class HashedProcAdddresses
  {

     public:
      //function name to to dll list -- useful for calling all functions of same name to different dlls
      map < std::string, std::list<LowLevelDLLFunction> > loaded_func_to_dlls;

      //DLL name to function list -- needs to be populated (can use ListDLLFunctions below)
      //ie if (loaded_dll_to_funcs["mylib.dll"].find("_MyLib_DoStuff")){
      //    (((void*)())loaded_dll_to_funcs["mylib.dll"]["_MyLib_DoStuff"].farproc();
      map < std::string, std::map<std::string, LowLevelDLLFunction> > loaded_dll_to_funcs;
  };


  //VCL class
  class CallDLL
  {
     public:
       HashedLibraries HashedLibs;
       HashedProcAdddresses HashedDLLFuncs;

       FARPROC GetFunction(const char* DLL, const char* Function, LowLevelDLLFunction* lowDLLfunc = NULL);

     public:
       //for loading a dll and calling a function accepting and returning a char*
       AnsiString __fastcall Str_Function_Str(char* DLL, char* Function, char* arg1);
       AnsiString __fastcall Str_Function_Int(char* DLL, char* Function, int arg1);
       int __fastcall Int_Function(char* DLL, char* Function);

         //for just calling a function which returns a string (char*)
       AnsiString __fastcall Str_Function(char* DLL, char* Function);
  };

  bool ListDLLFunctions(AnsiString sADllName, TStringList *slListOfDllFunctions);
  bool AddIfDLLFunctionExists(HANDLE DLLHandle, char* Function, TStringList* slListOfDllFunctions);
  void PopulateHashedProcs(AnsiString sADllName, TStringList* slListOfDllFunctions, HashedProcAdddresses& HashedDLLFuncsToPopulate);
}

//a global CallDLL instance
extern CallDLLInterface::CallDLL GlobalAppCallDLL;

//---------------------------------------------------------------------------
#endif
