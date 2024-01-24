#include "signal.hpp"

namespace SystemExplorer
{
	namespace Core
	{
		namespace Models
		{
			SignalType::SignalType(int id, SignalType &signalType, std::string _standard, std::string alias, std::string description)
				: _id(id), _signalType(signalType), _standard(standard), _alias(alias), _description(description)
			{
				
			}

			int GetId() const
			{
				return _id;
			}
			SignalType &GetSignalType()
			{
				return _signalType;
			}
			std::string GetStandard() const
			{
				return _standard;
			}
			std::string GetAlias() const
			{
				return _alias;
			}
			std::string GetDescription() const
			{
				return _description;
			}
		}
	}
}
