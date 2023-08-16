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

const int u_frag = 64;//u��������������һ��������y��ļн�
const int v_frag = 64;//v��������������һ��������x��ļн�
const float pi = 3.1415926;
std::vector<float> sphere_data;
std::vector<glm::vec3> sphere_vertexs;
std::vector<glm::vec2> sphere_texCoords;
std::vector<glm::vec3> sphere_normals;
std::vector<unsigned int> sphere_indices;
std::map<std::string, Model> Model_Map;
unsigned int indexCount;

float Last_X = SCR_WIDTH / 2.0f;//��¼��һ֡��Ⱦ�Ĺ��λ��
float Last_Y = SCR_HEIGHT / 2.0f;
float last_time = 0.0f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;//��¼��һ֡��ʱ��
bool is_firstMouse = true;
bool pauseRender = false;
bool isPressed_P = false;
bool isPressed_Escape = false;
bool isShowCursor = false;//�Ƿ���ʾ���

//��ʼ��Ⱦ���
bool start_Floor = false;
bool start_Sphere = false;
bool start_Model = false;
bool start_Cube = false;
bool start_ReflectSphere = false;
bool start_Clear = true;


Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
glm::mat4 view = camera.getViewMatrix();
glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
glm::vec3 lightPos(5.0f, 0.0f, 1.0f);//�趨��Դλ��
static int command;
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height){//֡�����С������Ҫһ��GLFWwindow��Ϊ���ĵ�һ��������
                                                                       //�Լ�����������ʾ���ڵ���ά��

    glViewport(0, 0, width, height);//����OpenGL��Ⱦ���ڵĳߴ��С�����ӿ�(Viewport)

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
     if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && !isPressed_P) {//��֤һֱ��P��Ҳ����������һ�δ���         
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
    cout << "================��ϵͳʹ�������п��ƣ���Ҫ��������Ⱦ�����壬��Ⱦ���壬����������ͼ�����ӹ���Ч��������ģ��=============="<<endl;
    cout << "A������ '1'��ӵذ�Ч��" << endl;
    cout << "B������ '2 '���Լ���������"<<endl;   
    cout << "C������ '3 '���Լ�������" << endl;    
    cout << "D������ '4 '���Լ��ؾ��з���Ч��������" << endl;   
    cout << "E������ '5 ' ���Լ���ģ��" << endl;    
}
float getFPS(float lats_Time);
#pragma region Function of Rendering Basic Graphics
//���ƻ���ͼ�ι��ܺ���
void DrawCube(unsigned int vao);
void DrawPlane(unsigned int vao);
#pragma endregion



