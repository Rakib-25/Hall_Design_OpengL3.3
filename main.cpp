//
//  main.cpp
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "cube.h"
#include "stb_image.h"
#include <iostream>
#include "Sphere2.h"
#pragma warning(disable:4996)


using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);
void shaderActivate(Shader& shader);
void bed(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture);
void room_structure(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture);
void table(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture);
void road(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture);
void FazlulHaqueHall(unsigned int& cubeVAO, unsigned int& cVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture);
void drawCylinder(unsigned int& cVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);
void piller(unsigned int& cubeVAO, unsigned int& cVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture);
void chair(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture); void chair(Cube& cube, Shader& lightingShaderWithTexture, Shader& lightingShader, glm::mat4 alTogether);
void Fan(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture);
void door(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture);
//void drawFish(Shader& lightingShader, glm::mat4 model);
//void pond(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture);
void pond(unsigned int& cubeVAO, unsigned int& cVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture);
void Tree_Making(unsigned int& cubeVAO, unsigned int& cVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture);
void Clock(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture);
void getCurrentTime(int& hours, int& minutes, int& seconds);
void Man_Making(unsigned int& cubeVAO, unsigned int& cVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture);





float rotateClock = 0;
float walk = 0;
bool sign1 = 1;




class Curve
{
public:
    vector<float> cntrlPoints;
    vector <float> coordinates;
    vector <float> normals;
    vector <int> indices;
    vector <float> vertices;
    const double pi = 3.14159265389;
    const int nt = 40;
    const int ntheta = 20;
    float red;
    float green;
    float blue;
    Curve(vector<float>& tmp)
    {
        this->cntrlPoints = tmp;
        this->fishVAO = hollowBezier(cntrlPoints.data(), ((unsigned int)cntrlPoints.size() / 3) - 1);
        
    }
    ~Curve()
    {
        glDeleteVertexArrays(1, &fishVAO);
        glDeleteVertexArrays(1, &bezierVAO);
        glDeleteBuffers(1, &bezierVBO);
        glDeleteBuffers(1, &bezierEBO);
    }
    void draw(Shader& lightingShader, glm::mat4 model, float red = 0, float green = 0.6, float blue = 0)
    {
        /// Fish
        lightingShader.use();
        lightingShader.setMat4("model", model);
        lightingShader.setVec3("material.ambient", glm::vec3(red, green, blue));
        lightingShader.setVec3("material.diffuse", glm::vec3(red, green, blue));
        lightingShader.setVec3("material.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setFloat("material.shininess", 32.0f);

        glBindVertexArray(fishVAO);
        glDrawElements(GL_TRIANGLES,                    // primitive type
            (unsigned int)indices.size(),          // # of indices
            GL_UNSIGNED_INT,                 // data type
            (void*)0);                       // offset to indices

        // unbind VAO
        glBindVertexArray(0);
        /// End Fish
    }
private:
    unsigned int fishVAO;
    unsigned int bezierVAO;
    unsigned int bezierVBO;
    unsigned int bezierEBO;


    unsigned int drawControlPoints()
    {
        unsigned int controlPointVAO;
        unsigned int controlPointVBO;

        glGenVertexArrays(1, &controlPointVAO);
        glGenBuffers(1, &controlPointVBO);

        glBindVertexArray(controlPointVAO);

        glBindBuffer(GL_ARRAY_BUFFER, controlPointVBO);
        glBufferData(GL_ARRAY_BUFFER, (unsigned int)cntrlPoints.size() * sizeof(float), cntrlPoints.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        return controlPointVAO;
    }

    long long nCr(int n, int r)
    {
        if (r > n / 2)
            r = n - r; // because C(n, r) == C(n, n - r)
        long long ans = 1;
        int i;

        for (i = 1; i <= r; i++)
        {
            ans *= n - r + i;
            ans /= i;
        }

        return ans;
    }
    void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L)
    {
        double y = 0;
        double x = 0;
        t = t > 1.0 ? 1.0 : t;
        for (int i = 0; i < L + 1; i++)
        {
            long long ncr = nCr(L, i);
            double oneMinusTpow = pow(1 - t, double(L - i));
            double tPow = pow(t, double(i));
            double coef = oneMinusTpow * tPow * ncr;
            x += coef * ctrlpoints[i * 3];
            y += coef * ctrlpoints[(i * 3) + 1];

        }
        xy[0] = float(x);
        xy[1] = float(y);
    }

    unsigned int hollowBezier(GLfloat ctrlpoints[], int L)
    {
        int i, j;
        float x, y, z, r;                //current coordinates
        float theta;
        float nx, ny, nz, lengthInv;    // vertex normal


        const float dtheta = 2 * pi / ntheta;        //angular step size

        float t = 0;
        float dt = 1.0 / nt;
        float xy[2];

        for (i = 0; i <= nt; ++i)              //step through y
        {
            BezierCurve(t, xy, ctrlpoints, L);
            r = xy[0];
            y = xy[1];
            theta = 0;
            t += dt;
            lengthInv = 1.0 / r;

            for (j = 0; j <= ntheta; ++j)
            {
                double cosa = cos(theta);
                double sina = sin(theta);
                z = r * cosa;
                x = r * sina;

                coordinates.push_back(x);
                coordinates.push_back(y);
                coordinates.push_back(z);

                // normalized vertex normal (nx, ny, nz)
                // center point of the circle (0,y,0)
                nx = (x - 0) * lengthInv;
                ny = (y - y) * lengthInv;
                nz = (z - 0) * lengthInv;

                normals.push_back(nx);
                normals.push_back(ny);
                normals.push_back(nz);

                theta += dtheta;
            }
        }
        // generate index list of triangles
        // k1--k1+1
        // |  / |
        // | /  |
        // k2--k2+1

        int k1, k2;
        for (int i = 0; i < nt; ++i)
        {
            k1 = i * (ntheta + 1);     // beginning of current stack
            k2 = k1 + ntheta + 1;      // beginning of next stack

            for (int j = 0; j < ntheta; ++j, ++k1, ++k2)
            {
                // k1 => k2 => k1+1
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);

                // k1+1 => k2 => k2+1
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }

        size_t count = coordinates.size();
        for (int i = 0; i < count; i += 3)
        {
            //cout << count << ' ' << i + 2 << endl;
            vertices.push_back(coordinates[i]);
            vertices.push_back(coordinates[i + 1]);
            vertices.push_back(coordinates[i + 2]);

            vertices.push_back(normals[i]);
            vertices.push_back(normals[i + 1]);
            vertices.push_back(normals[i + 2]);
        }

        glGenVertexArrays(1, &bezierVAO);
        glBindVertexArray(bezierVAO);

        // create VBO to copy vertex data to VBO
        glGenBuffers(1, &bezierVBO);
        glBindBuffer(GL_ARRAY_BUFFER, bezierVBO);           // for vertex data
        glBufferData(GL_ARRAY_BUFFER,                   // target
            (unsigned int)vertices.size() * sizeof(float), // data size, # of bytes
            vertices.data(),   // ptr to vertex data
            GL_STATIC_DRAW);                   // usage

        // create EBO to copy index data
        glGenBuffers(1, &bezierEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bezierEBO);   // for index data
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // target
            (unsigned int)indices.size() * sizeof(unsigned int),             // data size, # of bytes
            indices.data(),               // ptr to index data
            GL_STATIC_DRAW);                   // usage

        // activate attrib arrays
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        // set attrib arrays with stride and offset
        int stride = 24;     // should be 24 bytes
        glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, (void*)(sizeof(float) * 3));

        // unbind VAO, VBO and EBO
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        return bezierVAO;
    }

};
vector<float>Fish = {
-0.0100, 1.9950, 5.1000,
-0.0550, 1.9800, 5.1000,
-0.0950, 1.9350, 5.1000,
-0.1500, 1.8250, 5.1000,
-0.2250, 1.5900, 5.1000,
-0.2550, 1.3450, 5.1000,
-0.2050, 1.1700, 5.1000,
-0.1400, 1.0050, 5.1000,
-0.0400, 0.8600, 5.1000,
0.0400, 0.7300, 5.1000,
0.1300, 0.6350, 5.1000,
0.2400, 0.5050, 5.1000,
};

vector<float>Tree = {
-0.0500, 2.2050, 5.1000,
-0.1100, 2.2150, 5.1000,
-0.1850, 2.1950, 5.1000,
-0.2350, 2.1400, 5.1000,
-0.2450, 2.0400, 5.1000,
-0.2550, 1.9400, 5.1000,
-0.2650, 1.8450, 5.1000,
-0.2650, 1.7200, 5.1000,
-0.2650, 1.5700, 5.1000,
-0.2650, 1.4600, 5.1000,
-0.2700, 1.3300, 5.1000,
-0.2650, 1.1650, 5.1000,
-0.2650, 1.0350, 5.1000,
-0.2700, 0.8650, 5.1000,
-0.2850, 0.7200, 5.1000,
-0.3050, 0.6200, 5.1000,
-0.3150, 0.4700, 5.1000,
-0.3200, 0.3050, 5.1000,
-0.3600, 0.1700, 5.1000,
-0.3850, 0.1000, 5.1000,
-0.4100, 0.0200, 5.1000,
-0.4450, -0.0450, 5.1000,
-0.4600, -0.0750, 5.1000,
};

vector<float>leaf = {
-0.0100, 2.3600, 5.1000,
-0.0550, 2.3600, 5.1000,
-0.1550, 2.3800, 5.1000,
-0.2900, 2.3700, 5.1000,
-0.3900, 2.3350, 5.1000,
-0.4150, 2.2350, 5.1000,
-0.4450, 2.1200, 5.1000,
-0.5000, 2.0900, 5.1000,
-0.5700, 2.0800, 5.1000,
-0.6350, 2.0700, 5.1000,
-0.7000, 2.0450, 5.1000,
-0.8000, 2.0100, 5.1000,
-0.8650, 1.9800, 5.1000,
-0.9100, 1.9300, 5.1000,
-0.9200, 1.8900, 5.1000,
-0.9400, 1.7750, 5.1000,
-0.9300, 1.5650, 5.1000,
-0.9150, 1.5200, 5.1000,
-0.8150, 1.4450, 5.1000,
-0.7250, 1.4000, 5.1000,
-0.6000, 1.3650, 5.1000,
-0.4650, 1.3550, 5.1000,
-0.3700, 1.3550, 5.1000,
-0.2800, 1.3550, 5.1000,
-0.2300, 1.3550, 5.1000,
-0.1800, 1.3650, 5.1000,
-0.1250, 1.3750, 5.1000,
-0.1150, 1.3800, 5.1000,
-0.0650, 1.3850, 5.1000,
};

vector<float>body = {
-0.1300, 2.1900, 5.1000,
-0.2100, 2.1850, 5.1000,
-0.2700, 2.1850, 5.1000,
-0.3450, 2.1500, 5.1000,
-0.3750, 2.0800, 5.1000,
-0.3900, 1.9450, 5.1000,
-0.4300, 1.7800, 5.1000,
-0.4850, 1.6200, 5.1000,
-0.5450, 1.4600, 5.1000,
-0.6150, 1.2700, 5.1000,
-0.7000, 1.1500, 5.1000,
-0.7850, 1.0600, 5.1000,
-0.7450, 0.7400, 5.1000,
-0.7400, 0.7350, 5.1000,
-0.8100, 0.8850, 5.1000,
-0.5550, 0.5800, 5.1000,
-0.2050, 0.4800, 5.1000,
-0.1050, 0.4700, 5.1000,
-0.0350, 0.4650, 5.1000,
};
vector<float>leg = {
0.0650, 1.8500, 5.1000,
-0.0500, 1.8650, 5.1000,
-0.1900, 1.8450, 5.1000,
-0.2700, 1.8250, 5.1000,
-0.3050, 1.7350, 5.1000,
-0.3050, 1.5350, 5.1000,
-0.2700, 1.3400, 5.1000,
-0.2500, 1.2000, 5.1000,
-0.2950, 1.0750, 5.1000,
-0.2300, 1.0700, 5.1000,
-0.2550, 0.9300, 5.1000,
-0.3150, 0.8650, 5.1000,
-0.2600, 0.7550, 5.1000,
-0.2350, 0.6900, 5.1000,
-0.1950, 0.5450, 5.1000,
-0.1450, 0.4000, 5.1000,
-0.0750, 0.2950, 5.1000,
-0.0500, 0.1950, 5.1000,
-0.0300, 0.1350, 5.1000,
0.0150, 0.1100, 5.1000,
0.0550, 0.0900, 5.1000,
0.0850, 0.0650, 5.1000,
0.1150, 0.0400, 5.1000,
0.1500, 0.0200, 5.1000,
};








// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 800;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;

// camera
Camera camera(glm::vec3(0.0f, 1.1f, 5.2f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);


// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.0f,  15.5f,  -30.75f),
    glm::vec3(0.0f,  2.5f,  -15.5f),
    glm::vec3(100.0f,  100.0f,  100.0f),
    glm::vec3(-0.8f,  2.0f,  0.0f),
    glm::vec3(-0.8f,  2.5f,  4.5f),
    glm::vec3(-0.8f,  2.5f,  -5.5f),
    glm::vec3(-0.8f,  2.5f,  -25.5f),
    glm::vec3(-20.8f,  9.5f,  -43.7f),
    glm::vec3(4.2f,  9.5f,  -35.5f),




};

//specular LIght
PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.1f, 0.0f, 0.0f,     // ambient
    1, 0, 0,      // diffuse
    0.7f, 0.7f, 0.7f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);


//point_light
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.1f, 0.0f, 0.0f,     // ambient
    1.0f, 0.0f, 0.0f,      // diffuse
    0.7f, 0.0f, 0.0f,         // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);

//directional Light = 3
PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.4f, 0.4f, 0.4f,     // ambient
    0.7f, 0.7f, 0.7f,      // diffuse
    0.7f, 0.7f, 0.7f,         // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);


PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.07f, 0.07f, 0.07f,     // ambient
    0.7f, 0.7f, 0.7f,      // diffuse
    0.7f, 0.7f, 0.7f,         // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
);

PointLight pointlight5(

    pointLightPositions[4].x , pointLightPositions[4].y , pointLightPositions[4].z ,  // position
    0.0f, 0.1f, 0.0f,     // ambient
    0.0f, 1.0f, 0.0f,      // diffuse
    0.0f, 0.7f, 0.0f,         // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    5      // light number
);

PointLight pointlight6(

    pointLightPositions[5].x, pointLightPositions[5].y, pointLightPositions[5].z,  // position
    0.0f, 0.0f, 0.1f,     // ambient
    0.0f, 0.0f, 0.7f,      // diffuse
    0.f, 0.f, 0.7f,         // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    6      // light number
);


PointLight pointlight7(

    pointLightPositions[6].x, pointLightPositions[6].y, pointLightPositions[6].z,  // position
    0.1f, 0.1f, 0.1f,     // ambient
    0.9f, 0.9f, 0.9f,      // diffuse
    0.9f, 0.9f, 0.7f,         // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    7      // light number
);


PointLight pointlight8(

    pointLightPositions[7].x, pointLightPositions[7].y, pointLightPositions[7].z,  // position
    0.1f, 0.1f, 0.1f,     // ambient
    0.9f, 0.9f, 0.9f,      // diffuse
    0.9f, 0.9f, 0.7f,         // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    8      // light number
);


PointLight pointlight9(

    pointLightPositions[8].x, pointLightPositions[8].y, pointLightPositions[8].z,  // position
    0.1f, 0.1f, 0.1f,     // ambient
    0.9f, 0.9f, 0.9f,      // diffuse
    0.9f, 0.9f, 0.7f,         // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    9      // light number
);





// light settings
bool onOffPointToggle = true;
bool onOffSpotToggle = true;
bool onOffDirectToggle = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;

//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//glm::mat4 view = camera.GetViewMatrix();
glm::mat4 projection;
glm::mat4 view;

string diffuseMapPath;
string specularMapPath;
unsigned int diffMap;
unsigned int specMap;

//Cube cube;

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;
float rotateFan = 0;
float doorangle = 0;
float moveFish = 5, sign = 1;

float door_open = 0;
float door_open1 = 0;
float door_closed = 0;
float door_closed1 = 0;

Curve* fis, *tre , *lef , *bod , *legg;
Cube* tmp,*foam,*footpath,*fazlul, *clock1, *room_window, *gate_left, *gate_right, *fazlul_lekha, *sher, *pillow, *pillow1,*head,*relling, *head1, *floor1, *roof, *wall, *road1, *devider, *grass, * field, *door1, *field1, * field2;
Sphere2* sphere;


Sphere2 clock_bell = Sphere2();

