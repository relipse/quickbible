//---------------------------------------------------------------------------

#ifndef LowLevelDLLFunctionH
#define LowLevelDLLFunctionH

#include "HighLevelDLLFunction.h"

namespace CallDLLInterface
{

  class LowLevelDLLFunction
  {
     public:
     HANDLE handle;
     FARPROC farproc;

     //*Constructors
     LowLevelDLLFunction() : handle(NULL), farproc(NULL)
     {

     }
     LowLevelDLLFunction(HANDLE h, FARPROC f)
      : handle(h), farproc(f)
     {

     }

     LowLevelDLLFunction(HANDLE h, const char* function)
      : handle(h)
     {
        if (h != NULL)
        {
            farproc = GetProcAddress(h, function);
        }
        else farproc = NULL;
     }


     /**
     LowLevelDLLFunction(HighLevelDLLFunction* highDLLfunc)
     {
        (*this) = highDLLfunc->toLowLevel();
     }
     **/
     //*Overloaded
     LowLevelDLLFunction & operator=(const LowLevelDLLFunction &rhs)
     {
       handle = rhs.handle;
       farproc = rhs.farproc;
       return (*this);
     }
  };

}; //end namespace

//---------------------------------------------------------------------------
#endif
 