#ifndef LIBOS_SIGNAL_MODEL_SIGNAL_HPP
#define LIBOS_SIGNAL_MODEL_SIGNAL_HPP

#include <string>

#include "signal_type.hpp"

namespace OS
{
	namespace Signal
	{
		namespace Model
		{
			class Signal
			{
				int _id;
				std::string _alias;
				SignalType _signalType;
				std::string _standard;
				std::string _description;
			public:
				Signal();
				Signal(int id, std::string alias, SignalType signalType, std::string _standard, std::string description);

				int GetId() const;
				std::string GetAlias() const;
				SignalType GetSignalType();
				std::string GetStandard() const;
				std::string GetDescription() const;
			};
		}
	}
}

#endif
