#include "signal_manager_impl.hpp"

namespace SystemExplorer
{
	namespace Core
	{
		SignalManager::SignalManager()
		{
		}

		std::vector<SignalType> SignalManager::GetSignalTypes()
		{
			return SignalTypesDict::GetInstance()->GetAll();
		}
		std::vector<Signal> SignalManager::GetSignals()
		{
			return SignalsDict::GetInstance()->GetAll();
		}
		std::vector<Signal> SignalManager::GetSignals(SignalType signalType)
		{
//			return _signals;
		}

		void SignalManager::SendSignal(pid_t pid, int signal)
		{
       		kill(pid, signal);
		}
	}
}
