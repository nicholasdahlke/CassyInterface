//
// Created by nicholas on 01.10.22.
//

#include "Gui.h"

#include <iostream>

void Gui::glfw_error_callback(int error, const char *description)
{
   std::cerr << "Glfw Error " << std::to_string(error) << description << "\n";
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

            if(!cassy_handle->connected)
            {
                if (ImGui::Button("Connect"))
                    cassy_handle->connect();
            }
            else
            {
                if (ImGui::Button("Disconnect"))
                {
                    cassy_handle->disconnect();
                    //delete cassy_handle;
                    cassy_handle = new Cassy();
                }
            }
            ImGui::SameLine();
            if(!cassy_handle->connected)
                ImGui::TextColored(ImVec4(1,0,0,1), "Disconnected");
            else
                ImGui::TextColored(ImVec4(0,1,0,1), "Connected");

            ImGui::Separator();

            if (cassy_handle->connected)
            {
                if(ImGui::BeginTable("Cassy Information", 2, ImGuiTableFlags_Borders))
                {

                    ImGui::TableSetupColumn("Info");
                    ImGui::TableSetupColumn("Value");
                    ImGui::TableHeadersRow();
                    ImGui::TableNextColumn();

                    ImGui::Text("Number of connected cassys");
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", static_cast<int>(cassy_handle->cassys.size()));
                    ImGui::TableNextColumn();

                    ImGui::Text("Number of relay channels");
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", static_cast<int>(cassy_handle->relays.size()));
                    ImGui::TableNextColumn();

                    ImGui::Text("Number of voltage channels");
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", static_cast<int>(cassy_handle->voltage_channels.size()));
                    ImGui::TableNextColumn();


                    ImGui::EndTable();
                }
                if (ImGui::BeginTabBar("##tabs"))
                {
                    for (int i = 0; i <cassy_handle->cassys.size(); ++i) {
                        std::string cassy_name = "Cassy " + std::to_string(i);
                        if(ImGui::BeginTabItem(cassy_name.c_str()))
                        {
                            std::string relay_name = "Relay " + std::to_string(i);
                            if(ImGui::Button(std::string("Turn on " + relay_name).c_str()))
                                cassy_handle->set_relay(cassy_handle->relays[i], true);
                            ImGui::SameLine();
                            if(ImGui::Button(std::string("Turn off " + relay_name).c_str()))
                                cassy_handle->set_relay(cassy_handle->relays[i], false);
                            for (int j = 0; j < cassy_handle->voltage_channels.size(); ++j) {
                                if(cassy_handle->voltage_channels[j].cassy_id == cassy_handle->cassys[i])
                                {
                                    ImGui::Text("Voltage Channel %d", j);
                                    ImGui::SameLine();
                                    ImGui::Text("%s", std::to_string(cassy_handle->read_voltage(cassy_handle->voltage_channels[j], cassy_handle->v4)).c_str());
                                }
                            }
                            ImGui::EndTabItem();
                        }

                    }
                    ImGui::EndTabBar();
                }
            }

            ImGui::End();
//######################################################################################################################

            ImGui::Begin("PID and Movement Control");
            ImGui::Text("Connect and control the motor in this window");
            if(ImGui::BeginCombo("##empty", serial_handle->serial_ports[selected_port_index].c_str()))
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
            ImGui::SameLine();
            if(!serial_handle->connected)
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
                {
                    serial_handle->disconnect();
                    serial_data.clear();
                }
            }
            ImGui::SameLine();
            if(!serial_handle->connected)
                ImGui::TextColored(ImVec4(1,0,0,1), "Disconnected");
            else
                ImGui::TextColored(ImVec4(0,1,0,1), "Connected");

            if (serial_handle->connected)
            {
                ImGui::InputText("##itm", data_to_send, IM_ARRAYSIZE(data_to_send));
                ImGui::SameLine();
                if (ImGui::Button("Send Data")) {
                    serial_handle->write_bytes(&data_to_send[0], sizeof(data_to_send) / sizeof(char));
                    strcpy(data_to_send, "");
                }

                if(ImGui::BeginCombo("##empty2", commands[selected_command_index].name.c_str()))
                {
                    for (int i = 0; i < commands.size(); ++i) {
                        bool isSelected = (selected_command_index == i);
                        if(ImGui::Selectable(commands[i].name.c_str(), isSelected))
                            selected_command_index = i;
                        if (isSelected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
                ImGui::SameLine();
                if (ImGui::Button("Send Command"))
                    send_serial_command(commands[selected_command_index]);
            }

            ImGui::Separator();
            ImGui::SliderFloat("Max angle", &max_angle, -180.0f, 180.0f);
            ImGui::SliderFloat("Period", &time, 0.0f, 5.0f);
            ImGui::SameLine();
            ImGui::Text("(Frequency:%f Hz)",1.0f / time);

            if(ImGui::BeginCombo("Function Type", functions[selected_function_index].c_str()))
            {
                for (int i = 0; i <functions.size(); ++i) {
                    bool isSelected = (selected_function_index == i);
                    if(ImGui::Selectable(functions[i].c_str(), isSelected))
                        selected_function_index = i;
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            if(selected_function_index == 0)
                calculate_angle_curve_sin(max_angle, time);
            if(selected_function_index == 1)
                calculate_angle_curve_linear(max_angle);
            calculate_angular_velocity_curve();
            if(ImPlot::BeginPlot("Motor Angle Curve"))
            {
                ImPlot::SetupAxes("Time in seconds", "Angle in degrees");
                ImPlot::SetupAxesLimits( 0.0, 5.0, max_angle * 2, max_angle * -2,ImPlotCond_Always);

                ImPlot::SetupAxis(ImAxis_Y2, "Angular Velocity in deg/sec^-1", ImPlotAxisFlags_AuxDefault);
                ImPlot::SetupAxisLimits(ImAxis_Y2, angular_velocity_max * 2 + 0.5, angular_velocity_max * -2 - 0.5, ImPlotCond_Always);

                ImPlot::SetupAxis(ImAxis_X2, "Samples", ImPlotAxisFlags_AuxDefault);
                ImPlot::SetupAxisLimits(ImAxis_X2, 0.0f, 500.0f, ImPlotCond_Always);

                ImPlot::PlotLine("Angle", angle_t, angle_y, curve_samples);
                ImPlot::SetAxes(ImAxis_X1, ImAxis_Y2);
                ImPlot::PlotLine("Angular Velocity", angle_t, angular_velocity, curve_samples);
                ImPlot::SetAxes(ImAxis_X2, ImAxis_Y1);
                if (serial_handle->connected)
                    ImPlot::PlotLine("Actual Angle", angle_readout_t, angle_readout_y, angle_readout_samples);
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
                ImGui::Text("%s" , functions[selected_function_index].c_str());
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
            ImGui::Separator();

            if (serial_handle->connected) {
                ImGui::Text("Motor Control");

                ImGui::SameLine();

                ImGui::PushStyleColor(ImGuiCol_Button, green);
                if (ImGui::Button(commands[motor_on].name.c_str()))
                {
                    send_serial_parameters(commands[send_period], time);
                    send_serial_parameters(commands[send_max_angle], max_angle);
                    send_serial_command(commands[motor_on]);
                }
                ImGui::PopStyleColor();

                ImGui::SameLine();

                ImGui::PushStyleColor(ImGuiCol_Button, red);
                if (ImGui::Button(commands[motor_off].name.c_str()))
                    send_serial_command(commands[motor_off]);
                ImGui::PopStyleColor();


                ImGui::SameLine();
                ImGui::Dummy(ImVec2(1, 0));
                ImGui::SameLine();



                ImGui::SameLine();
                ImGui::Dummy(ImVec2(1, 0));
                ImGui::SameLine();

                if(ImGui::Button(commands[move_forward].name.c_str()))
                {
                    send_serial_parameters(commands[send_manual_movement], manual_movement_size);
                    send_serial_command(commands[move_forward]);
                }
                ImGui::SameLine();
                if(ImGui::Button(commands[move_backwards].name.c_str()))
                {
                    send_serial_parameters(commands[send_manual_movement], manual_movement_size);
                    send_serial_command(commands[move_backwards]);
                }

                ImGui::SameLine();

                ImGui::InputFloat("Movement size", &manual_movement_size, 0.0f, 0.0f,  "%4.2f??");
                //read_serial();
                ImGui::BeginChild("Scrolling", ImVec2(0,0), true);
                //ImGui::SetScrollHereY(0.0f);

                for (int i = 0; i < serial_data.size(); ++i) {
                    ImGui::Text("%s", serial_data[i].c_str());
                }
                ImGui::EndChild();
            }
            ImGui::End();

//######################################################################################################################
            ImGui::Begin("ImGui Metrics");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
//######################################################################################################################
            ImGui::Begin("Data Capture");

            ImGui::BeginDisabled(data_capture->getCapturePath().empty() || data_capture->CaptureOpen());
            ImGui::PushStyleColor(ImGuiCol_Button, green);
            if (ImGui::Button("Start Capture"))
            {
                data_capture->startCapture();
            }
            ImGui::EndDisabled();

            ImGui::PopStyleColor();

            ImGui::SameLine();

            ImGui::BeginDisabled(data_capture->getCapturePath().empty() || !data_capture->CaptureOpen());
            ImGui::PushStyleColor(ImGuiCol_Button, red);
            if (ImGui::Button("Stop Capture"))
            {
                data_capture->stopCapture();
            }
            data_capture->dataCaptureMainLoop();
            ImGui::PopStyleColor();
            ImGui::EndDisabled();

            ImGui::SameLine();
            ImGui::Dummy(ImVec2(1, 0));
            ImGui::SameLine();

            if (ImGui::Button("Choose Capture Path"))
                ifd::FileDialog::Instance().Open("DirectoryOpenDialog", "Open a directory", "");

            if (ifd::FileDialog::Instance().IsDone("DirectoryOpenDialog")) {
                if (ifd::FileDialog::Instance().HasResult()) {
                   data_capture->setCapturePath(ifd::FileDialog::Instance().GetResult().generic_string());
                }
                ifd::FileDialog::Instance().Close();
            }


            if(!data_capture->getCapturePath().empty())
            {
                ImGui::SameLine();
                ImGui::Text(data_capture->getCapturePath().c_str());
            }

            ImGui::Separator();

            if(data_capture->CaptureOpen())
                show_results_table = true;

            if (show_results_table && ImGui::BeginTable("Data Capture", data_capture->category_string.size(), ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY))
            {
                for(std::string const& name: data_capture->category_string)
                    ImGui::TableSetupColumn(name.c_str());
                ImGui::TableHeadersRow();
                ImGui::TableNextColumn();

                for(std::string const& value: data_capture->value_string)
                {
                    ImGui::Text(value.c_str());
                    ImGui::TableNextColumn();
                }
                ImGui::EndTable();
            }

            ImGui::End();
        }
//######################################################################################################################
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

void Gui::set_serial_commands()
{
    serialCommand turn_on_led;
    turn_on_led.name = "Turn on led";
    turn_on_led.command_id = 97;

    serialCommand turn_off_led;
    turn_off_led.name = "Turn off led";
    turn_off_led.command_id = 98;

    serialCommand motor_on;
    motor_on.name = "Start movement";
    motor_on.command_id = 99;

    serialCommand motor_off;
    motor_off.name = "Stop movement";
    motor_off.command_id = 100;

    serialCommand send_period;
    send_period.name = "Send rotational period (Argument required)";
    send_period.command_id = 97;

    serialCommand send_max_angle;
    send_max_angle.name = "Send max angle (Argument required)";
    send_max_angle.command_id = 98;

    serialCommand send_manual_movement;
    send_manual_movement.name = "Send manual step size (Argument required)";
    send_manual_movement.command_id = 103;

    serialCommand move_forward;
    move_forward.name = "Move forward";
    move_forward.command_id = 101;

    serialCommand move_backwards;
    move_backwards.name = "Move backwards";
    move_backwards.command_id = 102;

    commands.push_back(motor_on); //0
    commands.push_back(motor_off); //1
    commands.push_back(turn_on_led); //2
    commands.push_back(turn_off_led); //3
    commands.push_back(send_period); //4
    commands.push_back(send_max_angle); //5
    commands.push_back(send_manual_movement); //6
    commands.push_back(move_forward); //7
    commands.push_back(move_backwards); //8
}

void Gui::send_serial_command(serialCommand command)
{
    if(serial_handle->connected) {
        serial_handle->write_bytes(&command.command_id, 1);
        serial_data.push_back("Send: " + std::to_string(command.command_id));
    }
}

void Gui::send_serial_parameters(serialCommand command, float parameter)
{
    int message_length = sizeof(std::to_string(parameter).c_str()) + 1;
    char buf[message_length];
    buf[0] = command.command_id;
    char parameter_str[message_length - 1];
    strcpy(parameter_str, std::to_string(parameter).c_str());
    strcpy(&buf[1], parameter_str);
    serial_handle->write_bytes(buf, message_length);
    serial_data.push_back("Send: " + std::to_string(buf[0]) + " " + std::string(&buf[1]));
}

void Gui::set_functions()
{
    functions.push_back("sin(x)");
    functions.push_back("x");
}

void Gui::calculate_angle_curve_sin(float max, float period)
{
    int iterator = 0;
    for (float i = 0; i<=5; i += 5.0f / curve_samples) {
        float b = (M_PI * 2) / period;
        angle_y[iterator] = std::sin(b*i) * max;
        angle_t[iterator] = i;
        iterator++;
    }
}

void Gui::calculate_angle_curve_linear(float max)
{
    int iterator = 0;
    for (float i = 0; i<=5; i += 5.0f / curve_samples) {
        angle_y[iterator] = max_angle;
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
        if(diff_angle == 0.0f)
            angular_velocity[i-1] = 0;
        else
            angular_velocity[i-1] = diff_angle / diff_time;
        if (angular_velocity[i-1] > angular_velocity_max)
            angular_velocity_max = angular_velocity[i-1];
    }
}

void Gui::read_serial()
{
    uint8_t recv_buf[10];
    int bytes_read = serial_handle->read_bytes(recv_buf, sizeof(recv_buf) / sizeof(uint8_t));
    char bytes_read_buf[bytes_read];
    for (int i = 0; i < bytes_read; ++i) {
        if(recv_buf[i] != 10)
            bytes_read_buf[i] = recv_buf[i];
    }
    if(!std::string(&bytes_read_buf[0]).empty())
    {
        serial_data.push_back("Recv: " + std::string(&bytes_read_buf[0]));
        if (angle_readout_samples >= 499)
            angle_readout_samples = 0;
        float val = static_cast<float>(std::atof(bytes_read_buf));
        angle_readout_y[angle_readout_samples] = val;
        angle_readout_t[angle_readout_samples] = angle_readout_samples;
        angle_readout_samples++;
    }
}

void Gui::set_capture_variables()
{
    data_capture = new DataCapture();

    data_capture->setCap(&selected_port_index, "selected_port");
    data_capture->setCap(&angular_velocity_max, "max_velocity");
    data_capture->setCap(&max_angle, "max_angle");
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
    set_serial_commands();
    set_functions();
    set_capture_variables();

    ifd::FileDialog::Instance().CreateTexture = [](uint8_t* data, int w, int h, char fmt) -> void* {
        GLuint tex;

        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, (fmt == 0) ? GL_BGRA : GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        return (void*)tex;
    };
    ifd::FileDialog::Instance().DeleteTexture = [](void * tex) {
        GLuint texID = (intptr_t)tex;
        glDeleteTextures(1, &texID);
    };
}