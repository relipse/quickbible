#ifndef TFN_SQLITEBIBLE_H
#define TFN_SQLITEBIBLE_H

#include "Bible.h"

#include <string>
#include <sstream>
using namespace std;

#include "SQLITE3CLASS.h"
namespace TearsForNations
{

	class SqliteBible : public Bible
	{
		public:
			//*Constructor
			SqliteBible(string version, string filename="")
			   : Bible(version)
			{
                                m_filename = filename;
				m_pSql = new SQLITE3(m_filename.c_str());
                                m_version = this->getAbbreviation();
			}
			//*Destructor
			~SqliteBible(){
				delete m_pSql;
			}

                        protected:
                        std::map<string, string> cache_map;

                        const string getDbStringValue(const string& column, const string& table)
                        {
                           //use cache for this!
                           if (cache_map.find(column) != cache_map.end())
                           {
                              return cache_map[column];
                           }

                           //warning this is not safe -- thats why this is a protected method
                           string query = "SELECT " + column + " FROM " + table + " LIMIT 1";
                           string value;
                           
                           nsSQLITE3::quick_results qres = m_pSql->quick_exe(query);
                           if (qres.rc == SQLITE_OK && qres.ncol == 1)
                           {
                              value = qres.result[qres.ncol];
                              cache_map[column] = value;
                           }
                           m_pSql->free(qres.result);
                           return value;
                        }


                        public:

                        virtual inline const string getDescription()
                        {
                           return getDbStringValue("description", "bible_info");
                        }

                        virtual inline const string getAbbreviation()
                        {
                           return getDbStringValue("abbreviation", "bible_info");
                        }

                        virtual inline const string getComments()
                        {
                           return getDbStringValue("comments", "bible_info");
                        }

                        virtual inline const string getFont()
                        {
                           return getDbStringValue("font", "bible_info");
                        }

                        virtual inline const string getFormat()
                        {
                           return getDbStringValue("format", "bible_info");
                        }

                        virtual inline const string getStrongs()
                        {
                           return getDbStringValue("strongs", "bible_info");
                        }

                        virtual const std::vector<FullBibleVerse> getScriptureVerseRange(int bk, int ch, int vs, int vs_max)
                        {
                           vector<FullBibleVerse> vct_bibleverses;
                           stringstream ss;
                           if (vs_max == 0){ vs_max = vs; }

                           ss << "SELECT rowid, t, b, c, v FROM bible_verses WHERE b = " << bk
                              << " AND c = " << ch;
                           if (vs > 0)
                           {
                             ss  << " AND v >= " << vs
                                 << " AND v <= " << vs_max;
                           }
                           m_pSql->exe(ss.str().c_str());



                           for (unsigned int i = 0; i < m_pSql->data_results.vdata.size(); i += 5)
                           {
                                FullBibleVerse bv;
				m_lastfound_absolute_vs = (m_pSql->data_results.vdata.size() > i+0 ? atoi(m_pSql->data_results.vdata[i+0].c_str()) : 0);
				m_lastfound = ( m_pSql->data_results.vdata.size() > i+1 ? m_pSql->data_results.vdata[i+1] : std::string(""));
                                m_lastfound_verse_ref.bk = ( m_pSql->data_results.vdata.size() > i+2 ? atoi(m_pSql->data_results.vdata[i+2].c_str()) : 0);
                                m_lastfound_verse_ref.ch = ( m_pSql->data_results.vdata.size() > i+3 ? atoi(m_pSql->data_results.vdata[i+3].c_str()) : 0);
                                m_lastfound_verse_ref.vs =( m_pSql->data_results.vdata.size() > i+4 ? atoi(m_pSql->data_results.vdata[i+4].c_str()) : 0);
                                m_lastfound_verse_ref.text = &m_lastfound;
                                bv.bible_verse = m_lastfound_verse_ref;
                                bv.text = m_lastfound;
                                  //OnFoundBibleVerse -- FIRE EVENT (the user can abort the search early if he wants
                                 if (m_eventOnFoundBibleVerse != NULL)
                                 {
                                    bool abortSearch = false;

                                    m_eventOnFoundBibleVerse(this, &bv, abortSearch);
                                    vct_bibleverses.push_back(bv);
                                    if (abortSearch)
                                    {
                                       break;
                                    }
                                 }
                                 else
                                 {
                                   vct_bibleverses.push_back(bv);
                                 }
                           }
                           return vct_bibleverses;
                        }

