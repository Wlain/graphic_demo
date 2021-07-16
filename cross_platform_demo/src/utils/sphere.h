#include <cmath>
#include <vector>
#include <glm/glm.hpp>
class Sphere
{
public:
	Sphere();
	explicit Sphere(int prec);
	int getNumVertices() const;
	int getNumIndices();
	std::vector<int> getIndices();
	std::vector<glm::vec3> getVertices();
	std::vector<glm::vec2> getTexCoords();
	std::vector<glm::vec3> getNormals();
	std::vector<glm::vec3> getTangents();

private:
    void init(int);

private:
    std::vector<int> m_indices;
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec2> m_texCoords;
    std::vector<glm::vec3> m_normals;
    std::vector<glm::vec3> m_tangents;
    int m_numVertices{};
    int m_numIndices{};
};