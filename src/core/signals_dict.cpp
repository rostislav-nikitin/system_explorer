#include "signals_dict_impl.hpp"

namespace SystemExplorer
{
	namespace Core
	{
		SignalsDict::SignalsDict()
		{
			// Term
			_signals.insert(std::make_pair("SIGALRM", 
				Signal(SIGALRM, "SIGALARM", SignalTypesDict::GetInstance()->Get("Term"), "P1990", "Timer signal from alarm(2)")));
			_signals.insert(std::make_pair("SIGPIPE", 
				Signal(SIGPIPE, "SIGPIPE", SignalTypesDict::GetInstance()->Get("Term"), "P1990", "Broken pipe: write to pipe with no readers; see pipe(7)")));
/*
			_signals.insert(std::make_pair("SIGEMT", 
				Signal(SIGEMT, "SIGEMT", SignalTypesDict::GetInstance()->Get("Term"), "", "Emulator trap")));
			_signals.insert(std::make_pair("SIGLOST", 
				Signal(SIGLOST, "SIGLOST", SignalTypesDict::GetInstance()->Get("Term"), "", "File lock lost (unused)")));
*/
			_signals.insert(std::make_pair("SIGHUP", 
				Signal(SIGHUP, "P1990", SignalTypesDict::GetInstance()->Get("Term"), "P1990", "Hangup detected on controlling terminal or death of controlling process")));
			_signals.insert(std::make_pair("SIGINT", 
				Signal(SIGINT, "SIGINT", SignalTypesDict::GetInstance()->Get("Term"), "P1990", "P1990")));
			_signals.insert(std::make_pair("SIGIO", 
				Signal(SIGIO, "SIGIO", SignalTypesDict::GetInstance()->Get("Term"), "", "I/O now possible (4.2BSD)")));
			_signals.insert(std::make_pair("SIGKILL", 
				Signal(SIGKILL, "SIGKILL", SignalTypesDict::GetInstance()->Get("Term"), "P1990", "Kill signal")));
			_signals.insert(std::make_pair("SIGPOLL", 
				Signal(SIGPOLL, "SIGPOLL", SignalTypesDict::GetInstance()->Get("Term"), "P2001", "Pollable event (Sys V); synonym for SIGIO")));
			_signals.insert(std::make_pair("SIGPWR",
				Signal(SIGPWR, "SIGPWR", SignalTypesDict::GetInstance()->Get("Term"), "", "Power failure (System V)")));
			_signals.insert(std::make_pair("SIGPROF", 
				Signal(SIGPROF, "SIGPROF", SignalTypesDict::GetInstance()->Get("Term"), "P2001", "Profiling timer expired")));
			_signals.insert(std::make_pair("SIGSTKFLT", 
				Signal(SIGSTKFLT, "SIGSTKFLT", SignalTypesDict::GetInstance()->Get("Term"), "", "Stack fault on coprocessor (unused)")));
			_signals.insert(std::make_pair("SIGTERM", 
				Signal(SIGTERM, "SIGTERM", SignalTypesDict::GetInstance()->Get("Term"), "P1990", "Termination signal")));
			_signals.insert(std::make_pair("SIGUSR1", 
				Signal(SIGUSR1, "SIGUSR1", SignalTypesDict::GetInstance()->Get("Term"), "P1990", "User-defined signal 1")));
			_signals.insert(std::make_pair("SIGUSR2", 
				Signal(SIGUSR2, "SIGUSR2", SignalTypesDict::GetInstance()->Get("Term"), "P1990", "User-defined signal 2")));
			_signals.insert(std::make_pair("SIGVTALRM", 
				Signal(SIGVTALRM, "SIGVTALRM", SignalTypesDict::GetInstance()->Get("Term"), "P2001", "Virtual alarm clock (4.2BSD)")));

			// Cont
			_signals.insert(std::make_pair("SIGCONT", 
				Signal(SIGCONT, "SIGCONT", SignalTypesDict::GetInstance()->Get("Cont"), "P1990", "Continue if stopped")));

			//	Core
			_signals.insert(std::make_pair("SIGABRT", 
				Signal(SIGABRT, "SIGABRT", SignalTypesDict::GetInstance()->Get("Core"), "P1990", "Abort signal from abort(3)")));
			_signals.insert(std::make_pair("SIGSYS", 
				Signal(SIGSYS, "SIGSYS", SignalTypesDict::GetInstance()->Get("Core"), "P2001", "Bad system call (SVr4); see also seccomp(2)")));
			_signals.insert(std::make_pair("SIGBUS", 
				Signal(SIGBUS, "SIGBUS", SignalTypesDict::GetInstance()->Get("Core"), "P2001", "Bus error (bad memory access)")));
			_signals.insert(std::make_pair("SIGXCPU", 
				Signal(SIGXCPU, "SIGXCPU", SignalTypesDict::GetInstance()->Get("Core"), "P2001", "CPU time limit exceeded (4.2BSD); see setrlimit(2)")));
			_signals.insert(std::make_pair("SIGXFSZ", 
				Signal(SIGXFSZ, "SIGXFSZ", SignalTypesDict::GetInstance()->Get("Core"), "P2001", "File size limit exceeded (4.2BSD); see setrlimit(2)")));
			_signals.insert(std::make_pair("SIGFPE", 
				Signal(SIGFPE, "SIGFPE", SignalTypesDict::GetInstance()->Get("Core"), "P1990", "Floating-point exception")));
			_signals.insert(std::make_pair("SIGILL", 
				Signal(SIGILL, "SIGILL", SignalTypesDict::GetInstance()->Get("Core"), "P1990", "Illegal Instruction")));
			_signals.insert(std::make_pair("SIGSEGV", 
				Signal(SIGSEGV, "SIGSEGV", SignalTypesDict::GetInstance()->Get("Core"), "P1990", "Invalid memory reference")));
			_signals.insert(std::make_pair("SIGIOT", 
				Signal(SIGIOT, "SIGIOT", SignalTypesDict::GetInstance()->Get("Core"), "", "IOT trap. A synonym for SIGABRT")));
			_signals.insert(std::make_pair("SIGQUIT", 
				Signal(SIGQUIT, "SIGQUIT", SignalTypesDict::GetInstance()->Get("Core"), "P1990", "Quit from keyboard")));
/*
			_signals.insert(std::make_pair("SIGUNUSED", 
				Signal(SIGUNUSED, "SIGUNUSED", SignalTypesDict::GetInstance()->Get("Core"), "", "Synonymous with SIGSYS")));
*/
			_signals.insert(std::make_pair("SIGTRAP", 
				Signal(SIGTRAP, "SIGTRAP", SignalTypesDict::GetInstance()->Get("Core"), "P2001", "Trace/breakpoint trap")));

			// Ign
			_signals.insert(std::make_pair("SIGCLD", 
				Signal(SIGCLD, "SIGCLD", SignalTypesDict::GetInstance()->Get("Ign"), "", "A synonym for SIGCHLD")));
			_signals.insert(std::make_pair("SIGCHLD", 
				Signal(SIGCHLD, "SIGCHLD", SignalTypesDict::GetInstance()->Get("Ign"), "P1990", "Child stopped or terminated")));
			_signals.insert(std::make_pair("SIGURG", 
				Signal(SIGURG, "SIGURG", SignalTypesDict::GetInstance()->Get("Ign"), "P2001", "Urgent condition on socket (4.2BSD)")));
			_signals.insert(std::make_pair("SIGWINCH", 
				Signal(SIGWINCH, "SIGWINCH", SignalTypesDict::GetInstance()->Get("Ign"), "", "Window resize signal (4.3BSD, Sun)")));

			// Stop
			_signals.insert(std::make_pair("SIGSTOP", 
				Signal(SIGSTOP, "SIGSTOP", SignalTypesDict::GetInstance()->Get("Stop"), "P1990", "Stop process")));
			_signals.insert(std::make_pair("SIGTSTP", 
				Signal(SIGTSTP, "SIGTSTP", SignalTypesDict::GetInstance()->Get("Stop"), "P1990", "Stop typed at terminal")));
			_signals.insert(std::make_pair("SIGTTIN", 
				Signal(SIGTTIN, "SIGTTIN", SignalTypesDict::GetInstance()->Get("Stop"), "P1990", "Terminal input for background process")));
			_signals.insert(std::make_pair("SIGTTOU", 
				Signal(SIGTTOU, "SIGTTOU", SignalTypesDict::GetInstance()->Get("Stop"), "P1990", "Terminal output for background process")));

			//	Other
			/*
			_signals.insert(std::make_pair("SIGINFO", 
				Signal(SIGINFO, "SIGINFO", SignalTypesDict::GetInstance()->Get("Other"), "", "A synonym for SIGPWR")));
			*/

/*
			_signals.insert(std::make_pair("SIGTTOU", 
				Signal(, "", SignalTypesDict::GetInstance()->Get("Term"), "", "")));
*/
		}

		SignalsDict* SignalsDict::GetInstance()
		{
			//TODO:Add locking
			if(_instance == nullptr)
				_instance = new SignalsDict();

			return _instance;
		}

		Signal SignalsDict::Get(std::string const &alias)
		{
			return _signals[alias];
		}

		std::vector<Signal> SignalsDict::GetAll() const
		{
			std::vector<Signal> result;

			std::for_each(_signals.begin(), _signals.end(), 
				[&result](typename std::map<std::string, Signal>::value_type const &item)
				{
					result.push_back(item.second);
				});

			return result;
		}
	
		SignalsDict *SignalsDict::_instance = nullptr;
	}
}
