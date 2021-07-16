#include "sphere.h"

#include <cmath>
#include <glm/glm.hpp>
#include <vector>
using namespace std;

Sphere::Sphere()
{
    init(48);
}

Sphere::Sphere(int prec)
{
    init(prec);
}

void Sphere::init(int prec)
{
    m_numVertices = (prec + 1) * (prec + 1);
    m_numIndices = prec * prec * 6;
    for (int i = 0; i < m_numVertices; i++)
    {
        m_vertices.emplace_back();
    }
    for (int i = 0; i < m_numVertices; i++)
    {
        m_texCoords.emplace_back();
    }
    for (int i = 0; i < m_numVertices; i++)
    {
        m_normals.emplace_back();
    }
    for (int i = 0; i < m_numVertices; i++)
    {
        m_tangents.emplace_back();
    }
    for (int i = 0; i < m_numIndices; i++)
    {
        m_indices.push_back(0);
    }

    // calculate triangle m_vertices
    for (int i = 0; i <= prec; i++)
    {
        for (int j = 0; j <= prec; j++)
        {
            auto y = (float)cos(glm::radians(180.0f - i * 180.0f / prec));
            float x = -(float)cos(glm::radians(j * 360.0f / prec)) * (float)abs(cos(asin(y)));
            float z = (float)sin(glm::radians(j * 360.0f / (float)(prec))) * (float)abs(cos(asin(y)));
            m_vertices[i * (prec + 1) + j] = glm::vec3(x, y, z);
            m_texCoords[i * (prec + 1) + j] = glm::vec2(((float)j / prec), ((float)i / prec));
            m_normals[i * (prec + 1) + j] = glm::vec3(x, y, z);

            // calculate tangent vector
            if (((x == 0) && (y == 1) && (z == 0)) || ((x == 0) && (y == -1) && (z == 0)))
            {
                m_tangents[i * (prec + 1) + j] = glm::vec3(0.0f, 0.0f, -1.0f);
            }
            else
            {
                m_tangents[i * (prec + 1) + j] = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(x, y, z));
            }
        }
    }
    // calculate triangle m_indices
    for (int i = 0; i < prec; i++)
    {
        for (int j = 0; j < prec; j++)
        {
            m_indices[6 * (i * prec + j) + 0] = i * (prec + 1) + j;
            m_indices[6 * (i * prec + j) + 1] = i * (prec + 1) + j + 1;
            m_indices[6 * (i * prec + j) + 2] = (i + 1) * (prec + 1) + j;
            m_indices[6 * (i * prec + j) + 3] = i * (prec + 1) + j + 1;
            m_indices[6 * (i * prec + j) + 4] = (i + 1) * (prec + 1) + j + 1;
            m_indices[6 * (i * prec + j) + 5] = (i + 1) * (prec + 1) + j;
        }
    }
}

int Sphere::getNumVertices() const
{
    return m_numVertices;
}
int Sphere::getNumIndices()
{
    return m_numIndices;
}
std::vector<int> Sphere::getIndices()
{
    return m_indices;
}
std::vector<glm::vec3> Sphere::getVertices()
{
    return m_vertices;
}
std::vector<glm::vec2> Sphere::getTexCoords()
{
    return m_texCoords;
}
std::vector<glm::vec3> Sphere::getNormals()
{
    return m_normals;
}
std::vector<glm::vec3> Sphere::getTangents()
{
    return m_tangents;
}