//---------------------------------------------------------------------------

#ifndef BibleVerseParseH
#define BibleVerseParseH

#include "BibleStructuresSimple.h"

#include <string.h>

#include <string>
#include <vector>
using namespace std;

namespace TearsForNations
{
   struct BibleVerseTokens
   {
      std::string book;
      int ch;
      int vs_min;
      int vs_max;
   };

   struct BibleVerseStrTokens
   {
      vector<string> tokens;
      std::string& book(){ return tokens[0]; }
      std::string& chapter(){ return tokens[1]; }
      std::string& verse_min(){ return tokens[2]; }
      std::string& verse_max(){ return tokens[3]; }
   };

   //Mat 5:1-28;
   BibleVerseStrTokens TokenizeParts(const std::string& verse)
   {
      char * pch;
      BibleVerseStrTokens bvtokens;
      vector<string> & tokens = bvtokens.tokens;
      char* str = const_cast<char*>(verse.c_str());

      pch = strtok (str," ");
      if (pch == NULL){
        tokens.push_back(verse);
        return bvtokens;
      }
      tokens.push_back(pch);

      pch = strtok (NULL,":");
      if (pch == NULL){ return bvtokens; }
      tokens.push_back(pch);

      pch = strtok (NULL,"-,");
      if (pch == NULL){ return bvtokens; }
      tokens.push_back(pch);

      while((pch = strtok (NULL,"-,")) != NULL)
      {
        tokens.push_back(pch);
      }
      return bvtokens;
   }

   //ie. Mat 5:1-28   returns true on parsable, false if not
   bool VerseRangeTokenize(const std::string& verseRange, BibleVerseTokens& tokens);

};
//---------------------------------------------------------------------------
#endif
 