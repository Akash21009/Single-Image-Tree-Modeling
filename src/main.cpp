#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/norm.hpp> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <map>
#include <algorithm>
#include "convex_hull.h"
#include "mesh.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <random>
#include <cstdlib>
#include <ctime>
#include "basic_math.h"


#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

/*
image should be place in images folder
run on terminal by command ./project <imagenamewithextention>
press "T" to toogle sketch mode

first is branchmode:
Draw strokes on visible branches. Remeber the first click should be root coordinate. You can draw straight line strokes by clicking two times to determine a single branch. Don't procced with only single point on screen because it will consider another point at infinity and brach can't be generated for that. After you done press "T" to change mode.

second is crownmode:
Place anchor points on boundary of foliage region via mouse clicks. Keep number of anchors optimum which is enough to determine foliage region. To many anchors can lead to rigorous branch rotation in z axes.After you done press "T" to change mode.

third is sketchtreemode:
You can view your generated tree with leaves. If you want to view only branches press "O" to toggle. You can toggle visibility of main branches via pressing "1", subbranches via pressing "2", subsubbranches via pressing "3" and cuboidal land via pressing "4";
*/

struct Vec3Compare //to compare glm::vec3
{
    bool operator()(const glm::vec3& a, const glm::vec3& b) const {
        // You can customize the comparison logic based on your requirements
        if (a.x != b.x) return a.x < b.x;
        if (a.y != b.y) return a.y < b.y;
        return a.z < b.z;
    }
};

std::vector<float> branchCoordinatesSketch; //store stokes coordinates sketch by user
std::vector<float> branchCoordinatesDisplay; //store normalized strokes coordinates sketch by user as per image dimentions
std::vector<float> anchorPoints; //store branch coordinates sketch by user
std::vector<float> anchorPointsDisplay; //store normalized branch coordinates sketch by user as per image dimentions

std::vector<std::vector<glm::vec3>> branches; //store branches coordinates as pair
std::vector<std::vector<glm::vec3>> branchesProcessed; //store set of nodes lie in single branch
std::map<glm::vec3,std::vector<glm::vec3>, Vec3Compare> nodes; //map which consist each tree node pointing to it's outward connected nodes(those who contributed to outdegree)
std::map<glm::vec3,std::vector<glm::vec3>, Vec3Compare> libNodes; //temperorily storing nodes data from tree library
std::vector<std::map<glm::vec3, std::vector<glm::vec3>, Vec3Compare>> sBranches; //subbranches
std::vector<std::map<glm::vec3, std::vector<glm::vec3>, Vec3Compare>> ssBranches; //subsubbranches
std::vector<Point> convexHullPoints; //boundry points of foliage region
glm::vec3 root; //root coordinate of tree
glm::vec3 libRoot;//temperorily storing root data from tree library

std::vector<std::string> lib={"b1.txt","b2.txt","b3.txt","b4.txt","b5.txt"}; //for accessing library
std::vector<glm::vec3> rPlane={glm::vec3(1.0,0.0,0.0),glm::vec3(0.0,1.0,0.0),glm::vec3(0.0,0.0,1.0),glm::vec3(1.0,1.0,0.0),glm::vec3(1.0,0.0,1.0),glm::vec3(0.0,1.0,1.0),glm::vec3(1.0,1.0,1.0),}; //for generating random roation plane
std::vector<float> rAngle={30.0,-30.0, 45.0,-45.0, 60.0, -60.0, 90.0,-90.0,120.0,-120.0,}; //for generating random roation angle



unsigned int leafSeed = static_cast<unsigned int>(std::time(0)); //for generationg leaves on random position
bool rot=true; //apply 3d roation of branches
bool showBranchesOnly=false; //show only branches(branches,subbranches and subsubbranches will have diffrent color) or branches with leaves; can be toggle via pressing "O"
bool visiblebranches=true; //show main branches; toggle by pressing "1"
bool visiblesbranches=true; //show subbranches; toggle by pressing "2"
bool visiblessbranches=true; //show subsubbranches; toggle by pressing "3"
bool visibleland=true; //show subsubbranches; toggle by pressing "4"

int sketchMode = 0; // Flag to indicate sketch mode(0-stroke mode, 1-foliage mode, 2-tree mode)

