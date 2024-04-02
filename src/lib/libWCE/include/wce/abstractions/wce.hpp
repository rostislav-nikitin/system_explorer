#include <string>

namespace WildCardExpr
{
    class IWildCardExpr
    {
    public:
        /// @brief The method that finds a wild cars expression within input string.
        /// @param in_str The parameter that represents an input string.
        /// @param in_pos The parameter that represents a position to start find from.
        /// @return The first position of the found wild card expression inside an input string.
        virtual size_t find(std::string const &in_str, size_t in_pos = 0) = 0;
    };
}