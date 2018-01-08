#include "ObjLoader.h"

namespace crank
{

ObjLoader::ObjLoader(const std::string& path)
{
    OnConstruction(path);
}

void ObjLoader::OnConstruction(const std::string& path)
{
    LoadBuffers(path);
    SortBuffers();
    SetupMesh();
}

void ObjLoader::LoadBuffers(const std::string& path)
{
    // Loads information from the file to the vectors

    std::ifstream ifs(path);
    if(!ifs)
    {
        LOGEL << "[OBJLOADER]";
        LOGE  << "Failed to load file at: " << path;
    }

    while(!ifs.eof())
    {
        std::string str, dump;
        ifs >> str;

        if(str == "#") // ignore comments
        {
            std::getline(ifs, dump);
            continue;
        }

        else if(str == "v") // collect 3d position
        {
            float a[3];
            for(int i = 0; i<3; i++)
                ifs >> a[i];

            glm::vec3 position;
            position.x = a[0];
            position.y = a[1];
            position.z = a[2];

            m_positions.push_back(position);
        }

        else if(str == "vn") // collect 3d normal
        {
            float a[3];
            for(int i = 0; i<3; i++)
                ifs >> a[i];

            glm::vec3 normal;
            normal.x = a[0];
            normal.y = a[1];
            normal.z = a[2];

            m_normals.push_back(normal);
        }

        else if(str == "vt") // collect texture coordinate
        {
            float a[2];
            for(int i = 0; i<2; i++)
                ifs >> a[i];

            glm::vec2 tex;
            tex.x = a[0];
            tex.y = a[1];

            m_texCoords.push_back(tex);
        }

        else if(str == "f") // collect face info
        {
            std::string s;
            std::getline(ifs, s);
            m_faceInfo.push_back(s);
        }

        else // ignore line
        {
            std::getline(ifs, dump);
            continue;
        }
    }

}

void ObjLoader::SortBuffers()
{
    std::vector<glm::vec3> sortedPositions;
    std::vector<glm::vec3> sortedNormals;
    std::vector<glm::vec2> sortedTexCoords;

    sortedPositions.reserve(m_positions.size());
    sortedTexCoords.reserve(m_texCoords.size());
    sortedNormals.reserve  (m_normals.size());

    int positionIndex, texIndex, normalIndex;
    char slash;
    for(int i = 0; i < m_faceInfo.size(); i++)
    {
        std::istringstream iss(m_faceInfo[i]); // For every line i

        for(int j=0; j<3; j++) // For each of the 3 vertices on the line
        {
            iss >> positionIndex >> slash;  // Position data is took as granted

            iss >> texIndex;
            if(iss.fail())                  // Failing means there are no texcoord data
            {
                iss.clear(std::ios_base::goodbit);
                texIndex = 0;
            }

            iss >> slash >> normalIndex;
            if(iss.fail())                  // Failing means there are no normals data
            {
                iss.clear(std::ios_base::goodbit);
                normalIndex = 0;
            }

            sortedPositions.push_back(m_positions[positionIndex - 1]);
            sortedNormals.push_back(m_normals[normalIndex - 1]);

            if(m_texCoords.size() > 0) // if there are tex coords in the model
                sortedTexCoords.push_back(m_texCoords[texIndex - 1]);
        }
    }

    if(m_texCoords.size() > 0) // if there are tex coords in the model
    {
        for(int i = 0; i < sortedPositions.size(); i++)
            m_vertices.push_back(Vertex{sortedPositions[i], sortedNormals[i], sortedTexCoords[i]});
    }

    for(int i = 0; i < sortedPositions.size(); i++)
        m_vertices.push_back(Vertex{sortedPositions[i], sortedNormals[i]});
}

void ObjLoader::Draw(const crank::Shader &shader)
{
    glBindVertexArray(m_vao);
    //glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0 , m_vertices.size());
    glBindVertexArray(0);
}

void ObjLoader::SetupMesh()
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));


    glBindVertexArray(0);
}


} // namespace crank
