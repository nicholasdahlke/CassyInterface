//
// Created by nicholas on 01.10.22.
//

#ifndef CASSYINTERFACE_GUI_H
#define CASSYINTERFACE_GUI_H
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include "Cassy.h"
#include "SerialPort.h"

class Gui {
public:
    Gui();
    int main_loop();

private:
    GLFWwindow *window;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    const char* glsl_version;
    Cassy* cassy_handle;
    SerialPort* serial_handle;
    int selected_port_index = 0;
    static void glfw_error_callback(int error, const char* description);
};


#endif //CASSYINTERFACE_GUI_H
