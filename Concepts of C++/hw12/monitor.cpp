#include "monitor.h"

#include <thread>



FileMonitor::FileMonitor(const std::string &targetpath, std::chrono::milliseconds interval, const std::string &logfilename)
    : logger{logfilename}, interval{interval}, directory{targetpath} {
    
}

// Monitor the targetpath for changes and pass information to the logger in case of differences
void FileMonitor::start(std::chrono::seconds duration) {
    auto startTime = std::chrono::system_clock::now();
    while (std::chrono::system_clock::now() - startTime < duration) {
        checkDirectory();
        std::this_thread::sleep_for(interval); // 使用类成员 interval
    }
}

void FileMonitor::checkDirectory() {
    std::unordered_map<std::string, std::filesystem::file_time_type> currentFiles;
    std::string filePath = file.path().string();
    for (const auto& file : std::filesystem::recursive_directory_iterator(directory)) {
        
            
        auto lastWriteTime = std::filesystem::last_write_time(file.path());
            
        currentFiles[filePath] = lastWriteTime;

        if (fileTimestamps.find(filePath) == fileTimestamps.end()) {
            logger.log(filePath, status::added);
            fileTimestamps[filePath] = lastWriteTime;
        } else if (fileTimestamps[filePath] != lastWriteTime) {
            logger.log(filePath, status::changed);
            fileTimestamps[filePath] = lastWriteTime;
        }
        
    }

    for (const auto& [path, time] : fileTimestamps) {
        if (currentFiles.find(path) == currentFiles.end()) {
            logger.log(path, status::removed);
        }
    }
    fileTimestamps = currentFiles;
}

