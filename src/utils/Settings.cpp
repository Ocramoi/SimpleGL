#include "Settings.hpp"

char Settings::delimiter = '=';

Settings::Settings(const string& path) {
    ifstream input{path, std::ifstream::in};
    if (!input.good()) {
        cerr << "settings.conf not found!" << endl;
        input.close();
        return;
    }

    string key, val,
        line; getline(input, line);
    while (input.good()) {
        stringstream ss{line};
        try {
            getline(ss, key, delimiter);
            getline(ss, val, delimiter);
            values[key] = val;
        } catch (const exception& err) {
            cerr << "Malformed settings file:";
            cerr << '\t' << err.what() << endl;
        }
        getline(input, line);
    }

    input.close();
}

float Settings::getValue(const string &key, float) {
    return stof(values.at(key));
}

int Settings::getValue(const string &key, int) {
    return stoi(values.at(key));
}

double Settings::getValue(const string &key, double) {
    return stod(values.at(key));
}
