#include "signal_type.hpp"

namespace SystemExplorer
{
	namespace Core
	{
		namespace Models
		{
			SignalType::SignalType() : SignalType(0, "", "") { }
			SignalType::SignalType(int id, std::string alias, std::string description) : _id(id), _alias(alias), _description(description)
			{
			}

			int SignalType::GetId() const
			{
				return _id;
			}
			std::string SignalType::GetAlias() const
			{
				return _alias;
			}
			std::string SignalType::GetDescription() const
			{
				return _description;
			}
		}
	}
}
