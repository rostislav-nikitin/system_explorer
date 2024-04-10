#include "../../../include/os/signal/model/signal.hpp"

namespace OS
{
	namespace Signal
	{
		namespace Model
		{
			
			Signal::Signal() : _id(0), _signalType(), _standard(), _alias(), _description() { }

			Signal::Signal(int id, std::string alias, SignalType signalType, std::string standard, std::string description)
				: _id(id), _alias(alias), _signalType(signalType), _standard(standard), _description(description)
			{
			}

			int Signal::GetId() const
			{
				return _id;
			}
			SignalType Signal::GetSignalType()
			{
				return _signalType;
			}
			std::string Signal::GetStandard() const
			{
				return _standard;
			}
			std::string Signal::GetAlias() const
			{
				return _alias;
			}
			std::string Signal::GetDescription() const
			{
				return _description;
			}
		}
	}
}
