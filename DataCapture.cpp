//
// Created by nicholas on 22.10.22.
//

#include <iostream>
#include <filesystem>
#include <ctime>
#include "DataCapture.h"
#define NEWLINE "\n"
#define DELIMITER ","

namespace fs = std::filesystem;

int DataCapture::setCap(int *var, std::string name) {
    if(var == nullptr)
    {
        std::cerr << "Error: Nullpointer passed \n";
        return -1;
    }

    if(name.empty())
    {
        std::cerr << "Error: Name can't be empty";
        return -2;
    }

    IntCap cap;
    cap.var = var;
    cap.name = name;

    int_caps.push_back(cap);
    return 0;
}

int DataCapture::setCap(double *var, std::string name) {
    if(var == nullptr)
    {
        std::cerr << "Error: Nullpointer passed \n";
        return -1;
    }

    if(name.empty())
    {
        std::cerr << "Error: Name can't be empty";
        return -2;
    }

    if(cap_open)
    {
        std::cerr << "Error: Capture already running";
        return -3;
    }

    DoubleCap cap;
    cap.var = var;
    cap.name = name;

    double_caps.push_back(cap);
    return 0;
}

int DataCapture::setCap(uint8_t *var, std::string name) {
    if(var == nullptr)
    {
        std::cerr << "Error: Nullpointer passed \n";
        return -1;
    }

    if(name.empty())
    {
        std::cerr << "Error: Name can't be empty\n";
        return -2;
    }
    if(cap_open)
    {
        std::cerr << "Error: Capture already running";
        return -3;
    }

    Uint8_tCap cap;
    cap.var = var;
    cap.name = name;

    uint8t_caps.push_back(cap);
    return 0;
}

int DataCapture::setCap(float *var, std::string name) {
    if(var == nullptr)
    {
        std::cerr << "Error: Nullpointer passed \n";
        return -1;
    }

    if(name.empty())
    {
        std::cerr << "Error: Name can't be empty\n";
        return -2;
    }
    if(cap_open)
    {
        std::cerr << "Error: Capture already running";
        return -3;
    }

    FloatCap cap;
    cap.var = var;
    cap.name = name;

    float_caps.push_back(cap);
    return 0;
}


int DataCapture::setCapturePath(std::string path) {
    fs::path cap_path_fs = fs::path(path);
    if (!fs::exists(cap_path_fs))
    {
        std::cerr << "Error: Path doesn't exist\n";
        return -1;
    }

    cap_path = path;
    return 0;
}

int DataCapture::startCapture() {
    std::string filename = getTimeString() + "_capture.csv";
    return startCapture(filename);
}

int DataCapture::startCapture(std::string filename) {
    if(cap_open)
    {
        std::cerr << "Error: Capture already open\n";
        return -1;
    }

#ifdef linux
    std::string filename_str = cap_path + "/" + filename;
#endif
#ifdef _WIN32
    std::string filename_str = cap_path + "\\" + filename;
#endif
    fs::path cap_path_fs = fs::path(filename_str);
    if (fs::exists(cap_path_fs))
    {
        std::cerr << "Error: File already exists\n";
        return -2;
    }

    cap_file.open(filename_str);

    if (!cap_file.is_open())
    {
        std::cerr << "Error: File couldn't be opened\n";
        return -3;
    }

    cap_open = true;

    current_index = 0;
    category_string.clear();

    cap_file << "time";
    cap_file << DELIMITER;
    category_string.push_back("time");

    cap_file << "index";
    cap_file << DELIMITER;
    category_string.push_back("index");

    if(!int_caps.empty())
        for (IntCap const& value: int_caps) {
            cap_file << value.name;
            cap_file << DELIMITER;
            category_string.push_back(value.name);
        }

    if(!double_caps.empty())
        for (DoubleCap const& value: double_caps) {
            cap_file << value.name;
            cap_file << DELIMITER;
            category_string.push_back(value.name);
        }

    if(!uint8t_caps.empty())
        for (Uint8_tCap const& value: uint8t_caps) {
            cap_file << value.name;
            cap_file << DELIMITER;
            category_string.push_back(value.name);
        }

    if(!float_caps.empty())
        for (FloatCap const& value: float_caps) {
            cap_file << value.name;
            cap_file << DELIMITER;
            category_string.push_back(value.name);
        }
    cap_file << NEWLINE;
    return 0;
}


int DataCapture::stopCapture() {
    if(cap_file.is_open())
        cap_file.close();
    if (option_file.is_open())
        option_file.close();
    cap_open = false;
    return 0;
}

DataCapture::~DataCapture() {
    if(cap_file.is_open())
        cap_file.close();
    if (option_file.is_open())
        option_file.close();
}

bool DataCapture::CaptureOpen() {
    return cap_open;
}

template<typename T>
int DataCapture::writeValue(T val, std::ofstream * file) {
    *file << std::to_string(val);
    *file << DELIMITER;
    return 0;
}

template<>
int DataCapture::writeValue(std::string val, std::ofstream * file) {
    *file << val;
    *file << DELIMITER;
    return 0;
}

int DataCapture::writeRow() {
    writeValue(getTimeString(), &cap_file);
    value_string.push_back(getTimeString());

    writeValue(current_index, &cap_file);
    value_string.push_back(std::to_string(current_index));

    current_index++;

    if(!int_caps.empty())
        for (IntCap const& value: int_caps) {
            writeValue(*value.var, &cap_file);
            value_string.push_back(std::to_string(*value.var));
        }

    if(!double_caps.empty())
        for (DoubleCap const& value: double_caps) {
            writeValue(*value.var, &cap_file);
            value_string.push_back(std::to_string(*value.var));
        }

    if(!uint8t_caps.empty())
        for (Uint8_tCap const& value: uint8t_caps) {
            writeValue(*value.var, &cap_file);
            value_string.push_back(std::to_string(*value.var));
        }

    if(!float_caps.empty())
        for (FloatCap const& value: float_caps) {
            writeValue(*value.var, &cap_file);
            value_string.push_back(std::to_string(*value.var));
        }
    cap_file << NEWLINE;
    return 0;
}

std::string DataCapture::getTimeString() {
    std::time_t current_time = std::time(nullptr);
    std::tm* current_local_time_ptr = std::localtime(&current_time);
    return std::to_string(current_local_time_ptr->tm_year + 1900) + \
            "_" + \
            std::to_string(current_local_time_ptr->tm_mon + 1) + \
            "_" + \
            std::to_string(current_local_time_ptr->tm_mday) + \
            "_" + \
            std::to_string(current_local_time_ptr->tm_hour) + \
            "_" + \
            std::to_string(current_local_time_ptr->tm_min) + \
            "_" + \
            std::to_string(current_local_time_ptr->tm_sec);
}

int DataCapture::dataCaptureMainLoop() {
    if(CaptureOpen())
    {
        writeRow();
    }
    return 0;
}

std::string DataCapture::getCapturePath() {
    return cap_path;
}

