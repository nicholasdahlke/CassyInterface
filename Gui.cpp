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
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
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
    ImGuiIO& io= ImGui::GetIO(); (void) io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    cassy_handle = new Cassy();
}