int ind = 0;


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);


    const int parts = 50;
    const float pi = 3.1415926535;
    const float angle = pi * 2.0f / parts;
    float points[200000]{}, radius = 1.0f;

    points[ind++] = 0.0f;
    points[ind++] = 0.0f;
    points[ind++] = 0.0f;
    for (int i = 1; i <= parts; i++) {
        points[ind++] = radius * sin(angle * i);
        points[ind++] = -radius * cos(angle * i);
        points[ind++] = 0.0f;
    }

    for (float r = radius - 0.005f, z = 0.005f; r > 0.0f; r -= 0.005f, z += 0.005f)
    {
        for (int i = 1; i <= parts + 1; i++) {
            points[ind++] = (r + 0.005) * sin(angle * i);
            points[ind++] = -(r + 0.005) * cos(angle * i);
            points[ind++] = z - 0.005f;

            points[ind++] = r * sin(angle * i);
            points[ind++] = -r * cos(angle * i);
            points[ind++] = z;
        }
    }
    for (float r = radius - 0.005f, z = -0.005f; r > 0.0f; r -= 0.005f, z -= 0.005f)
    {
        for (int i = 1; i <= parts + 1; i++) {
            points[ind++] = (r + 0.005) * sin(angle * i);
            points[ind++] = -(r + 0.005) * cos(angle * i);
            points[ind++] = z + 0.005f;

            points[ind++] = r * sin(angle * i);
            points[ind++] = -r * cos(angle * i);
            points[ind++] = z;
        }
    }

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------







    //cyllinder

    float ver_arr[] = {

    1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.809017f, 1.0f, 0.587785f, 0.809017f, 1.0f, 0.587785f,
    0.309017f, 1.0f, 0.951057f, 0.309017f, 1.0f, 0.951057f,
    -0.309017f, 1.0f, 0.951057f, -0.309017f, 1.0f, 0.951057f,
    -0.809017f, 1.0f, 0.587785f, -0.809017f, 1.0f, 0.587785f,
    -1.0f, 1.0f, 1.22465e-16f, -1.0f, 1.0f, 1.22465e-16f,
    -0.809017f, 1.0f, -0.587785f, -0.809017f, 1.0f, -0.587785f,
    -0.309017f, 1.0f, -0.951057f, -0.309017f, 1.0f, -0.951057f,
    0.309017f, 1.0f, -0.951057f, 0.309017f, 1.0f, -0.951057f,
    0.809017f, 1.0f, -0.587785f, 0.809017f, 1.0f, -0.587785f,

    1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f,
    0.809017f, -1.0f, 0.587785f, 0.809017f, -1.0f, 0.587785f,
    0.309017f, -1.0f, 0.951057f, 0.309017f, -1.0f, 0.951057f,
    -0.309017f, -1.0f, 0.951057f, -0.309017f, -1.0f, 0.951057f,
    -0.809017f, -1.0f, 0.587785f, -0.809017f, -1.0f, 0.587785f,
    -1.0f, -1.0f, 1.22465e-16f, -1.0f, -1.0f, 1.22465e-16f,
    -0.809017f, -1.0f, -0.587785f, -0.809017f, -1.0f, -0.587785f,
    -0.309017f, -1.0f, -0.951057f, -0.309017f, -1.0f, -0.951057f,
    0.309017f, -1.0f, -0.951057f, 0.309017f, -1.0f, -0.951057f,
    0.809017f, -1.0f, -0.587785f, 0.809017f, -1.0f, -0.587785f,


    1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f,
    0.809017f, -1.0f, 0.587785f, 0.809017f, -1.0f, 0.587785f,
    0.309017f, -1.0f, 0.951057f, 0.309017f, -1.0f, 0.951057f,
    -0.309017f, -1.0f, 0.951057f, -0.309017f, -1.0f, 0.951057f,
    -0.809017f, -1.0f, 0.587785f, -0.809017f, -1.0f, 0.587785f,
    -1.0f, -1.0f, 1.22465e-16f, -1.0f, -1.0f, 1.22465e-16f,
    -0.809017f, -1.0f, -0.587785f, -0.809017f, -1.0f, -0.587785f,
    -0.309017f, -1.0f, -0.951057f, -0.309017f, -1.0f, -0.951057f,
    0.309017f, -1.0f, -0.951057f, 0.309017f, -1.0f, -0.951057f,
    0.809017f, -1.0f, -0.587785f, 0.809017f, -1.0f, -0.587785f,

    1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.809017f, 1.0f, 0.587785f, 0.809017f, 1.0f, 0.587785f,
    0.309017f, 1.0f, 0.951057f, 0.309017f, 1.0f, 0.951057f,
    -0.309017f, 1.0f, 0.951057f, -0.309017f, 1.0f, 0.951057f,
    -0.809017f, 1.0f, 0.587785f, -0.809017f, 1.0f, 0.587785f,
    -1.0f, 1.0f, 1.22465e-16f, -1.0f, 1.0f, 1.22465e-16f,
    -0.809017f, 1.0f, -0.587785f, -0.809017f, 1.0f, -0.587785f,
    -0.309017f, 1.0f, -0.951057f, -0.309017f, 1.0f, -0.951057f,
    0.309017f, 1.0f, -0.951057f, 0.309017f, 1.0f, -0.951057f,
    0.809017f, 1.0f, -0.587785f, 0.809017f, 1.0f, -0.587785f,


    0.0,-1.0,0.0, 0.0,-1.0,0.0,
    0.0,1.0,0.0, 0.0,1.0,0.0

    };

    unsigned int ind_arr[] = {
        0, 11, 1,
        11, 0, 10,
        1, 12, 2,
        12, 1, 11,
        2, 13, 3,
        13, 2, 12,
        3, 14, 4,
        14, 3, 13,
        4, 15, 5,
        15, 4, 14,
        5, 16, 6,
        16, 5, 15,
        6, 17, 7,
        17, 6, 16,
        7, 18, 8,
        18, 7, 17,
        8, 19, 9,
        19, 8, 18,
        9, 10, 0,
        10, 9, 19,

        40,20,21,
        40,21,22,
        40,22,23,
        40,23,24,
        40,24,25,
        40,25,26,
        40,26,27,
        40,27,28,
        40,28,29,
        40,29,20,

        41,30,31,
        41,31,32,
        41,32,33,
        41,33,34,
        41,34,35,
        41,35,36,
        41,36,37,
        41,37,38,
        41,38,39,
        41,39,30


        /*
        21,10,11,
        21,11,12,
        21,12,13,
        21,13,14,
        21,14,15,
        21,15,16,
        21,16,17,
        21,17,18,
        21,18,19,
        21,19,10*/

    };

    unsigned int cVAO, cVBO, cEBO;
    glGenVertexArrays(1, &cVAO);
    glGenBuffers(1, &cVBO);
    glGenBuffers(1, &cEBO);

    glBindVertexArray(cVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ver_arr), ver_arr, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind_arr), ind_arr, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    //end of cylingder














    float cube_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };

    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



    //ourShader.use();
    //lightingShader.use();

    // render loop
    // -----------

    diffuseMapPath = "images/wood.jpg";
    specularMapPath = "images/wood.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube1 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 2.0f, 1.0f);
    //cube1.drawCubeWithTexture(lightingShaderWithTexture, model);
    tmp = &cube1;



    diffuseMapPath = "images/foam.jpg";
    specularMapPath = "images/foam.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube2 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 2.0f, 1.0f);
    foam = &cube2;
    

    diffuseMapPath = "images/pillow.jpg";
    specularMapPath = "images/pillow.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube3 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 2.0f, 1.0f);
    pillow = &cube3;

    diffuseMapPath = "images/pillow2.jpg";
    specularMapPath = "images/pillow2.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube4 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 2.0f, 1.0f);
    pillow1 = &cube4;



    diffuseMapPath = "images/head1.jpg";
    specularMapPath = "images/head1.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube5 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 2.0f, 1.0f);
    head = &cube5;




    diffuseMapPath = "images/head1.jpg";
    specularMapPath = "images/head1.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube6 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 2.0f, 1.0f);
    //cube6.drawCubeWithTexture(lightingShaderWithTexture, model);
    head1 = &cube6;



    diffuseMapPath = "images/floor1.jpg";
    specularMapPath = "images/floor1.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube7 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 3.0f, 5.0f);
    floor1 = &cube7;


    diffuseMapPath = "images/steel.png";
    specularMapPath = "images/steel.png";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube20 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.f);
    relling = &cube20;



    diffuseMapPath = "images/roof.jpg";
    specularMapPath = "images/roof.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube8 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 6.0f, 1.0f);
    roof = &cube8;


    diffuseMapPath = "images/wall.png";
    specularMapPath = "images/wall.png";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube9 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    wall = &cube9;

    diffuseMapPath = "images/door.png";
    specularMapPath = "images/door.png";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube16 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    door1 = &cube16;



    diffuseMapPath = "images/road.jpg";
    specularMapPath = "images/road.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube10 = Cube(diffMap, specMap, 32, 0.0f, 0.0f, 3.0f, 4.0f);
    road1 = &cube10;


    diffuseMapPath = "images/brick.jpg";
    specularMapPath = "images/brick.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube11 = Cube(diffMap, specMap, 32, 0.0f, 0.0f, 1.0f, 1.0f);
    devider = &cube11;


    diffuseMapPath = "images/grass.png";
    specularMapPath = "images/grass.png";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube12 = Cube(diffMap, specMap, 32, 0.0f, 0.0f, 1.0f, 20.0f);
    grass = &cube12;


    


    Cube cube13 = Cube(diffMap, specMap, 32, 0.0f, 0.0f, 25.0f, 20.0f);
    field = &cube13;

    Cube cube14 = Cube(diffMap, specMap, 32, 0.0f, 0.0f, 3.0f, 20.0f);
    field1 = &cube14;


    Cube cube15 = Cube(diffMap, specMap, 32, 0.0f, 0.0f, 40.0f, 40.0f);
    field2 = &cube15;

    diffuseMapPath = "images/footpath.png";
    specularMapPath = "images/footpath.png";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube30 = Cube(diffMap, specMap, 32, 0.0f, 0.0f, 10.0f, 5.0f);
    footpath = &cube30;

    diffuseMapPath = "images/fazlul.png";
    specularMapPath = "images/fazlul.png";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube31 = Cube(diffMap, specMap, 32, 0.0f, 0.0f, 1.0f, 1.0f);
    fazlul = &cube31;


    //clock
    diffuseMapPath = "images/clock.png";
    specularMapPath = "images/clock.png";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube38 = Cube(diffMap, specMap, 32, 0.0f, 0.0f, 1.0f, 1.0f);
    clock1 = &cube38;

    //kata










    diffuseMapPath = "images/sher.png";
    specularMapPath = "images/sher.png";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube32 = Cube(diffMap, specMap, 32, 0.0f, 0.0f, 1.0f, 1.0f);
    sher = &cube32;


    diffuseMapPath = "images/fazlul_lekha.png";
    specularMapPath = "images/fazlul_lekha.png";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube33 = Cube(diffMap, specMap, 32, 0.0f, 0.0f, 1.0f, 1.0f);
    fazlul_lekha = &cube33;



    diffuseMapPath = "images/gate_left.png";
    specularMapPath = "images/gate_left.png";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube34 = Cube(diffMap, specMap, 32, 0.0f, 0.0f, 1.0f, 1.0f);
    gate_left = &cube34;


    diffuseMapPath = "images/gate_right.png";
    specularMapPath = "images/gate_right.png";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube35 = Cube(diffMap, specMap, 32, 0.0f, 0.0f, 1.0f, 1.0f);
    gate_right = &cube35;


    diffuseMapPath = "images/room_window.png";
    specularMapPath = "images/room_window.png";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube36 = Cube(diffMap, specMap, 32, 0.0f, 0.0f, 1.0f, 1.0f);
    room_window = &cube36;





    //sphere started
    diffuseMapPath = "images/bulb.png";
    specularMapPath = "images/bulb.png";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    Sphere2 sp(1.0, 36, 18, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, diffMap, specMap, 0, 1, 0, 1);

    sphere = &sp;


    //fish1VAO = hollowBezier(cntrlPoints1.data(), ((unsigned int)cntrlPoints1.size() / 3) - 1);
    

    Curve fish(Fish);
    fis = &fish;
    Curve tree(Tree);
    tre = &tree;

    Curve Leaf(leaf);
    lef = &Leaf;

    Curve Body(body);
    bod = &Body;

    Curve Leg(leg);
    legg = &Leg;


    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.529, 0.808, 0.922, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
        pointlight4.setUpPointLight(lightingShader);

        pointlight5.setUpPointLight(lightingShader);

        pointlight6.setUpPointLight(lightingShader);

        pointlight7.setUpPointLight(lightingShader);
        pointlight8.setUpPointLight(lightingShader);
        pointlight9.setUpPointLight(lightingShader);

        
        
        // activate shader
        lightingShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        // glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        // glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        view = camera.GetViewMatrix();
        lightingShader.setMat4("view", view);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model,translate2;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix ;


        
        lightingShader.setMat4("model", model);
        //road(cubeVAO, lightingShader, model, lightingShaderWithTexture);
        //fis->draw(lightingShader, model);
        FazlulHaqueHall(cubeVAO,cVAO, lightingShader, model, lightingShaderWithTexture);
        //model = glm::mat4(1.0f);
        //translate = glm::mat4(1.0f);
        //translate2 = glm::mat4(1.0f);
        //scale = glm::mat4(1.0f);
        /*scale = glm::scale(model, glm::vec3(3, 3, 3));
        translate = glm::translate(model, glm::vec3(0.0, 4.08, 0));
        glm::mat4 rev = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(-270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = alTogether * translate * scale;*/
        //drawFish(lightingShader, model);
        //pond(cubeVAO, cVAO, lightingShader, model, lightingShaderWithTexture);
        


        
        

        /*building(cubeVAO, lightingShader, model);
        road(cubeVAO, lightingShader, model);*/


        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);
        //for (unsigned int i = 0; i < 5; i++)
        //{
        //    model = glm::mat4(1.0f);
        //    model = glm::translate(model, pointLightPositions[i]);
        //    model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        //    ourShader.setMat4("model", model);
        //    ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
        //    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //    //glDrawArrays(GL_TRIANGLES, 0, 36);
        //}



        //red
        model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[0]);
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        ourShader.setMat4("model", model);
        ourShader.setVec3("color", glm::vec3(0.8f, 0.0f, 0.0f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


        //white
        model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[2]);
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        ourShader.setMat4("model", model);
        ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


        //red
        model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[1]);
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        ourShader.setMat4("model", model);
        ourShader.setVec3("color", glm::vec3(0.8f, 0.0, 0.0));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


        //green
        model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[4]);
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        ourShader.setMat4("model", model);
        ourShader.setVec3("color", glm::vec3(0.0f, 0.8f, 0.0f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //Blue
        model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[5]);
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        ourShader.setMat4("model", model);
        ourShader.setVec3("color", glm::vec3(0.0f, 0.0f, 1.0f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


        //white
        model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[6]);
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        ourShader.setMat4("model", model);
        ourShader.setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[7]);
        model = glm::scale(model, glm::vec3(0.5f)); // Make it a smaller cube
        ourShader.setMat4("model", model);
        ourShader.setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


        model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[8]);
        model = glm::scale(model, glm::vec3(0.5f)); // Make it a smaller cube
        ourShader.setMat4("model", model);
        ourShader.setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);






        

        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X+1, translate_Y+1, translate_Z + 1));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.5, scale_Y * 0.5, scale_Z * 0.5));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);
        lightingShaderWithTexture.setMat4("view", view);
        lightingShaderWithTexture.setMat4("projection", projection);

        lightingShaderWithTexture.use();
        // point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        // point light 4
        pointlight4.setUpPointLight(lightingShaderWithTexture);

        pointlight5.setUpPointLight(lightingShaderWithTexture);

        pointlight6.setUpPointLight(lightingShaderWithTexture);
        pointlight7.setUpPointLight(lightingShaderWithTexture);
        pointlight8.setUpPointLight(lightingShaderWithTexture);
        pointlight9.setUpPointLight(lightingShaderWithTexture);



      


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}




float angle = 0;
float angle2 = 0;
float var = 0;
void pond(unsigned int& cubeVAO, unsigned int& cVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture) {
    float pondh = 0.02;
    float pondl = 70;
    float pondw = 30;
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(pondw, pondh, pondl));
    translate = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
    model = alTogether * translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);
    //, 0.0f, 0.5f, 0.7f
    float fishlength = 3;
    float fishwidth = 2;
    float fishheight = 0.01;

    moveFish += sign * 1.5;
    if (moveFish >= 60)
    {
        sign *= -1;
        angle += 180;
    }
    if (moveFish <= 5)
    {
        sign *= -1;
        angle += 180;
    }
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(0.8, 1.5, 1.5));
    translate = glm::translate(model, glm::vec3(10.0, 0.08, moveFish));
    glm::mat4 rev = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(-270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = alTogether * translate * rot * rev * scale;
    //
    // drawFish(lightingShader, model);

    angle2 += 10;
    if (var > 1)
        var -= 0.01;
    else
        var += 0.01;
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(0.8, 1.5, 1.5));
    translate = glm::translate(model, glm::vec3(20.0, 0.00 + var, 30));
    rot = glm::rotate(glm::mat4(1.0f), glm::radians(angle2), glm::vec3(1.0f, 0.0f, 0.0f));
    model = alTogether * translate * rot * scale;
    //drawFish(lightingShader, model);

    // Boarder
    float bl = 30;
    float bl2 = 70;
    float bw = 1;
    float bh = 1;
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(bl, bw, bh));
    translate = glm::translate(model, glm::vec3(0, 0, 0 - bh));
    model = alTogether * translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);


    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(bh, bw, bl2 + bh));
    translate = glm::translate(model, glm::vec3(bl, 0, 0 - bh));
    model = alTogether * translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);

    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(bl + bw, bw, bh));
    translate = glm::translate(model, glm::vec3(0, 0, bl2));
    model = alTogether * translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);


    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(bh, bw, bl2 + bh));
    translate = glm::translate(model, glm::vec3(0, 0, 0 - bh));
    model = alTogether * translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);
}



void shaderActivate(Shader& shader)
{
    shader.use();
    shader.setVec3("viewPos", camera.Position);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
}



void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();

    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", 32.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}



