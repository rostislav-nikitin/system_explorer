#ifndef PROCESS_DATA_HPP
#define PROCESS_DATA_HPP

#include <wx/clntdata.h>

namespace SystemExplorer
{
	namespace Gui
	{
		class ProcessData : public wxClientData

		{
			bool _nameMatch;
		public:
			ProcessData(bool nameMatch = false);

			bool GetNameMatch() const;
		};
	}
}

#endif
