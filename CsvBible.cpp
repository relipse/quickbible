#include "CsvBible.h"

#include <sstream>
const string& TearsForNations::CsvBible::find(string str, int start_absolute_vs)
{
	string buffer;
	
	int line_num = 0;
	
	if (m_pfile->fail()){
		this->openFile(m_filename);
	}

	m_pfile->seekg(0, ios_base::beg);

	m_lastfound = "";
	m_lastfound_absolute_vs = 0;

#ifndef DO_NOT_USE_CSVPARSER_CLASS
  CSVParser parser;
#endif
	//printf("finding %s", str.c_str());

	//search until end of file
	while (!m_pfile->eof())
	{
	   int abs_id, book_id, chapter, verse, length;
	   string scripture;


	  //each line is a csv
	  getline(*m_pfile, buffer);
	  line_num++;


	  //ignore top 3 lines
	  if (line_num <= 3) continue;

#ifndef DO_NOT_USE_CSVPARSER_CLASS

    parser << buffer; // Feed the line to the parser

    // Now extract the columns from the line
    parser >> abs_id >> book_id >> chapter >> verse >> length >> scripture;
#endif 
#ifdef DO_NOT_USE_CSVPARSER_CLASS
	  stringstream ss;
	  ss << buffer;

	  //int abs_id;
	  ss >> abs_id;
	  
	  //if neccessary skip lines until we get to the one we want to start the search at
	  if (start_absolute_vs > abs_id){ continue; }
      
	  char c; 
	  //int book_id, chapter, verse, length;
	  //string scripture;

      ss >> c >> book_id >> c >> chapter >> c >> verse >> c >> length >> c;
	  
	  //take care of first quote
	  ss.get(c);
	  if (c == '"'){ ss.ignore(); }

	  //the rest of the string contains the scripture
	  for (ss.get(c);!ss.eof();ss.get(c))
	  {
		  if (c == '"')
		  {	  
	        if ( ss.peek() == '"')
		    {
			   ss.ignore(); //skip extra quote
		    }
			else //one single quote, must be end of verse
			{
				break; //if not 2 quotes in a row, then don't add it
			}
		  }
		  //some lines end with commas -- terminate it
		  else if (c == ',' && ss.peek() == -1)
		  {
			  break;
		  }
		  scripture += c;
	  }
#endif //*DO_NOT_USE_CSVPARSER_CLASS

	  //printf("%d abs, %d bk, %d ch, %d vs %s", abs_id, bk, ch, vs, scripture.c_str());

	  if (scripture.find(str) != string::npos)
	  {
		  m_lastfound = scripture;
		  m_lastfound_absolute_vs = abs_id;
		  return m_lastfound;
	  }
	}
	return m_lastfound;
}

const string& TearsForNations::CsvBible::getScripture(int absolute_vs)
{
  return m_lastfound;
}
const string& TearsForNations::CsvBible::getScripture(int bk, int ch, int vs)
{
  return m_lastfound;
}
