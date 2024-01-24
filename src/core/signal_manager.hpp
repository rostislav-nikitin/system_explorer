#ifndef SIGNAL_MANAGER_HPP
#define SIGNAL_MANAGER_HPP

#include <map>

#include "signal_type.hpp"
#include "signal.hpp"

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

			static std::map<std::string, SignalType> GetSignalTypes() const;
			static std::map<std::string, Signal> GetSignals() const;

			void SendSignal(pid_t pid, Signal &signal) const;
		};
	}
}

#endif