                        private:
                           static std::string generate_find_where_clause(const string& str, BibleSearchRange* pSearchRange = NULL)
                           {
                               //TODO: figure out the search variables using the BibleSearchRange
                               std::string whereclause;

                               //allow this to find case sensitive searches
                               if (pSearchRange && !pSearchRange->ignore_case) //str.find("cs") == 0)
                               {
                                  //GLOB method is case-sensitive, LIKE is NOT case sensitive!
                                  whereclause =  "WHERE t GLOB '*" + SQLITE3::quotequote(str) + "*' ";
                               }
                               else
                               {
                                 //LIKE is NOT case sensitive!
                                 whereclause = "WHERE t LIKE '%" + SQLITE3::quotequote(str) + "%'";
                               }



                               if (pSearchRange)
                               {
                                  if (pSearchRange->min_abs_vs > 0){
                                     whereclause += " AND rowid >= " + to_string(pSearchRange->min_abs_vs);
                                  }
                                  if (pSearchRange->max_abs_vs > 0){
                                     whereclause += " AND rowid <= " + to_string(pSearchRange->max_abs_vs);
                                  }
                               }

                               return whereclause;
                           }
                        public:

                        //this should match the findAll() query except instead uses COUNT(*) aggregate function
                        virtual int findAllCount(const string& str, BibleSearchRange* pSearchRange = NULL)
                        {
                                nsSQLITE3::quick_results qres = m_pSql->quick_exe(
                                        std::string(
                                        "SELECT count(*) FROM bible_verses ")
                                        + SqliteBible::generate_find_where_clause(str, pSearchRange));
                                 int count = 0;
                                 stringstream ss;
                                 if (qres.rc == SQLITE_OK && qres.ncol == 1)
                                 {
                                   ss << qres.result[qres.ncol];
                                   ss >> count;
                                 }
                                 m_pSql->free(qres.result);
                                 return count;
                        }

                        //find all the bible verses within the searchrange, store them in a vector
                        //TODO: implement BibleSearchRange to limit the query
                        virtual const std::vector<FullBibleVerse> findAll(const string& str, BibleSearchRange* pSearchRange = NULL)
                        {
                            vector<FullBibleVerse> vct_bibleverses;

                              stringstream query;

                              query << "SELECT rowid, t, b, c, v FROM bible_verses "
                                    << SqliteBible::generate_find_where_clause(str, pSearchRange);
                                    
                              m_pSql->exe(query.str());

                              FullBibleVerse bv;

                              for (unsigned int i = 0; i < m_pSql->data_results.vdata.size(); i += 5)
                              {
                                 bv.bible_verse.abs_vs = (m_pSql->data_results.vdata.size() > i + 1 ? atoi(m_pSql->data_results.vdata[i].c_str()) : 0);
                                 bv.text = (m_pSql->data_results.vdata.size() > i + 1 ? m_pSql->data_results.vdata[i + 1] : std::string(""));
                    
                                 bv.bible_verse.bk = (m_pSql->data_results.vdata.size() > i + 2 ? atoi(m_pSql->data_results.vdata[i + 2].c_str()) : 0);
                                 bv.bible_verse.ch = (m_pSql->data_results.vdata.size() > i + 3 ? atoi(m_pSql->data_results.vdata[i + 3].c_str()) : 0);
                                 bv.bible_verse.vs = (m_pSql->data_results.vdata.size() > i + 4 ? atoi(m_pSql->data_results.vdata[i + 4].c_str()) : 0);
                    
                                 //set the bible verse text pointer to the text
                                 bv.bible_verse.text = &bv.text;

                                 //set the lastfound settings
                                 m_lastfound = bv.text;
                                 m_lastfound_absolute_vs = bv.bible_verse.abs_vs;
                                 m_lastfound_verse_ref = bv.bible_verse;

                                 bool abortSearch = false;
                                 //OnFoundBibleVerse -- FIRE EVENT (the user can abort the search early if he wants
                                 if (m_eventOnFoundBibleVerse != NULL)
                                 {
                                    m_eventOnFoundBibleVerse(this, &bv, abortSearch);
                                    vct_bibleverses.push_back(bv);
                                    if (abortSearch == true)
                                    {
                                       return vct_bibleverses;
                                    }
                                 }
                                 else
                                 {
                                    vct_bibleverses.push_back(bv);
                                 }
                              }
                              return vct_bibleverses;

                        }
                       private:
                        //assume SELECT rowid, t, b, c, v FROM ...
                        FullBibleVerse storeResults(int offset = 0) const
                        {
                                FullBibleVerse bv;
                                bv.bible_verse.abs_vs = ( m_pSql->data_results.vdata.size() > offset + 1 ? atoi(m_pSql->data_results.vdata[offset + 0].c_str()) : 0);
                                bv.text = ( m_pSql->data_results.vdata.size() > offset + 1 ? m_pSql->data_results.vdata[offset + 1] : std::string(""));
                                bv.bible_verse.bk = ( m_pSql->data_results.vdata.size() > offset + 2 ? atoi(m_pSql->data_results.vdata[offset + 2].c_str()) : 0);
                                bv.bible_verse.ch = ( m_pSql->data_results.vdata.size() > offset + 3 ? atoi(m_pSql->data_results.vdata[offset + 3].c_str()) : 0);
                                bv.bible_verse.vs = ( m_pSql->data_results.vdata.size() > offset + 4 ? atoi(m_pSql->data_results.vdata[offset + 4].c_str()) : 0);
                                bv.bible_verse.text = &bv.text;
                                return bv;
                        }

