#ifndef SIGNALS_DICT_HPP
#define SIGNALS_DICT_HPP

#include <string>
#include <vector>
#include <map>

#include "models/signal.hpp"

namespace SystemExplorer
{
	namespace Core
	{
		using namespace SystemExplorer::Core::Models;

		class SignalsDict
		{
			static SignalsDict *_instance;

			std::map<std::string, Signal> _signals;
			SignalsDict();
		public:
			static SignalsDict* GetInstance();

			Signal Get(std::string const &alias);
			std::vector<Signal> GetAll() const;
	
		};
	}
}

#endif
