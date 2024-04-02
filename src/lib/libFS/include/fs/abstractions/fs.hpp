#ifndef LIBFS_ABSTRACTIONS_FS_HPP
#define LIBFS_ABSTRACTIONS_FS_HPP

#include <string>

/// @brief The namespace that contians components dedicated to provice a files/directoriess access.
namespace FS
{
    /// @brief The class that represents file systems's file and includes the methods to work with it.
    class IFile
    {
        /// @brief The method that gets a name of the file.
        /// @return Returns a name of the file.
        virtual std::string get_name() const = 0;

        /// @brief The method that gets a path of the file.
        /// @return Returns a path of the file.
        virtual std::string get_path() const = 0;

        /// @brief The method that get a full name(path and name) of the file.
        /// @return Returns a full name(path and name) of the file.
        virtual std::string get_full_name() const = 0;
    };

    /// @brief The class that represents file systems's directory and includes the methods to work with it.
    class IDirectory
    {
    };
}

#endif