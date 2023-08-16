#include <glad/glad.h>
#include <glfw3.h>
#include<algorithm>
#include <iostream>
#include"Shader.h"
#include"Texture.h"
#include<ImageLib/stb_image.h>
#include"OpenGLINC.h"
#include"Camera.h"
#include"ResourceManager.h"
#include"DirLight.h"
#include"PointLight.h"
#include"Model.h"

using namespace std;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const int u_frag = 64;//u是与球面上任意一点连线与y轴的夹角
const int v_frag = 64;//v是与球面上任意一点连线与x轴的夹角
const float pi = 3.1415926;
std::vector<float> sphere_data;
std::vector<glm::vec3> sphere_vertexs;
std::vector<glm::vec2> sphere_texCoords;
std::vector<glm::vec3> sphere_normals;
std::vector<unsigned int> sphere_indices;
std::map<std::string, Model> Model_Map;
unsigned int indexCount;

float Last_X = SCR_WIDTH / 2.0f;//记录上一帧渲染的光标位置
float Last_Y = SCR_HEIGHT / 2.0f;
float last_time = 0.0f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;//记录上一帧的时间
bool is_firstMouse = true;
bool pauseRender = false;
bool isPressed_P = false;
bool isPressed_Escape = false;
bool isShowCursor = false;//是否显示光标

//开始渲染标记
bool start_Floor = false;
bool start_Sphere = false;
bool start_Model = false;
bool start_Cube = false;
bool start_ReflectSphere = false;
bool start_Clear = true;


Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
glm::mat4 view = camera.getViewMatrix();
glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
glm::vec3 lightPos(5.0f, 0.0f, 1.0f);//设定光源位置
static int command;
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height){//帧缓冲大小函数需要一个GLFWwindow作为它的第一个参数，
                                                                       //以及两个整数表示窗口的新维度

    glViewport(0, 0, width, height);//告诉OpenGL渲染窗口的尺寸大小，即视口(Viewport)

    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos); 
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !isPressed_Escape) {
        isShowCursor = !isShowCursor; 
        isPressed_Escape = true;
        if (isShowCursor) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }        
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
        isPressed_Escape = false;
    }
       
       // glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
        isPressed_Escape = false;
    }
     if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyBoard(FORWARD, deltaTime);
     if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyBoard(BACKWARD, deltaTime);
     if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyBoard(LEFT, deltaTime);
     if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyBoard(RIGHT, deltaTime);
     if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && !isPressed_P) {//保证一直按P，也不会运行这一段代码         
        pauseRender = !pauseRender;
        isPressed_P = true;
     }
     if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) {
         isPressed_P = false;
     }
         
    
}
//InitialCommand
void ShowInfo() {
    cout << "================================================OpenGL Rendering Machine================================================" << endl;
    cout << "================本系统使用命令行控制，主要功能有渲染立方体，渲染球体，增加纹理贴图，增加光照效果，导入模型=============="<<endl;
    cout << "A：输入 '1'添加地板效果" << endl;
    cout << "B：输入 '2 '用以加载立方体"<<endl;   
    cout << "C：输入 '3 '用以加载球体" << endl;    
    cout << "D：输入 '4 '用以加载具有反射效果的球体" << endl;   
    cout << "E：输入 '5 ' 用以加载模型" << endl;    
}
float getFPS(float lats_Time);
#pragma region Function of Rendering Basic Graphics
//绘制基本图形功能函数
void DrawCube(unsigned int vao);
void DrawPlane(unsigned int vao);
#pragma endregion