void FazlulHaqueHall(unsigned int& cubeVAO,unsigned int&cVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture)
{
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 translate3 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    glm::mat4 scale2 = glm::mat4(1.0f);
    glm::mat4 rotate = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(1, 1, 1));
    translate = glm::translate(model, glm::vec3(-0.5, -2, -12));
    model = alTogether * scale * translate;


    road(cubeVAO, lightingShader, model, lightingShaderWithTexture);

    translate2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.5, 0, 0));
    model = alTogether * translate2* scale * translate;

    Tree_Making(cubeVAO, cVAO, lightingShader, model, lightingShaderWithTexture);


    
    
    /*if (moveFish >= 20)
    {
        sign *= -1;
        angle += 180;
    }
    if (walk <= 5)
    {
        sign *= 1;
        angle += 180;
    }*/

   /* if (walk > 20)
        walk -= 0.5;*/

    if (walk >= 20) {
        sign1 = 0;
    }
    else if (walk <= -20) {
        sign1 = 1;
    }
    if (sign1 == 1) {
        walk += .3;
    }
    else
        walk -= .3;




    //cout << walk << endl;

    translate2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.5, -0.8, walk));
    model = alTogether * translate2 * scale * translate;

    Man_Making(cubeVAO, cVAO, lightingShader, model, lightingShaderWithTexture);
    
    for (int i = -3; i < 3; i++) {

        translate2 = glm::translate(glm::mat4(1.0f), glm::vec3(-i*20 + 0.5, 0, 18));
        model = alTogether * translate2 * scale * translate;

        Tree_Making(cubeVAO, cVAO, lightingShader, model, lightingShaderWithTexture);
    }


    for (int i = 0; i < 3; i++) {

        translate2 = glm::translate(glm::mat4(1.0f), glm::vec3(i * 20 + 0.5, 0, -45));
        model = alTogether * translate2 * scale * translate;

        Tree_Making(cubeVAO, cVAO, lightingShader, model, lightingShaderWithTexture);
    }

    
    
    
    for (int i = -25; i < 15; i+=10) {
        model = glm::mat4(1.0f);
        translate = glm::mat4(1.0f);
        translate2 = glm::mat4(1.0f);
        translate3 = glm::mat4(1.0f);
        scale = glm::mat4(1.0f);
        translate2 = glm::translate(model, glm::vec3(0, 0, i));

        //scale = glm::scale(model, glm::vec3(0.1, 3, 0.1));
        translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
        model = alTogether * scale *translate* translate2;




        piller(cubeVAO, cVAO, lightingShader, model, lightingShaderWithTexture);


        translate3 = glm::translate(model, glm::vec3(-0.08, 2.8, -0.02));

        scale = glm::scale(model, glm::vec3(0.3, 0.3, 0.3));
        ////translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
        rotate = glm::rotate(rotate, glm::radians(2.0f), glm::vec3(0, 0, 1));
        model =   translate3* rotate * glm::mat4(1.0f);
        sphere->drawSphereWithTexture(lightingShaderWithTexture, model);

        model = glm::mat4(1.0f);
        translate = glm::mat4(1.0f);
        translate2 = glm::mat4(1.0f);
        scale = glm::mat4(1.0f);
        scale = glm::scale(model, glm::vec3(0.8, 1.5, 1.5));
        translate = glm::translate(model, glm::vec3(10.0, 0.08, 0));
        glm::mat4 rev = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(-270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = alTogether * translate * rot * rev * scale;
        //drawFish(lightingShader, model);

     }
}


void Tree_Making(unsigned int& cubeVAO, unsigned int& cVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture)
{
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 translate3 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    glm::mat4 scale2 = glm::mat4(1.0f);
    glm::mat4 rotate = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(1, 4, 1));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * scale * translate;

    tre->draw(lightingShader, model);

    rotate = glm::mat4(1.0f);


    translate2 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.3, 5, -0.2));
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(0.7, 1.5, 0.6));

    rotate = glm::rotate(rotate, glm::radians(45.0f), glm::vec3(0, 0, 1));
    model = alTogether *  translate2 * rotate * scale * translate  * glm::mat4(1.0f);
    tre->draw(lightingShader, model);


    rotate = glm::mat4(1.0f);


    translate2 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.2, 6, -0.2));
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(0.6, 1.2, 0.5));

    rotate = glm::rotate(rotate, glm::radians(-45.0f), glm::vec3(0, 0, 1));
    model = alTogether * translate2 * rotate * scale * translate * glm::mat4(1.0f);
    tre->draw(lightingShader, model);

    rotate = glm::mat4(1.0f);


    translate2 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.3, 7, -0.2));
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(0.5, 1, 0.4));

    rotate = glm::rotate(rotate, glm::radians(45.0f), glm::vec3(0, 0, 1));
    model = alTogether * translate2 * rotate * scale * translate * glm::mat4(1.0f);
    tre->draw(lightingShader, model);

    rotate = glm::mat4(1.0f);


    translate2 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.2, 7.5, -0.2));
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(0.4, 0.8, 0.3));

    rotate = glm::rotate(rotate, glm::radians(-45.0f), glm::vec3(0, 0, 1));
    model = alTogether * translate2 * rotate * scale * translate * glm::mat4(1.0f);
    tre->draw(lightingShader, model);


    rotate = glm::mat4(1.0f);


    translate2 = glm::translate(glm::mat4(1.0f), glm::vec3(3.6, -1, 1.3));
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(8, 6, 4));

    rotate = glm::rotate(rotate, glm::radians(-45.0f), glm::vec3(0, 0, 1));
    model = alTogether * translate2 * scale * translate * glm::mat4(1.0f);
    lef->draw(lightingShader, model);


}

void Man_Making(unsigned int& cubeVAO, unsigned int& cVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture)
{
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 translate3 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    glm::mat4 scale2 = glm::mat4(1.0f);
    glm::mat4 rotate = glm::mat4(1.0f);
    glm::mat4 rotatez = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(1, 1, 1));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    translate2 = glm::translate(model, glm::vec3(-4, 2, 0));

    model = alTogether * scale * translate * translate2;
    bod->draw(lightingShader, model, 0.3, 0.3, 0.5);

    model = glm::mat4(1.0f);

    scale2 = glm::scale(model, glm::vec3(0.7, -1, 0.7));

    translate2 = glm::translate(model, glm::vec3(-4 - 0.2, 1+2 - 0.2, 0));
    rotate = glm::rotate(rotate, glm::radians(-10.0f), glm::vec3(0, 0, 1));
    rotatez = glm::rotate(rotate, glm::radians(rotateClock), glm::vec3(1, 0, 0));

    model = alTogether * scale * translate *  translate2 * rotatez * rotate* scale2 ;
    legg->draw(lightingShader, model, 0.9, 0.8, 0.5);



   

    rotate = glm::mat4(1.0f);

    model = glm::mat4(1.0f);

    scale2 = glm::scale(model, glm::vec3(0.7, -1, 0.7));

    translate2 = glm::translate(model, glm::vec3(-4 + 0.2, 1 + 2 - 0.2, 0));
    rotate = glm::rotate(rotate, glm::radians(10.0f), glm::vec3(0, 0, 1));
    rotatez = glm::rotate(rotate, glm::radians(-rotateClock), glm::vec3(1, 0, 0));

    model = alTogether * scale * translate * translate2 * rotatez * rotate * scale2;
    legg->draw(lightingShader, model, 0.9, 0.8, 0.5);







    //hand
    model = glm::mat4(1.0f);
    rotate = glm::mat4(1.0f);

    scale2 = glm::scale(model, glm::vec3(0.7, -0.7, 0.7));

    translate2 = glm::translate(model, glm::vec3(-4 + 0.2, 4, 0));
    rotate = glm::rotate(rotate, glm::radians(15.0f), glm::vec3(0, 0, 1));
    rotatez = glm::rotate(rotate, glm::radians(rotateClock), glm::vec3(1, 0, 0));

    model = alTogether * scale * translate * translate2 * rotatez * rotate * scale2;
    //model = alTogether * scale * translate * translate2 * rotate * scale2;
    legg->draw(lightingShader, model, 0.9, 0.8, 0.5);

    model = glm::mat4(1.0f);
    rotate = glm::mat4(1.0f);

    scale2 = glm::scale(model, glm::vec3(0.7, -0.7, 0.7));

    translate2 = glm::translate(model, glm::vec3(-4 - 0.2, 4, 0));
    rotate = glm::rotate(rotate, glm::radians(-15.0f), glm::vec3(0, 0, 1));
    rotatez = glm::rotate(rotate, glm::radians(-rotateClock), glm::vec3(1, 0, 0));

    model = alTogether * scale * translate * translate2 * rotatez * rotate * scale2;
    //model = alTogether * scale * translate * translate2 * rotate * scale2;
    legg->draw(lightingShader, model, 0.9, 0.8, 0.5);

    /*model = glm::mat4(1.0f);
    rotate = glm::mat4(1.0f);

    scale2 = glm::scale(model, glm::vec3(0.7, 0.7, 0.7));

    translate2 = glm::translate(model, glm::vec3(-4 - 1.2, 2.8, 0));
    rotate = glm::rotate(rotate, glm::radians(-35.0f), glm::vec3(0, 0, 1));


    model = alTogether * scale * translate * translate2 * rotate * scale2;
    legg->draw(lightingShader, model, 0.9, 0.8, 0.5);*/



    model = glm::mat4(1.0f);
    rotate = glm::mat4(1.0f);

    scale2 = glm::scale(model, glm::vec3(0.45, 0.45, 0.45));

    translate2 = glm::translate(model, glm::vec3(-4 , 4.8, 0));
    rotate = glm::rotate(rotate, glm::radians(-35.0f), glm::vec3(0, 0, 1));

    model = alTogether * scale * translate * translate2 * rotate * scale2;
    sphere->drawSphereWithTexture(lightingShaderWithTexture, model);


}

void piller(unsigned int& cubeVAO, unsigned int& cVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture)
{
    float width = 0.2;
    float height = 3;


    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, height, width));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * scale * translate;
    shaderActivate(lightingShader);
    //drawCubedrawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);
    drawCylinder(cVAO, lightingShader, model, 0 / 255.0, 84 / 255.0, 147 / 255.0);

}




// ********time*********
void getCurrentTime(int& hours, int& minutes, int& seconds) {
    time_t currentTime = time(nullptr); // Get current UNIX timestamp
    struct tm* timeinfo;
    timeinfo = localtime(&currentTime);

    seconds = timeinfo->tm_sec;
    minutes = timeinfo->tm_min;
    hours = timeinfo->tm_hour;
}