void keyCallback( GLFWwindow *window, int key, int scancode, int action, int mods ); //accessing keyborad
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods); //accesing mouse
void branchMode(GLFWwindow* window, const std::string& imagePath); //sketch sktokes to determine visible branch/trunk
void crownMode(GLFWwindow* window, const std::string& imagePath); //determine foliage reagion
void treeSketchingMode(GLFWwindow* window); //sketch 3d tree
void writeNodesToFile( const std::string& filename); //to upload new tree in library(only branches)
void readFromFile(const std::string& filename); //access tree library
void rotateBranches(glm::vec3 n); //apply 3d roation in tree
void processNodes(std::vector<std::vector<glm::vec3>> branches); //generate 3d roated nodes
std::map<glm::vec3, std::vector<glm::vec3>, Vec3Compare> transformTree(const std::map<glm::vec3, std::vector<glm::vec3>, Vec3Compare>& nodes, const glm::vec3& translation, const glm::vec3& scale, const glm::mat4& rotationMatrix, const glm::vec3& rotationPoint); //transform whole tree around single point
void transformNode(std::vector<glm::vec3>& node, const glm::vec3& translation, const glm::vec3& scale, const glm::mat4& rotationMatrix, const glm::vec3& rotationPoint); //transform node around single point
void toggleSketchMode(); //switch sketch mode via pressing "T"

//CAMERA parameters
GLfloat rotationX = 0.0f;
GLfloat rotationY = 0.0f;
GLfloat zoom = 1.0f;
GLfloat translateX = 0.0;
GLfloat translateY = 0.0;

//main
int main(int argc, char* argv[])
{
    if (argc != 2) //if image path is provided
    {
        std::cerr << "Usage: " << argv[0] << " <image_path>\n";
        return -1;
    }


    const char* imageName = argv[1];
    std::string imagePath = "images/" + std::string(imageName);

    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit())
    {
        return -1;
    }

    if (sketchMode == 0)
    {
        branchMode(window, imagePath.c_str());
    }
    

    if (sketchMode == 1)
    {
        crownMode(window, imagePath.c_str());
    }
    
    //populate convexHullPoints
    std::vector<Point> points;
    int x, y;
    

    std::ifstream f("textfiles/foliage.txt");

    if (!f.is_open())
    {
        std::cerr << "Unable to open the file for reading.\n";
        return 1;
    }

    int anchorIndex = 0;
    

    std::string line2;
    while (std::getline(f, line2))
    {
        if (line2.find("anchor") != std::string::npos)
        {
            // Parse anchor coordinates
            std::istringstream iss(line2.substr(line2.find(":") + 1));
            iss >> x >> y;
            points.push_back({x, y});
            
        }
    }

    f.close();

    // Compute the convex hull
    convexHullPoints = convexHull(points);

    
    
    if (sketchMode == 2)
    {
    std::ifstream inFile("textfiles/branches.txt");
    if (!inFile.is_open())
    {
        std::cerr << "Unable to open the file for reading.\n";
        
    }

    glm::vec3 p1, p2;
    std::string line;
    bool base=false;
    while (std::getline(inFile, line))
    {
        if (line.find("Stroke") != std::string::npos)
        {
            if (line.find("Stroke 1") != std::string::npos)
            {
                base=true;
                 
            }
            else{
                base=false;
            }
        }
        else if (line.find("Point 1:") != std::string::npos)
        {
            // Parse Point 1 coordinates
            std::istringstream iss(line.substr(line.find(":") + 1));
            iss >> p1.x >> p1.y >> p1.z;
            if(base==true){
                root=p1; //storing root data
            }
        }
        else if (line.find("Point 2:") != std::string::npos)
        {
            
            std::istringstream iss(line.substr(line.find(":") + 1));
            iss >> p2.x >> p2.y >> p2.z;
            if(p1.y>p2.y)
            {
                glm::vec3 p=p1;
                p1=p2;
                p2=p;
            }
            std::vector<glm::vec3> v;
            v.push_back(p1);
            v.push_back(p2);

            
            branches.push_back(v);

        }
        
    }
    inFile.close();

    processNodes(branches);  
    treeSketchingMode(window);

    }

    glfwTerminate();

    return 0;
}



void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    const GLfloat rotationSpeed = 10.0f;
    const GLfloat zoomSpeed = 0.1f;
    const GLfloat translationSpeed = 10.0f;

    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
        case GLFW_KEY_UP:
            rotationX -= rotationSpeed;
            break;
        case GLFW_KEY_DOWN:
            rotationX += rotationSpeed;
            break;
        case GLFW_KEY_RIGHT:
            rotationY += rotationSpeed;
            break;
        case GLFW_KEY_LEFT:
            rotationY -= rotationSpeed;
            break;
        case GLFW_KEY_Z: // Zoom in
            zoom += zoomSpeed;
            break;
        case GLFW_KEY_X: // Zoom out
            zoom = glm::max(0.1f, zoom - zoomSpeed);
            break;
        case GLFW_KEY_A: // Pan left
            translateX += translationSpeed;
            break;
        case GLFW_KEY_D: // Pan right
            translateX -= translationSpeed;
            break;
        case GLFW_KEY_W: // Pan Up
            translateY -= translationSpeed;
            break;
        case GLFW_KEY_S: // Pan Down
            translateY += translationSpeed;
            break;
        case GLFW_KEY_O: // toggle to show only branches
            showBranchesOnly=!showBranchesOnly;
            break;
        case GLFW_KEY_1: 
            visiblebranches=!visiblebranches;
            break;
        case GLFW_KEY_2: 
            visiblesbranches=!visiblesbranches;
            break;
        case GLFW_KEY_3: 
            visiblessbranches=!visiblessbranches;
            break;
        case GLFW_KEY_4: 
            visibleland=!visibleland;
            break;
        case GLFW_KEY_T: // Toggle sketch mode
            toggleSketchMode();
             glfwSetWindowShouldClose(window, GLFW_TRUE); 
            break;
        }
    }
}



