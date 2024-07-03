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

#include "CRYP256C.h"



char key[] = "0923674598076234598672345";

std::string TargetFiles = ".cryp256c";


namespace fs = std::filesystem;


namespace Odyssey {
    bool end_with(std::string full_path) {
        std::transform(full_path.begin(), full_path.end(), full_path.begin(), [](unsigned char c) { return std::tolower(c); });
        std::cout << full_path << std::endl;

        if (full_path.size() >= TargetFiles.size() &&
            full_path.compare(full_path.size() - TargetFiles.size(), TargetFiles.size(), TargetFiles) == 0) {
            
            return true;
        }

        return false;
    }

    void Start() {
        for (const auto& entry : fs::recursive_directory_iterator(".")) {
            if (fs::is_regular_file(entry.status())) {
                std::string full_path = entry.path().string();

                if (end_with(full_path)) {
                    CRYP256C().start(full_path, key);
                }
            }
        }
    }
}


int main() {
    Odyssey::Start();

    return 0;
}