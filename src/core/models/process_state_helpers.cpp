#include "process_state_helpers_impl.hpp"

namespace SystemExplorer
{
    namespace Core
    {
        namespace Models
        {
            ProcessState GetProcessState(char state)
            {
                static std::map<char, ProcessState> mappings(
                    {
                        {'R', ProcessState::Running},
                        {'S', ProcessState::Sleeping},
                        {'D', ProcessState::Waiting},
                        {'Z', ProcessState::Zombie},
                        {'T', ProcessState::Stopped},
                        {'t', ProcessState::TracingStop},
                        {'W', ProcessState::Paging},
                        {'X', ProcessState::Dead1},
                        {'x', ProcessState::Dead2},
                        {'K', ProcessState::Wakekill},
                        {'W', ProcessState::Waking},
                        {'P', ProcessState::Parked},
                        {'I', ProcessState::Idle}
                    });

                if(mappings.find(state) == mappings.end())
                    return ProcessState::Unknown;
                    
                return mappings[state];
            }

            std::string to_string(ProcessState state)
            {
                static std::map<ProcessState, std::string> mappings(
                    {
                        {ProcessState::Unknown, "Unknown"},
                        {ProcessState::Running, "Running"},
                        {ProcessState::Sleeping, "Sleeping"},
                        {ProcessState::Waiting, "Waiting"},
                        {ProcessState::Zombie, "Zombie"},
                        {ProcessState::Stopped, "Stopped"},
                        {ProcessState::TracingStop, "TracingStop"},
                        {ProcessState::Paging, "Paging"},
                        {ProcessState::Dead1, "Dead1"},
                        {ProcessState::Dead2, "Dead2"},
                        {ProcessState::Wakekill, "Wakekill"},
                        {ProcessState::Waking, "Waking"},
                        {ProcessState::Parked, "Parked"},
                        {ProcessState::Idle, "Idle"}
                    });

                if(mappings.find(state) == mappings.end())
                    return mappings[ProcessState::Unknown];
                    
                return mappings[state];

            }
        }
    }
}