void toggleSketchMode()
{
    
    sketchMode+=1;
    if(sketchMode==3){
        sketchMode=0;
    }
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (sketchMode==0 && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        

        double xpos, ypos;
        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        glfwGetCursorPos(window, &xpos, &ypos);

        // Store the anchor point
        branchCoordinatesSketch.push_back((float)xpos);
        branchCoordinatesSketch.push_back(SCREEN_HEIGHT - (float)ypos);
        branchCoordinatesSketch.push_back(0.0);

        // Normalize the coordinates to the range [-1, 1]
        float normalizedX = static_cast<float>(2 * xpos / SCREEN_WIDTH - 1);
        float normalizedY = static_cast<float>(1 - 2 * ypos / SCREEN_HEIGHT);

        // display the anchor point
        branchCoordinatesDisplay.push_back(normalizedX);
        branchCoordinatesDisplay.push_back(normalizedY);
    }

     if (sketchMode == 1 && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        glfwGetCursorPos(window, &xpos, &ypos);

        anchorPoints.push_back((float)xpos);
        anchorPoints.push_back(SCREEN_HEIGHT - (float)ypos);

        // Normalize the coordinates to the range [-1, 1]
        float normalizedX = static_cast<float>(2 * xpos / SCREEN_WIDTH - 1);
        float normalizedY = static_cast<float>(1 - 2 * ypos / SCREEN_HEIGHT);

        // Store the anchor point
        anchorPointsDisplay.push_back(normalizedX);
        anchorPointsDisplay.push_back(normalizedY);

        
    }

}


void transformNode(std::vector<glm::vec3>& node, const glm::vec3& translation, const glm::vec3& scale, const glm::mat4& rotationMatrix, const glm::vec3& rotationPoint) {
    // Reverse translate the node so that the rotation point becomes the origin
    for (glm::vec3& coordinate : node) {
        coordinate -= rotationPoint;
    }

    // Apply translation, scale, and rotation to each coordinate in the node
    for (glm::vec3& coordinate : node) {
        // Apply translation
        coordinate += translation;

        // Apply scale
        coordinate = glm::vec3(glm::scale(glm::mat4(1.0f), scale) * glm::vec4(coordinate, 1.0f));

        // Apply rotation
        coordinate = glm::vec3(rotationMatrix * glm::vec4(coordinate, 1.0f));
    }

    // Translate the node back to the original position
    for (glm::vec3& coordinate : node) {
        coordinate += rotationPoint;
    }
}


std::map<glm::vec3, std::vector<glm::vec3>, Vec3Compare> transformTree(const std::map<glm::vec3, std::vector<glm::vec3>, Vec3Compare>& nodes, const glm::vec3& translation, const glm::vec3& scale, const glm::mat4& rotationMatrix, const glm::vec3& rotationPoint) {
    std::map<glm::vec3, std::vector<glm::vec3>, Vec3Compare> transformedTree;

    for (const auto& pair : nodes) {
        // Copy the key and value
        glm::vec3 transformedKey = pair.first;
        std::vector<glm::vec3> transformedValue = pair.second;

        // Apply transformation to the key
        transformedKey += translation;
        transformedKey = glm::vec3(glm::scale(glm::mat4(1.0f), scale) * glm::vec4(transformedKey - rotationPoint, 1.0f)) + rotationPoint;
        transformedKey = glm::vec3(rotationMatrix * glm::vec4(transformedKey - rotationPoint, 1.0f)) + rotationPoint;

        // Apply transformation to the value
        transformNode(transformedValue, translation, scale, rotationMatrix, rotationPoint);

        // Insert the transformed key and value into the new map
        transformedTree[transformedKey] = transformedValue;
    }

    return transformedTree;
}

