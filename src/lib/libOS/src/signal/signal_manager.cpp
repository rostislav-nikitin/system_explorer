#include "../../include/os/signal/signal_manager_impl.hpp"

namespace OS
{
	namespace Signal
	{
		SignalManager::SignalManager()
		{
		}

		std::vector<Model::SignalType> SignalManager::GetSignalTypes()
		{
			return SignalTypesDict::GetInstance()->GetAll();
		}
		std::vector<Model::Signal> SignalManager::GetSignals()
		{
			return SignalsDict::GetInstance()->GetAll();
		}
		std::vector<Model::Signal> SignalManager::GetSignals(Model::SignalType signalType)
		{
//			return _signals;
		}
		Model::Signal SignalManager::GetSignal(std::string const &alias)
		{
			return SignalsDict::GetInstance()->Get(alias);
		}

		void SignalManager::SendSignal(pid_t pid, int signal)
		{
       		kill(pid, signal);
		}
	}
}
