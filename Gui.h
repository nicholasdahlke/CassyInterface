//
// Created by nicholas on 01.10.22.
//

#ifndef CASSYINTERFACE_GUI_H
#define CASSYINTERFACE_GUI_H
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "implot/implot.h"
#include <GLFW/glfw3.h>
#include "Cassy.h"
#include "SerialPort.h"
#include <cmath>

class Gui {
public:
    Gui();
    int main_loop();

private:
    struct serialCommand
    {
        std::string name;
        char command_id;
    };
    GLFWwindow *window;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    const char* glsl_version;
    Cassy* cassy_handle;
    SerialPort* serial_handle;
    int selected_port_index = 0;
    int selected_command_index = 0;
    static constexpr int curve_samples = 360 * 4;
    float angle_y[curve_samples];
    float angle_t[curve_samples];
    float angular_velocity[curve_samples];
    float angular_velocity_max = 0;
    float max_angle = 1;
    float time = 1;

    char data_to_send[128] = "";


    std::vector<serialCommand> commands;

    static void glfw_error_callback(int error, const char* description);
    void calculate_angle_curve(float max, float period);
    void calculate_angular_velocity_curve();
    void set_serial_commands();
};


#endif //CASSYINTERFACE_GUI_H
