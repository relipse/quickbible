/**
 * Encapsulate sqlite3_get_table() functionality
 * Freely taken from
 * http://souptonuts.sourceforge.net/code/simplesqlite3cpp2.cc.html
 */
#ifndef SQLITE3CLASS_H
#define SQLITE3CLASS_H

#include <string>
#include <vector>
#include <iterator>
#include <sqlite3.h>



//a cute little namespace
namespace nsSQLITE3
{
   const int ROW_ASSOC = 1; const int ASSOC = ROW_ASSOC;
   const int COL_ASSOC = 2;
   const int ARRAY = 3;

   struct results_t
   {
     std::map<std::string, size_t> map_col_head;
     std::vector<std::string> vcol_head;
     std::vector<std::string> vdata;
   };

   struct quick_results
   {
     int rc;
     int nrow,ncol;
     char **result;
     char* zErrMsg;
   };

}

class SQLITE3 {
private:
  sqlite3 *db;
  char *zErrMsg;
  char **result;   //gets populated, then freed right away
  int rc;
  int nrow,ncol;
  int db_open;
  std::string errMsg;
  std::string lastQuery;
public:
  std::string getErrorMsg(){ return errMsg; }
  std::string getLastQuery(){ return lastQuery; }
public:
  nsSQLITE3::results_t data_results;

//some static methods
public:
  //stolen and modified from from bobcat's open source string repository (escape1.cc)
  static string escape(string const &str, char const *series = "'", bool backslash = false);

static string glob_escape(string const& str){
  return escape(str, "?*[]\\", true);
}

//do this if we are using single quotes in a LIKE, GLOB or =
//"can't" becomes "can''t"
static string quotequote(string const& str){
  return escape(str, "'", false);
}

public:

  SQLITE3 (std::string tablename): zErrMsg(0), rc(0),db_open(0) {
    rc = sqlite3_open(tablename.c_str(), &db);
    if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      sqlite3_close(db);
    }
    db_open=1;
  }


  inline int xy_to_absolute_index(int row, int col)
  {
     return row*ncol+col;
  }

  char* data_at(int row, int col)
  {
    if ((0 <= row && row < nrow) &&
         (0 <= col && col < ncol)  )
    {
       return result[row*ncol+col];
    }
    return NULL;  //out of bounds
  }

  char* data(int row, int col)
  {
     return result[row*ncol+col];
  }

  //be sure to call free(qres.result);
  nsSQLITE3::quick_results quick_exe(const std::string& s_exe)
  {
      nsSQLITE3::quick_results qres;
      qres.rc = 0;

      qres.rc = sqlite3_get_table(
			db,              /* An open database */
			s_exe.c_str(),       /* SQL to be executed */
			&qres.result,       /* Result written to a char *[]  that this points to */
			&qres.nrow,             /* Number of result rows written here */
			&qres.ncol,          /* Number of result columns written here */
			&qres.zErrMsg          /* Error msg written here */
			);
      lastQuery = s_exe;
      return qres;
  }

  void free(char** results_to_free)
  {
      sqlite3_free_table(results_to_free);
  }


  int exe_no_free(const std::string& s_exe)
  {
      rc = sqlite3_get_table(
			db,              /* An open database */
			s_exe.c_str(),       /* SQL to be executed */
			&result,       /* Result written to a char *[]  that this points to */
			&nrow,             /* Number of result rows written here */
			&ncol,          /* Number of result columns written here */
			&zErrMsg          /* Error msg written here */
			);
      lastQuery = s_exe;
      return rc;
  }
  int cols(){ return ncol; }
  int rows(){ return nrow; }

  void free()
  {
      sqlite3_free_table(result);
  }


  int exe(std::string s_exe, nsSQLITE3::results_t* ptrDataResults = NULL, bool store_results_and_free = true) {
      //do not store results, and do not free the result** array (we must call free())
      if (!store_results_and_free){ return exe_no_free(s_exe); }


      if (ptrDataResults == NULL)
      {
         //just use private member to store results
         ptrDataResults = &data_results;
      }
      rc = sqlite3_get_table(
			db,              /* An open database */
			s_exe.c_str(),       /* SQL to be executed */
			&result,       /* Result written to a char *[]  that this points to */
			&nrow,             /* Number of result rows written here */
			&ncol,          /* Number of result columns written here */
			&zErrMsg          /* Error msg written here */
			);
	 
     lastQuery = s_exe;

      if (ptrDataResults->map_col_head.size() > 0){ ptrDataResults->map_col_head.clear(); }
      if(ptrDataResults->vcol_head.size() > 0) {ptrDataResults->vcol_head.clear();}
      if(ptrDataResults->vdata.size()>0) {ptrDataResults->vdata.clear();}

     if( rc == SQLITE_OK ){
      for(int i=0; i < ncol; ++i)
      {
	ptrDataResults->vcol_head.push_back(result[i]); /* First row heading */
        ptrDataResults->map_col_head[ptrDataResults->vcol_head[i]] = i; //"column_name" to index;
      }
      for(int i=0; i < ncol*nrow; ++i)
	ptrDataResults->vdata.push_back(result[ncol+i]);
      }


     else
     {
         errMsg = sqlite3_errmsg(db);
		 fprintf(stderr, "Error: %s\n SQL: %s", errMsg.c_str(), s_exe.c_str());
     }
     sqlite3_free_table(result);
      return rc;
  }

  ~SQLITE3(){
      sqlite3_close(db);
  } 
};


string SQLITE3::escape(string const &str, char const *series, bool backslash)
{
    string ret;
    string::size_type left = 0;

    while (true)
    {
        string::size_type right = str.find_first_of(series, left);

        ret += str.substr(left, right - left);  // append until separator

        if (right == string::npos)                  // done when all copied
            return ret;

        if (backslash) ret += "\\";         // append backslash
        else ret += str[right];             // append special char (twice) "can't" becomes "can''t"

        ret += str[right];                  // append the special char
        left = right + 1;
    }
}

//convert anything  to a string
#include <sstream>
template <class T>
inline std::string to_string (const T& t)
{
  std::stringstream ss;
  ss << t;
  return ss.str();
}

#endif //*SQLITE3CLASS_H
