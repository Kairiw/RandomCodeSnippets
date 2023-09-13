#pragma once

#include <string>
#include <istream>
#include <vector>
#include <sstream>


/*
A very simple csv file parser I wrote for something
Not idea what I wrote it for or why, probably shouldn't be used by anyone

*/
class csvparser {
    
public:
    
    std::string const& operator[](std::size_t index) const {
        return m_data[index];
    }

    std::size_t size() const {
        return m_data.size();
    }

    void readNextRow(std::istream& str) {
        std::string line;
        std::getline(str, line);

        std::stringstream lineStream(line);
        std::string cell;

        m_data.clear();
        while(std::getline(lineStream, cell, ',')) {
            m_data.push_back(cell);
        }
        if(!lineStream && cell.empty()) {
            m_data.push_back("");
        }
    }

    friend std::istream& operator>>(std::istream& str, csvparser& data) {
        data.readNextRow(str);
        return str;
    }

private:
    std::vector<std::string> m_data;

};