void writeNodesToFile( const std::string& filename) {
    std::ofstream outputFile(filename);

    // Check if the file is opened successfully
    if (!outputFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    // Write coordinates of root to the file
    outputFile << "Root: " << root.x << " " << root.y << " " << root.z << std::endl;

    // Write key-value pairs to the file
    for (const auto& entry : nodes) {
        const glm::vec3& key = entry.first;
        const std::vector<glm::vec3>& values = entry.second;

        // Write key
        outputFile <<"key: "<< key.x << " " << key.y << " " << key.z<<"\n";

        // Write values
        for (const auto& value : values) {
            outputFile << "value: " << value.x << " " << value.y << " " << value.z << "\n";
        }
    }

    // Close the file
    outputFile.close();
}

void readFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    glm::vec3 key;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        iss >> token;
        
        if (token == "Root:") {
            iss >> libRoot.x >> libRoot.y >> libRoot.z;
        } else if (token == "key:") {
            iss >> key.x >> key.y >> key.z;
        }
        else if(token == "value:"){
            glm::vec3 v;
            iss >> v.x >> v.y >> v.z;
            if(libNodes.find(key)==libNodes.end()){
                std::vector<glm::vec3> val;
                val.push_back(v);
                libNodes[key]=val;
            }
            else{
                libNodes[key].push_back(v);
            }
        }
    }

    file.close();
}




void branchMode(GLFWwindow *window, const std::string& imagePath){
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "branch mode", NULL, NULL);

    // Set the mouse button callback
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glfwSetKeyCallback( window, keyCallback );
    glfwSetInputMode( window, GLFW_STICKY_KEYS, 1 );
    
    
    int screenWidth, screenHeight;
    glfwGetFramebufferSize( window, &screenWidth, &screenHeight );

    if ( !window )
    {
        glfwTerminate( );
        return;
        
    }
    
    // Make the window's context current
    glfwMakeContextCurrent( window );

    float vertices[] = {
        1.0, 1.0, 0.0,  // top right corner
        -1.0, 1.0, 0.0, // top left corner
        -1.0, -1.0, 0.0, // bottom left corner
        1.0, -1.0, 0.0 // bottom right corner
    };

    float texCoords[] = {
        1.0, 0.0, // top right corner
        0.0, 0.0, // top left corner
        0.0, 1.0, // bottom left corner
        1.0, 1.0  // bottom right corner
    };

    // Load texture
    int width, height, channels;
    unsigned char* image = stbi_load(imagePath.c_str(), &width, &height, &channels, 0);

    if (!image)
    {
        fprintf(stderr, "Failed to load image from path: %s\n", imagePath.c_str());
        glfwTerminate();
        return;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    stbi_image_free(image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {

        
        glClear(GL_COLOR_BUFFER_BIT);

        // Render image
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
        glVertexPointer(3, GL_FLOAT, 0, vertices);

        // Set color for the image
        glColor3f(1.0f, 1.0f, 1.0f); // White
        glDrawArrays(GL_QUADS, 0, 4);

        // Render anchor points
        
        glEnableClientState(GL_VERTEX_ARRAY);

        // Set color for the anchor points
       glColor3f(1.0f, 0.55f, 0.0f); // Dark Orange
        glVertexPointer(2, GL_FLOAT, 0, branchCoordinatesDisplay.data());
        glPointSize(7.0f);
        glDrawArrays(GL_POINTS, 0, branchCoordinatesDisplay.size() / 2);
       

        // Render odd solid strokes between anchor points
        glColor3f(1.0f, 1.0f, 0.0f); // Pink
        glBegin(GL_LINES);
        for (size_t i = 1; i < branchCoordinatesDisplay.size(); i += 4) // Draw every odd line
        {
            glVertex2f(branchCoordinatesDisplay[i - 1], branchCoordinatesDisplay[i]);
            glVertex2f(branchCoordinatesDisplay[i + 1], branchCoordinatesDisplay[i + 2]);
        }
        glEnd();
        

            // Open the file outside the loop
            std::ofstream outFile("textfiles/branches.txt");

            // Draw only odd-numbered lines
            for (int i = 0; i < branchCoordinatesSketch.size() / 3; i += 2)
            {
                if (outFile.is_open()) {
                    int cylinderIndex = i / 2;  // Use a separate counter for cylinder vertices
                    outFile << "Stroke " << cylinderIndex + 1 << ":\n";
                    outFile << "Point 1: " << branchCoordinatesSketch[i * 3] << " " << branchCoordinatesSketch[i * 3 + 1] << " " << "-500" << "\n";
                    outFile << "Point 2: " << branchCoordinatesSketch[i * 3 + 3] << " " << branchCoordinatesSketch[i * 3 + 4] << " " << "-500" << "\n";
                    outFile << "\n";
                } else {
                    std::cerr << "Unable to open the file for writing.\n";
                }
            }
            

            // Close the file after the loop
            outFile.close();

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);

        glDisable(GL_TEXTURE_2D);

        
        glfwSwapBuffers(window);

        
        glfwPollEvents();
    }
}

