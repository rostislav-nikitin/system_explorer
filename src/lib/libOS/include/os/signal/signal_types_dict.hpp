#ifndef LIBOS_SIGNAL_SIGNAL_TYPE_DICT_HPP
#define LIBOS_SIGNAL_SIGNAL_TYPE_DICT_HPP

#include <string>
#include <vector>
#include <map>

#include "model/signal_type.hpp"

namespace OS
{
	namespace Signal
	{
		class SignalTypesDict
		{
			static SignalTypesDict *_instance;

			std::map<std::string, Model::SignalType> _signalTypes;
			SignalTypesDict();
		public:
			static SignalTypesDict* GetInstance();

			Model::SignalType Get(std::string const &alias);
			std::vector<Model::SignalType> GetAll() const;
	
		};
	}
}

#endif
