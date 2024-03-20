#ifndef FS_HPP
#define FS_HPP

#include <vector>
#include <string>

namespace FS
{
    class File
    {
        static const char PATH_DELIMITER = '/';

        std::string _name;
        std::string _path;
    public:
        File(std::string name, std::string path);
        
        std::string get_name() const;
        std::string get_path() const;
        std::string get_full_name() const;

        static bool exists(std::string path);
    };

    class Directory
    {
    public:
        static std::string get_home_directory();
        static bool exists(std::string path);
        static void create(std::string path);
        static std::vector<File> get_files(std::string path);
    };
}

#endif