void crownMode(GLFWwindow *window, const std::string& imagePath){
     
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Crown Mode", NULL, NULL);

    
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback( window, keyCallback );
    glfwSetInputMode( window, GLFW_STICKY_KEYS, 1 );
    
    
    int screenWidth, screenHeight;
    glfwGetFramebufferSize( window, &screenWidth, &screenHeight );

    if ( !window )
    {
        glfwTerminate( );
        return;
    }
    
    
    glfwMakeContextCurrent( window );


    float vertices[] = {
        1.0, 1.0, 0.0,  // top right corner
        -1.0, 1.0, 0.0, // top left corner
        -1.0, -1.0, 0.0, // bottom left corner
        1.0, -1.0, 0.0 // bottom right corner
    };

    float texCoords[] = {
        1.0, 0.0, // top right corner
        0.0, 0.0, // top left corner
        0.0, 1.0, // bottom left corner
        1.0, 1.0  // bottom right corner
    };

    // Load texture
    int width, height, channels;
    
    unsigned char* image = stbi_load(imagePath.c_str(), &width, &height, &channels, 0);

    if (!image)
    {
        fprintf(stderr, "Failed to load image from path: %s\n", imagePath.c_str());
        glfwTerminate();
        return ;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    stbi_image_free(image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
  


    
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // Render image
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
        glVertexPointer(3, GL_FLOAT, 0, vertices);

        // Set color for the image
        glColor3f(1.0f, 1.0f, 1.0f); // White
        glDrawArrays(GL_QUADS, 0, 4);

        // Render anchor points
        glEnableClientState(GL_VERTEX_ARRAY);

        // Set color for the anchor points
        glColor3f(1.0f, 0.0f, 0.0f); // Green
        glVertexPointer(2, GL_FLOAT, 0, anchorPointsDisplay.data());
        glPointSize(5.0f);
        glDrawArrays(GL_POINTS, 0, anchorPointsDisplay.size() / 2);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);

        glDisable(GL_TEXTURE_2D);

        glfwSwapBuffers(window);

        glfwPollEvents();

         // Save points to foliage.txt
        std::ofstream foliageFile("textfiles/foliage.txt");
        if (foliageFile.is_open()) {
            for (int i = 0; i < anchorPoints.size(); i+=2) {
                foliageFile << "anchor " << i/2 + 1 << ": " << anchorPoints[i] << " " << anchorPoints[i+1] << " " <<  "-500" <<"\n";
            }
            foliageFile.close();
        } else {
            std::cerr << "Unable to open foliage.txt for writing.\n";
        }

        }
}

void treeSketchingMode(GLFWwindow *window){
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tree Generation", NULL, NULL);

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback( window, keyCallback );
    glfwSetInputMode( window, GLFW_STICKY_KEYS, 1 );
    
    
    int screenWidth, screenHeight;
    glfwGetFramebufferSize( window, &screenWidth, &screenHeight );

    if ( !window )
    {
        glfwTerminate( );
        return;
    }
    
    
    glfwMakeContextCurrent( window );
    glViewport( 0.0f, 0.0f, screenWidth, screenHeight ); // specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
    glMatrixMode( GL_PROJECTION ); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
    glLoadIdentity( ); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
    glOrtho( 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1000 ); // essentially set coordinate system
    glMatrixMode( GL_MODELVIEW ); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
    glLoadIdentity( ); // same as above comment
    
    GLfloat halfScreenWidth = SCREEN_WIDTH / 2;
    GLfloat halfScreenHeight = SCREEN_HEIGHT / 2;


    while (!glfwWindowShouldClose(window))
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
            // Draw cylinders
            glLoadIdentity(); // Reset the modelview matrix

        // Apply transformations
        glTranslatef(halfScreenWidth + translateX, halfScreenHeight + translateY, -500);
        glScalef(zoom, zoom, zoom); // Apply zoom
        glRotatef(rotationX, 1, 0, 0);
        glRotatef(rotationY, 0, 1, 0);
        glTranslatef(-halfScreenWidth, -halfScreenHeight, 500);


    // Calculate and print angles between vectors in the xy-plane
    glm:: vec3 branchColor;
    glm:: vec3 sbranchColor;
    glm:: vec3 ssbranchColor;

    if(showBranchesOnly){
        branchColor=glm::vec3(0.0f, 0.0f, 1.0f);
        sbranchColor=glm::vec3(0.0f, 1.0f, 0.0f);
        ssbranchColor=glm::vec3(1.0f, 0.0f, 0.0f);
    }
    else{
        branchColor=glm::vec3(0.3f, 0.2f, 0.1f);
        sbranchColor=glm::vec3(0.3f, 0.2f, 0.1f);
        ssbranchColor=glm::vec3(0.3f, 0.2f, 0.1f);
    }

    if(visiblebranches){
        for (auto& n : nodes) {

            for(auto& n2:n.second){
                bool leaves=false;
                Point p;
                Point p2;
                p.x=n.first.x;
                p.y=n.first.y;
                p2.x=n2.x;
                p2.y=n2.y;
                if(nodes.find(n2)==nodes.end() && showBranchesOnly==false){
                    leaves=true;
                }
                DrawCylinder(n.first,n2,10.0f,branchColor,leaves,10.0f,20,leafSeed,glm::vec3(0.0,0.6,0.0));
            }
        }
    }

    
    if(visiblesbranches){
        for(auto& transformedNodes:sBranches){
            for (auto& n : transformedNodes) {
                for(auto& n2:n.second){
                    DrawCylinder(n.first,n2,6.0f,sbranchColor,!showBranchesOnly,8.0f,8,leafSeed,glm::vec3(0.0,0.5,0.0));
                }
            }

        }
    }

    if(visiblessbranches){
        for(auto& transformedNodes:ssBranches){
            for (auto& n : transformedNodes) {
                for(auto& n2:n.second){
                    DrawCylinder(n.first,n2,2.5f,ssbranchColor,!showBranchesOnly,4.0f,5,leafSeed,glm::vec3(0.0,0.3,0.0));
                }
            }

        }
    }

    
    
    

    if(visibleland) DrawCuboid(root, 700.0f, 30.0f, 500.0f, glm::vec3(0.6f, 0.4f, 0.2f));



    // Draw lines along the axes
    glColor3f(1.0f, 0.0f, 0.0f); // Red color for X-axis
    glBegin(GL_LINES);
    glVertex3f(halfScreenWidth, halfScreenHeight, -500);
    glVertex3f(halfScreenWidth + 20.0f, halfScreenHeight, -500);
    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f); // Green color for Y-axis
    glBegin(GL_LINES);
    glVertex3f(halfScreenWidth, halfScreenHeight, -500);
    glVertex3f(halfScreenWidth, halfScreenHeight + 20.0f, -500);
    glEnd();

    glColor3f(0.0f, 0.0f, 1.0f); // Blue color for Z-axis
    glBegin(GL_LINES);
    glVertex3f(halfScreenWidth, halfScreenHeight, -500);
    glVertex3f(halfScreenWidth, halfScreenHeight, -500 - 20.0f);
    glEnd();

        
        glfwSwapBuffers( window );
        

        glfwPollEvents( );
        
      
    }
}


