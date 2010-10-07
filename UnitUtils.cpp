//---------------------------------------------------------------------------


#pragma hdrstop

#include "UnitUtils.h"
//create a new StringList of all the files in given directory
TStringList* __fastcall ListFilesInDirectory(AnsiString Dir)
{
    WIN32_FIND_DATA findFileData;

    HANDLE hFind = FindFirstFile(Dir.c_str(), &findFileData);

    TStringList* sl = new TStringList();
    if(hFind  == INVALID_HANDLE_VALUE) {
        return sl;
    }
    int fileNumber = 0;
    sl->Add(findFileData.cFileName);
    while(FindNextFile(hFind, &findFileData)) {
       sl->Add(findFileData.cFileName);
    }
    FindClose(hFind);
    return sl;
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
 