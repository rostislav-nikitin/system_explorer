#include "fs_impl.hpp"

#include <iostream>

namespace FS
{
    File::File(std::string path, std::string name) : _path(path), _name(name)
    {
    }
    std::string File::get_path() const
    {
        return _path;
    }
    std::string File::get_name() const
    {
        return _name;
    }
    std::string File::get_full_name() const
    {
        return _path + std::string(1, '/') + _name;
    }

    bool File::exists(std::string path)
    {
        bool result = access(path.c_str(), F_OK) == 0;

        return result;
    }

    std::string Directory::PathSeparator = "/";

    std::string Directory::get_home_directory()
    {
        struct passwd *pw = getpwuid(getuid());

        const char *homedir = pw->pw_dir;

        return std::string(homedir);
    }

    bool Directory::exists(std::string path)
    {
        bool result = false;

        DIR* dir = opendir(path.c_str());
        if (dir) 
        {
            result = true;
            closedir(dir);
        }

        return result;
    }

    void Directory::create(std::string path)
    {
        struct stat st = {0};

        if (stat(path.c_str(), &st) == -1) {
            mkdir(path.c_str(), 0700);
        }
    }
    std::vector<File> Directory::get_files(std::string path)
    {
        std::vector<File> result;

        DIR *dir;
        dir = opendir(path.c_str());
         
        dirent *ent;
        while((ent = readdir(dir)) != NULL)
        {
            File file(path, ent->d_name);
            if(file.get_name() != "." && file.get_name() != "..")
            {
                result.push_back(file);
                //std::cout << file.get_name() << "::" << file.get_full_name() << std::endl;
            }
        }

        closedir(dir);

        return result;
   }

}