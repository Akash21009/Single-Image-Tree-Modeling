#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <ctime>

struct Point {
    int x, y;

    // Comparison function for sorting points
    static bool compare(const Point& p1, const Point& p2) {
        return (p1.y < p2.y) || (p1.y == p2.y && p1.x < p2.x);
    }
};

// Function to compute the orientation of three points (p, q, r)
int orientation(const Point& p, const Point& q, const Point& r) {
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;  // colinear
    return (val > 0) ? 1 : 2; // clock or counterclockwise
}

// Function to compute the convex hull of a set of points
std::vector<Point> convexHull(std::vector<Point>& points) {
    int n = points.size();
    if (n < 3) {
        // Convex hull is not possible with less than 3 points
        return std::vector<Point>();
    }

    // Find the point with the lowest y-coordinate (and leftmost if ties)
    int minY = points[0].y, minIndex = 0;
    for (int i = 1; i < n; i++) {
        int y = points[i].y;
        if ((y < minY) || (y == minY && points[i].x < points[minIndex].x)) {
            minY = y;
            minIndex = i;
        }
    }

    // Swap the lowest point with the first point
    std::swap(points[0], points[minIndex]);

    // Sort the remaining points based on polar angle in counterclockwise order
    std::sort(points.begin() + 1, points.end(), [points](const Point& p1, const Point& p2) {
        int o = orientation(points[0], p1, p2);
        if (o == 0) return (points[0].x + points[0].y) < (p1.x + p1.y + p2.x + p2.y);
        return (o == 2);
    });

    // Initialize the convex hull
    std::vector<Point> hull;
    hull.push_back(points[0]);
    hull.push_back(points[1]);

    // Process the remaining points
    for (int i = 2; i < n; i++) {
        while (hull.size() > 1 && orientation(hull[hull.size() - 2], hull[hull.size() - 1], points[i]) != 2) {
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }

    return hull;
}

// Function to check if a point is on the boundary of the convex hull
bool isOnBoundary(const std::vector<Point>& hull, const Point& point) {
    for (const Point& hullPoint : hull) {
        if (point.x == hullPoint.x && point.y == hullPoint.y) {
            return true;
        }
    }
    return false;
}

// Function to check if a point is inside the convex hull
bool isPointInsideConvexHull(const std::vector<Point>& convexHull, const Point& point) {
    // Check if the point is on the boundary of the convex hull
    if (isOnBoundary(convexHull, point)) {
        return false;
    }

    // Check if the point is to the left of all edges of the convex hull
    int count = 0;
    int n = convexHull.size();

    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;

        if ((convexHull[i].y > point.y) != (convexHull[j].y > point.y)) {
            double slope = static_cast<double>(convexHull[j].y - convexHull[i].y) / (convexHull[j].x - convexHull[i].x);
            double xIntersection = static_cast<double>(point.y - convexHull[i].y) / slope + convexHull[i].x;

            if (point.x < xIntersection) {
                ++count;
            }
        }
    }

    return count % 2 == 1;
}

bool isPointOutsideConvexHull(const std::vector<Point>& convexHull, const Point& point) {
    // Check if the point is on the boundary of the convex hull
    if (isOnBoundary(convexHull, point)) {
        return false;
    }

    // Check if the point is to the left of all edges of the convex hull
    int count = 0;
    int n = convexHull.size();

    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;

        if ((convexHull[i].y > point.y) != (convexHull[j].y > point.y)) {
            double slope = static_cast<double>(convexHull[j].y - convexHull[i].y) / (convexHull[j].x - convexHull[i].x);
            double xIntersection = static_cast<double>(point.y - convexHull[i].y) / slope + convexHull[i].x;

            if (point.x < xIntersection) {
                ++count;
            }
        }
    }

    return count % 2 == 0;
}