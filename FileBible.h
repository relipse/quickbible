#ifndef TFN_FILEBIBLE_H
#define TFN_FILEBIBLE_H

#include "Bible.h"

#include <string>
#include <fstream>
using namespace std;

namespace TearsForNations
{
	class FileBible : public Bible
	{
	public:
		FileBible(string version, string filename=""):Bible(version), m_filename(""), m_pfile(NULL), m_lastfound_fpos(0)
		{
			//if the filename exists, go ahead and open it
			if (filename != ""){
				m_pfile = new ifstream(filename.c_str());
				if (m_pfile->is_open()) m_filename = filename;
			}
		}
		virtual ~FileBible(){ if (m_pfile){ m_pfile->close(); delete m_pfile; } }

		//*Methods
		virtual const string& find(string str, int start_absolute_vs = 0) = 0;
		virtual const string& getScripture(int bk, int ch, int vs) = 0;
		virtual const string& getScripture(int absolute_vs) = 0;
	public:
		virtual inline bool openFile(const string& filename)
		{
			if (!m_pfile){
				m_pfile = new ifstream(filename.c_str());
			}else if (!m_pfile->is_open() || m_pfile->fail()){
				m_pfile->open(filename.c_str());
			}else {
				//file already open, do nothing
				return true;
			}
			if (m_pfile->is_open())
			{
				m_filename = filename;
				return true;
			}
			else //file could not be opened
			{
				return false;
			}
		}

	protected:
		string m_filename;
		ifstream* m_pfile;
		int m_lastfound_fpos;
	};
};

#endif //*TFN_FILEBIBLE_H
