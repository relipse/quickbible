//---------------------------------------------------------------------------
#include "BibleVerseParse.h"

   bool TearsForNations::VerseRangeTokenize(const std::string& verseRange, BibleVerseTokens& tokens)
   {
      BibleVerseStrTokens verseParts = TokenizeParts(verseRange);
      unsigned int numTokens = verseParts.tokens.size();
      if (numTokens >= 2 && numTokens <= 4)
      {
         tokens.book = verseParts.book();
         tokens.ch = atoi(verseParts.chapter().c_str());
         if (numTokens >= 4)
         {
            tokens.vs_max = atoi(verseParts.verse_max().c_str());
            tokens.vs_min = atoi(verseParts.verse_min().c_str());
         }
         else
         {
           tokens.vs_max = 0;
           if (numTokens == 3)
           {
             tokens.vs_min = atoi(verseParts.verse_min().c_str());
           }
           else
           {
             tokens.vs_min = 0;
           }
         }

         
         
         return true;
      }
      return false;
   }
//---------------------------------------------------------------------------
