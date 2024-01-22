#include "process_data.hpp"

namespace SystemExplorer
{
	namespace Gui
	{
		ProcessData::ProcessData(bool nameMatch) : _nameMatch(nameMatch) 
		{
		}

		bool ProcessData::GetNameMatch() const
		{
			return _nameMatch;
		}
	}
}
