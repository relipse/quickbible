#ifndef TFN_BIBLE_H
#define TFN_BIBLE_H

#include <string>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

#include "BibleAbbreviationsEn.h"
#include "BibleStructures.h"

namespace TearsForNations
{
        class Bible;
        //Event Types --
        //ie: void OnVerseFound(Bible* bible, FullBibleVerse* verse, bool& abort){ }
        typedef void (*FunctionEventVerseFound)(Bible*, FullBibleVerse*, bool&);

	class Bible
	{
        protected:
        //Events
        FunctionEventVerseFound m_eventOnFoundBibleVerse;
        public:
             setEventOnFoundBibleVerse(FunctionEventVerseFound fpt){m_eventOnFoundBibleVerse = fpt; }
             const FunctionEventVerseFound& getEventOnFoundBibleVerse() const {return m_eventOnFoundBibleVerse; }
	public:
		//*Constructor & Destructor
		Bible(string version) : m_eventOnFoundBibleVerse(NULL), m_version(version), m_lastfound(""), m_lastfound_absolute_vs(0){}
		virtual ~Bible(){}

		//*Methods -- Derived classes should make sure to call m_eventOnFoundBibleVerse if it is not NULL
		virtual const string& find(const string& str, int start_absolute_vs = 0) = 0;
		virtual inline const string& findNext(const string& str)
			{ return find(str, m_lastfound_absolute_vs + 1); }
                        
                virtual int findAllCount(const string& str, BibleSearchRange* pSearchRange = NULL) = 0;
                virtual const std::vector<FullBibleVerse> findAll(const string& str, BibleSearchRange* pSearchRange = NULL) = 0;
                virtual const std::vector<FullBibleVerse> getScriptureVerseRange(int bk, int ch, int vs, int vs_max) = 0;



		virtual const string& getScripture(int bk, int ch, int vs) = 0;
		virtual const string& getScripture(int absolute_vs) = 0;

                //override these methods in Derived classes (each bible should contain such info)
                virtual inline const string getDescription(){ return string(); }
                virtual inline const string getAbbreviation(){ return m_version; }
                virtual inline const string getComments(){ return string(); }
                virtual inline const string getFont(){ return string(); }
                virtual inline const string getFormat(){ return string(); }
                virtual inline const string getStrongs(){ return string(); }

		//*Accessors
		virtual const string& getVersion(){ return m_version; }



                const BibleVerse& getLastFoundVerse(){ return m_lastfound_verse_ref; }
		const string& getLastFoundScripture(){ return m_lastfound; }
		const int getLastFoundAbsoluteVerse(){ return m_lastfound_absolute_vs; }
                static string BookToAbbr(int bk)
                {
                   if (bk >= 1 && bk <= 66)
                   {
                      string abbr = "";
                      abbr += bible_book_en_abbr[bk][0];
                      //this next if statement will convert the 2nd char
                      //to lowercase if and only if the first is not a digit
                      //(ie. "1TI" becomes "1Ti", whereas "MAT" becomes "Mat"
                      if (!isdigit(bible_book_en_abbr[bk][0]))
                      {
                        abbr += tolower(bible_book_en_abbr[bk][1]);
                      }
                      else
                      {
                        abbr += bible_book_en_abbr[bk][1];
                      }
                      abbr += tolower(bible_book_en_abbr[bk][2]);
                      return abbr;
                   }
                   else return "";
                }
                static int AbbrToBook(char* abbr)
                {
                   if (strlen(abbr) < 3){ return 0; }

                   for (int i = 1; i <= 66; ++i)
                   {
                     //all 3 letters match!
                     if (toupper(abbr[0]) == bible_book_en_abbr[i][0] &&
                         toupper(abbr[1]) == bible_book_en_abbr[i][1] &&
                         toupper(abbr[2]) == bible_book_en_abbr[i][2]   )
                     {
                        return i;
                     }
                   }
                   return 0;
                }
	protected:
		string m_version;
		string m_lastfound;
		int m_lastfound_absolute_vs;
                BibleVerse m_lastfound_verse_ref;
            //    std::vector<BibleVerse> m_vct_verse_ref_history;
	};


        class BibleList : public Bible
        {
           private:
              std::vector<Bible*> m_compositeBible;
           public:
           inline BibleList(const std::vector<Bible*>& compositeBible)
                :m_compositeBible(compositeBible), Bible("BibleList Composite")
              {

              }
           inline virtual const std::vector<FullBibleVerse> findAll(const string& str, BibleSearchRange* pSearchRange = NULL)
           {
              for (unsigned int i = 0; i < m_compositeBible.size(); i++)
              {
                 m_compositeBible[i]->findAll(str, pSearchRange);
              }
           };
           inline virtual const std::vector<FullBibleVerse> getScriptureVerseRange(int bk, int ch, int vs, int vs_max)
           {
              for (unsigned int i = 0; i < m_compositeBible.size(); i++)
              {
                 m_compositeBible[i]->getScriptureVerseRange(bk, ch, vs, vs_max);
              }
           };

         virtual inline const string getDescription()
         {
              string desc("Composite: ");
              for (unsigned int i = 0; i < m_compositeBible.size(); i++)
              {
                 if (i > 0){ desc += ", "; }
                 desc += m_compositeBible[i]->getDescription();
              }
         }

         virtual inline const string getAbbreviation()
         {
              string abbr("");
              for (unsigned int i = 0; i < m_compositeBible.size(); i++)
              {
                 if (i > 0){ abbr += ", "; }
                 abbr += m_compositeBible[i]->getAbbreviation();
              }
         }


        };
};

#endif //*TFN_BIBLE_H*//
