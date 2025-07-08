#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class StyleManager {
private:
    string stylesheetContent;

public:
    StyleManager() {
        // Default constructor
    }

    bool loadStylesheet(const string& filepath) {
        ifstream file(filepath);
        if (!file.is_open()) {
            cerr << "Error: Could not open stylesheet file: " << filepath << endl;
            return false;
        }

        string line;
        stylesheetContent = "";
        
        while (getline(file, line)) {
            stylesheetContent += line + "\n";
        }
        
        file.close();
        return true;
    }

    string getStylesheet() const {
        return stylesheetContent;
    }

    static StyleManager& getInstance() {
        static StyleManager instance;
        return instance;
    }
};

#endif // STYLEMANAGER_H 