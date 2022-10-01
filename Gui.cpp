//
// Created by nicholas on 01.10.22.
//

#include "Gui.h"

#include <iostream>

void Gui::glfw_error_callback(int error, const char *description)
{
   std::cerr << "Glfw Error " << std::to_string(error) << description << "\n";
}

void Gui::calculate_angle_curve(float max, float period)
{
    int iterator = 0;
    for (float i = 0; i<=5; i += 5.0f / curve_samples) {
        float b = (M_PI * 2) / period;
        angle_y[iterator] = std::sin(b*i) * max;
        angle_t[iterator] = i;
        iterator++;
    }
}

void Gui::calculate_angular_velocity_curve()
{
    angular_velocity_max = 0;
    for (int i = 1; i < curve_samples; ++i) {
        float diff_angle = angle_y[i] - angle_y[i-1];
        float diff_time = angle_t[i] - angle_t[i-1];
        angular_velocity[i-1] = diff_angle / diff_time;
        if (angular_velocity[i-1] > angular_velocity_max)
            angular_velocity_max = angular_velocity[i-1];
    }
}


int Gui::main_loop()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Cassy 2 Control");
            ImGui::Text("Connect and control the cassy in this window.");

            if(cassy_handle->connected == false)
            {
                if (ImGui::Button("Connect"))
                    cassy_handle->connect();
            }
            else
            {
                if (ImGui::Button("Disconnect"))
                    cassy_handle->disconnect();
            }
            ImGui::SameLine();
            if(cassy_handle->connected == false)
                ImGui::TextColored(ImVec4(1,0,0,1), "Disconnected");
            else
                ImGui::TextColored(ImVec4(0,1,0,1), "Connected");

            ImGui::End();
//######################################################################################################################

            ImGui::Begin("PID and Movement Control");
            ImGui::Text("Connect and control the motor in this window");
            if(ImGui::BeginCombo("Serial Port", serial_handle->serial_ports[selected_port_index].c_str()))
            {
                for (int i = 0; i < serial_handle->serial_ports.size(); ++i) {
                    bool isSelected = (selected_port_index == i);
                    if(ImGui::Selectable(serial_handle->serial_ports[i].c_str(), isSelected))
                        selected_port_index = i;
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            if(serial_handle->connected == false)
            {
                if (ImGui::Button("Connect"))
                {
                    serial_handle->serial_port_name = serial_handle->serial_ports[selected_port_index];
                    serial_handle->baud = SerialPort::SER_BAUD_9600;
                    serial_handle->connect();
                }
            }
            else
            {
                if (ImGui::Button("Disconnect"))
                    serial_handle->disconnect();
            }
            ImGui::SameLine();
            if(serial_handle->connected == false)
                ImGui::TextColored(ImVec4(1,0,0,1), "Disconnected");
            else
                ImGui::TextColored(ImVec4(0,1,0,1), "Connected");

            ImGui::SliderFloat("Max angle", &max_angle, -180.0f, 180.0f);
            ImGui::SliderFloat("Period", &time, 0.0f, 5.0f);
            ImGui::SameLine();
            ImGui::Text("(Frequency:%f Hz)",1.0f / time);
            calculate_angle_curve(max_angle, time);
            calculate_angular_velocity_curve();
            if(ImPlot::BeginPlot("Motor Angle Curve"))
            {
                ImPlot::SetupAxes("Time in seconds", "Angle in degrees");
                ImPlot::SetupAxesLimits( 0.0, 5.0, max_angle * 2, max_angle * -2,ImPlotCond_Always);

                ImPlot::SetupAxis(ImAxis_Y2, "Angular Velocity in deg/sec^-1", ImPlotAxisFlags_AuxDefault);
                ImPlot::SetupAxisLimits(ImAxis_Y2, angular_velocity_max * 2, angular_velocity_max * -2, ImPlotCond_Always);

                ImPlot::PlotLine("Angle", angle_t, angle_y, curve_samples);
                ImPlot::SetAxes(ImAxis_X1, ImAxis_Y2);
                ImPlot::PlotLine("Angular Velocity", angle_t, angular_velocity, curve_samples);

                ImPlot::EndPlot();
            }
            if(ImGui::BeginTable("Curve Information", 2, ImGuiTableFlags_Borders))
            {

                ImGui::TableSetupColumn("Info");
                ImGui::TableSetupColumn("Value");
                ImGui::TableHeadersRow();
                ImGui::TableNextColumn();

                ImGui::Text("Function");
                ImGui::TableNextColumn();
                ImGui::Text("sin");
                ImGui::TableNextColumn();

                ImGui::Text("Max Angle (degrees)");
                ImGui::TableNextColumn();
                ImGui::Text("%f", max_angle);
                ImGui::TableNextColumn();

                ImGui::Text("Period (s)");
                ImGui::TableNextColumn();
                ImGui::Text("%f", time);
                ImGui::TableNextColumn();

                ImGui::Text("Frequency (Hz)");
                ImGui::TableNextColumn();
                ImGui::Text("%f", 1.0f / time);
                ImGui::TableNextColumn();

                ImGui::Text("Maximum Angular Velocity (deg/sec^-1)");
                ImGui::TableNextColumn();
                ImGui::Text("%f", angular_velocity_max);
                ImGui::TableNextColumn();


                ImGui::EndTable();
            }

            ImGui::End();
        }
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0,0,display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

Gui::Gui()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        std::cerr << "Error initializing gui\n";
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#else
    glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif
    window = glfwCreateWindow(1280, 720, "Wind Tunnel Control Software", NULL, NULL);
    if (window == NULL)
        std::cerr << "Error creating window \n";
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io= ImGui::GetIO(); (void) io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    cassy_handle = new Cassy();
    serial_handle = new SerialPort();
}