#pragma region Function of Rendering Scene
//���Ƴ���ͼ�ι��ܺ���
void DrawSkyBox(Shader& shader, Texture tex, unsigned int vao, glm::mat4 projection_mat4 = projection, glm::mat4 view_mat4 = view);
void DrawLight(Shader& shader, unsigned int vao, glm::vec3 lightPos, glm::mat4 projection_mat4 = projection, glm::mat4 view_mat4 = view);
void DrawModel(Model& model, Shader& shader, float shininess, glm::vec3 viewPos, glm::mat4 model_mat4, glm::mat4 projection_mat4 = projection, glm::mat4 view_mat4 = view);
void DrawFloor(Shader& shader, Texture tex, unsigned int vao, float shininess, glm::vec3 viewPos, glm::mat4 model, glm::mat4 projection_mat4 = projection, glm::mat4 view_mat4 = view);//��viewPos��shininess����Ϊ����в��ʺ͹��գ��ͻ�Եذ�Ҳ����һЩЧ��
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
    //���ǳ�ʼ������
   
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__//�����MACϵͳ����Ҫ������������
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
#pragma region InitializeWindow 
    // glfw window creation
   // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Kick your Ass!", NULL, NULL);//����window���󣬺�����Ҫ����ʹ��
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //---------------���ûص�����--------------------//
    glfwMakeContextCurrent(window);//�����Ǵ��ڵ�����������Ϊ��ǰ�̵߳���������,��GLFW�Ŀ�������
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//�ô˺�����ע��ص�����������һΪwindow�����ǻص�����
    glfwSetCursorPosCallback(window,mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //---------------���ûص�����--------------------//


    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))//��ʼ��GLAD��GLAD����������Open_GL�ĺ�����
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
#pragma region ʮ���������λ��
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
#pragma region �����嶥������
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
#pragma region ���������Ķ�������
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
#pragma region ��������������������������嶥������
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
    //dirǰ׺ָdirLight
    glm::vec3 direction(-0.2f, -1.0f, -0.3f);
    glm::vec3 dir_ambient(0.5f, 0.5f, 0.5f);
    glm::vec3 dir_diffuse(0.4f, 0.4f, 0.4f);
    glm::vec3 dir_specular(0.5f, 0.5f, 0.5f);
    //plǰ׺ָpointLight
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
#pragma region ��պеĶ�������
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
#pragma region ��պе�����ͼƬ��
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
    
   

    //=========����ʹ��shader1�ĺ����������������Ԫ��1���ã���ֻ����������һ������������ʣ�һ���Ǿ��淴�����=========//
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
    //VBO�����Ǵ��������ݣ���������������������������ݼ�����Ҫ���������壬���Ը����Լ�����Ҫ�󶨵��Լ��趨��VAO��
    unsigned int VBO, VAO, EBO,floor_VBO,sky_box_VAO,sky_box_VBO;//���Ǳ���Ͷ����
    unsigned int sphere_VA0, sphere_VBO, sphere_EBO;
    unsigned int light_VAO;//���ǹ�Դ����
    unsigned int floor_VAO;//�ذ�����
    unsigned int reflectCube_VAO;//��������������
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_Tex), vertices_Tex, GL_STATIC_DRAW);//���ݲ��ı�:GL_STATIC_DRAW  
                                                                              //�������ݸ�VBO ��Ϊ֮ǰ1�󶨵���VBO������������
                                                                             //֮ǰ��glBindBuffer�󶨵���VB0_1���Ǿ��Ǵ���VBO_1   
    // explain Position data;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);//���ʹ�������  0����Layout(Location=0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(2);
    /*=================��Դ�����ݣ�����������Ϊ��Դ��*/
    glBindVertexArray(light_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);//ֻ��Ҫ����λ�ã�����Ҫ����������Ϣ
    glEnableVertexAttribArray(0);
    //=================�ذ����������
    glBindVertexArray(floor_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, floor_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_floor), vertices_floor, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(2);

    //==================��պ����������
    glBindVertexArray(sky_box_VAO);
    glBindBuffer(GL_ARRAY_BUFFER,sky_box_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);      
    //===================����������ͷ�����������������
    glBindVertexArray(reflectCube_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);    

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    //��������
    BindBufferSphere(sphere_VA0, sphere_VBO, sphere_EBO, sphere_data, sphere_indices);
    Texture tex1, tex2, floor_tex, sky_tex_cube,lifan,girl;
    //������Textureʱ�ı�׼�ǣ�������TextureҪ��Shader��Ĳ�����SamplerND����һ��������Ϊ��ͳһ��Mesh���еõ���Ӧ��Texture��Ԫ
    tex1 = ResourceManager::getInstance()->LoadTexture2D("D:/Resource/Pic/container2.png","texture1");
    tex2 = ResourceManager::getInstance()->LoadTexture2D("D:/Resource/Pic/container2_specular.png","texture2");
    lifan = ResourceManager::getInstance()->LoadTexture2D("D:/Resource/Pic/lifan.jpg", "lifan");
    floor_tex = ResourceManager::getInstance()->LoadTexture2D("D:/Resource/Pic/floor.png","floor_texture");
    sky_tex_cube = ResourceManager::getInstance()->LoadCubeTexture(faces, "skybox_texture");
    girl= ResourceManager::getInstance()->LoadTexture2D("D:/Resource/Pic/girl.jpg", "girl_texture");
    shader1.Use(); // don't forget to activate/use the shader before setting uniforms!
    shader1.SetInt("material.diffuse", 0);//������ɫ����������texture1����0������Ԫ����ͬ      
    shader1.SetInt("material.specular", 1);
    skyShader.SetInt("skybox", 0);   
    reflectShader.SetInt("skybox", 0);
    // render loop ��Ⱦѭ�������е���Ⱦ������Ӧ�ڴ�ѭ����ʵ�֣�������Ӻ�����ȥ
    // -----------
    
    while (!glfwWindowShouldClose(window))
    {
        if (pauseRender == true) {
            cout << "������ָ��" << endl;
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
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);//������ɫ���嶼�ᱻ���ΪglClearColor�������õ���ɫ
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//��glClearColor����ɫ�����������,��ÿһ֡��Ⱦǰ�����Ȼ�����Ϣ������ǰһ֡�Ļ��������

        
        //----------------------------------------------------------------------        
        float alpha = (sin(currentFrame) / 2.0f) + 0.5f;
       // float x = sin(currentFrame) * 10.0f;
       // float y = cos(currentFrame) * 10.0f;

        //---------------------�����������MODEL VIEW PROJECTION-------------------//           
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
            cout << "����������λ�� x,y,z" << endl;
            cin >> x >> y >> z;
            cube_pos = glm::vec3(x, y, z);
            cout << "�����С" << endl;
            cin >> size;
            cube_size = size;
            cout << "������������1 ��ǰϵͳ�е��������� ��texture1����texture2����lifan����floor_texture��" << endl;
            cin >> texture_name1;
            cout << "������������2 ��ǰϵͳ�е��������� ��texture1����texture2����lifan����floor_texture��" << endl;
            cin >> texture_name2;
            command = -1;
            break;
        case 3:
            start_Sphere = true;
            cout << "����������������" << endl;
            cin >> x >> y >> z;
            sphere_pos = glm::vec3(x, y, z);
            cout << "���������С" << endl;
            cin >> size;
            sphere_size = size;
            cout << "������������1 ��ǰϵͳ�е��������� ��texture1����texture2����lifan����floor_texture��" << endl;
            cin >> texture_name1;
            command = -1;
            break;
        case 4:
            start_ReflectSphere = true;
            cout << "����������������" << endl;
            cin >> x >> y >> z;
            sphere_pos = glm::vec3(x, y, z);
            cout << "���������С" << endl;
            cin >> size;
            sphere_size = size;
            command = -1;
            break;
        case 5:
            start_Model = true;
            cout << "����ģ��λ�� x,y,z" << endl;
            cin >> x >> y >> z;
            model_pos = glm::vec3(x, y, z);
            cout << "�����С" << endl;
            cin >> size;
            model_size = size;
            command = -1;
            break;
        case 6:
            start_Clear = true;
            cout << "���������Ⱦͼ��" << endl;
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
        //ʹ�õƹ�
        DrawLight(lightShader, light_VAO, lightPos);
        //DrawLight(lightShader, light_VAO, glm::vec3(5,0,0));
        //��ģ��
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f)); // translate it down so it's at the center of the scene
        //model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
        //DrawModel(Ourmodel, modelShader, 64.0f,camera.Position, model);
        //������������
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
        //����պ�
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
        glfwSwapBuffers(window);//˫�������ú���
        glfwPollEvents(); //���������û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬��
                         //�����ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã���
       // cout << getFPS(last_time) << endl;
    }   
    //��Ⱦѭ�����������VAO,VBO�Լ�ɾ����ɫ���������
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
    float offset_y = Last_Y - ypos;//���Ҫ�෴����Ϊ��
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
    view_mat4 = glm::mat4(glm::mat3(camera.getViewMatrix()));//�Ƴ���λ�Ƶ����ԣ�����LookAt����Ͳ�����Ϊλ�ƶ�ӳ����
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
    bool oddRow = false;//�ж��Ƿ�Ϊ��
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

        // ������ż�ֿ�������е���ģ���ż�ֿ���ӣ�������β�������Լ������ñʻ�һ��
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
    for (int i = 0; i < sphere_vertexs.size(); i++) {//data�����ݼ���   position.x position.y position.z |noraml.x normal.y normal.z |texCoord.x texCoord.y
        sphere_data.push_back(sphere_vertexs[i].x);         //                           ����                              ������                    ��������
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


