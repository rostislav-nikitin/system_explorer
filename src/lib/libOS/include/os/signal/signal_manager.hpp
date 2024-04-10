#ifndef LIBOS_SIGNAL_SIGNAL_MANAGER_HPP
#define LIBOS_SIGNAL_SIGNAL_MANAGER_HPP

#include <vector>
#include <map>

#include "model/signal_type.hpp"
#include "model/signal.hpp"

namespace OS
{
	namespace Signal
	{
		class SignalManager
		{
			static std::map<std::string, Model::SignalType> _signalTypes;
			static std::map<std::string, Model::Signal> _signals;
		public:
			SignalManager();

			static std::vector<Model::SignalType> GetSignalTypes();
			static std::vector<Model::Signal> GetSignals();
			static std::vector<Model::Signal> GetSignals(Model::SignalType signalType);
			static Model::Signal GetSignal(std::string const &alias);

			static void SendSignal(pid_t pid, int signal);
		};
	}
}

#endif
