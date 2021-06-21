#ifndef MESH_H
#define MESH_H

#include "Vec3.h"
#include "Vec2.h"

#include <string>
#include <vector>

#include "Shader.h"
#include "TextureLoading.h"

struct Vertex {
    Vec3 Position;
    Vec3 Normal;
    Vec2 TexCoords;
    //����������� ������
    Vec3 Tangent;
    //������, ���������������� ������������ ������� � ������� �������
    Vec3 Bitangent;
};

class Mesh {
public:
    //������ mesh
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<TextureLoading> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureLoading> textures);

    void Draw(Shader& shader);

private:
    unsigned int VBO, EBO, VAO;

    void Setup();
};

#endif