#pragma region Function of Rendering Scene
//绘制场景图形功能函数
void DrawSkyBox(Shader& shader, Texture tex, unsigned int vao, glm::mat4 projection_mat4 = projection, glm::mat4 view_mat4 = view);
void DrawLight(Shader& shader, unsigned int vao, glm::vec3 lightPos, glm::mat4 projection_mat4 = projection, glm::mat4 view_mat4 = view);
void DrawModel(Model& model, Shader& shader, float shininess, glm::vec3 viewPos, glm::mat4 model_mat4, glm::mat4 projection_mat4 = projection, glm::mat4 view_mat4 = view);
void DrawFloor(Shader& shader, Texture tex, unsigned int vao, float shininess, glm::vec3 viewPos, glm::mat4 model, glm::mat4 projection_mat4 = projection, glm::mat4 view_mat4 = view);//加viewPos和shininess是因为如果有材质和光照，就会对地板也产生一些效果
void BindBufferSphere(unsigned int vao, unsigned int vbo, unsigned int ebo, std::vector<float> data, std::vector<unsigned int> indices);
void ComputeSphereData();
void DrawSphere(unsigned int vao);
void DrawReflectSphere(unsigned int vao, Shader reflectShader, Texture reflectTex, glm::vec3 viewPos, glm::mat4 model_mat4, glm::mat4 projection_mat4 = projection, glm::mat4 view_mat4 = view);
void DrawReflectCube(unsigned int vao, Shader reflectShader, Texture reflectTex, glm::vec3 viewPos, glm::mat4 model_mat4, glm::mat4 projection_mat4 = projection, glm::mat4 view_mat4 = view);
void DrawNormalCube(Shader& shader, Texture tex1, Texture tex2, unsigned int vao, float shininess, glm::vec3 viewPos, glm::mat4 model_mat4, glm::mat4 projection_mat4 = projection, glm::mat4 view_mat4 = view);
void DrawNormalSphere(Shader& shader, Texture tex1, unsigned int vao, float shininess, glm::vec3 viewPos, glm::mat4 model_mat4, glm::mat4 projection_mat4 = projection, glm::mat4 view_mat4 = view);
#pragma endregion


int main()
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float size=0.0f;
    float cube_size=0.0f;
    float model_size = 0.0f;
    float sphere_size = 0.0f;
    glm::vec3 cube_pos;
    glm::vec3 model_pos;
    glm::vec3 sphere_pos;
    glm::vec3 reflectsphere_pos;
    std::string texture_name1="";
    std::string texture_name2 ="";
    ShowInfo();
    ComputeSphereData();
    //都是初始化函数
   
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__//如果是MAC系统，需要加下面这段语句
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
#pragma region InitializeWindow 
    // glfw window creation
   // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Kick your Ass!", NULL, NULL);//创建window对象，后续需要大量使用
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //---------------设置回调函数--------------------//
    glfwMakeContextCurrent(window);//将我们窗口的上下文设置为当前线程的主上下文,是GLFW的开发流程
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//用此函数来注册回调函数，参数一为window，二是回调函数
    glfwSetCursorPosCallback(window,mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //---------------设置回调函数--------------------//


    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))//初始化GLAD，GLAD是用来管理Open_GL的函数的
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    
#pragma endregion
float vertices[]={
        -0.5f, -0.5f, 0.0f,  0.0f,0.0f, // left  
         0.5f, -0.5f, 0.0f,  1.0f,0.0f,// right 
         0.0f,  0.5f, 0.0f,  0.5f,1.0f // top   
    };
