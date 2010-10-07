//---------------------------------------------------------------------------

#ifndef HighLevelDLLFunctionH
#define HighLevelDLLFunctionH

#include "CallDLLInterfaceFunction.h"
#include "LowLevelDLLFunction.h"

namespace CallDLLInterface
{
  class HashedLibraries;
  
  class HighLevelDLLFunction
  {
     public:
     std::string filename;
     std::string function;
     HashedLibraries* pHashedLibs;

     //*Constructors
     HighLevelDLLFunction(std::string file = "", std::string func = "", HashedLibraries* pHshLibs = NULL)
       : filename(file), function(func), pHashedLibs(pHshLibs){ }


     private:
     //used hashed library if we can
     HANDLE LoadLibraryToHandle() const
     {
         if (pHashedLibs != NULL)
         {
            return NULL; //pHashedLibs->LoadHashedLibrary(filename);
         }
         else
         {
            return LoadLibrary(filename.c_str());
         }
     }

     public:

     bool toLowLevel(HANDLE& h, FARPROC& f)
     {
       h = this->LoadLibraryToHandle();
       f = GetProcAddress(h, function.c_str());
       return (h && f);
     }
     /**
     LowLevelDLLFunction toLowLevel() const
     {
         LowLevelDLLFunction lowDLLfunc;
         lowDLLfunc.handle = this->LoadLibraryToHandle();
         lowDLLfunc.farproc = GetProcAddress(lowDLLfunc.handle, function.c_str());
         return lowDLLfunc;
     }
     **/
  
  };

};//end namespace
//---------------------------------------------------------------------------
#endif
 