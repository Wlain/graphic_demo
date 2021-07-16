#include "torus.h"

#include <glm/gtc/matrix_transform.hpp>
#include <vector>

Torus::Torus()
{
    m_prec = 48;
    m_inner = 0.5f;
    m_outer = 0.2f;
    init();
}

Torus::Torus(float in, float out, int precIn)
{
    m_prec = precIn;
    m_inner = in;
    m_outer = out;
    init();
}

void Torus::init()
{
    m_numVertices = (m_prec + 1) * (m_prec + 1);
    m_numIndices = m_prec * m_prec * 6;
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
        m_sTangents.emplace_back();
    }
    for (int i = 0; i < m_numVertices; i++)
    {
        m_tTangents.emplace_back();
    }
    for (int i = 0; i < m_numIndices; i++)
    {
        m_indices.push_back(0);
    }

    // calculate first ring
    for (int i = 0; i < m_prec + 1; i++)
    {
        float amt = glm::radians((float)i * 360.0f / (float)m_prec);

        glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::vec3 initPos(rMat * glm::vec4(m_outer, 0.0f, 0.0f, 1.0f));

        m_vertices[i] = glm::vec3(initPos + glm::vec3(m_inner, 0.0f, 0.0f));
        m_texCoords[i] = glm::vec2(0.0f, ((float)i / (float)m_prec));

        rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 0.0f, 1.0f));
        m_tTangents[i] = glm::vec3(rMat * glm::vec4(0.0f, -1.0f, 0.0f, 1.0f));

        m_sTangents[i] = glm::vec3(glm::vec3(0.0f, 0.0f, -1.0f));
        m_normals[i] = glm::cross(m_tTangents[i], m_sTangents[i]);
    }
    // rotate the first ring about Y to get the other rings
    for (int ring = 1; ring < m_prec + 1; ring++)
    {
        for (int i = 0; i < m_prec + 1; i++)
        {
            float amt = glm::radians((float)ring * 360.0f / (float)m_prec);

            glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
            m_vertices[ring * (m_prec + 1) + i] = glm::vec3(rMat * glm::vec4(m_vertices[i], 1.0f));

            m_texCoords[ring * (m_prec + 1) + i] = glm::vec2((float)ring * 2.0f / (float)m_prec, m_texCoords[i].t);
            if (m_texCoords[ring * (m_prec + 1) + i].s > 1.0) m_texCoords[ring * (m_prec + 1) + i].s -= 1.0f;

            rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
            m_sTangents[ring * (m_prec + 1) + i] = glm::vec3(rMat * glm::vec4(m_sTangents[i], 1.0f));

            rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
            m_tTangents[ring * (m_prec + 1) + i] = glm::vec3(rMat * glm::vec4(m_tTangents[i], 1.0f));

            rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
            m_normals[ring * (m_prec + 1) + i] = glm::vec3(rMat * glm::vec4(m_normals[i], 1.0f));
        }
    }
    // calculate triangle m_indices
    for (int ring = 0; ring < m_prec; ring++)
    {
        for (int i = 0; i < m_prec; i++)
        {
            m_indices[((ring * m_prec + i) * 2) * 3 + 0] = ring * (m_prec + 1) + i;
            m_indices[((ring * m_prec + i) * 2) * 3 + 1] = (ring + 1) * (m_prec + 1) + i;
            m_indices[((ring * m_prec + i) * 2) * 3 + 2] = ring * (m_prec + 1) + i + 1;
            m_indices[((ring * m_prec + i) * 2 + 1) * 3 + 0] = ring * (m_prec + 1) + i + 1;
            m_indices[((ring * m_prec + i) * 2 + 1) * 3 + 1] = (ring + 1) * (m_prec + 1) + i;
            m_indices[((ring * m_prec + i) * 2 + 1) * 3 + 2] = (ring + 1) * (m_prec + 1) + i + 1;
        }
    }
}
int Torus::getNumVertices() const
{
    return m_numVertices;
}
int Torus::getNumIndices() const
{
    return m_numIndices;
}
std::vector<int> Torus::getIndices()
{
    return m_indices;
}
std::vector<glm::vec3> Torus::getVertices()
{
    return m_vertices;
}
std::vector<glm::vec2> Torus::getTexCoords()
{
    return m_texCoords;
}
std::vector<glm::vec3> Torus::getNormals()
{
    return m_normals;
}
std::vector<glm::vec3> Torus::getStangents()
{
    return m_sTangents;
}
std::vector<glm::vec3> Torus::getTtangents()
{
    return m_tTangents;
}