#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>
#include <cstdlib> // Include this header for std::rand and std::srand
#include <ctime>   // Include this header for std::time

void DrawSphere(const glm::vec3 &position, float radius, const glm::vec3 &color)
{
    const int stacks = 10;
    const int slices = 10;

    glColor3f(color.r, color.g, color.b);

    for (int i = 0; i <= stacks; ++i)
    {
        float lat0 = glm::pi<float>() * (-0.5 + (float)(i - 1) / stacks);
        float z0 = radius * sin(lat0);
        float zr0 = radius * cos(lat0);

        float lat1 = glm::pi<float>() * (-0.5 + (float)i / stacks);
        float z1 = radius * sin(lat1);
        float zr1 = radius * cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices; ++j)
        {
            float lng = 2 * glm::pi<float>() * (float)(j - 1) / slices;
            float x = cos(lng);
            float y = sin(lng);

            glVertex3f(position.x + x * zr0, position.y + y * zr0, position.z + z0);
            glVertex3f(position.x + x * zr1, position.y + y * zr1, position.z + z1);
        }
        glEnd();
    }
}

void DrawCylinder(const glm::vec3& p1, const glm::vec3& p2, float radius,glm::vec3 color, bool leaves, float leafRadius,int numSpheres, unsigned int seed,glm::vec3 leafColor)
{
    glm::vec3 direction = p2 - p1;
    float height = glm::length(direction);
    glm::vec3 up(0.0f, 1.0f, 0.0f);

    // Calculate rotation angles
    glm::vec3 axis = glm::cross(up, direction);
    float angle = glm::degrees(glm::acos(glm::dot(up, glm::normalize(direction))));

    glPushMatrix();

    // Translate to the center of the bottom circular face (p1)
    glTranslatef(p1.x, p1.y, p1.z);

    // Apply rotations to orient the cylinder correctly
    glRotatef(angle, axis.x, axis.y, axis.z);

    // Draw the cylinder
    const int slices = 10;
    const float step = 2.0f * glm::pi<float>() / slices;

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // glColor3f(0.3f=, 0.2f, 0.1f); // Light brown color for the filled interior
    glColor3f(color.x, color.y, color.z); 

    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices; ++i)
    {
        float angle = i * step;
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        // Bottom vertex
        glVertex3f(x, 0.0f, z);

        // Top vertex
        glVertex3f(x, height, z);
    }
    glEnd();

    // Draw solid lines
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3f(0.0f, 0.0f, 0.0f);

    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices; ++i)
    {
        float angle = i * step;
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        // Bottom vertex
        glVertex3f(x, 0.0f, z);

        // Top vertex
        glVertex3f(x, height, z);
    }
    glEnd();

    glPopMatrix(); // Restore the previous transformation matrix

    if(leaves==true){
        // Populating the surface of the cylinder with spheres
        std::srand(seed);

        // Populating the surface of the cylinder with spheres
        for (int i = 0; i < numSpheres; ++i)
        {
            // Generate a random position on the surface of the cylinder
            float randAngle = static_cast<float>(std::rand()) / RAND_MAX * (2.0f * glm::pi<float>());
            float randHeight = static_cast<float>(std::rand()) / RAND_MAX * height;
            glm::vec3 spherePosition = p1 + glm::normalize(direction) * randHeight +
                                    radius * glm::vec3(cos(randAngle), sin(randAngle), 0.0f);

            // Draw a red sphere of radius 7.0 at the random position
            DrawSphere(spherePosition, leafRadius, leafColor);
        }
    }
    
}

void DrawCuboid(const glm::vec3& p, float length, float height, float width, const glm::vec3& color)
{
    // Calculate half extents
    float halfLength = length / 2.0f;
    float halfHeight = height / 2.0f;
    float halfWidth = width / 2.0f;

    // Define the eight vertices of the cuboid
    glm::vec3 vertices[8] = {
        glm::vec3(p.x - halfLength, p.y - halfHeight, p.z - halfWidth), // 0: bottom-left-back
        glm::vec3(p.x + halfLength, p.y - halfHeight, p.z - halfWidth), // 1: bottom-right-back
        glm::vec3(p.x - halfLength, p.y + halfHeight, p.z - halfWidth), // 2: top-left-back
        glm::vec3(p.x + halfLength, p.y + halfHeight, p.z - halfWidth), // 3: top-right-back
        glm::vec3(p.x - halfLength, p.y - halfHeight, p.z + halfWidth), // 4: bottom-left-front
        glm::vec3(p.x + halfLength, p.y - halfHeight, p.z + halfWidth), // 5: bottom-right-front
        glm::vec3(p.x - halfLength, p.y + halfHeight, p.z + halfWidth), // 6: top-left-front
        glm::vec3(p.x + halfLength, p.y + halfHeight, p.z + halfWidth)  // 7: top-right-front
    };

    // Define the indices for drawing the cuboid
    GLuint indices[] = {
        0, 1, 3, 2, // Back face
        4, 5, 7, 6, // Front face
        0, 2, 6, 4, // Left face
        1, 3, 7, 5, // Right face
        2, 3, 7, 6, // Top face
        0, 1, 5, 4  // Bottom face
    };

    // Draw the filled faces of the cuboid
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    for (int i = 0; i < 6; ++i)
    {
        glBegin(GL_QUADS);
        glColor3f(color.r, color.g, color.b);
        for (int j = 0; j < 4; ++j)
        {
            glVertex3f(vertices[indices[i * 4 + j]].x, vertices[indices[i * 4 + j]].y, vertices[indices[i * 4 + j]].z);
        }
        glEnd();
    }

    // Draw the black lines on the border
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3f(0.0f, 0.0f, 0.0f);

    glBegin(GL_QUADS);
    for (int i = 0; i < 24; ++i)
    {
        glVertex3f(vertices[indices[i]].x, vertices[indices[i]].y, vertices[indices[i]].z);
    }
    glEnd();
}