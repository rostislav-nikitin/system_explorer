#ifndef LIBOS_SIGNAL_MODEL_SIGNAL_TYPE_HPP
#define LIBOS_SIGNAL_MODEL_SIGNAL_TYPE_HPP

#include <string>

namespace OS
{
	namespace Signal
	{
		namespace Model
		{
			class SignalType
			{
				int _id;
				std::string _alias;
				std::string _description;
			public:
				SignalType();
				SignalType(int id, std::string alias, std::string description);

				int GetId() const;
				std::string GetAlias() const;
				std::string GetDescription() const;
			};
		}
	}
}

#endif
