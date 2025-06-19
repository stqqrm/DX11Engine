#pragma once
#include "engine/engine.hpp"

class Game : public Engine {
public:
    Game(void);
    ~Game(void);
    bool Initialize(void);
    int Run(void);
    void OnInit(void);
    void OnUpdate(void);
    void OnRender(void);
    void OnQuit(void);
    // Helper functions
    void ConfineCursor(void);
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
    GLFWwindow* m_window;
    Mesh cube;
    float delta_x, delta_y;
};