                        void FullBibleVerseToLastFound(const FullBibleVerse& bv)
                        {
                           m_lastfound_absolute_vs = bv.bible_verse.abs_vs;
                           m_lastfound_verse_ref = bv.bible_verse;
                           m_lastfound = bv.text;
                        }

                       public:
			//find a bible verse containing the given string
			virtual const string& find(const string& str, int start_absolute_vs = 0)
			{
				stringstream query;
				query << "SELECT rowid, t, b, c, v FROM bible_verses WHERE rowid > " << start_absolute_vs
					  << " AND t LIKE '%" << str << "%' LIMIT 1";

				m_pSql->exe(query.str());

                                FullBibleVerse bv = storeResults();
                                FullBibleVerseToLastFound(bv);
                                
                                 bool abortSearch = false;
                                 //OnFoundBibleVerse -- FIRE EVENT (the user can abort the search early if he wants
                                 if (m_eventOnFoundBibleVerse != NULL)
                                 {
                                    m_eventOnFoundBibleVerse(this, &bv, abortSearch);
                                    if (abortSearch){
                                      return m_lastfound; //do not update lastfound verse
                                    }
                                 }

				return m_lastfound;
			}

			virtual const string& getScripture(int absolute_vs)
			{
				stringstream query;
				query << "SELECT rowid, t, b, c, v FROM bible_verses WHERE rowid = " << absolute_vs << " LIMIT 1";
				m_pSql->exe(query.str());

                                FullBibleVerse bv = storeResults();
                                FullBibleVerseToLastFound(bv);
                                 bool abortSearch = false;

                                 //OnFoundBibleVerse -- FIRE EVENT (the user can abort the search early if he wants
                                 if (m_eventOnFoundBibleVerse != NULL)
                                 {
                                    m_eventOnFoundBibleVerse(this, &bv, abortSearch);
                                    if (abortSearch){
                                      return m_lastfound; //do not update lastfound verse
                                    }
                                 }


				return m_lastfound;
			}

			virtual const string& getScripture(int bk, int ch, int vs)
			{
				stringstream ss;
				ss << "SELECT rowid, t, b, c, v FROM bible_verses WHERE b = " << bk
				   << " AND c = " << ch
				   << " AND v = " << vs;

				m_pSql->exe(ss.str().c_str());

				FullBibleVerse bv = storeResults();
                                FullBibleVerseToLastFound(bv);

                                  bool abortSearch = false;
                                 //OnFoundBibleVerse -- FIRE EVENT (the user can abort the search early if he wants
                                 if (m_eventOnFoundBibleVerse != NULL)
                                 {
                                    m_eventOnFoundBibleVerse(this, &bv, abortSearch);
                                    if (abortSearch){
                                      return m_lastfound; //do not update lastfound verse
                                    }
                                 }
                                 

				return m_lastfound;
			}

			SQLITE3* getSqlite3DB(){ return m_pSql; }
	protected:
		string m_filename;
		SQLITE3* m_pSql;
	};
};
#endif //*TFN_SQLITEBIBLE_H

