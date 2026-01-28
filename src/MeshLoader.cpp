#include "MeshLoader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <glm/glm.hpp>

void debugPrint(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& uvPos, std::vector<std::string>& vertesies){
    for(glm::vec3& position: positions){
        std::cout << "Position: " << position.x << ", " << position.y << ", " << position.z << std::endl;
    }
    for(glm::vec3& normal: normals){
        std::cout << "Normal: " << normal.x << ", " << normal.y << ", " << normal.z << std::endl;
    }
    for(glm::vec2& uv: uvPos){
        std::cout << "uv: " << uv.x << ", " << uv.y << std::endl;
    }
    for(std::string& vertex: vertesies){
        std::cout << "Vertex: " << vertex << std::endl;
    }
}

void tokenize(std::string& line,const std::string& delimeter, std::vector<std::string>& tokens){
    size_t pos = 0;
    std::string token;
    while((pos = line.find(delimeter)) != std::string::npos){
        token = line.substr(0, pos);
        tokens.push_back(token);
        line.erase(0, pos + delimeter.length());
    }
    tokens.push_back(line);
}

Mesh* MeshLoader::loadObj(const std::string& path){
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvPos;

    std::vector<std::string> vertesies;
    std::vector<float> vertexData;

    std::ifstream file;
    std::string line;

    file.open(path);

    std::stringstream objStream;

    objStream << file.rdbuf();

    file.close();


    while(std::getline(objStream, line)){
        std::vector<std::string> tokens;
        tokenize(line," ",tokens);
        if(tokens[0] == "#" || tokens[0] == "s"){
            continue;
        }
        if(tokens[0] == "o"){
            std::cout << "Object: " << tokens[1] << std::endl;
        }
        if(tokens[0] == "v"){
            positions.emplace_back(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));
        }
        if(tokens[0] == "vn"){
            normals.emplace_back(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));
        }
        if(tokens[0] == "vt"){
            uvPos.emplace_back(std::stof(tokens[1]), std::stof(tokens[2]));
        }
        if(tokens[0] == "f"){
            vertesies.push_back(tokens[1]);
            vertesies.push_back(tokens[2]);
            vertesies.push_back(tokens[3]);
        }
    }

    //debugPrint(positions, normals, uvPos, vertesies);

    for(std::string& vertex: vertesies){
        std::vector<std::string> tokens;
        tokenize(vertex,"/",tokens);
        //token[0] - position
        //token[1] - normal
        //token[2] - uv
        int positionIndex = std::stoi(tokens[0]);
        int uvIndex = std::stoi(tokens[1]);
        int normalIndex = std::stoi(tokens[2]);
        glm::vec3& position = positions[positionIndex - 1];
        glm::vec3& normal = normals[normalIndex - 1];
        glm::vec2& uv = uvPos[uvIndex - 1];

        vertexData.push_back(position.x);
        vertexData.push_back(position.y);
        vertexData.push_back(position.z);
        vertexData.push_back(normal.x);
        vertexData.push_back(normal.y);
        vertexData.push_back(normal.z);
        vertexData.push_back(uv.x);
        vertexData.push_back(uv.y);
    }

    // std::cout << vertexData.size() << " " << vertexData.size() * sizeof(float) << std::endl;
    Mesh* mesh = new Mesh(vertexData.data(), vertexData.size() * sizeof(float));
    
    mesh->addVertexAttribPointer(3);
    mesh->addVertexAttribPointer(3);
    mesh->addVertexAttribPointer(2);
    mesh->enableVertexAttribPointer();
    return mesh;
}