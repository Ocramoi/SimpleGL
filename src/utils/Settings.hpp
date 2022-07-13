#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <optional>

using std::string;
using std::unordered_map;
using std::ifstream;
using std::cerr;
using std::endl;
using std::getline;
using std::stringstream;
using std::exception;
using std::is_same;
using std::stof;
using std::optional;

class Settings {
    private:
        unordered_map<string, string> values;
        float getValue(const string& key, float);
        int getValue(const string& key, int);
        double getValue(const string& key, double);
    public:
        static char delimiter;
        Settings(const string& path);

        template <typename T>
        optional<T> getValue(const string& key) {
            if (values.find(key) == values.end()) return {};
            return getValue(key, T{});
        }
};

#endif // SETTINGS_H_
