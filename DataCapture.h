//
// Created by nicholas on 22.10.22.
//

#ifndef CASSYINTERFACE_DATACAPTURE_H
#define CASSYINTERFACE_DATACAPTURE_H
#include <string>
#include <vector>
#include <fstream>

class DataCapture {
public:

    ~DataCapture();
    struct IntCap
    {
        int* var;
        std::string name;
    };

    struct DoubleCap
    {
        double* var;
        std::string name;
    };

    struct Uint8_tCap
    {
        uint8_t* var;
        std::string name;
    };

    struct FloatCap
    {
       float* var;
       std::string name;
    };

    int setCap(int * var, std::string name);
    int setCap(double * var, std::string name);
    int setCap(uint8_t * var, std::string name);
    int setCap(float * var, std::string name);
    int setCapturePath(std::string cap_path);
    std::string getCapturePath();

    bool CaptureOpen();

    int startCapture(std::string filename);
    int startCapture();
    int stopCapture();

    int writeRow();

    int dataCaptureMainLoop();

    std::vector<std::string> category_string;
    std::vector<std::string> value_string;

private:
    std::vector<IntCap> int_caps;
    std::vector<DoubleCap> double_caps;
    std::vector<Uint8_tCap> uint8t_caps;
    std::vector<FloatCap> float_caps;

    std::string cap_path;

    std::ofstream cap_file;
    std::ofstream option_file;

    bool cap_open = false;

    int current_index = 0;

    template<typename T>
    int writeValue(T val, std::ofstream * file);

    std::string getTimeString();
};


#endif //CASSYINTERFACE_DATACAPTURE_H
