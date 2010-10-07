#include "VPLBible.h"

const string& TearsForNations::VPLBible::getScripture(int bk, int ch, int vs)
{
   //TODO: umm convert to absolute verse
	return getLastFoundScripture();
}


const string& TearsForNations::VPLBible::getScripture(int absolute_vs)
{
	string buffer;
	int line_num = 0;

	if (m_pfile->fail()){
		this->openFile(m_filename);
	}

	m_pfile->seekg(0, ios_base::beg);

	m_lastfound = "";
	m_lastfound_absolute_vs = 0;


	//search until end of file
	while (!m_pfile->eof())
	{
	  //each line is a verse
	  getline(*m_pfile, buffer);
	  line_num++;	  

	  if (line_num == absolute_vs)
	  {
		  m_lastfound = buffer;
		  m_lastfound_absolute_vs = line_num;
		  return m_lastfound;
	  }
	}
	return m_lastfound;
}
const string& TearsForNations::VPLBible::find(string str, int start_absolute_vs)
{
	string buffer;
	int line_num = 0;
	
	if (m_pfile->fail()){
		this->openFile(m_filename);
	}

	m_pfile->seekg(0, ios_base::beg);

	m_lastfound = "";
	m_lastfound_absolute_vs = 0;

	//printf("finding %s", str.c_str());

	//search until end of file
	while (!m_pfile->eof())
	{
		
	  //each line is a verse
	  getline(*m_pfile, buffer);
	  line_num++;

	  //if neccessary skip lines until we get to the one we want to start the search at
	  if (start_absolute_vs > line_num){ continue; }

	  if (buffer.find(str) != string::npos)
	  {
		  m_lastfound = buffer;
		  m_lastfound_absolute_vs = line_num;
		  return m_lastfound;
	  }
	}
	return m_lastfound;
}


