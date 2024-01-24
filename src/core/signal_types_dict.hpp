#ifndef SIGNAL_TYPE_DICT_HPP
#define SIGNAL_TYPE_DICT_HPP

#include <string>
#include <vector>
#include <map>

#include "models/signal_type.hpp"

namespace SystemExplorer
{
	namespace Core
	{
		using namespace SystemExplorer::Core::Models;

		class SignalTypesDict
		{
			static SignalTypesDict *_instance;

			std::map<std::string, SignalType> _signalTypes;
			SignalTypesDict();
		public:
			static SignalTypesDict* GetInstance();

			SignalType Get(std::string const &alias);
			std::vector<SignalType> GetAll() const;
	
		};
	}
}

#endif
