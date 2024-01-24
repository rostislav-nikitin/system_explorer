#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <string>

#include "signal_type.hpp"

namespace SystemExplorer
{
	namespace Core
	{
		namespace Models
		{
			class Signal
			{
				int _id;
				SignalType &_signalType;
				std::string _standard;
				std::string _alias;
				std::string _description;
			public:
				SignalType(int id, SignalType &signalType, std::string _standard, std::string alias, std::string description);

				int GetId() const;
				SignalType &GetSignalType();
				std::string GetStandard() const;
				std::string GetAlias() const;
				std::string GetDescription() const;
			};
		}
	}
}

#endif
