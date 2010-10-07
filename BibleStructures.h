//---------------------------------------------------------------------------

#include <string>
using namespace std;

#include "BibleStructuresSimple.h"

#ifndef BibleStructuresH
#define BibleStructuresH
namespace TearsForNations
{
        struct BibleVerse
        {
          int bk;
          int ch;
          int vs;
          int abs_vs;
          std::string* text;
        };

        struct FullBibleVerse
        {
           BibleVerse bible_verse;
           std::string text;
        };


        struct BibleVersesRange
        {
           IntBibleVerseRef min;
           IntBibleVerseRef max;
        };

        bool operator==(const BibleVersesRange& lhs, const BibleVersesRange& rhs)
        {
          return (lhs.min == rhs.min && lhs.max == rhs.max);
        }

        struct BibleSearchRange
        {
          int min_abs_vs;
          int max_abs_vs;
          BibleVersesRange verses_range;
          bool ignore_case;
          IntBibleVerseRef** list_bible_refs_to_search;
        };

        bool operator==(const BibleSearchRange& lhs, const BibleSearchRange& rhs)
        {
          return (lhs.min_abs_vs == rhs.min_abs_vs &&
                  lhs.max_abs_vs == rhs.max_abs_vs &&
                  lhs.verses_range == rhs.verses_range &&
                  lhs.ignore_case == rhs.ignore_case &&
                  lhs.list_bible_refs_to_search == rhs.list_bible_refs_to_search
                  );
        }
}

//---------------------------------------------------------------------------
#endif
 