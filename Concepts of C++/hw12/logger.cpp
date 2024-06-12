#include "logger.h"


#include <iostream>
#include <filesystem>
#include <chrono>
#include <iomanip>

Logger::Logger(const std::string& filename) {
    // Check if the file exists
    if (std::filesystem::exists(filename)) {
        // Construct the backup filename
        std::string new_filename = filename + "_old";
        // Rename the existing file to the backup file
        std::filesystem::rename(filename, new_filename);
    }

    // Open the new log file
    file.open(filename, std::ios::out);

    // Write the initial log message
    if (file.is_open()) {
        std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        file << "Commencing logging for directory: "
             << std::filesystem::current_path()
             << std::endl
             << "Current time is: "
             << std::ctime(&time)
             << std::endl;
    } else {
        std::cerr << "Failed to open log file: " << filename << std::endl;
    }
    
}

Logger::~Logger() {
    // explicit file closing needed in Windows
    file.close();
}

void Logger::log(const std::string &path, status what) {
    if (not file.is_open()) {
        throw std::runtime_error("File could not be opened!");
    }

    switch (what) {
    case status::added :
        file << "+ File was added:    " << path << std::endl;
        break;
    case status::changed :
        file << "! File was modified: " << path << std::endl;
        break;
    case status::removed :
        file << "~ File was deleted:  " << path << std::endl;
        break;
    default:
        file << "? Error! Unknown file status detected for: " << path << std::endl;
        break;
    }
}
