#ifndef SIGNAL_MANAGER_HPP
#define SIGNAL_MANAGER_HPP

#include <map>

#include "models/signal_type.hpp"
#include "models/signal.hpp"

namespace SystemExplorer
{
	namespace Core
	{
		class SignalManager
		{
			static std::map<std::string, SignalType> _signalTypes;
			static std::map<std::string, Signal> _signals;
		public:
			SignalManager();

			static std::vector<SignalType> GetSignalTypes();
			static std::vector<Signal> GetSignals();
			static std::vector<Signal> GetSignals(SignalType signalType);

			void SendSignal(pid_t pid, Signal &signal) const;
		};
	}
}

#endif
