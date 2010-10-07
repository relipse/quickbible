//---------------------------------------------------------------------------

#ifndef BibleStructuresSimpleH
#define BibleStructuresSimpleH

namespace TearsForNations
{
        struct IntBibleVerseRef
        {
           int bk;
           int ch;
           int vs;
        };

        bool operator==(const IntBibleVerseRef& lhs, const IntBibleVerseRef& rhs)
        {
          return (lhs.bk == rhs.bk && lhs.ch == rhs.ch && lhs.vs == rhs.vs);
        }
        struct IntBibleVerseAbsRef
        {
           IntBibleVerseRef bvref;
           int abs_vs;
        };

        struct VarcharBibleVerseRef
        {
           IntBibleVerseAbsRef bvaref;
           char* text;
           char* version;
        };
}
//---------------------------------------------------------------------------
#endif
 