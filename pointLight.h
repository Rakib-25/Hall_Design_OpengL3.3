//
//  pointLight.h
//  test
//
//  Created by Nazirul Hasan on 22/9/23.
//

#ifndef pointLight_h
#define pointLight_h

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shader.h"

class PointLight {
public:
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float k_c;
    float k_l;
    float k_q;
    int lightNumber;

    PointLight(float posX, float posY, float posZ, float ambR, float ambG, float ambB, float diffR, float diffG, float diffB, float specR, float specG, float specB, float constant, float linear, float quadratic, int num) {

        position = glm::vec3(posX, posY, posZ);
        ambient = glm::vec3(ambR, ambG, ambB);
        diffuse = glm::vec3(diffR, diffG, diffB);
        specular = glm::vec3(specR, specG, specB);
        k_c = constant;
        k_l = linear;
        k_q = quadratic;
        lightNumber = num;
    }
    void setUpPointLight(Shader& lightingShader)
    {
        lightingShader.use();

        if (lightNumber == 2) {
            lightingShader.setVec3("pointLights[0].position", position);
            lightingShader.setVec3("pointLights[0].ambient", ambientOn * ambient);
            lightingShader.setVec3("pointLights[0].diffuse", diffuseOn * diffuse);
            lightingShader.setVec3("pointLights[0].specular", specularOn * specular);
            lightingShader.setFloat("pointLights[0].k_c", k_c);
            lightingShader.setFloat("pointLights[0].k_l", k_l);
            lightingShader.setFloat("pointLights[0].k_q", k_q);
        }

        else if (lightNumber == 1)
        {
            lightingShader.setVec3("spotLights[0].position", position);
            lightingShader.setVec3("spotLights[0].ambient", ambientOn * ambient);
            lightingShader.setVec3("spotLights[0].diffuse", diffuseOn * diffuse);
            lightingShader.setVec3("spotLights[0].specular", specularOn * specular);
            lightingShader.setFloat("spotLights[0].k_c", k_c);
            lightingShader.setFloat("spotLights[0].k_l", k_l);
            lightingShader.setFloat("spotLights[0].k_q", k_q);
        }
        else if (lightNumber == 3)
        {
            lightingShader.setVec3("directionLights[0].position", position);
            lightingShader.setVec3("directionLights[0].ambient", ambientOn * ambient);
            lightingShader.setVec3("directionLights[0].diffuse", diffuseOn * diffuse);
            lightingShader.setVec3("directionLights[0].specular", specularOn * specular);
            lightingShader.setFloat("directionLights[0].k_c", k_c);
            lightingShader.setFloat("directionLights[0].k_l", k_l);
            lightingShader.setFloat("directionLights[0].k_q", k_q);
        }
        
        else
        {
            lightingShader.setVec3("emissionLights[0].position", position);
            lightingShader.setVec3("emissionLights[0].ambient", ambientOn * ambient);
            lightingShader.setVec3("emissionLights[0].diffuse", diffuseOn * diffuse);
            lightingShader.setVec3("emissionLights[0].specular", specularOn * specular);
            lightingShader.setFloat("emissionLights[0].k_c", k_c);
            lightingShader.setFloat("emissionLights[0].k_l", k_l);
            lightingShader.setFloat("emissionLights[0].k_q", k_q);
        }
    }
    bool ison = 1;
    bool isOn()
    {
        return ison;
    }
    void turnOff()
    {
        ison = 0;
        ambientOn = 0.0;
        diffuseOn = 0.0;
        specularOn = 0.0;
    }
    void turnOn()
    {
        ison = 1;
        ambientOn = 1.0;
        diffuseOn = 1.0;
        specularOn = 1.0;
    }
    void turnAmbientOn()
    {
        ambientOn = 1.0;
        //diffuseOn = 0.0;
        //specularOn = 0.0;
    }
    void turnAmbientOff()
    {
        ambientOn = 0.0;
        //diffuseOn = 0.0;
        //specularOn = 0.0;
    }
    void turnDiffuseOn()
    {
        //ambientOn = 0.0;
        diffuseOn = 1.0;
        //specularOn = 0.0;
    }
    void turnDiffuseOff()
    {
        //ambientOn = 0.0;
        diffuseOn = 0.0;
        //specularOn = 0.0;
    }
    void turnSpecularOn()
    {
        /*ambientOn = 0.0;
        diffuseOn = 0.0;*/
        specularOn = 1.0;
    }
    void turnSpecularOff()
    {
        /*
        ambientOn = 0.0;
        diffuseOn = 0.0;*/
        specularOn = 0.0;
    }
private:
    float ambientOn = 1.0;
    float diffuseOn = 1.0;
    float specularOn = 1.0;
};

#endif /* pointLight_h */