//process nodes
void processNodes(std::vector<std::vector<glm::vec3>> branches){
    float closeness=15.0f;

    for(int i=0;i<branches.size()-1;i++){
        bool c=false;
        for(int j=i+1;j<branches.size();j++){
                if(closeToLine(branches[j][0],branches[i][0],branches[i][1])<=closeness ){
                    // glm::vec3 cp=closestPointOnLine(branches[j][0],branches[i][0],branches[i][1]);
                    c=true;
                    branches[i].insert(branches[i].begin() + 1, branches[j][0]);
                    
                }
                else if(closeToLine(branches[j][1],branches[i][0],branches[i][1])<=closeness ){
                    // glm::vec3 cp=closestPointOnLine(branches[j][1],branches[i][0],branches[i][1]);
                    c=true;
                    branches[i].insert(branches[i].begin() + 1, branches[j][1]);
                    
                }
        
            }
        
    }
    


    for(int i=0;i<branches.size();i++){
        sortMiddleValues(branches[i]);
        for(int j=0;j<branches[i].size()-1;j++){
            std::vector<glm::vec3> v;
            v.push_back(branches[i][j]);
            v.push_back(branches[i][j+1]);
            branchesProcessed.push_back(v);

        }
    }

    

    for (const auto& branch : branchesProcessed) {
        if (!branch.empty()) {
            glm::vec3 sourceNode = branch.front();
            glm::vec3 targetNode = branch.back();
            
            // If sourceNode is not already in the map, insert it with an empty vector
            if (nodes.find(sourceNode) == nodes.end()) {
                nodes[sourceNode] = std::vector<glm::vec3>();
            }

            // Add targetNode to the vector associated with sourceNode
            nodes[sourceNode].push_back(targetNode);
        }
    }

    //mainbranches cover convex hull
    for(int i=0;i<convexHullPoints.size();i++){
        float minDistance = std::numeric_limits<float>::max();
        glm::vec3 v=glm::vec3(convexHullPoints[i].x,convexHullPoints[i].y,-500.0);
        glm::vec3 c=glm::vec3(0.0);
        for(auto& n : nodes){
            float distance = glm::distance(v, n.first);
            Point p;
            p.x=n.first.x;
            p.y=n.first.y;
            if (distance < minDistance && !isOnBoundary(convexHullPoints,p) && n.first!=root)
            {
                minDistance = distance;
                c=n.first;
            }
            for(auto& l : n.second){
                distance = glm::distance(v, l);
                p.x=l.x;
                p.y=l.y;
                if (distance < minDistance && !isOnBoundary(convexHullPoints,p) )
                {
                    minDistance = distance;
                    c=l;
                }
            }            
        }

        auto it=nodes.find(c);
        if(it==nodes.end()){
            std::vector<glm::vec3> vec;
            vec.push_back(v);
            nodes[c]=vec;
        }
        else{
            if(nodes[c].size()==1){
                nodes[c].push_back(v);
            }
            else{
                glm::vec3 m1 = (c + nodes[c][0]) / 2.0f;
                glm::vec3 m2 = (c + nodes[c][1]) / 2.0f;
                float d1 = glm::distance(v, m1);
                float d2 = glm::distance(v, m2);
                std::vector<glm::vec3> vec;

                if(d1<d2){
                    std::vector<glm::vec3> nw;
                    std::vector<glm::vec3> tem=nodes[c];
                    glm::vec3 temvec=c;

                    nodes.erase(c);
                    nw.push_back(m1);
                    nw.push_back(tem[1]);
                    nodes[temvec]=nw;

                    vec.push_back(tem[0]);
                    vec.push_back(v);
                    nodes[m1]=vec;
                }
                else{
                    std::vector<glm::vec3> nw;
                    std::vector<glm::vec3> tem=nodes[c];
                    glm::vec3 temvec=c;

                    nodes.erase(c);
                    nw.push_back(tem[0]);
                    nw.push_back(m2);
                    nodes[temvec]=nw;

                    vec.push_back(tem[1]);
                    vec.push_back(v);
                    nodes[m2]=vec;
                }

            }
        }
    }

    //rotate brnches in z space 
    if(rot){
        rotateBranches(root);
    }

    // writeNodesToFile("textfiles/branchLibrary/b5.txt");

    //subbranch generation
    for(const auto& n : nodes){
            Point p1;
            p1.x=n.first.x;
            p1.y=n.first.y;
        for(const auto& n2: n.second){
            Point p2;
            p2.x=n2.x;
            p2.y=n2.y;
            if(!isPointOutsideConvexHull(convexHullPoints,p1) && !isPointOutsideConvexHull(convexHullPoints,p2)){
                glm::vec3 m = (n.first + n2) / 2.0f;
                glm::vec3 m1 = (n.first + m) / 2.0f;
                glm::vec3 m2 = (n2 + m) / 2.0f;
                glm::vec3 m3 = (n.first + m1) / 2.0f;
                glm::vec3 m4 = (m1 + m) / 2.0f;
                glm::vec3 m5 = (m + m2) / 2.0f;
                glm::vec3 m6 = (m2 + n2) / 2.0f;

                std::vector mp={m1,m2,m3,m4,m5,m6};

                for(int j=0;j<6;j++){
                    m=mp[j];
                    std::map<glm::vec3, std::vector<glm::vec3>, Vec3Compare> transformedNodes;
                    std::random_device rd;
                    std::random_device rd2;
                    std::mt19937 gen(rd());
                    std::mt19937 gen2(rd2());

                    // Create a uniform distribution for the indices of the vector
                    std::uniform_int_distribution<> distribution(0, rPlane.size() - 1);
                    std::uniform_int_distribution<> distribution2(0, rAngle.size() - 1);

                    // Generate a random index
                    int randomIndex = distribution(gen);
                    int randomIndex2 = distribution2(gen2);

                    glm::vec3 rv=rPlane[randomIndex];

                    float s=0.4;
                    if(j>=2) s=0.2;

                    int iend=lib.size();
                    int i=0;

                    while(i<iend){
                        libNodes.clear();

                    
                        if(i>0) readFromFile("textfiles/branchLibrary/"+lib[i-1]);
                        
                        glm::vec3 translation;

                        if(i>0) translation=glm::vec3(m.x-libRoot.x, m.y-libRoot.y, m.z-libRoot.z);
                        else translation=glm::vec3(m.x-root.x, m.y-root.y, m.z-root.z);
                        glm::vec3 scale(s, s, s); 
                        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rAngle[randomIndex2]), rv);

                        if(i==0) transformedNodes = transformTree(nodes, translation, scale, rotationMatrix,m);
                        else transformedNodes = transformTree(libNodes, translation, scale, rotationMatrix,m);

                        bool inside=true;

                        for(auto& t:transformedNodes){
                            for(auto& t2:t.second){
                                Point p3;
                                p3.x=t2.x;
                                p3.y=t2.y;
                                if(isPointOutsideConvexHull(convexHullPoints,p3)){
                                    inside=false;
                                    break;
                                }
                            }
                            if(inside==false) break;
                        }


                        if(inside==true){
                            sBranches.push_back(transformedNodes);
                            break;
                        }
                        i++;
                    }

                    if(j>=2 && nodes.find(n2)!=nodes.end()){
                        break;
                    }
                }             
            }
        }
        
    }

    //subsubbranch generation
