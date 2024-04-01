#ifndef FS_HPP
#define FS_HPP

#include <vector>
#include <string>

/// @brief The namespace that contians components dedicated to provice a files/directoriess access.
namespace FS
{
    /// @brief The class that represents file systems's file and includes the methods to work with it.
    class File
    {
        std::string _name;
        std::string _path;
    public:
        /// @brief The constant the represents path separator.
        static char const PathSeparator = '/';

        /// @brief The constructor.
        /// @param name The parameter that represents a name of the file.
        /// @param path The parameter that represents a path to the file.
        File(std::string name, std::string path);
        
        /// @brief The method that gets a name of the file.
        /// @return Returns a name of the file.
        std::string get_name() const;

        /// @brief The method that gets a path of the file.
        /// @return Returns a path of the file.
        std::string get_path() const;

        /// @brief The method that get a full name(path and name) of the file.
        /// @return Returns a full name(path and name) of the file.
        std::string get_full_name() const;

        /// @brief The method that checks does file exists of not.
        /// @param The parameter that represents a path of the file.
        /// @return Returns true if file exists otherwise false.
        static bool exists(std::string path);
    };
    /// @brief The class that represents file systems's directory and includes the methods to work with it.
    class Directory
    {
    public:
        /// @brief The method that gets a current user's home directory path.
        /// @return Returns a current users' hime directory path,
        static std::string get_home_directory();
        /// @brief The method that checkes does directory exists or not.
        /// @param path The parameter that represents a path to check.
        /// @return Returns true if directory exists otherwise false. 
        static bool exists(std::string path);
        /// @brief The method that create a directory by the provided path.
        /// @param path The parameter that specifies a path for the directory to create.
        static void create(std::string path);
        /// @brief The method that gets files inside a directory.
        /// @param path The parameter that represents a path to the directory.
        /// @return Returns the vector of files inside a specified directory.
        static std::vector<File> get_files(std::string path);
    };
}

#endif