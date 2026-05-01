#include <iostream>
#include <filesystem>
#include <string>
#include <unistd.h>
#include <limits.h>

namespace fs = std::filesystem;

class ApplicationPath {
private:
    fs::path executable_path;
    fs::path executable_dir;
    fs::path current_work_dir;
    
public:
    ApplicationPath(char* argv0) {
        // 1. Получаем путь к исполняемому файлу
        executable_path = get_executable_path(argv0);
        executable_dir = executable_path.parent_path();
        
        // 2. Получаем текущую рабочую директорию
        current_work_dir = fs::current_path();
    }
    
    // Получить абсолютный путь к исполняемому файлу
    fs::path get_executable_path() const {
        return executable_path;
    }
    
    // Получить директорию исполняемого файла
    fs::path get_executable_dir() const {
        return executable_dir;
    }
    
    // Получить текущую рабочую директорию
    fs::path get_current_work_dir() const {
        return current_work_dir;
    }
    
    // Получить абсолютный путь относительно директории исполняемого файла
    fs::path get_path_relative_to_exe(const std::string& relative_path) const {
        return fs::absolute(executable_dir / relative_path);
    }
    
    // Получить абсолютный путь относительно текущей директории
    fs::path get_path_relative_to_cwd(const std::string& relative_path) const {
        return fs::absolute(current_work_dir / relative_path);
    }
    
    // Умный поиск файла (сначала относительно exe, потом CWD)
    fs::path find_file(const std::string& filename) const {
        // Сначала ищем рядом с исполняемым файлом
        fs::path exe_path = executable_dir / filename;
        if (fs::exists(exe_path)) {
            return fs::canonical(exe_path);
        }
        
        // Затем в текущей рабочей директории
        fs::path cwd_path = current_work_dir / filename;
        if (fs::exists(cwd_path)) {
            return fs::canonical(cwd_path);
        }
        
        // Если не нашли, возвращаем путь относительно exe как наиболее вероятный
        return fs::absolute(exe_path);
    }
    
private:
    fs::path get_executable_path(const char* argv0) {
        // Способ 1: через /proc/self/exe (Linux)
        char buffer[PATH_MAX];
        ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
        if (len != -1) {
            buffer[len] = '\0';
            return fs::path(buffer);
        }
        
        // Способ 2: через argv0 (fallback)
        fs::path path(argv0);
        if (path.is_absolute()) {
            return path;
        }
        
        // Способ 3: комбинация с PATH
        const char* path_env = getenv("PATH");
        if (path_env) {
            std::string path_str(path_env);
            size_t start = 0;
            size_t end;
            do {
                end = path_str.find(':', start);
                std::string dir = path_str.substr(start, end - start);
                fs::path full_path = fs::path(dir) / argv0;
                if (fs::exists(full_path)) {
                    return fs::canonical(full_path);
                }
                start = end + 1;
            } while (end != std::string::npos);
        }
        
        // Последний шанс - текущая директория
        return fs::absolute(argv0);
    }
};