for(auto& tnodes:sBranches){
        for(const auto& n : tnodes){
            Point p1;
            p1.x=n.first.x;
            p1.y=n.first.y;
        for(const auto& n2: n.second){
            Point p2;
            p2.x=n2.x;
            p2.y=n2.y;
            if(!isPointOutsideConvexHull(convexHullPoints,p1) && !isPointOutsideConvexHull(convexHullPoints,p2)){
                glm::vec3 m = (n.first + n2) / 2.0f;
                
                    std::map<glm::vec3, std::vector<glm::vec3>, Vec3Compare> transformedNodes;
                    std::random_device rd;
                    std::random_device rd2;
                    std::mt19937 gen(rd());
                    std::mt19937 gen2(rd2());

                    // Create a uniform distribution for the indices of the vector
                    std::uniform_int_distribution<> distribution(0, rPlane.size() - 1);
                    std::uniform_int_distribution<> distribution2(0, rAngle.size() - 1);

                    // Generate a random index
                    int randomIndex = distribution(gen);
                    int randomIndex2 = distribution2(gen2);

                    glm::vec3 rv=rPlane[randomIndex];

                    float s=0.15;

                    int iend=lib.size();
                    int i=0;
                    while(i<iend){
                        libNodes.clear();
                        glm::vec3 translation;
                        if(i>0) readFromFile("textfiles/branchLibrary/"+lib[i-1]);
                        if(i>0) translation=glm::vec3(m.x-libRoot.x, m.y-libRoot.y, m.z-libRoot.z);
                        else translation=glm::vec3(m.x-root.x, m.y-root.y, m.z-root.z);
                        glm::vec3 scale(s, s, s); 
                        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rAngle[randomIndex2]), rv);
                        if(i>0) transformedNodes = transformTree(libNodes, translation, scale, rotationMatrix,m);
                        else transformedNodes = transformTree(nodes, translation, scale, rotationMatrix,m);

                        bool inside=true;

                        for(auto& t:transformedNodes){
                            for(auto& t2:t.second){
                                Point p3;
                                p3.x=t2.x;
                                p3.y=t2.y;
                                if(isPointOutsideConvexHull(convexHullPoints,p3)){
                                    inside=false;
                                    break;
                                }
                            }
                            if(inside==false) break;
                        }


                        if(inside==true){
                            ssBranches.push_back(transformedNodes);
                            break;
                        }
                        i++;
                    }

            }
        }
        
    }
}
    
}

