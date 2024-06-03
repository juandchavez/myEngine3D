/*
    This project is a personal refresher on Cpp
*/
#include "myConsoleGameEngine.h"
#include <vector>
/*
    This will be the three points that define the 
    outline of the triangle
*/
struct vec3d {
    float x,y,z;
}

/*
    A triangle using 3 verticies
*/
struct triangle {
    vec3d point[3];
}

/*
    How we will create our shapes
*/
struct mesh {
    vector<triangle> tris;
}
/*
    This is our projection matrix
*/
struct mat4x4{
    float matrix[4][4] = {0};   //  Init to zero
}

private:
    mesh meshCube; 
    mat4x4 matProj;
    /*
        Applies a transform using a vector and a projection matrix to output a 
        new vector on a newly projected space. Linear algebra
        Input: A triangle to transform
        Output: A triangle that has been transformed via the projection matrix
    */
    void multiplyMatrixVector(vec3d &input, vec3d&output, mat4x4 &mat){

        float w = 0;    // The fourth element

        output.x = input.x * mat.matrix[0][0] + input.y * mat.matrix[1][0] + input.z * mat.matrix[2][0] + mat.matrix[3][0];
        output.y = input.x * mat.matrix[0][1] + input.y * mat.matrix[1][1] + input.z * mat.matrix[2][1] + mat.matrix[3][1];
        output.z = input.x * mat.matrix[0][0] + input.y * mat.matrix[1][2] + input.z * mat.matrix[2][2] + mat.matrix[3][2];
        w = input.x * mat.matrix[0][3] + input.y * mat.matrix[1][3] + input.z * mat.matrix[2][3] + mat.matrix[3][3];

        // Divide the output values by the fourth element
        if (w != 0.0f){
            output.x /= w;
            output.y /= w;
            output.z /= w;
        }

    }

class myEngine3D : public myConsoleGameEngine {
    public:
    //Constructors
    myEngine3D(){
        m_sAppName = L"3D Demo";

    }

    public:
    bool onUserCreate() override{
        /*
            Check note on how the meshCude works
        */
        meshCube.tris = {
            // Southern face
            {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  1.0f, 1.0f, 0.0f},    // Remember that this is a triangle [p1,p2,p3]
            {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f},    // with its vectors {x,y,z}

            // Eastern face
            {1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f},
            {1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 1.0f},

            // Northern face
            {1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 1.0f},
            {1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f},
            
            // Western face
            {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f},

            // Top face
            {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f},
            {0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f},

            // Bottom face
            {1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f}

            // Projection matrix
            // screen and fov wont change in this app
            float fNear = 0.1f      // Shortest distance for the object
            float fFar = 1000.0f    // Furthest distance an object can be
            float fFov = 90.0f      // Our field of view in degrees
            float fAspectRatio = (float)screenHeight() / (float)screenWidth();  // Grabbed from the console
            float fFovRad = 1.0f/tanf(fFov * 0.5f/180.0f * 3.14159f);   // The tangent calc

            // We fill the matrix with the respective calculations for the multiplication
            matProj.m[0][0] = fAspectRatio * fFovRad;
            matProj.m[1][1] = fFovRad;
            matProj.m[2][2] = (-fFar * fNear)/(fFar - fNear);
            matProj.m[2][3] = 1.0f;
            matProj.m[3][3] = 0.0f;
        }
        return true;    // Everything is fine
    }

    bool onUserUpdate(float fElapsedTime) override{
        // Clear screen with 'blank' pixels
        fill(0, 0, screenWidth(), screenLength(), PIXEL_SOLID, FG_BLACK);

        // Draw the triangles
        // This is where we are gonna have to use projection 
        for (auto tri; meshCube.tris){
            // Triangle that has been transformed
            triangle triProjected;
            multiplyMatrixVector(tri.point[0], triProjected.point[0], matProj);
            multiplyMatrixVector(tri.point[1], triProjected.point[1], matProj);
            multiplyMatrixVector(tri.point[2], triProjected.point[2], matProj);

            DrawTriangle(triProjected.p[0].x, triProjected.p[0].y,
            triProjected.p[1].x, triProjected.p[1].y,
            triProjected.p[2].x, triProjected.p[2].y,
            PIXEL_SOLID, FG_WHITE)

        }
        return true;
    }
}


int main(){

    myEngine3D demo;    // Create an instance of the engine

    if(demo.constructConsole(256, 240, 4, 4)){
        demo.start();

    }
    return 0;
}