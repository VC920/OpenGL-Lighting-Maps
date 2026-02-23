#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/texture.h>

float vertices[] = {
    // 位置                  // 法向量              // 纹理坐标
    // 前面
    -0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    0.0f, 1.0f,
    
    // 后面
    -0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.0f, 1.0f,
    
    // 左面
    -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
    
    // 右面
     0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
    
    // 上面
    -0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,    0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,    1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,    0.0f, 0.0f,
    
    // 下面
    -0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,    1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
};
unsigned int indices[] = {
    // 前面
    0, 1, 2,
    2, 3, 0,
    // 后面
    4, 6, 5,
    6, 4, 7,
    // 左面
    8, 9, 10,
    10, 11, 8,
    // 右面
    12, 13, 14,
    14, 15, 12,
    // 上面
    16, 17, 18,
    18, 19, 16,
    // 下面
    20, 21, 22,
    22, 23, 20
};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

int main() {
    /* 初始化 INIT */
    // 初始化SDL和OpenGL属性
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // 创建SDL窗口
    int windowWidth = 1920, windowHeight = 1080;
    SDL_Window *window = SDL_CreateWindow(
        "lighting maps",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth, windowHeight,
        SDL_WINDOW_OPENGL |
        SDL_WINDOW_RESIZABLE
    );
    // 创建OpenGL上下文
    SDL_GLContext context = SDL_GL_CreateContext(window);
    // 加载OpenGL函数
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    // 设置SDL和OpenGL状态
    SDL_SetRelativeMouseMode(SDL_TRUE);     // 相对坐标模式
    SDL_GL_SetSwapInterval(1);              // 开启垂直同步
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);   // 设置清屏颜色
    glEnable(GL_DEPTH_TEST);                // 开启深度测试
    glEnable(GL_CULL_FACE);
    // 初始化变量
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));                 // 初始化相机
    Shader baseShader("shaders/base.vs", "shaders/base.fs");    // 初始化基础着色器
    Shader lightmapShader("shaders/lightmap.vs", "shaders/lightmap.fs");    // 初始化光照着色器

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);   // 光源颜色

    // 设置VAO，VBO，EBO
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO); // 绑定VAO

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 绑定VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // 设置VBO信息

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // 绑定EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);    // 设置EBO信息

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);   // 设置顶点属性 0 （顶点）
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // 设置顶点属性 1 （法线）
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // 设置顶点属性 2 （纹理坐标）
    glEnableVertexAttribArray(2);

    // 设置贴图
    unsigned int diffuseMap = loadTexture("res/container.png"); // 设置漫反射贴图
    unsigned int specularMap = loadTexture("res/container_specular.png"); // 设置镜面反射贴图

    lightmapShader.use();

    lightmapShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    lightmapShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    lightmapShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    lightmapShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);

    lightmapShader.setInt("material.diffuse", 0);
    lightmapShader.setInt("material.specular", 1);
    lightmapShader.setFloat("material.shininess", 32.0f);

    // 主循环
    bool running = true;
    const Uint8* keyState = SDL_GetKeyboardState(NULL);
    SDL_Event event;
    while (running) {
        // 计算deltaTime
        static float deltaTime = 0.0f;
        static float lastFrame = 0.0f;
        float currentFrame = SDL_GetTicks() / 1000.0f;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        /* 处理事件 HANDLEENENT */
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    windowWidth = event.window.data1;
                    windowHeight = event.window.data2;
                    glViewport(0, 0, windowWidth, windowHeight);
                }
            }
            if (event.type == SDL_MOUSEMOTION) {
                camera.ProcessMouseMovement(event.motion.xrel, -event.motion.yrel);
            }
        }
        if (keyState[SDL_SCANCODE_ESCAPE])
            running = false;
        if (keyState[SDL_SCANCODE_W])
            camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
        if (keyState[SDL_SCANCODE_S])
            camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
        if (keyState[SDL_SCANCODE_A])
            camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
        if (keyState[SDL_SCANCODE_D])
            camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
        if (keyState[SDL_SCANCODE_E])
            camera.ProcessKeyboard(Camera_Movement::UP, deltaTime);
        if (keyState[SDL_SCANCODE_Q])
            camera.ProcessKeyboard(Camera_Movement::DOWN, deltaTime);

        /* 帧更新 UPDTAE */
        glm::mat4 mvp = glm::mat4(1.0f);
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
        
        /* 渲染 RENDER */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 绘制 Cube
        lightmapShader.use();
        lightmapShader.setVec3("viewPos", camera.Position);
        lightmapShader.setVec3("light.position", lightPos);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        
        for(unsigned int i = 0; i < 10; i++)
        {   
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);

            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            mvp = projection * view * model;
            lightmapShader.setMat4("mvp", mvp);
            lightmapShader.setMat4("model", model);

            glm::mat3 normalMatrix = glm::transpose(glm::inverse(model));
            lightmapShader.setMat3("normalMatrix", normalMatrix);

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        // 绘制光源
        baseShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        mvp = projection * view * model;
        baseShader.setMat4("mvp", mvp);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);  // 交换窗口缓冲区
    }

    // 清理资源
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(baseShader.ID);
    glDeleteProgram(lightmapShader.ID);

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}