float vertices_1[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
float vertices_floor[] = {
    //position           //TexCoord
    5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
   -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
   -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

    5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
   -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
    5.0f, -0.5f, -5.0f,  2.0f, 2.0f
};
unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
#pragma region 十个立方体的位置
    glm::vec3 cubePositions[] = {
   glm::vec3(0.0f,  0.0f,  0.0f),
   glm::vec3(2.0f,  5.0f, -15.0f),
   glm::vec3(-1.5f, -2.2f, -2.5f),
   glm::vec3(-3.8f, -2.0f, -12.3f),
   glm::vec3(2.4f, -0.4f, -3.5f),
   glm::vec3(-1.7f,  3.0f, -7.5f),
   glm::vec3(1.3f, -2.0f, -2.5f),
   glm::vec3(1.5f,  2.0f, -2.5f),
   glm::vec3(1.5f,  0.2f, -1.5f),
   glm::vec3(-1.3f,  1.0f, -1.5f)
    };
#pragma endregion  
#pragma region 立方体顶点数据
    float vertices6[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
#pragma endregion
#pragma region 带法向量的顶点数据
    float vertices_Normal[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
#pragma endregion
#pragma region 带法向量，带纹理坐标的立方体顶点坐标
    float vertices_Tex[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
#pragma endregion
#pragma region InitLightProperties
    //dir前缀指dirLight
    glm::vec3 direction(-0.2f, -1.0f, -0.3f);
    glm::vec3 dir_ambient(0.5f, 0.5f, 0.5f);
    glm::vec3 dir_diffuse(0.4f, 0.4f, 0.4f);
    glm::vec3 dir_specular(0.5f, 0.5f, 0.5f);
    //pl前缀指pointLight
    glm::vec3 pl_position(2.0f,0.0f,0.0f);
    glm::vec3 pl_ambient(0.5f,0.5f,0.5f);
    glm::vec3 pl_diffuse(0.8f, 0.8f, 0.8f);
    glm::vec3 pl_specular(1.0f, 1.0f, 1.0f);
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.030f;
    DirLight dirLight(dir_ambient, dir_diffuse, dir_specular, direction);
    PointLight pointLight(pl_ambient, pl_diffuse, pl_specular, pl_position, constant, linear, quadratic);
#pragma endregion
#pragma region 天空盒的顶点数据
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
#pragma endregion
#pragma region 天空盒的纹理图片名
    vector<std::string> faces
    {
        "right.jpg",
        "left.jpg",
        "top.jpg",
        "bottom.jpg",
        "back.jpg",
        "front.jpg"
    };
#pragma endregion
    
   

    //=========对于使用shader1的函数，里面会有纹理单元的1设置，且只会有两个，一个是漫反射材质，一个是镜面反射材质=========//
    Shader shader1=ResourceManager::getInstance()->LoadShader("Texture.vs", "Texture_Frag.fs",nullptr,"cubeShader");

    Shader lightShader= ResourceManager::getInstance()->LoadShader("lightVertex.vs", "lightFragment.fs", nullptr, "lightShader");  
    Shader normalShader = ResourceManager::getInstance()->LoadShader("normalShow.vs", "normalShow.fs", "normalShow.gs", "normalShader");
    Shader modelShader = ResourceManager::getInstance()->LoadShader("model.vs", "model.fs", nullptr,"modelShader");
    Shader skyShader = ResourceManager::getInstance()->LoadShader("skybox.vs", "skybox.fs", nullptr, "skyBox");
    Shader reflectShader = ResourceManager::getInstance()->LoadShader("reflectEffect.vs", "reflectEffect.fs", nullptr, "reflectShader");
    Shader sphereShader = ResourceManager::getInstance()->LoadShader("sphere.vs", "sphere.fs", nullptr, "sphereShader");   
    Model Ourmodel("D:/Resource/Model/nanosuit/nanosuit.obj","nanosuit");
    //Model_Map["nanosuit"] = Ourmodel;
    //Model chairModel("D:/Resource/Model/cc548dfe3a4c5c2d82c35751a2612947/Armchair Cappellini Wanders Tulip N110121/Armchair Cappellini Wanders Tulip N110121.obj");
    //Model ironModel("D:/Resource/Model/ironman/Iron Cyborg.obj");
    //VBO变量是带顶点数据，法向量，纹理坐标的立方体数据集，若要绘制立方体，可以根据自己的需要绑定到自己设定的VAO中
    unsigned int VBO, VAO, EBO,floor_VBO,sky_box_VAO,sky_box_VBO;//这是被光投射物
    unsigned int sphere_VA0, sphere_VBO, sphere_EBO;
    unsigned int light_VAO;//这是光源数据
    unsigned int floor_VAO;//地板数据
    unsigned int reflectCube_VAO;//反射立方体数据
    glGenVertexArrays(1, &sphere_VA0);    
    glGenVertexArrays(1, &VAO);
    glGenVertexArrays(1, &sky_box_VAO);
    glGenVertexArrays(1, &light_VAO);
    glGenVertexArrays(1, &floor_VAO);
    glGenVertexArrays(1, &reflectCube_VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &floor_VBO);
    glGenBuffers(1, &sky_box_VBO);
    glGenBuffers(1, &sphere_VBO);
    glGenBuffers(1, &sphere_EBO);
    

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_Tex), vertices_Tex, GL_STATIC_DRAW);//数据不改变:GL_STATIC_DRAW  
                                                                              //传递数据给VBO 因为之前1绑定的是VBO，如果这个函数
                                                                             //之前的glBindBuffer绑定的是VB0_1，那就是传给VBO_1   
    // explain Position data;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);//解释传的数据  0代表Layout(Location=0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(2);
    /*=================光源的数据（以立方体作为光源）*/
    glBindVertexArray(light_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);//只需要顶点位置，不需要纹理坐标信息
    glEnableVertexAttribArray(0);
    //=================地板纹理的数据
    glBindVertexArray(floor_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, floor_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_floor), vertices_floor, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(2);

    //==================天空盒纹理的数据
    glBindVertexArray(sky_box_VAO);
    glBindBuffer(GL_ARRAY_BUFFER,sky_box_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);      
    //===================带顶点坐标和法向量的立方体数据
    glBindVertexArray(reflectCube_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);    

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    //球体数据
    BindBufferSphere(sphere_VA0, sphere_VBO, sphere_EBO, sphere_data, sphere_indices);
    Texture tex1, tex2, floor_tex, sky_tex_cube,lifan,girl;
    //在声明Texture时的标准是，声明的Texture要与Shader里的采样器SamplerND名称一样，这是为了统一在Mesh类中得到对应的Texture单元
    tex1 = ResourceManager::getInstance()->LoadTexture2D("D:/Resource/Pic/container2.png","texture1");
    tex2 = ResourceManager::getInstance()->LoadTexture2D("D:/Resource/Pic/container2_specular.png","texture2");
    lifan = ResourceManager::getInstance()->LoadTexture2D("D:/Resource/Pic/lifan.jpg", "lifan");
    floor_tex = ResourceManager::getInstance()->LoadTexture2D("D:/Resource/Pic/floor.png","floor_texture");
    sky_tex_cube = ResourceManager::getInstance()->LoadCubeTexture(faces, "skybox_texture");
    girl= ResourceManager::getInstance()->LoadTexture2D("D:/Resource/Pic/girl.jpg", "girl_texture");
    shader1.Use(); // don't forget to activate/use the shader before setting uniforms!
    shader1.SetInt("material.diffuse", 0);//告诉着色器采样器，texture1属于0号纹理单元，下同      
    shader1.SetInt("material.specular", 1);
    skyShader.SetInt("skybox", 0);   
    reflectShader.SetInt("skybox", 0);
    // render loop 渲染循环，所有的渲染操作都应在此循环中实现，可以添加函数进去
    // -----------
    
    while (!glfwWindowShouldClose(window))
    {
        if (pauseRender == true) {
            cout << "请输入指令" << endl;
            cin>>command;
            pauseRender = false;
        }
      
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        // -----
        processInput(window);

        //render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);//整个颜色缓冲都会被填充为glClearColor里所设置的颜色
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//用glClearColor的颜色清空整个窗口,在每一帧渲染前清楚深度缓冲信息，否则前一帧的会继续保留

        
        //----------------------------------------------------------------------        
        float alpha = (sin(currentFrame) / 2.0f) + 0.5f;
       // float x = sin(currentFrame) * 10.0f;
       // float y = cos(currentFrame) * 10.0f;

        //---------------------设置三大矩阵，MODEL VIEW PROJECTION-------------------//           
        view = camera.getViewMatrix();
        projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 model = glm::mat4(1.0f);
       
        // directional light
        dirLight.Use(shader1);
        dirLight.Use(modelShader);
        // point light 1
        float x_p = 4.0f*cos(glfwGetTime());
        float z_p = 4.0f*sin(glfwGetTime());
        lightPos = glm::vec3(x_p, 0.0f, z_p);
        //pointLight.SetPosition(glm::vec3(5, 0, 0));
        pointLight.SetPosition(lightPos);
        pointLight.Use(shader1);
        pointLight.Use(modelShader);    
        switch (command)
        {
        case 1:
            start_Floor = true;
            command = -1;
            break;
        case 2:
            start_Cube = true;
            cout << "输入立方体位置 x,y,z" << endl;
            cin >> x >> y >> z;
            cube_pos = glm::vec3(x, y, z);
            cout << "输入大小" << endl;
            cin >> size;
            cube_size = size;
            cout << "输入纹理名称1 当前系统中的纹理名有 ‘texture1’‘texture2’‘lifan’‘floor_texture’" << endl;
            cin >> texture_name1;
            cout << "输入纹理名称2 当前系统中的纹理名有 ‘texture1’‘texture2’‘lifan’‘floor_texture’" << endl;
            cin >> texture_name2;
            command = -1;
            break;
        case 3:
            start_Sphere = true;
            cout << "输入球体球心坐标" << endl;
            cin >> x >> y >> z;
            sphere_pos = glm::vec3(x, y, z);
            cout << "输入球体大小" << endl;
            cin >> size;
            sphere_size = size;
            cout << "输入纹理名称1 当前系统中的纹理名有 ‘texture1’‘texture2’‘lifan’‘floor_texture’" << endl;
            cin >> texture_name1;
            command = -1;
            break;
        case 4:
            start_ReflectSphere = true;
            cout << "输入球体球心坐标" << endl;
            cin >> x >> y >> z;
            sphere_pos = glm::vec3(x, y, z);
            cout << "输入球体大小" << endl;
            cin >> size;
            sphere_size = size;
            command = -1;
            break;
        case 5:
            start_Model = true;
            cout << "输入模型位置 x,y,z" << endl;
            cin >> x >> y >> z;
            model_pos = glm::vec3(x, y, z);
            cout << "输入大小" << endl;
            cin >> size;
            model_size = size;
            command = -1;
            break;
        case 6:
            start_Clear = true;
            cout << "清除所有渲染图形" << endl;
            command = -1;
            break;
        default:
            break;
        }
        if (start_Model) {            
            model = glm::mat4(1.0f);
            model = glm::translate(model, model_pos);
            model = glm::scale(model, glm::vec3(model_size, model_size, model_size));
            DrawModel(Ourmodel, shader1, 64.0f, camera.Position, model);
        }
        if (start_Floor) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(10.0f, 0.0f, 10.0f));
            DrawFloor(shader1, floor_tex, floor_VAO, 128.0f, camera.Position, model);
        }
        if (start_Cube) {
            
            model = glm::mat4(1.0f);
            model = glm::translate(model, cube_pos);
            model = glm::scale(model, glm::vec3(cube_size, cube_size, cube_size));
            DrawNormalCube(shader1, ResourceManager::getInstance()->GetTexture(texture_name1), ResourceManager::getInstance()->GetTexture(texture_name2), VAO, 64.0f, camera.Position, model);
        }
        if (start_Sphere) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, sphere_pos);
            model = glm::scale(model, glm::vec3(sphere_size, sphere_size, sphere_size));
            DrawNormalSphere(shader1, ResourceManager::getInstance()->GetTexture(texture_name1), sphere_VA0, 64.0f, camera.Position, model);
        }
        if (start_ReflectSphere) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, sphere_pos);
            model = glm::scale(model, glm::vec3(sphere_size, sphere_size, sphere_size));
            DrawReflectSphere(sphere_VA0, reflectShader,sky_tex_cube, camera.Position,model);
        }
        /*if (start_Clear==true) {
             start_Floor = false;
             start_Sphere = false;
             start_Model = false;
             start_Cube = false;
             start_ReflectSphere = false;             
        }*/
        //使用灯光
        DrawLight(lightShader, light_VAO, lightPos);
        //DrawLight(lightShader, light_VAO, glm::vec3(5,0,0));
        //画模型
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f)); // translate it down so it's at the center of the scene
        //model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
        //DrawModel(Ourmodel, modelShader, 64.0f,camera.Position, model);
        //画反射立方体
        /*reflectShader.Use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
        reflectShader.SetMatrix4("view",view);
        reflectShader.SetMatrix4("projection", projection);
        reflectShader.SetMatrix4("model", model);
        reflectShader.SetVector3f("cameraPos", camera.Position);
        glActiveTexture(GL_TEXTURE0);
        sky_tex.Bind(TextureType::TEXTURE_CUBE);*/
        //DrawCube(reflectCube_VAO);
        //ironModel.Draw(reflectShader);
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(0.0f, 5.0f, 0.0f));
        //DrawReflectCube(reflectCube_VAO, reflectShader, sky_tex_cube, camera.Position, model);
        // 
        //画天空盒
        DrawSkyBox(skyShader,sky_tex_cube,sky_box_VAO);      

       // for (int i = 0; i < 10; i++) {
       //     glm::mat4 model = glm::mat4(1.0f);
       //     model = glm::translate(model, cubePositions[i]);
       //     DrawNormalCube(shader1, tex1, tex2, VAO, 64.0f, camera.Position, model);
       // }
       /* sphereShader.Use();
        model = glm::mat4(1.0f);
        model = glm::rotate(model,glm::radians(90.0f), glm::vec3(0, 1, 0));
        sphereShader.SetMatrix4("view", view);
        sphereShader.SetMatrix4("projection", projection);
        sphereShader.SetMatrix4("model", model);
        sphereShader.SetInt("texture1", 0);
        glActiveTexture(GL_TEXTURE0);
        tex1.Bind(TextureType::TEXTURE_2D);
        DrawSphere(sphere_VA0);*/
        //model = glm::mat4(1.0f); 
       // model = glm::translate(model, glm::vec3(2.0f, 5.0f, 0.0f));
       // model = glm::translate(model, lightPos);
       // DrawReflectSphere(sphere_VA0, reflectShader, sky_tex_cube, camera.Position,model);
       

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);//双缓冲设置函数
        glfwPollEvents(); //函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，
                         //并调用对应的回调函数（可以通过回调方法手动设置）。
       // cout << getFPS(last_time) << endl;
    }   
    //渲染循环结束，解绑VAO,VBO以及删除着色器程序对象
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &light_VAO);
    glDeleteVertexArrays(1, &floor_VAO);
    glDeleteVertexArrays(1, &sky_box_VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &floor_VBO);
    glDeleteBuffers(1, &sky_box_VBO);
    glDeleteProgram(shader1.ID);
    glDeleteProgram(lightShader.ID);
    glDeleteProgram(skyShader.ID);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if (is_firstMouse) {
        Last_X = xpos;
        Last_Y = ypos;
        is_firstMouse = false;
    }
    float offset_x = xpos - Last_X;
    float offset_y = Last_Y - ypos;//这个要相反，因为得
    /*cout << "xpos=" << xpos << endl;
    cout << "ypos=" << ypos << endl;
    cout << "LastX=" << Last_X << endl;
    cout << "LastY=" << Last_Y << endl;
    cout << "offset_x=" << offset_x << endl;
    cout << "offset_y=" << offset_y << endl;
    cout << endl;*/
    
    Last_X = xpos;
    Last_Y = ypos;

    camera.ProcessMouseClick(offset_x, offset_y);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    camera.ProcessScrollMouse(yoffset);
}
float getFPS(float lats_Time)
{
    float new_Time = (float)glfwGetTime();
    float FPS = 1 / (new_Time - last_time);
    last_time = new_Time;
    return FPS;
}
void DrawCube(unsigned int vao) {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
void DrawPlane(unsigned int vao) {   
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
void DrawSkyBox(Shader& shader,Texture tex,unsigned int vao,glm::mat4 projection_mat4, glm::mat4 view_mat4) {
    view_mat4 = glm::mat4(glm::mat3(camera.getViewMatrix()));//移除了位移的属性，这样LookAt矩阵就不会因为位移而映射了
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    shader.Use();   
    shader.SetMatrix4("view", view_mat4);
    shader.SetMatrix4("projection", projection_mat4);
    glActiveTexture(GL_TEXTURE0);
    tex.Bind(TextureType::TEXTURE_CUBE);
    DrawCube(vao);
    glDepthFunc(GL_LESS); // set depth function back to default
}
void DrawLight(Shader& shader, unsigned int vao, glm::vec3 lightPos,glm::mat4 projection_mat4,glm::mat4 view_mat4) {
    shader.Use();    
    shader.SetMatrix4("projection", projection_mat4);
    shader.SetMatrix4("view", view_mat4);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    shader.SetMatrix4("model", model);
    DrawCube(vao);
}
void DrawModel(Model& model, Shader& shader, float shininess, glm::vec3 viewPos, glm::mat4 model_mat4, glm::mat4 projection_mat4,glm::mat4 view_mat4) {
    shader.Use();
    shader.SetMatrix4("projection", projection_mat4);
    shader.SetMatrix4("view", view_mat4);
    shader.SetVector3f("viewPos", viewPos);
    shader.SetFloat("material.shininess", shininess);   
    shader.SetMatrix4("model", model_mat4);
    model.Draw(shader);
}
void DrawFloor(Shader& shader, Texture tex, unsigned int vao, float shininess, glm::vec3 viewPos, glm::mat4 model, glm::mat4 projection_mat4, glm::mat4 view_mat4) {
    shader.Use();
    shader.SetFloat("material.shininess", shininess);
    shader.SetMatrix4("view", view_mat4);
    shader.SetMatrix4("projection", projection_mat4);
    shader.SetVector3f("viewPos", viewPos);
    glActiveTexture(GL_TEXTURE0);
    tex.Bind(TextureType::TEXTURE_2D);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);    
    shader.SetMatrix4("model", model);
    DrawPlane(vao);
}

void ComputeSphereData() {
    for (int i = 0; i <= u_frag; i++) {
        for (int j = 0; j <= v_frag; j++) {
            float x = sin((float)i * pi / u_frag) * cos((float)j * pi * 2.0f / v_frag);
            float y = cos((float)i * pi / u_frag);
            float z = sin((float)i * pi / u_frag) * sin((float)j * pi * 2.0f / v_frag);
            glm::vec3 vertex(x, y, z);
            sphere_vertexs.push_back(vertex);            
            float u = (float)i / (float)(u_frag);
            float v = (float)j / (float)(v_frag);            
            glm::vec2 texCoord(u, v);
            sphere_texCoords.push_back(texCoord);
            sphere_normals.push_back(glm::vec3(x, y, z));
        }
    }
    bool oddRow = false;//判断是否为奇
    for (unsigned int y = 0; y < v_frag; ++y)
    {
        if (!oddRow) // even rows: y == 0, y == 2; and so on
        {
            for (unsigned int x = 0; x <= u_frag; ++x)
            {
                sphere_indices.push_back(y * (u_frag + 1) + x);
                sphere_indices.push_back((y + 1) * (u_frag + 1) + x);
            }
        }

        // 这里奇偶分开添加是有道理的，奇偶分开添加，就能首尾相连，自己可以拿笔画一画
        else
        {
            for (int x = u_frag; x >= 0; --x)
            {
                sphere_indices.push_back((y + 1) * (u_frag + 1) + x);
                sphere_indices.push_back(y * (u_frag + 1) + x);
            }
        }
        oddRow = !oddRow;
    }
    indexCount = sphere_indices.size();
    for (int i = 0; i < sphere_vertexs.size(); i++) {//data的数据集是   position.x position.y position.z |noraml.x normal.y normal.z |texCoord.x texCoord.y
        sphere_data.push_back(sphere_vertexs[i].x);         //                           顶点                              法向量                    纹理坐标
        sphere_data.push_back(sphere_vertexs[i].y);
        sphere_data.push_back(sphere_vertexs[i].z);
        if (sphere_normals.size() > 0)
        {
            sphere_data.push_back(sphere_normals[i].x);
            sphere_data.push_back(sphere_normals[i].y);
            sphere_data.push_back(sphere_normals[i].z);
        }
        if (sphere_texCoords.size() > 0)
        {
            sphere_data.push_back(sphere_texCoords[i].x);
            sphere_data.push_back(sphere_texCoords[i].y);
        }
    }
}
void BindBufferSphere(unsigned int vao,unsigned int vbo,unsigned int ebo,std::vector<float> data,std::vector<unsigned int> indices) {      
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);   
}
void DrawSphere(unsigned int vao) {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void DrawReflectSphere(unsigned int vao, Shader reflectShader, Texture reflectTex, glm::vec3 viewPos, glm::mat4 model_mat4, glm::mat4 projection_mat4, glm::mat4 view_mat4)
{
    reflectShader.Use();
    reflectShader.SetMatrix4("view", view_mat4);
    reflectShader.SetMatrix4("projection", projection_mat4);
    reflectShader.SetMatrix4("model", model_mat4);
    reflectShader.SetVector3f("cameraPos", viewPos);
    glActiveTexture(GL_TEXTURE0);
    reflectTex.Bind(TextureType::TEXTURE_CUBE);
    DrawSphere(vao);
}

void DrawReflectCube(unsigned int vao, Shader reflectShader, Texture reflectTex, glm::vec3 viewPos, glm::mat4 model_mat4, glm::mat4 projection_mat4, glm::mat4 view_mat4)
{
    reflectShader.Use();   
    reflectShader.SetMatrix4("view", view_mat4);
    reflectShader.SetMatrix4("projection", projection_mat4);
    reflectShader.SetMatrix4("model", model_mat4);
    reflectShader.SetVector3f("cameraPos", viewPos);
    glActiveTexture(GL_TEXTURE0);
    reflectTex.Bind(TextureType::TEXTURE_CUBE); 
    DrawCube(vao);
}


void DrawNormalCube(Shader& shader, Texture tex1,Texture tex2,unsigned int vao, float shininess, glm::vec3 viewPos, glm::mat4 model_mat4, glm::mat4 projection_mat4, glm::mat4 view_mat4)
{
    shader.Use();
    shader.SetMatrix4("projection", projection_mat4);
    shader.SetMatrix4("view", view_mat4);
    shader.SetMatrix4("model", model_mat4);
    shader.SetFloat("material.shininess", shininess);
    shader.SetVector3f("viewPos", viewPos);
    glActiveTexture(GL_TEXTURE0);
    tex1.Bind(TextureType::TEXTURE_2D);
    glActiveTexture(GL_TEXTURE1);
    tex2.Bind(TextureType::TEXTURE_2D);
    DrawCube(vao);
}

void DrawNormalSphere(Shader& shader, Texture tex, unsigned int vao, float shininess, glm::vec3 viewPos, glm::mat4 model_mat4, glm::mat4 projection_mat4, glm::mat4 view_mat4)
{
    shader.Use();
    shader.SetMatrix4("projection", projection_mat4);
    shader.SetMatrix4("view", view_mat4);
    shader.SetMatrix4("model", model_mat4);
    shader.SetFloat("material.shininess", shininess);
    shader.SetVector3f("viewPos", viewPos);
    glActiveTexture(GL_TEXTURE0);
    tex.Bind(TextureType::TEXTURE_2D);
    DrawSphere(vao);
}


