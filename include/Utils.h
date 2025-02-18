/*
 Copyright (C) 2024 BeamMP Ltd., BeamMP team and contributors.
 Licensed under AGPL-3.0 (or later), see <https://www.gnu.org/licenses/>.
 SPDX-License-Identifier: AGPL-3.0-or-later
*/

#pragma once
#include <string>
#include <vector>
#include <algorithm>

namespace Utils {
    inline std::vector<std::string> Split(const std::string& String, const std::string& delimiter) {
        std::vector<std::string> Val;
        size_t pos;
        std::string token, s = String;
        while ((pos = s.find(delimiter)) != std::string::npos) {
            token = s.substr(0, pos);
            if (!token.empty())
                Val.push_back(token);
            s.erase(0, pos + delimiter.length());
        }
        if (!s.empty())
            Val.push_back(s);
        return Val;
    }

    inline std::string ToLower(const std::string& str) {
        std::string lowerStr = str;
        std::transform(str.begin(), str.end(), lowerStr.begin(), ::tolower);
        return lowerStr;
    }

    inline std::string Trim(const std::string& s) {
        auto start = s.find_first_not_of(" \t\n\r");
        auto end = s.find_last_not_of(" \t\n\r");
        return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
    }

    #if defined(__APPLE__)
        inline std::pair<std::string, int> runCommand(const char* cmd) {
            std::array<char, 128> buffer;
            std::string result;
            std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
            if (!pipe) {
                throw std::runtime_error("failed to run popen()");
            }
            while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
                result += buffer.data();
            }    
            int returnCode = pclose(pipe.release());
            return { result, returnCode };
        }
    #endif
}