void Clock(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture)
{

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    glm::mat4 rotateYMatrix = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));


    // clock
    shaderActivate(lightingShaderWithTexture);
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(0.1, 0.3, 0.3));
    translate = glm::translate(model, glm::vec3(-1.35f, 1.1f + 0.13f - 0.03 - 0.03, -3.15f + 0.03f - 0.02 - 0.05));
    model = alTogether * translate * scale;
    /*ourShader.setMat4("model", model);
    ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    */
    //shaderActivate(lightingShader);
    clock1->drawCubeWithTexture(lightingShaderWithTexture, model);

    //shaderActivate(ourShader);
    if (rotateClock >= 40) {
        sign = 0;
    }
    else if (rotateClock <= -40) {
        sign = 1;
    }
    if (sign == 1) {
        rotateClock += 5;
    }
    else
        rotateClock -= 5;

    glm::mat4 translate_origin = glm::mat4(1.0f);
    glm::mat4 translate4 = glm::mat4(1.0f);
    model = glm::mat4(1.0f);
    translate_origin = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
    scale = glm::scale(model, glm::vec3(0.02, -0.3, 0.03));
    translate = glm::translate(model, glm::vec3(-1.35f, 0.83f + 0.3f + 0.1, -3.04f));
    glm::mat4 rotateM = glm::rotate(model, glm::radians(0.0f + rotateClock), glm::vec3(1.0f, 0.0f, 0.0f));
    model = alTogether * translate * rotateM * translate_origin * scale;
    
   
    drawCube(cubeVAO, lightingShader, model, 1, 0.4, 0.06);


    shaderActivate(lightingShaderWithTexture);
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(0.02, 0.04, 0.04));
    glm::mat4 rotateb = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    translate = glm::translate(model, glm::vec3(-1.35f, 0.8f + 0.3f + 0.1, -3.03f));
    translate4 = glm::translate(model, glm::vec3(0, 0.3, 0));
    model = alTogether * translate * rotateM * rotateb * translate4 * translate_origin * scale;
    sphere->drawSphere(lightingShaderWithTexture, model);
    shaderActivate(lightingShader);

    int hours, minutes, seconds;
    getCurrentTime(hours, minutes, seconds);
    hours = (hours + 6) % 12;


    // second
    shaderActivate(lightingShaderWithTexture);
    model = glm::mat4(1.0f);
    translate_origin = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
    scale = glm::scale(model, glm::vec3(0.02, -0.1, 0.001));
    translate = glm::translate(model, glm::vec3(-1.25f, 0.83f + 0.3f + 0.2f, -3.04f));
    glm::mat4 rotateSecond = glm::rotate(model, glm::radians(-(seconds - 30) * 6.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    translate4 = glm::translate(model, glm::vec3(-1.25f, 0.83f + 0.3f + 0.2f, -3.04f));
    model = alTogether * translate * rotateSecond * translate_origin * scale;
    //model = alTogether * translate * translate_origin * rotateM * translate_origin * scale;
    /*ourShader.setMat4("model", model);
    ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/
    drawCube(cubeVAO, lightingShader, model, 0.5, 0.5, 0.5);


    // minute
    model = glm::mat4(1.0f);
    translate_origin = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
    scale = glm::scale(model, glm::vec3(0.02, -0.08, 0.01));
    translate = glm::translate(model, glm::vec3(-1.25f, 0.83f + 0.3f, -3.04f));
    translate2 = glm::translate(model, glm::vec3(0.0f, 0.2f, 0.0f));
    glm::mat4 rotateMinute = glm::rotate(model, glm::radians(-(minutes * 60 + seconds - 30) * 0.1f + 180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = alTogether * translate2 * translate * rotateMinute * translate_origin * scale;
    //model = alTogether * translate * translate_origin * rotateM * translate_origin * scale;
    /*ourShader.setMat4("model", model);
    ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/
    drawCube(cubeVAO, lightingShader, model, 0.5, 0.5, 0.5);

    // hour
    model = glm::mat4(1.0f);
    translate_origin = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
    scale = glm::scale(model, glm::vec3(0.02, -0.06, 0.01));
    translate = glm::translate(model, glm::vec3(-1.25f, 0.83f + 0.3f, -3.04f));
    translate2 = glm::translate(model, glm::vec3(0.0f, 0.2f, 0.0f));
    glm::mat4 rotateHour = glm::rotate(model, glm::radians(-(hours * 3600 + minutes * 60 + seconds - 30) * (1.0f / 120.0f)), glm::vec3(1.0f, 0.0f, 0.0f));
    model = alTogether * translate2 * translate * rotateHour * translate_origin * scale;
    //model = alTogether * translate * translate_origin * rotateM * translate_origin * scale;
    /*ourShader.setMat4("model", model);
    ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/
    drawCube(cubeVAO, lightingShader, model, 0.5, 0.5, 0.5);
}







void road(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture)
{
    float road_width = 11;
    float road_length = 40;
    float road_height = 0.3;

    glm::mat4 rotate = glm::mat4(1.0f);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    //glm::mat4 rotate = glm::mat4(1.0f);
    rotate = glm::mat4(1.0f);

    scale = glm::scale(model, glm::vec3(road_width, road_height, road_length));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * scale * translate;
    shaderActivate(lightingShaderWithTexture);
    //drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);
    road1->drawCubeWithTexture(lightingShaderWithTexture, model);

    //devider
    for (int i = (-road_length / 2); i < (road_length / 2)-5; i++) {
        model = glm::mat4(1.0f);
        translate = glm::mat4(1.0f);
        translate2 = glm::mat4(1.0f);
        scale = glm::mat4(1.0f);
        translate2 = glm::translate(model, glm::vec3(-1, road_height, i+5));
        scale = glm::scale(model, glm::vec3(0.5, 0.5, 1));
        translate = glm::translate(model, glm::vec3(-0.5, 0, -0));
        model = alTogether * translate2 * scale * translate;
        shaderActivate(lightingShader);
        //drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);
        devider->drawCubeWithTexture(lightingShaderWithTexture, model);
    }

    for (int i = (-road_length / 2); i < (road_length / 2)-1 - 5; i++) {
        model = glm::mat4(1.0f);
        translate = glm::mat4(1.0f);
        translate2 = glm::mat4(1.0f);
        scale = glm::mat4(1.0f);
        translate2 = glm::translate(model, glm::vec3(1, road_height, i+5));
        scale = glm::scale(model, glm::vec3(0.5, 0.5, 1));
        translate = glm::translate(model, glm::vec3(-0.5, 0, 0));
        model = alTogether * translate2 * scale * translate;
        shaderActivate(lightingShader);
        //drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);
        devider->drawCubeWithTexture(lightingShaderWithTexture, model);
    }

    for (int i = -road_length / 2; i < road_length / 2; i++) {
        model = glm::mat4(1.0f);
        translate = glm::mat4(1.0f);
        translate2 = glm::mat4(1.0f);
        scale = glm::mat4(1.0f);
        translate2 = glm::translate(model, glm::vec3(road_width/2, 0, i));
        scale = glm::scale(model, glm::vec3(0.5, road_height + 0.5, 1));
        translate = glm::translate(model, glm::vec3(-0, 0, -0));
        model = alTogether * translate2 * scale * translate;
        shaderActivate(lightingShader);
        //drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);
        devider->drawCubeWithTexture(lightingShaderWithTexture, model);
    }

    for (int i = -road_length / 2; i < road_length / 2; i++) {
        model = glm::mat4(1.0f);
        translate = glm::mat4(1.0f);
        translate2 = glm::mat4(1.0f);
        scale = glm::mat4(1.0f);
        translate2 = glm::translate(model, glm::vec3(-road_width / 2, 0, i));
        scale = glm::scale(model, glm::vec3(0.5, road_height + 0.5, 1));
        translate = glm::translate(model, glm::vec3(-1, 0, 0));
        model = alTogether * translate2 * scale * translate;
        shaderActivate(lightingShader);
        //drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);
        devider->drawCubeWithTexture(lightingShaderWithTexture, model);
    }



    for (float i = -1; i <=  1; i+=0.5) {
        model = glm::mat4(1.0f);
        translate = glm::mat4(1.0f);
        translate2 = glm::mat4(1.0f);
        scale = glm::mat4(1.0f);
        translate2 = glm::translate(model, glm::vec3(i, road_height, (road_length / 2) - 1));
        scale = glm::scale(model, glm::vec3(0.5, 0.5, 1));
        translate = glm::translate(model, glm::vec3(-0.5, 0, 0));
        model = alTogether * translate2 * scale * translate;
        shaderActivate(lightingShader);
        //drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);
        devider->drawCubeWithTexture(lightingShaderWithTexture, model);
    }


    for (float i = -1; i <= 1; i += 0.5) {
        model = glm::mat4(1.0f);
        translate = glm::mat4(1.0f);
        translate2 = glm::mat4(1.0f);
        scale = glm::mat4(1.0f);
        translate2 = glm::translate(model, glm::vec3(i, road_height, -(road_length / 2) + 4));
        scale = glm::scale(model, glm::vec3(0.5, 0.5, 1));
        translate = glm::translate(model, glm::vec3(-0.5, 0, 0));
        model = alTogether * translate2 * scale * translate;
        shaderActivate(lightingShader);
        //drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);
        devider->drawCubeWithTexture(lightingShaderWithTexture, model);
    }

    

    



    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(0, road_height,5/2 ));
    scale = glm::scale(model, glm::vec3(1.5, 0.3, road_length-1-5));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    shaderActivate(lightingShader);
    //drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);
    grass->drawCubeWithTexture(lightingShaderWithTexture, model);




    //field
    float field_width = 40;

    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(road_width/2 + field_width/2 +0.5 , 0, 0));
    scale = glm::scale(model, glm::vec3(field_width, road_height + 0.5, road_length-2));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    shaderActivate(lightingShader);
    //drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);
    field->drawCubeWithTexture(lightingShaderWithTexture, model);



    //fieldInside

    float field_width1 = 200;
    float height = 5;
    float field_length = 200;

    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(30, 0,-field_length/2 +road_length/2 ));
    scale = glm::scale(model, glm::vec3(field_width1, -height ,field_length ));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    shaderActivate(lightingShader);
    //drawCube(cubeVAO, lightingShader, model, 0.6, 0.4, 0.2);
    field2->drawCubeWithTexture(lightingShaderWithTexture, model);



    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(-(road_width / 2 + 2 + 0.5), 0, 0));
    scale = glm::scale(model, glm::vec3(4, road_height + 0.5, road_length-2));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    shaderActivate(lightingShader);
    //drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);
    field1->drawCubeWithTexture(lightingShaderWithTexture, model);



    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(-4-6, 0, -road_length/2 - 6));
    scale = glm::scale(model, glm::vec3(30, 0.8, 12));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    shaderActivate(lightingShader);
    //drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);
    footpath->drawCubeWithTexture(lightingShaderWithTexture, model);


    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(-17, 4, -road_length / 2 - 12 + 0.3));
    scale = glm::scale(model, glm::vec3(2.4, 3, 0.1));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    shaderActivate(lightingShader);
    //drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);
    fazlul->drawCubeWithTexture(lightingShaderWithTexture, model);


    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(-17, 3, -road_length / 2 - 12 + 0.3));
    scale = glm::scale(model, glm::vec3(2.4, 0.8, 0.1));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    shaderActivate(lightingShader);
    //drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);
    sher->drawCubeWithTexture(lightingShaderWithTexture, model);










    


    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(-6-door_open + door_closed, 0, -road_length / 2 - 12 + 0.3));
    scale = glm::scale(model, glm::vec3(8, 8, 0.2));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    shaderActivate(lightingShader);
    //drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);
    gate_left->drawCubeWithTexture(lightingShaderWithTexture, model);



    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(-60 , 2, -road_length / 2 - 9));
    scale = glm::scale(model, glm::vec3(1, 5, 5));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    rotate = glm::rotate(rotate, glm::radians(270.0f), glm::vec3(0, 1, 0));
    glm::mat4 scale2 = glm::mat4(1.0f);
    model = alTogether  * scale * translate ;
    scale2 = glm::scale(model, glm::vec3(1, 2, 2));

    model = alTogether * translate2 * rotate*scale2;
    shaderActivate(lightingShader);
    Clock(cubeVAO, lightingShader, model, lightingShaderWithTexture);







    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(+2, 0, -road_length / 2 - 12 + 0.3));
    scale = glm::scale(model, glm::vec3(8, 8, 0.2));
    translate = glm::translate(model, glm::vec3(-0.5+door_open1 + door_closed1, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    shaderActivate(lightingShader);
    //drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);
    gate_right->drawCubeWithTexture(lightingShaderWithTexture, model);


    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(-3, 8, -road_length / 2 - 12 + 0.3));
    scale = glm::scale(model, glm::vec3(16, 4, 0.2));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    shaderActivate(lightingShader);
    //drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);
    fazlul_lekha->drawCubeWithTexture(lightingShaderWithTexture, model);
    
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(-3, 8, -road_length / 2 - 12 ));
    scale = glm::scale(model, glm::vec3(16, 4, 0.2));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    shaderActivate(lightingShader);
    wall->drawCubeWithTexture(lightingShaderWithTexture, model);

    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(-3, 8, -road_length / 2 - 12 - 6));
    scale = glm::scale(model, glm::vec3(16, 6, 0.6));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    rotate = glm::rotate(rotate, glm::radians(90.0f), glm::vec3(1, 0, 0));

    model = alTogether * translate2 * rotate* scale * translate;
    shaderActivate(lightingShader);
    wall->drawCubeWithTexture(lightingShaderWithTexture, model);



    for (float i = 0; i <= field_width; i += 0.5) {
        model = glm::mat4(1.0f);
        translate = glm::mat4(1.0f);
        translate2 = glm::mat4(1.0f);
        scale = glm::mat4(1.0f);
        translate2 = glm::translate(model, glm::vec3(i+road_width/2+0.5, 0, (road_length / 2) - 1));
        scale = glm::scale(model, glm::vec3(0.5, road_height+0.5, 1));
        translate = glm::translate(model, glm::vec3(-0.5, 0, 0));
        model = alTogether * translate2 * scale * translate;
        shaderActivate(lightingShader);
        //drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);
        devider->drawCubeWithTexture(lightingShaderWithTexture, model);
    }


    for (float i = 0; i <= 4; i += 0.5) {
        model = glm::mat4(1.0f);
        translate = glm::mat4(1.0f);
        translate2 = glm::mat4(1.0f);
        scale = glm::mat4(1.0f);
        translate2 = glm::translate(model, glm::vec3(-(i + road_width / 2 + 0.5), 0, (road_length / 2) - 1));
        scale = glm::scale(model, glm::vec3(0.5, road_height + 0.5, 1));
        translate = glm::translate(model, glm::vec3(-0.5, 0, 0));
        model = alTogether * translate2 * scale * translate;
        shaderActivate(lightingShader);
        //drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);
        devider->drawCubeWithTexture(lightingShaderWithTexture, model);
    }

    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    rotate = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(1, 1, 1));

    rotate = glm::rotate(rotate, glm::radians(180.0f), glm::vec3(0, 1, 0));
    //model = rotate * ;
    for (int i = 0; i < 7; i++) {
        
        translate2 = glm::translate(model, glm::vec3((road_width / 2) + 4 + i*8, 0, -(road_length / 2) - 6));
        //translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
        
        model = alTogether * translate2 * rotate * scale;
        shaderActivate(lightingShader);
        //drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);
        //room_structure->drawCubeWithTexture(lightingShaderWithTexture, model);
        room_structure(cubeVAO, lightingShader, model, lightingShaderWithTexture);
        model = glm::mat4(1.0f);
    }

    for (int i = 0; i < 7; i++) {

        translate2 = glm::translate(model, glm::vec3((road_width / 2) + 4 + i * 8, 8.2, -(road_length / 2) - 6));
        //translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));

        model = alTogether * translate2 * rotate * scale;
        shaderActivate(lightingShader);
        //drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);
        //room_structure->drawCubeWithTexture(lightingShaderWithTexture, model);
        room_structure(cubeVAO, lightingShader, model, lightingShaderWithTexture);
        model = glm::mat4(1.0f);
    }




    rotate = glm::rotate(rotate, glm::radians(90.0f), glm::vec3(0, 1, 0));

    for (int i = 1; i < 5; i++) {

        translate2 = glm::translate(model, glm::vec3((road_width / 2) + 6 + 7 * 8, 0, -(road_length / 2) - 8 - i*8));
        //translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));

        model = alTogether * translate2 * rotate * scale;
        shaderActivate(lightingShader);
        //drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);
        //room_structure->drawCubeWithTexture(lightingShaderWithTexture, model);
        room_structure(cubeVAO, lightingShader, model, lightingShaderWithTexture);
        model = glm::mat4(1.0f);
    }

    for (int i = 1; i < 5; i++) {

        translate2 = glm::translate(model, glm::vec3((road_width / 2) + 6 + 7 * 8, 8.1, -(road_length / 2) - 8 - i * 8));
        //translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));

        model = alTogether * translate2 * rotate * scale;
        shaderActivate(lightingShader);
        //drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);
        //room_structure->drawCubeWithTexture(lightingShaderWithTexture, model);
        room_structure(cubeVAO, lightingShader, model, lightingShaderWithTexture);
        model = glm::mat4(1.0f);
    }



    rotate = glm::rotate(rotate, glm::radians(90.0f), glm::vec3(0, 1, 0));

    for (int i = 1; i < 10; i++) {

        translate2 = glm::translate(model, glm::vec3((road_width / 2) + 4 + 7 * 8 - i*8, 0, -(road_length / 2) - 10- 5 * 8));
        //translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));

        model = alTogether * translate2 * rotate * scale;
        shaderActivate(lightingShader);
        //drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);
        //room_structure->drawCubeWithTexture(lightingShaderWithTexture, model);
        room_structure(cubeVAO, lightingShader, model, lightingShaderWithTexture);
        model = glm::mat4(1.0f);
    }

    for (int i = 1; i < 10; i++) {

        translate2 = glm::translate(model, glm::vec3((road_width / 2) + 4 + 7 * 8 - i * 8, 8, -(road_length / 2) - 10 - 5 * 8));
        //translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));

        model = alTogether * translate2 * rotate * scale;
        shaderActivate(lightingShader);
        //drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);
        //room_structure->drawCubeWithTexture(lightingShaderWithTexture, model);
        room_structure(cubeVAO, lightingShader, model, lightingShaderWithTexture);
        model = glm::mat4(1.0f);
    }
  



    rotate = glm::rotate(rotate, glm::radians(90.0f), glm::vec3(0, 1, 0));

    for (int i = 1; i < 6; i++) {

        translate2 = glm::translate(model, glm::vec3((road_width / 2) + 6 - 2 * 8 - 12, 0, -(road_length / 2) - 8 - i * 8));
        //translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));

        model = alTogether * translate2 * rotate * scale;
        shaderActivate(lightingShader);
        //drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);
        //room_structure->drawCubeWithTexture(lightingShaderWithTexture, model);
        room_structure(cubeVAO, lightingShader, model, lightingShaderWithTexture);
        model = glm::mat4(1.0f);
    }

    for (int i = 1; i < 6; i++) {

        translate2 = glm::translate(model, glm::vec3((road_width / 2) + 6 - 2 * 8 - 12, 8, -(road_length / 2) - 8 - i * 8));
        //translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));

        model = alTogether * translate2 * rotate * scale;
        shaderActivate(lightingShader);
        //drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);
        //room_structure->drawCubeWithTexture(lightingShaderWithTexture, model);
        room_structure(cubeVAO, lightingShader, model, lightingShaderWithTexture);
        model = glm::mat4(1.0f);
    }



    //rotate = glm::rotate(rotate, glm::radians(90.0f), glm::vec3(0, 1, 0));

    for (int i = 1; i < 6; i++) {

        translate2 = glm::translate(model, glm::vec3((road_width / 2) + 6 - 2 * 8 - 12, 8, -(road_length / 2) - 8 - i * 8));
        //translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));

        model = alTogether * translate2 * rotate * scale;
        shaderActivate(lightingShader);
        //drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);
        //room_structure->drawCubeWithTexture(lightingShaderWithTexture, model);
        room_structure(cubeVAO, lightingShader, model, lightingShaderWithTexture);
        model = glm::mat4(1.0f);
    }



    


    


}









void drawCylinder(unsigned int& cVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b)
{
    lightingShader.use();

    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    lightingShader.setFloat("material.shininess", 60.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cVAO);
    glDrawElements(GL_TRIANGLES, 120, GL_UNSIGNED_INT, 0);
}









void bed(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture)
{
    float baseHeight = 0.3;
    float width = 1;
    float length = 2;
    float pillowWidth = 0.3;
    float pillowLength = 0.15;
    float blanketWidth = 0.8;
    float blanketLength = 0.7;
    float headHeight = 0.5;

    //base
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, baseHeight, length));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * scale * translate;
    //drawCube(cubeVAO, lightingShader, model, 0.471, 0.196, 0.039);
    shaderActivate(lightingShaderWithTexture);
    tmp->drawCubeWithTexture(lightingShaderWithTexture, model);
    //shaderActivate(lightingShader);

    //foam
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(0, baseHeight, 0));
    scale = glm::scale(model, glm::vec3(width, 0.06, length));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    //drawCube(cubeVAO, lightingShader, model, 0.804, 0.361, 0.361);
    
    foam->drawCubeWithTexture(lightingShaderWithTexture, model);

    //pillow 1
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(0, baseHeight + 1 * 0.06, (length / 2) - (0.025 + pillowWidth / 2)));
    scale = glm::scale(model, glm::vec3(pillowWidth, 0.08, pillowLength));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
   // drawCube(cubeVAO, lightingShader, model, 1, 0.20, 0.18);
    
    pillow->drawCubeWithTexture(lightingShaderWithTexture, model);

    //pillow 2
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(-width / 2 + 0.2 / 2, baseHeight + 1 * 0.06, 0));
    scale = glm::scale(model, glm::vec3(pillowWidth - 0.1, 0.1, pillowLength + 1));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    //drawCube(cubeVAO, lightingShader, model, .02, 1, 0.18);
    
    pillow1->drawCubeWithTexture(lightingShaderWithTexture, model);


    

    //head
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(0, 0, (length / 2 - 0.02 / 2) + 0.02));
    scale = glm::scale(model, glm::vec3(width, headHeight, 0.02));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    //drawCube(cubeVAO, lightingShader, model, 0.545, 0.271, 0.075);
    
    head->drawCubeWithTexture(lightingShaderWithTexture, model);


    //head
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(0, 0, (-length / 2 + 0.02 / 2) - 0.02));
    scale = glm::scale(model, glm::vec3(width, headHeight, 0.02));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    //drawCube(cubeVAO, lightingShader, model, 0.545, 0.271, 0.075);
    head1->drawCubeWithTexture(lightingShaderWithTexture, model);
    
}


void room_structure(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture)
{


    //floor
    float floor_height = 0.5;
    float floor_width = 8.0;
    float floor_length = 12;

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 rotate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 translate3 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(floor_width+3, floor_height, floor_length+6));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.36));

    model = alTogether * scale * translate;
    shaderActivate(lightingShaderWithTexture);
    //drawCube(cubeVAO, lightingShader, model, 1, 0.4, 0.06);
    
    floor1->drawCubeWithTexture(lightingShaderWithTexture, model);

    //relling
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    rotate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    translate3 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);

    translate2 = glm::translate(model, glm::vec3(0, 3, floor_length-1 ));
    scale = glm::scale(model, glm::vec3( floor_width + 1, .2,  .2));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));

    model = alTogether * translate2 * scale * translate;
    relling->drawCubeWithTexture(lightingShaderWithTexture, model);
    model = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(0, 1, floor_length - 1));

    model = alTogether * translate2 * scale * translate;
    relling->drawCubeWithTexture(lightingShaderWithTexture, model);



    model = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(0, 0, floor_length - 1));
    scale = glm::scale(model, glm::vec3(0.2, 3, .2));

    model = alTogether * translate2 * scale * translate;
    relling->drawCubeWithTexture(lightingShaderWithTexture, model);

    




    //roof


    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    rotate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    translate3 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);

    translate2 = glm::translate(model, glm::vec3(0, 8, 0.0));
    scale = glm::scale(model, glm::vec3(floor_width+2, .2, floor_length+2));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));

    model = alTogether * translate2 * scale * translate;
    //drawCube(cubeVAO, lightingShader, model, 1, 0.04, 0.4);


    
    roof->drawCubeWithTexture(lightingShaderWithTexture, model);
    //right wall

    float height = 8;
    float width = floor_height;
    float length = 12;
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, height, length));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    translate2 = glm::translate(model, glm::vec3(-floor_width / 2, 0, 0.0));
    model = alTogether * translate2 * scale * translate;
    //drawCube(cubeVAO, lightingShader, model, 1, .4, 0.06);
    
    wall->drawCubeWithTexture(lightingShaderWithTexture, model);
    //left wall

    height = 8;
    width = floor_height+0.5;
    length = 14;
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, height, length));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    translate2 = glm::translate(model, glm::vec3(floor_width / 2, 0, 0.0));
    model = alTogether * translate2 * scale * translate;
    //drawCube(cubeVAO, lightingShader, model, 1, 0.4, 0.06);
    
    wall->drawCubeWithTexture(lightingShaderWithTexture, model);
    //back wall

    height = 8;
    width = floor_width;
    length = floor_height;
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, height, length));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    translate2 = glm::translate(model, glm::vec3(0, 0.0, -floor_length / 2));
    model = alTogether * translate2 * scale * translate;
    wall->drawCubeWithTexture(lightingShaderWithTexture, model);


    //front_wall
    height = 3;
    width = floor_width;
    length = floor_height;
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, height, length));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    translate2 = glm::translate(model, glm::vec3(0, 5.0, floor_length / 2+floor_height/2));
    model = alTogether * translate2 * scale * translate;
    wall->drawCubeWithTexture(lightingShaderWithTexture, model);


    height = 5;
    width = 3.8;
    length = floor_height;
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, height, length));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    translate2 = glm::translate(model, glm::vec3(-(floor_width/2 - width/2), 0.0, floor_length / 2 + floor_height / 2));
    model = alTogether * translate2 * scale * translate;
    wall->drawCubeWithTexture(lightingShaderWithTexture, model);

    height = 5;
    width = 2.3;
    length = floor_height;
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, height, length));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    translate2 = glm::translate(model, glm::vec3((floor_width / 2 - width / 2), 0.0, floor_length / 2 + floor_height / 2));
    model = alTogether * translate2 * scale * translate;
    wall->drawCubeWithTexture(lightingShaderWithTexture, model);
    
    //wall->drawCubeWithTexture(lightingShaderWithTexture, model);

    //door
    scale = glm::scale(model, glm::vec3(1, 2, 1));
    translate2 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.3,0,6));
    model = alTogether *  translate2  *   glm::mat4(1.0f);
    
    door(cubeVAO, lightingShader, model, lightingShaderWithTexture);




    //window
    float wx = 3;
    float wy = 3;
    float wz = 1;




    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(-3.5, height/2, -floor_length/2 - 0.5));
    scale = glm::scale(model, glm::vec3(wx, wy, wz + 0.1));
    translate = glm::translate(model, glm::vec3(0, 0, 0));
    model = alTogether * translate2 * scale * translate;
    shaderActivate(lightingShaderWithTexture);
    room_window->drawCubeWithTexture(lightingShaderWithTexture, model);

    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(0.5, height / 2, -floor_length / 2 - 0.5));
    scale = glm::scale(model, glm::vec3(wx, wy, wz + 0.1));
    translate = glm::translate(model, glm::vec3(0, 0, 0));
    model = alTogether * translate2 * scale * translate;
    shaderActivate(lightingShaderWithTexture);
    room_window->drawCubeWithTexture(lightingShaderWithTexture, model);


    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(-3.5, height / 2, floor_length / 2 -0.2 ));
    scale = glm::scale(model, glm::vec3(wx-1, wy, wz ));
    translate = glm::translate(model, glm::vec3(0, 0, 0));
    model = alTogether * translate2 * scale * translate;
    shaderActivate(lightingShaderWithTexture);
    room_window->drawCubeWithTexture(lightingShaderWithTexture, model);

    

    //bed 1
    translate2 = glm::translate(glm::mat4(1.0f), glm::vec3(-floor_width / 2 + 0.5 + floor_height, floor_height, -floor_length / 2 + 1 + floor_height));
    glm::mat4 rotateYMatrix;

    rotate = glm::rotate(rotate, glm::radians(180.0f), glm::vec3(0, 1, 0));
    model = alTogether * translate2 * rotate * glm::mat4(1.0f);
    //bed(cubeVAO, lightingShader, model);
    bed(cubeVAO, lightingShader, model, lightingShaderWithTexture);
    rotate = glm::mat4(1.0f);

    //bed 2

    translate2 = glm::translate(glm::mat4(1.0f), glm::vec3((-floor_width / 2 + 0.5 + floor_height), floor_height, floor_length / 2 - 1 - floor_height));


    rotate = glm::rotate(rotate, glm::radians(2 * 180.0f), glm::vec3(0, 1, 0));
    model = alTogether * translate2 * rotate * glm::mat4(1.0f);
   // bed(cubeVAO, lightingShader, model);
    bed(cubeVAO, lightingShader, model, lightingShaderWithTexture);
    rotate = glm::mat4(1.0f);

    //bed 3

    translate2 = glm::translate(glm::mat4(1.0f), glm::vec3(floor_width / 2 - 0.5 - floor_height, floor_height, -floor_length / 2 + 1 + floor_height));


    rotate = glm::rotate(rotate, glm::radians(3 * 180.0f), glm::vec3(0, 1, 0));
    model = alTogether * translate2 * rotate * glm::mat4(1.0f);
    //bed(cubeVAO, lightingShader, model);
    bed(cubeVAO, lightingShader, model, lightingShaderWithTexture);
    rotate = glm::mat4(1.0f);

    //bed 4

    translate2 = glm::translate(glm::mat4(1.0f), glm::vec3(floor_width / 2 - 0.5 - floor_height, floor_height, floor_length / 2 - 1 - floor_height));


    rotate = glm::rotate(rotate, glm::radians(4 * 180.0f), glm::vec3(0, 1, 0));
    model = alTogether * translate2 * rotate * glm::mat4(1.0f);
    //bed(cubeVAO, lightingShader, model);
    bed(cubeVAO, lightingShader, model, lightingShaderWithTexture);
    rotate = glm::mat4(1.0f);


    //table1

    //model = glm::mat4(1.0f);

    translate2 = glm::translate(glm::mat4(1.0f), glm::vec3(-floor_width / 2 + 0.5 + floor_height, floor_height, floor_length / 2 - 1 - floor_height - 2.5));

    model = alTogether * translate2 * glm::mat4(1.0f);
    //table(cubeVAO, lightingShader, model);
    table(cubeVAO, lightingShader, model, lightingShaderWithTexture);


    model = glm::mat4(1.0f);
    translate3 = glm::translate(glm::mat4(1.0f), glm::vec3(0.5,0,0));
    rotate = glm::rotate(rotate, glm::radians( 90.0f), glm::vec3(0, 1, 0));
    model = alTogether *translate3* translate2 * rotate * glm::mat4(1.0f);
    chair(cubeVAO, lightingShader, model, lightingShaderWithTexture);

    model = alTogether * translate2 * glm::mat4(1.0f);



    //Fan1
    model = glm::mat4(1.0f);
    translate3 = glm::translate(glm::mat4(1.0f), glm::vec3(0.5, 7, 0));
    rotate = glm::rotate(rotate, glm::radians(90.0f), glm::vec3(0, 1, 0));
    model = alTogether * translate3 * translate2 * glm::mat4(1.0f);
    Fan(cubeVAO, lightingShader, model, lightingShaderWithTexture);

    model = alTogether * translate2 * glm::mat4(1.0f);

    //table2



    translate2 = glm::translate(glm::mat4(1.0f), glm::vec3(-floor_width / 2 + 0.5 + floor_height, floor_height, -floor_length / 2 + 1 + floor_height + 1 * 2.5));

    model = alTogether * translate2  * glm::mat4(1.0f);
    //table(cubeVAO, lightingShader, model);
    table(cubeVAO, lightingShader, model, lightingShaderWithTexture);

    model = glm::mat4(1.0f);
    translate3 = glm::translate(glm::mat4(1.0f), glm::vec3(0.5, 0, 0));
    rotate = glm::rotate(rotate, glm::radians(-90.0f), glm::vec3(0, 1, 0));
    model = alTogether * translate3 * translate2 * rotate * glm::mat4(1.0f);
    chair(cubeVAO, lightingShader, model, lightingShaderWithTexture);

    model = alTogether * translate2 * glm::mat4(1.0f);



    model = glm::mat4(1.0f);
    translate3 = glm::translate(glm::mat4(1.0f), glm::vec3(0.5, 7, 0));
    rotate = glm::rotate(rotate, glm::radians(0.0f), glm::vec3(0, 1, 0));
    model = alTogether * translate3 * translate2  * glm::mat4(1.0f);
    Fan(cubeVAO, lightingShader, model, lightingShaderWithTexture);

    model = alTogether * translate2 * glm::mat4(1.0f);



    //table3

    translate2 = glm::translate(glm::mat4(1.0f), glm::vec3(floor_width / 2 - 0.5 - floor_height, floor_height, -floor_length / 2 + 1 + floor_height + 1 * 2.5));

    model = alTogether * translate2  * glm::mat4(1.0f);
   // table(cubeVAO, lightingShader, model);
    table(cubeVAO, lightingShader, model, lightingShaderWithTexture);

    model = glm::mat4(1.0f);
    translate3 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5, 0, 0));
    rotate = glm::rotate(rotate, glm::radians(180.0f), glm::vec3(0, 1, 0));
    model = alTogether * translate3 * translate2 * rotate * glm::mat4(1.0f);
    chair(cubeVAO, lightingShader, model, lightingShaderWithTexture);

    model = alTogether * translate2 * glm::mat4(1.0f);


    //Fan3

    model = glm::mat4(1.0f);
    translate3 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5, 7, 0));
    rotate = glm::rotate(rotate, glm::radians(180.0f), glm::vec3(0, 1, 0));
    model = alTogether * translate3 * translate2  * glm::mat4(1.0f);
    Fan(cubeVAO, lightingShader, model, lightingShaderWithTexture);

    model = alTogether * translate2 * glm::mat4(1.0f);

    //table4

    translate2 = glm::translate(glm::mat4(1.0f), glm::vec3(floor_width / 2 - 0.5 - floor_height, floor_height, floor_length / 2 - 1 - floor_height - 1 * 2.5));

    model = alTogether * translate2  * glm::mat4(1.0f);
    //table(cubeVAO, lightingShader, model);
    table(cubeVAO, lightingShader, model, lightingShaderWithTexture);

    model = glm::mat4(1.0f);
    translate3 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5, 0, 0));
    rotate = glm::rotate(rotate, glm::radians(180.0f), glm::vec3(0, 1, 0));
    model = alTogether * translate3 * translate2 * rotate * glm::mat4(1.0f);
    chair(cubeVAO, lightingShader, model, lightingShaderWithTexture);

    model = alTogether * translate2 * glm::mat4(1.0f);


    //Fan4
    model = glm::mat4(1.0f);
    translate3 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5, 7, 0));
    rotate = glm::rotate(rotate, glm::radians(0.0f), glm::vec3(0, 1, 0));
    model = alTogether * translate3 * translate2  * glm::mat4(1.0f);
    Fan(cubeVAO, lightingShader, model, lightingShaderWithTexture);

    model = alTogether * translate2 * glm::mat4(1.0f);




}




//chair
void chair(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture)
{
    float backheight = 0.5;
    float backwidth = 0.03;
    float backlength = 0.5;

    float hight = 0.04;
    float length = 0.5;
    float width = 0.3;
    float supporthight = 0.5;
    float supportwidth = 0.04;
    float supportlength = 0.04;
    shaderActivate(lightingShader);

    // leg1
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(supportlength, supporthight, supportwidth));
    translate = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
    model = alTogether * translate * scale;
    //drawCube(cubeVAO, lightingShader, model, 0.545, 0.271, 0.075);
    drawCube(cubeVAO, lightingShader, model, 1, 0.4, 0.06);

    //leg2
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(supportlength, supporthight, supportwidth));
    translate = glm::translate(model, glm::vec3(length - supportwidth, 0.0, 0.0));
    model = alTogether * translate * scale;
    //drawCube(cubeVAO, lightingShader, model, 0.545, 0.271, 0.075);
    drawCube(cubeVAO, lightingShader, model, 1, 0.4, 0.06);

    //leg3
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(supportlength, supporthight, supportwidth));
    translate = glm::translate(model, glm::vec3(0.0, 0.0, width - supportlength));
    model = alTogether * translate * scale;
    //drawCube(cubeVAO, lightingShader, model, 0.545, 0.271, 0.075);
    drawCube(cubeVAO, lightingShader, model, 1, 0.4, 0.06);

    //leg4
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(supportlength, supporthight, supportwidth));
    translate = glm::translate(model, glm::vec3(length - supportwidth, 0.0, width - supportlength));
    model = alTogether * translate * scale;
    //drawCube(cubeVAO, lightingShader, model, 0.545, 0.271, 0.075);
    drawCube(cubeVAO, lightingShader, model, 1, 0.4, 0.06);

    //base
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(length, hight, width));
    translate = glm::translate(model, glm::vec3(0.0, supporthight, 0.0));
    model = alTogether * translate * scale;
    //drawCube(cubeVAO, lightingShader, model, 0.545, 0.271, 0.075);
    drawCube(cubeVAO, lightingShader, model, 1, 0.4, 0.06);

    //back
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(backlength, backheight, backwidth));
    translate = glm::translate(model, glm::vec3(0.0, supporthight, width - supportlength));
    model = alTogether * translate * scale;
    //drawCube(cubeVAO, lightingShader, model, 0.545, 0.271, 0.075);
    drawCube(cubeVAO, lightingShader, model, 1, 0.4, 0.06);


}

void Fan(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture)
{
    float bladel = 1.5;
    float bladew = 0.2;
    float bladeh = 0.01;

    //glm::mat4 modelForSphere = glm::mat4(1.0f);
    //modelForSphere = glm::translate(alTogether, glm::vec3(1.5f, 1.2f, 0.5f));
    //sphere->setColor(glm::vec3(0.5, 0.2, 0.5));
    //sphere->drawSphere(lightingShader, modelForSphere);

    // Center
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(0.27, 0.3, 0.27));
    translate = glm::translate(model, glm::vec3(-0.67, 0.0, -0.4));
    model = alTogether * scale * translate;
    //drawCube(cubeVAO, lightingShader, model, 0.545, 0.271, 0.075);
    shaderActivate(lightingShader);
    drawCube(cubeVAO, lightingShader, model, 1, 0.4, 0.06);
    //cube.drawCubeWithTexture(lightingShaderWithTexture, model);

    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(bladel, bladeh, bladew));
    translate = glm::translate(model, glm::vec3(0.01, 0.0, 0.0));
    glm::mat4 rotateM = glm::rotate(model, glm::radians(45.0f + rotateFan), glm::vec3(0.0f, 1.0f, 0.0f));
    model = alTogether * rotateM * scale * translate;
    shaderActivate(lightingShader);
    drawCube(cubeVAO, lightingShader, model, 1, 0.4, 0.06);
    //cube.drawCubeWithTexture(lightingShaderWithTexture, model);

    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(bladel, bladeh, bladew));
    translate = glm::translate(model, glm::vec3(0, 0.01, 0.0));
    rotateM = glm::rotate(model, glm::radians(165.0f + rotateFan), glm::vec3(0.0f, 1.0f, 0.0f));
    model = alTogether * rotateM * scale * translate;
    shaderActivate(lightingShader);
    drawCube(cubeVAO, lightingShader, model, 1, 0.4, 0.06);

    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(bladel, bladeh, bladew));
    translate = glm::translate(model, glm::vec3(0.01, 0.01, 0.0));
    rotateM = glm::rotate(model, glm::radians(285.0f + rotateFan), glm::vec3(0.0f, 1.0f, 0.0f));
    model = alTogether * rotateM * scale * translate;
    shaderActivate(lightingShader);
    drawCube(cubeVAO, lightingShader, model, 1, 0.4, 0.06);
    ////leg1
    //model = glm::mat4(1.0f);
    //translate = glm::mat4(1.0f);
    //translate2 = glm::mat4(1.0f);
    //scale = glm::mat4(1.0f);
    //translate2 = glm::translate(model, glm::vec3(0, 0.02, 0));
    //scale = glm::scale(model, glm::vec3(supportlength, -supporthight, supportwidth));
    //translate = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
    //model = alTogether * translate2 * scale * translate;
    //cube.drawCube2(lightingShader, model, 0.804, 0.361, 0.361);
}


void door(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture)
{
    float dy = 5.0;
    float dx = 2.0;
    float dz = 0.1;
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    glm::mat4 rotate = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(dx, dy, dz));
    translate = glm::translate(model, glm::vec3(0, 0, 0));
    glm::mat4 rotateM = glm::rotate(model, glm::radians(doorangle), glm::vec3(0.0f, 1.0f, 0.0f));
    model = alTogether * rotateM * scale * translate;
    shaderActivate(lightingShader);
    //drawCube(cubeVAO, lightingShader, model, 1, 0.4, 0.06);
    //cube.drawCube2(lightingShader, model, 0.4,0.5,0.1);
    door1->drawCubeWithTexture(lightingShaderWithTexture, model);
}














void table(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Shader& lightingShaderWithTexture)
{
    float table_top = 1;
    float width = 0.8;
    float length = 1.5;
    float height = 0.08;


    //top
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, height, length));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    translate2 = glm::translate(model, glm::vec3(0, table_top, 0));
    //scale = glm::scale(model, glm::vec3(width, headHeight, 0.02));
    //translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    //drawCube(cubeVAO, lightingShader, model, 1, 0, 0);
    shaderActivate(lightingShaderWithTexture);
    drawCube(cubeVAO, lightingShader, model, 1, 0.4, 0.06);
    /*diffuseMapPath = "images/table_top.jpg";
    specularMapPath = "images/table_top.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube1 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    cube1.drawCubeWithTexture(lightingShaderWithTexture, model);*/

    //leg 1

    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);

    scale = glm::scale(model, glm::vec3(width * 0.1, table_top, length * 0.05));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    translate2 = glm::translate(model, glm::vec3((-width + width * 0.1) / 2, 0, (-length + length * 0.05) / 2));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 1, 0, 0);
    




    //leg 2

    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);

    scale = glm::scale(model, glm::vec3(width * 0.1, table_top, length * 0.05));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    translate2 = glm::translate(model, glm::vec3((-width + width * 0.1) / 2, 0, -(-length + length * 0.05) / 2));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 1, 0, 0);


    //leg 3

    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);

    scale = glm::scale(model, glm::vec3(width * 0.1, table_top, length * 0.05));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    translate2 = glm::translate(model, glm::vec3(-(-width + width * 0.1) / 2, 0, (-length + length * 0.05) / 2));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 1, 0, 0);


    //leg 4

    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);

    scale = glm::scale(model, glm::vec3(width * 0.1, table_top, length * 0.05));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    translate2 = glm::translate(model, glm::vec3(-(-width + width * 0.1) / 2, 0, -(-length + length * 0.05) / 2));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 1, 0, 0);


}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime*2);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime * 2);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime * 2);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime * 2);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        rotateFan += 20.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        doorangle += 10;
        doorangle = min(doorangle, 80.0f);
        //std::cout << doorangle << endl;
    }

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        door_open += 0.5;
        door_open1 += 0.1;
        door_open = min(door_open, 6.0f);
        door_open1 = min(door_open1, 6.0f);
        //std::cout << doorangle << endl;
    }

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        door_open -= 0.5;
        door_open1 -= 0.15;
        door_open = max(door_open, -0.0f);
        door_open1 = max(door_open1, -0.0f);
        //std::cout << doorangle << endl;
    }   //std::cout << doorangle << endl;
    



    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        doorangle -= 10;
        doorangle = max(doorangle, 0.0f);
    }

    /*if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        if (rotateAxis_X) rotateAngle_X -= 0.1;
        else if (rotateAxis_Y) rotateAngle_Y -= 0.1;
        else rotateAngle_Z -= 0.1;
    }*/
    /*if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) translate_Y += 0.001;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) translate_Y -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) translate_X += 0.001;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) translate_X -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) translate_Z += 0.001;*/
    //if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) translate_Z -= 0.001;
    //if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) scale_X += 0.001;
    //if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) scale_X -= 0.001;
    //if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) scale_Y += 0.001;
    //if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) scale_Y -= 0.001;
    //if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) scale_Z += 0.001;
    //if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) scale_Z -= 0.001;

    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        rotateAngle_X += 0.1;
        rotateAxis_X = 1.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 0.0;
    }
    
    

    
   // if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
   // {
   //     if (onOffPointToggle)
   //     {
   //         pointlight1.turnOff();
   //         
   //         onOffPointToggle = false;
   //     }
   //     else
   //     {
   //         pointlight1.turnOn();
   //       
   //         onOffPointToggle = true;
   //     }
   //    // pointlight3.turnOff();
   //    // pointlight4.turnOff();

   // }
   // 

   // if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
   // {
   //     
   //     if (onOffSpotToggle)
   //     {
   //        
   //         pointlight2.turnOff();
   //         onOffSpotToggle = false;
   //     }
   //     else
   //     {
   //         pointlight2.turnOn();
   //         onOffSpotToggle = true;
   //     }
   // }

   // if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
   // {

   //     if (onOffDirectToggle)
   //     {

   
   
   //         pointlight3.turnOff();
   //         onOffDirectToggle = false;
   //     }
   //     else
   //     {
   //         pointlight3.turnOn();
   //         onOffDirectToggle = true;
   //     }
   // }
   // 
   // if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
   // {
   //     pointlight1.turnAmbientOn();
   //     pointlight2.turnAmbientOn();
   //    // pointlight3.turnAmbientOn();
   //    // pointlight4.turnAmbientOn();
   // }
   // if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
   // {
   //     pointlight1.turnAmbientOff();
   //     pointlight2.turnAmbientOff();
   //   //  pointlight3.turnAmbientOff();
   //   //  pointlight4.turnAmbientOff();
   // }
   // if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
   // {
   //     pointlight1.turnDiffuseOn();
   //     pointlight2.turnDiffuseOn();
   //  //   pointlight3.turnDiffuseOn();
   // //    pointlight4.turnDiffuseOn();
   // }
   // if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
   // {
   //     pointlight1.turnDiffuseOff();
   //     pointlight2.turnDiffuseOff();
   ////     pointlight3.turnDiffuseOff();
   // //    pointlight4.turnDiffuseOff();
   // }
   // if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
   // {
   //     pointlight1.turnSpecularOn();
   //     pointlight2.turnSpecularOn();
   // //    pointlight3.turnSpecularOn();
   // //    pointlight4.turnSpecularOn();
   // }
   // if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
   // {
   //     pointlight1.turnSpecularOff();
   //     pointlight2.turnSpecularOff();
   ////     pointlight3.turnSpecularOff();
   // //    pointlight4.turnDiffuseOff();
   // }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        pointlight2.turnOn();
        // pointlight3.turnOff();
        // pointlight4.turnOff();

    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
    {
        pointlight2.turnOff();
        // pointlight3.turnOff();
        // pointlight4.turnOff();

    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        pointlight3.turnOn();
        // pointlight3.turnOff();
        // pointlight4.turnOff();

    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    {
        pointlight3.turnOff();
        // pointlight3.turnOff();
        // pointlight4.turnOff();

    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    {
        pointlight1.turnOn();
        // pointlight3.turnOff();
        // pointlight4.turnOff();

    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        pointlight1.turnOff();
        // pointlight3.turnOff();
        // pointlight4.turnOff();

    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        /*if (diffuseToggle)
        {*/
        if (pointlight1.isOn())
            pointlight1.turnAmbientOn();
        if (pointlight2.isOn())
            pointlight2.turnAmbientOn();
        if (pointlight3.isOn())
            pointlight3.turnAmbientOn();
        //pointlight4.turnDiffuseOn();
        //diffuseToggle = !diffuseToggle;
    //}
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        /*if (diffuseToggle)
        {*/
        if (pointlight1.isOn())
            pointlight1.turnAmbientOff();
        if (pointlight2.isOn())
            pointlight2.turnAmbientOff();
        if (pointlight3.isOn())
            pointlight3.turnAmbientOff();
        //pointlight4.turnDiffuseOff();
        //diffuseToggle = !diffuseToggle;
    //}
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        /*if (diffuseToggle)
        {*/
        if (pointlight1.isOn())
            pointlight1.turnDiffuseOn();
        if (pointlight2.isOn())
            pointlight2.turnDiffuseOn();
        if (pointlight3.isOn())
            pointlight3.turnDiffuseOn();
        //pointlight4.turnAmbientOn();
        //diffuseToggle = !diffuseToggle;
        //}
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        /*if (diffuseToggle)
        {*/
        if (pointlight1.isOn())
            pointlight1.turnDiffuseOff();
        if (pointlight2.isOn())
            pointlight2.turnDiffuseOff();
        if (pointlight3.isOn())
            pointlight3.turnDiffuseOff();
        //diffuseToggle = !diffuseToggle;
        //}
    }


    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        /*if (diffuseToggle)
        {*/
        if (pointlight1.isOn())
            pointlight1.turnSpecularOn();
        if (pointlight2.isOn())
            pointlight2.turnSpecularOn();
        if (pointlight3.isOn())
            pointlight3.turnSpecularOn();
        //pointlight4.turnSpecularOn();
        //diffuseToggle = !diffuseToggle;
        //}
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
        /*if (diffuseToggle)
        {*/
        /*cout << "1 " << pointlight1.isOn() << endl;
        cout << pointlight2.isOn() << endl;
        cout << pointlight3.isOn() << endl;*/
        if (pointlight1.isOn())
            pointlight1.turnSpecularOff();
        if (pointlight2.isOn())
            pointlight2.turnSpecularOff();
        if (pointlight3.isOn())
            pointlight3.turnSpecularOff();
        //pointlight4.turnSpecularOff();
        //diffuseToggle = !diffuseToggle;
        //}
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
