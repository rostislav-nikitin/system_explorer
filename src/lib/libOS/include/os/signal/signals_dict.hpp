#ifndef LIBOS_SIGNAL_SIGNALS_DICT_HPP
#define LIBOS_SIGNAL_SIGNALS_DICT_HPP

#include <string>
#include <vector>
#include <map>

#include "model/signal.hpp"

namespace OS
{
	namespace Signal
	{
		class SignalsDict
		{
			static SignalsDict *_instance;

			std::map<std::string, Model::Signal> _signals;
			SignalsDict();
		public:
			static SignalsDict* GetInstance();

			Model::Signal Get(std::string const &alias);
			std::vector<Model::Signal> GetAll() const;
	
		};
	}
}

#endif
