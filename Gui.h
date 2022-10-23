//
// Created by nicholas on 01.10.22.
//

#ifndef CASSYINTERFACE_GUI_H
#define CASSYINTERFACE_GUI_H
#define GL_GLEXT_PROTOTYPES
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "implot/implot.h"
#include <GLFW/glfw3.h>
#include "Cassy.h"
#include "SerialPort.h"
#include "DataCapture.h"
#include <cmath>
#include "ImFileDialog/ImFileDialog.h"

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
    DataCapture* data_capture;

    int selected_port_index = 0;
    int selected_command_index = 0;
    int selected_function_index = 0;
    static constexpr int curve_samples = 360 * 4;
    float angle_y[curve_samples];
    float angle_t[curve_samples];
    float angle_readout_y[500];
    float angle_readout_t[500];
    int angle_readout_samples = 0;
    float angular_velocity[curve_samples];

    float angular_velocity_max = 0;
    float max_angle = 1;
    float time = 1;
    float manual_movement_size = 10;
    char data_to_send[128] = "";

    bool show_results_table = false;

    const ImVec4 green = ImVec4(0.0588, 0.788, 0.172, 1);
    const ImVec4 red = ImVec4(1, 0, 0, 1);

    std::vector<std::string> functions;
    std::vector<std::string> serial_data;

    std::vector<serialCommand> commands;

    static void glfw_error_callback(int error, const char* description);
    void calculate_angle_curve_sin(float max, float period);
    void calculate_angle_curve_linear(float max);
    void calculate_angular_velocity_curve();
    void set_serial_commands();
    void set_functions();
    void set_capture_variables();;

    void read_serial();

    void send_serial_parameters(serialCommand command, float parameter);
    void send_serial_command(serialCommand command);

    enum commands_index
    {
        motor_on,
        motor_off,
        turn_on_led,
        turn_off_led,
        send_period,
        send_max_angle,
        send_manual_movement,
        move_forward,
        move_backwards
    };
};


#endif //CASSYINTERFACE_GUI_H
