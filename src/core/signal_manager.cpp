#include "signal_manager_impl.hpp"

namespace SystemExplorer
{
	namespace Core
	{
		class SignalManager
		{
		public:
			SignalManager();

			static std::map<std::string, SignalType> GetSignalTypes() const
			{
				return _signalTypes;
			}
			static std::map<std::string, Signal> GetSignals() const
			{
				return _signals;
			}

			void SendSignal(pid_t pid, Signal &signal) const
			{
           		kill(pid, signal.GetId());
			}
		};

		static std::map<std::string, SignalType> SignalManger::_signalTypes;
		static std::map<std::string, Signal> SignalManger::_signals;
	}
}

#endif
