#ifndef VPLBibleH
#define VPLBibleH

#include "FileBible.h"
#include <string>
using namespace std;

namespace TearsForNations
{

	class VPLBible : public FileBible
	{
	public:
		//*Constructor
		VPLBible(string version, string filename="")
		   : FileBible(version, filename)
		{
			//use the default directory if the user did not specify
			if (!m_pfile || !m_pfile->is_open())
			{
			   openFile(string("C:\\Utility\\bibles\\vpl\\") + m_version + ".vpl.txt");
			}
		}

		//find a bible verse containing the given string
		virtual const string& find(string str, int start_absolute_vs = 0);
		virtual const string& getScripture(int bk, int ch, int vs);
		virtual const string& getScripture(int absolute_vs);

	};
};

#endif
