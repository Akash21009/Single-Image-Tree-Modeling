#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/norm.hpp> 
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/rotate_vector.hpp>

int closeToLine(glm::vec3 p, glm::vec3 l1, glm::vec3 l2) {
    // Calculate the direction vector of the line
    glm::vec3 lineDirection = l2 - l1;

    // Calculate the vector from l1 to the point p
    glm::vec3 toPoint = p - l1;

    // Calculate the distance using the formula
    float distance = glm::length(glm::cross(toPoint, lineDirection)) / glm::length(lineDirection);
    

    // Check if the distance is less than or equal to d
    return distance;
}

glm::vec3 closestPointOnLine(glm::vec3 p, glm::vec3 l1, glm::vec3 l2) {
    glm::vec3 lineDirection = glm::normalize(l2 - l1);
    glm::vec3 pointOnLine = l1 + glm::dot(p - l1, lineDirection) * lineDirection;
    return pointOnLine;
}

bool doLineSegmentsIntersect(glm::vec3 p1, glm::vec3 p2, glm::vec3 q1, glm::vec3 q2) {
    glm::vec3 u = p2 - p1;
    glm::vec3 v = q2 - q1;
    glm::vec3 w0 = p1 - q1;

    float a = glm::dot(u, u);
    float b = glm::dot(u, v);
    float c = glm::dot(v, v);
    float d = glm::dot(u, w0);
    float e = glm::dot(v, w0);

    float det = a * c - b * b;

    // Check if the lines are parallel
    if (det == 0.0f) {
        return false;
    }

    float s = (b * e - c * d) / det;
    float t = (a * e - b * d) / det;

    // Check if the intersection point is within the line segments
    if (s >= 0.0f && s <= 1.0f && t >= 0.0f && t <= 1.0f) {
        // Intersection point is within the line segments
        return true;
    }

    // No intersection
    return false;
}

bool compareByY(const glm::vec3& a, const glm::vec3& b) {
    return a.y < b.y;
}

void sortMiddleValues(std::vector<glm::vec3>& v) {
    // Check if there are more than 2 elements in the vector
    if (v.size() > 2) {
        // Exclude the first and last elements from sorting
        std::sort(v.begin() + 1, v.end() - 1, compareByY);
    }
}

float radiansToDegrees(float radians) {
    return radians * (180.0f / M_PI);
}

float degreesToRadians(float degrees) {
    return degrees * (M_PI / 180.0f);
}