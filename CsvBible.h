#ifndef TFN_CSVBIBLE_H
#define TFN_CSVBIBLE_H

#include "FileBible.h"

#include "csvparser.h"

#include <string>
using namespace std;

namespace TearsForNations
{
	class CsvBible : public FileBible
	{
	public:
		//*Constructor
		CsvBible(string version, string filename="")
		   : FileBible(version, filename)
		{
			//use the default directory if the user did not specify
			if (!m_pfile || !m_pfile->is_open())
			{
			   openFile(string("C:\\Utility\\bibles\\csv\\") + m_version + ".csv");
			}
		}

		//find a bible verse containing the given string
		virtual const string& find(string str, int start_absolute_vs = 0);
		virtual const string& getScripture(int bk, int ch, int vs);
		virtual const string& getScripture(int absolute_vs);

	};

};

#endif //*TFN_CSVBIBLE_H