//roatate branches
void rotateBranches(glm::vec3 n){
    // std::cout<<n.x<<" "<<n.y<<" "<<n.z<<std::endl;
    auto it = nodes.find(n);
    if (it == nodes.end()) {
        // std::cout<<"1"<<std::endl;
        return;
    } 
    if(nodes[n].size()<2){
        nodes[n][0].z=n.z;
        // std::cout<<"2"<<std::endl;
        rotateBranches(nodes[n][0]);
        return;
    }

    
    // std::cout<<"3"<<std::endl;
    nodes[n][0].z=n.z;
    nodes[n][1].z=n.z;

    glm::vec3 vector1 = nodes[n][0] - n;
    glm::vec3 vector2 = nodes[n][1] - n;

    // Calculate angle in xy-plane
    float ag_xy = glm::orientedAngle(glm::normalize(glm::vec2(vector1.x, vector1.y)),
                                    glm::normalize(glm::vec2(vector2.x, vector2.y)));

    float degrees = radiansToDegrees(ag_xy);

    glm::vec3 rotatedVector;
    glm::vec3 rotatedVector2;

    
    rotatedVector = glm::rotate(nodes[n][0], degreesToRadians(degrees), nodes[n][1]);
    rotatedVector2 = glm::rotate(nodes[n][1], degreesToRadians(degrees), nodes[n][0]);
    


    glm::vec3 old0=nodes[n][0];
    old0.z=-500;
    glm::vec3 old1=nodes[n][1];
    old1.z=-500;


    nodes[n][0].z=rotatedVector.z;
    // std::cout<<"rotated n0 :"<<nodes[n][0].x<<" "<<nodes[n][0].y<<" "<<nodes[n][0].z<<std::endl;
    it = nodes.find(old0);
    if (it != nodes.end()) {
        std::vector<glm::vec3> values = std::move(it->second);
        nodes.erase(it); // Remove the old key-value pair
        // Insert the new key-value pair
        nodes[nodes[n][0]] = std::move(values);
    } 
    rotateBranches(nodes[n][0]);

    nodes[n][1].z=rotatedVector2.z;
    // std::cout<<"rotated n1 :"<<nodes[n][1].x<<" "<<nodes[n][1].y<<" "<<nodes[n][1].z<<std::endl;
    it = nodes.find(old1);
    if (it != nodes.end()) {
        std::vector<glm::vec3> values = std::move(it->second);
        nodes.erase(it); // Remove the old key-value pair
        // Insert the new key-value pair
        nodes[nodes[n][1]] = std::move(values);
    } 
    rotateBranches(nodes[n][1]);
}