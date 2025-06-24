#include "game.hpp"

Game::Game(void) : Engine(), m_window(nullptr) {
    if (!glfwInit()) {
        std::cout << "Failed to initialize glfw" << std::endl;
    }
}

Game::~Game(void) {
    glfwTerminate();
}

bool Game::Initialize(void) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_window = glfwCreateWindow(1280, 720, "Test", nullptr, nullptr);
    if (!m_window) {
        std::cout << "Failed to create window" << std::endl;
        return false;
    }

    //glfwRawMouseMotionSupported();
    glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwSetWindowUserPointer(m_window, this);
    glfwSetKeyCallback(m_window, KeyCallback);
    glfwSetCursorPosCallback(m_window, MouseCallback);

    ConfineCursor();
    Pipeline::Initialize(m_window);
    return true;
}

int Game::Run(void) {
    if (!Initialize()) {
        return -1;
    }
    OnInit();
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
        OnUpdate();
        OnRender();
    }
    OnQuit();
    return 0;
}

struct MatrixBuffer {
    glm::mat4x4 world;
    glm::mat4x4 view;
    glm::mat4x4 proj;
    //mat4x4 worldInvTranspose;
};

wrl::ComPtr<ID3D11Buffer> cb;
wrl::ComPtr<ID3D11Buffer> vbuffer;
wrl::ComPtr<ID3D11Buffer> ibuffer;
unsigned int indexCount;

void Game::OnInit(void) {
    auto tupleVs = Pipeline::CompileVertexShader("src\\engine\\shaders\\shader.hlsl", "VS_Main");
    auto ps = Pipeline::CompilePixelShader("src\\engine\\shaders\\shader.hlsl", "PS_Main");
    cb = Pipeline::CreateConstantBuffer<MatrixBuffer>();

    auto vs = std::get<wrl::ComPtr<ID3D11VertexShader>>(tupleVs);
    auto inputLayout = std::get<wrl::ComPtr<ID3D11InputLayout>>(tupleVs);

    Pipeline::SetVertexShader(vs);
    Pipeline::SetPixelShader(ps);
    Pipeline::SetInputLayout(inputLayout);

    cube.Load("src\\engine\\models\\cube.obj");
}

void Game::OnUpdate(void) {
    static Camera& cam = GetCamera();
    float speed = 20.0f;
    float rotSpeed = 200.0f;
    float delta = (float)glfwGetTime();
    glfwSetTime(0.0f);

    std::string fps = "fps " + std::to_string(1000.f / delta);
    glfwSetWindowTitle(m_window, fps.c_str());

    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE)) {
        this->~Game();
        exit(0);
    }

    if (glfwGetWindowAttrib(m_window, GLFW_FOCUSED)) {
        ConfineCursor();
    }

    // Camera rotation (Mouse)
    if (delta_x || delta_y) {
        cam.RotateRelative({ rotSpeed * delta * delta_y, rotSpeed * delta * delta_x, 0.0f });
        printf("pitch %f yaw %f\n", delta_y, delta_x);
        delta_x = 0.0f, delta_y = 0.0f;
    }

    // Movement
    if (glfwGetKey(m_window, GLFW_KEY_W)) {
        cam.MoveRelative((cam.GetForwardVector() * delta) * speed);
    }
    if (glfwGetKey(m_window, GLFW_KEY_A)) {
        cam.MoveRelative((cam.GetRightVector() * delta) * -speed);
    }
    if (glfwGetKey(m_window, GLFW_KEY_S)) {
        cam.MoveRelative((cam.GetForwardVector() * delta) * -speed);
    }
    if (glfwGetKey(m_window, GLFW_KEY_D)) {
        cam.MoveRelative((cam.GetRightVector() * delta) * speed);
    }
    // Up/down
    if (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL)) {
        cam.MoveRelative((cam.GetDefaultUpVector() * delta) * -speed);
    }
    if (glfwGetKey(m_window, GLFW_KEY_SPACE)) {
        cam.MoveRelative((cam.GetDefaultUpVector() * delta) * speed);
    }
    // Rotation
    if (glfwGetKey(m_window, GLFW_KEY_I)) {
        cam.RotateRelative((cam.GetDefaultForwardVector() * delta) * rotSpeed);
    }
    if (glfwGetKey(m_window, GLFW_KEY_J)) {
        cam.RotateRelative((cam.GetDefaultRightVector() * delta) * -rotSpeed);
    }
    if (glfwGetKey(m_window, GLFW_KEY_K)) {
        cam.RotateRelative((cam.GetDefaultForwardVector() * delta) * -rotSpeed);
    }
    if (glfwGetKey(m_window, GLFW_KEY_L)) {
        cam.RotateRelative((cam.GetDefaultRightVector() * delta) * rotSpeed);
    }
    // Reset
    if (glfwGetKey(m_window, GLFW_KEY_R)) {
        cam.MoveAbsolute({ 0.0f, 0.0f, 0.0f });
        cam.RotateAbsolute({ 0.0f, 0.0f, 0.0f });
        system("cls");
    }
    cam.Update();
}

void Game::OnRender(void) {
    static Camera& cam = GetCamera();
    Engine::Clear(0.5f, 0.6f, 0.5f);

    MatrixBuffer buffer{
        glm::transpose(glm::identity<glm::mat4x4>()),
        glm::transpose(cam.GetViewMatrix()),
        glm::transpose(cam.GetProjectionMatrix())
    };

    Pipeline::UpdateConstantBuffer(cb, buffer, 0);

    Engine::DrawMesh(cube);
    
    Engine::Present(false);
}

void Game::OnQuit(void) {
    // glfw cleanup
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Game::ConfineCursor(void) {
    HWND hWnd = glfwGetWin32Window(m_window);
    RECT cr;
    GetClientRect(hWnd, &cr);
    POINT topLeft = { cr.left, cr.top };
    POINT bottomRight = { cr.right, cr.bottom };
    ClientToScreen(hWnd, &topLeft);
    ClientToScreen(hWnd, &bottomRight);
    RECT clipRect = { topLeft.x, topLeft.y, bottomRight.x, bottomRight.y };
    ClipCursor(&clipRect);

    POINT p;
    GetCursorPos(&p);
    RECT wr;
    GetWindowRect(hWnd, &wr);
    int newx = wr.left + (cr.right / 2) + 1;
    int newy = wr.top + (cr.bottom / 2);
    SetCursorPos(newx, newy);
    delta_x -= p.x - newx;
    delta_y -= p.y - newy;
}

void Game::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    static Camera& cam = game->GetCamera();

    glm::vec3 pos = cam.GetPositionVector();
    glm::vec3 rot = cam.GetPositionVector();
    std::cout << "vec3(" << pos.x << ", " << pos.y << ", " << pos.z << ")    ";
    std::cout << "vec3(" << rot.x << ", " << rot.y << ", " << rot.z << ")\n";
}

void Game::MouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));

    static double lastX = xpos;
    static double lastY = ypos;

    game->delta_x += lastX - xpos;
    game->delta_y += ypos - lastY;

    lastX = xpos;
    lastY = ypos;
}