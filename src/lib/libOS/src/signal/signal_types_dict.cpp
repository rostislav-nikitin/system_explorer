#include "../../include/os/signal/signal_types_dict_impl.hpp"

namespace OS
{
	namespace Signal
	{
		SignalTypesDict::SignalTypesDict()
		{
			_signalTypes.insert(std::make_pair("Term", Model::SignalType(1, "Term", "Default action is to terminate the process.")));
			_signalTypes.insert(std::make_pair("Ign", Model::SignalType(2, "Ign", "Default action is to ignore the signal.")));
			_signalTypes.insert(std::make_pair("Core", Model::SignalType(3, "Core", "Default action is to terminate the process and dump core (see core(5)).")));
			_signalTypes.insert(std::make_pair("Stop", Model::SignalType(4, "Stop", "Default action is to stop the process.")));
			_signalTypes.insert(std::make_pair("Cont", Model::SignalType(5, "Cont", "Default action is to continue the process if it is currently stopped.")));
			//_signalTypes.insert(std::make_pair("Other", SignalType(6, "Other", "Othrt signals.")));
		}

		SignalTypesDict* SignalTypesDict::GetInstance()
		{
			//TODO:Add locking
			if(_instance == nullptr)
				_instance = new SignalTypesDict();

			return _instance;
		}

		Model::SignalType SignalTypesDict::Get(std::string const &alias)
		{
			return _signalTypes[alias];
		}

		std::vector<Model::SignalType> SignalTypesDict::GetAll() const
		{
			std::vector<Model::SignalType> result;

			std::for_each(_signalTypes.begin(), _signalTypes.end(), 
				[&result](typename std::map<std::string, Model::SignalType>::value_type const &item)
				{
					result.push_back(item.second);
				});

			return result;
		}
	
		SignalTypesDict *SignalTypesDict::_instance = nullptr;
	}
}
