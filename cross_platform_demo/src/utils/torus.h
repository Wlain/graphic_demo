#include <cmath>
#include <glm/glm.hpp>
#include <vector>
class Torus
{
public:
    Torus();
    Torus(float inner, float outer, int prec);
    int getNumVertices() const;
    int getNumIndices() const;
    std::vector<int> getIndices();
    std::vector<glm::vec3> getVertices();
    std::vector<glm::vec2> getTexCoords();
    std::vector<glm::vec3> getNormals();
    std::vector<glm::vec3> getStangents();
    std::vector<glm::vec3> getTtangents();

private:
    void init();

private:
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec2> m_texCoords;
    std::vector<glm::vec3> m_normals;
    std::vector<glm::vec3> m_sTangents;
    std::vector<glm::vec3> m_tTangents;
    std::vector<int> m_indices;
    int m_numVertices{};
    int m_numIndices{};
    int m_prec{};
    float m_inner{};
    float m_outer{};
};