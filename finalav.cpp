#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib> 


#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


#pragma comment(lib, "glew32.lib") 

using namespace std;




static int brushType = 0; 
static bool sprayMode = false; 
static bool patternMode = false; 

const int numVertices = 50;
static int width, height;
static float pointSize = 4.0;
static int shape = 4;
#define PI 3.14159265358979324
static float R, G, B = 0;
static float brushR=0.6, brushG=0.5, brushB =0.5;
static float lineThickness = 1.0f; 
static bool brushMode = false;
static bool EraserMode = false;




void drawGrid(int gridSize) {
    glColor3f(0.85f, 0.85f, 0.85f); 
    glLineWidth(1.0f);         

    glBegin(GL_LINES);

    for (int x = 0; x <= width; x += gridSize) {
        glVertex2f(x, 0);
        glVertex2f(x, height);
    }

    for (int y = 0; y <= height; y += gridSize) {
        glVertex2f(0, y);
        glVertex2f(width, y);
    }

    glEnd();
}






void saveScreenshot(const char* filename)
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    int width = viewport[2];
    int height = viewport[3];

    unsigned char* pixels = new(std::nothrow) unsigned char[3 * width * height];
    if (!pixels) {
        std::cerr << "Error: Could not allocate memory for pixels.\n";
        return;
    }

    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    unsigned char* flippedPixels = new(std::nothrow) unsigned char[3 * width * height];
    if (!flippedPixels) {
        delete[] pixels;
        std::cerr << "Error: Could not allocate memory for flippedPixels.\n";
        return;
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            flippedPixels[(height - y - 1) * width * 3 + x * 3 + 0] = pixels[y * width * 3 + x * 3 + 0];
            flippedPixels[(height - y - 1) * width * 3 + x * 3 + 1] = pixels[y * width * 3 + x * 3 + 1];
            flippedPixels[(height - y - 1) * width * 3 + x * 3 + 2] = pixels[y * width * 3 + x * 3 + 2];
        }
    }

    std::string path = std::string(getenv("HOME")) + "/Desktop/" + filename;
    if (!stbi_write_png(path.c_str(), width, height, 3, flippedPixels, width * 3)) {
        std::cerr << "Error: Could not save screenshot to file.\n";
    }

    delete[] pixels;
    delete[] flippedPixels;

    std::cout << "Screenshot saved to desktop as " << filename << std::endl;
}




void setLineThickness(float thickness) {
    if (thickness > 0.0f) { 
        lineThickness = thickness;
    }
}




void TriangleButton(void)
{
    glColor3f(0.78f, 0.78f, 0.78f); 

    float buttonWidth = 0.08 * width;
    float buttonHeight = 0.08 * height;

    float buttonX1 = 0.005 * width;
    float buttonY1 = 0.62 * height;

  
    glBegin(GL_QUADS);
    glVertex2f(buttonX1, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight);
    glVertex2f(buttonX1, buttonY1 + buttonHeight);
    glEnd();

   
glColor3f(0.25f, 0.38f, 0.42f);


float triangleX1 = buttonX1 + buttonWidth / 2;               
float triangleY1 = buttonY1 + buttonHeight - 0.01 * height;  

float triangleX2 = buttonX1 + 0.01 * width;                
float triangleY2 = buttonY1 + 0.01 * height;            

float triangleX3 = buttonX1 + buttonWidth - 0.01 * width;   
float triangleY3 = triangleY2;                             


glBegin(GL_TRIANGLES);
glVertex2f(triangleX1, triangleY1);
glVertex2f(triangleX2, triangleY2);
glVertex2f(triangleX3, triangleY3);
glEnd();

    
    glColor3f(0.25f, 0.38f, 0.42f); 
    glLineWidth(2); 

    glBegin(GL_LINE_LOOP);
    glVertex2f(buttonX1, buttonY1); 
    glVertex2f(buttonX1 + buttonWidth, buttonY1);  
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight); 
    glVertex2f(buttonX1, buttonY1 + buttonHeight); 
    glEnd();
}


void lineButton(void)
{
    glColor3f(0.78f, 0.78f, 0.78f); 

    float buttonWidth = 0.08 * width;
    float buttonHeight = 0.08 * height;

    float buttonX1 = 0.005 * width;
    float buttonY1 = 0.72 * height;

    
    glBegin(GL_QUADS);
    glVertex2f(buttonX1, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight);
    glVertex2f(buttonX1, buttonY1 + buttonHeight);
    glEnd();

    
    glColor3f(0.25f, 0.38f, 0.42f); 

    float lineX1 = buttonX1 + 0.01 * width;  
    float lineY1 = buttonY1 + buttonHeight / 2; 

    float lineX2 = buttonX1 + buttonWidth - 0.01 * width;
    float lineY2 = lineY1; 

    glBegin(GL_LINES);
    glVertex2f(lineX1, lineY1);
    glVertex2f(lineX2, lineY2);
    glEnd();

   
    glColor3f(0.25f, 0.38f, 0.42f);  
    glLineWidth(2);  

    glBegin(GL_LINE_LOOP);
    glVertex2f(buttonX1, buttonY1);  
    glVertex2f(buttonX1 + buttonWidth, buttonY1);  
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight); 
    glVertex2f(buttonX1, buttonY1 + buttonHeight);  
    glEnd();
}



void circleButton(void)
{
    glColor3f(0.78f, 0.78f, 0.78f);

    float buttonWidth = 0.08 * width;
    float buttonHeight = 0.08 * height;

    float buttonX1 = 0.005 * width;
    float buttonY1 = 0.82 * height;

    glBegin(GL_QUADS);
    glVertex2f(buttonX1, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight);
    glVertex2f(buttonX1, buttonY1 + buttonHeight);
    glEnd();

    glColor3f(0.25f, 0.38f, 0.42f);

    float circleRadius = 0.021 * width;

    float circleCenterX = buttonX1 + buttonWidth / 2;
    float circleCenterY = buttonY1 + buttonHeight / 2;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(circleCenterX, circleCenterY);
    for (int i = 0; i <= 800; i++) {
        float angle = i * 3.14159f / 180.0f;
        float x = circleCenterX + cos(angle) * circleRadius;
        float y = circleCenterY + sin(angle) * circleRadius;
        glVertex2f(x, y);
    }
    glEnd();

    glColor3f(0.25f, 0.38f, 0.42f);
    glLineWidth(2);

    glBegin(GL_LINE_LOOP);
    glVertex2f(buttonX1, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight);
    glVertex2f(buttonX1, buttonY1 + buttonHeight);
    glEnd();
}

void squareButton(void)
{
    glColor3f(0.78f, 0.78f, 0.78f);

    float buttonWidth = 0.08 * width;
    float buttonHeight = 0.08 * height;

    float buttonX1 = 0.005 * width;
    float buttonY1 = 0.92 * height;

    glBegin(GL_QUADS);
    glVertex2f(buttonX1, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight);
    glVertex2f(buttonX1, buttonY1 + buttonHeight);
    glEnd();

    glColor3f(0.25f, 0.38f, 0.42f);

    float squareSize = 0.042 * width;

    float squareX = buttonX1 + (buttonWidth - squareSize) / 2;
    float squareY = buttonY1 + (buttonHeight - squareSize) / 2;

    glBegin(GL_QUADS);
    glVertex2f(squareX, squareY);
    glVertex2f(squareX + squareSize, squareY);
    glVertex2f(squareX + squareSize, squareY + squareSize);
    glVertex2f(squareX, squareY + squareSize);
    glEnd();

    glColor3f(0.25f, 0.38f, 0.42f);
    glLineWidth(2);

    glBegin(GL_LINE_LOOP);
    glVertex2f(buttonX1, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight);
    glVertex2f(buttonX1, buttonY1 + buttonHeight);
    glEnd();
}

void gridButton(void)
{
    glColor3f(0.72f, 0.6f, 0.5f);

    float buttonWidth = 0.08 * width;
    float buttonHeight = 0.08 * height;

    float buttonX1 = 0.005 * width;
    float buttonY1 = 0.12 * height;

    glBegin(GL_QUADS);
    glVertex2f(buttonX1, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight);
    glVertex2f(buttonX1, buttonY1 + buttonHeight);
    glEnd();

    glColor3f(0.72f, 0.6f, 0.5f);
    glLineWidth(2);

    glBegin(GL_LINE_LOOP);
    glVertex2f(buttonX1, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight);
    glVertex2f(buttonX1, buttonY1 + buttonHeight);
    glEnd();

    glColor3f(0.96f, 0.9f, 0.79f);

    float textX = buttonX1 + (buttonWidth / 2) - 26;
    float textY = buttonY1 + (buttonHeight / 2) - 5;

    glRasterPos2f(textX, textY);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)"  Spray");
}

void PatternButton(void)
{
    glColor3f(0.72f, 0.6f, 0.5f);

    float buttonWidth = 0.08 * width;
    float buttonHeight = 0.08 * height;

    float buttonX1 = 0.005 * width;
    float buttonY1 = 0.02 * height;

    glBegin(GL_QUADS);
    glVertex2f(buttonX1, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight);
    glVertex2f(buttonX1, buttonY1 + buttonHeight);
    glEnd();

    glColor3f(0.72f, 0.6f, 0.5f);
    glLineWidth(2);

    glBegin(GL_LINE_LOOP);
    glVertex2f(buttonX1, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight);
    glVertex2f(buttonX1, buttonY1 + buttonHeight);
    glEnd();

    glColor3f(0.96f, 0.9f, 0.79f);

    float textX = buttonX1 + (buttonWidth / 2) - 26;
    float textY = buttonY1 + (buttonHeight / 2) - 5;

    glRasterPos2f(textX, textY);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)"  pattern");
}

void EraserButton(void)
{
    glColor3f(0.3f, 0.35f, 0.25f);

    float buttonWidth = 0.08 * width;
    float buttonHeight = 0.08 * height;

    float buttonX1 = 0.005 * width;
    float buttonY1 = 0.22 * height;

    glBegin(GL_QUADS);
    glVertex2f(buttonX1, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight);
    glVertex2f(buttonX1, buttonY1 + buttonHeight);
    glEnd();

    glColor3f(0.3f, 0.35f, 0.25f);
    glLineWidth(2);

    glBegin(GL_LINE_LOOP);
    glVertex2f(buttonX1, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight);
    glVertex2f(buttonX1, buttonY1 + buttonHeight);
    glEnd();

    glColor3f(0.8f, 0.9f, 0.7f);

    float textX = buttonX1 + (buttonWidth / 2) - 26;
    float textY = buttonY1 + (buttonHeight / 2) - 5;

    glRasterPos2f(textX, textY);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)"  Eraser");
}

void clearButton(void)
{
    glColor3f(0.3f, 0.35f, 0.25f);

    float buttonWidth = 0.08 * width;
    float buttonHeight = 0.08 * height;

    float buttonX1 = 0.005 * width;
    float buttonY1 = 0.32 * height;

    glBegin(GL_QUADS);
    glVertex2f(buttonX1, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight);
    glVertex2f(buttonX1, buttonY1 + buttonHeight);
    glEnd();

    glColor3f(0.3f, 0.35f, 0.25f);
    glLineWidth(2);

    glBegin(GL_LINE_LOOP);
    glVertex2f(buttonX1, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight);
    glVertex2f(buttonX1, buttonY1 + buttonHeight);
    glEnd();

    glColor3f(0.8f, 0.9f, 0.7f);

    float textX = buttonX1 + (buttonWidth / 2) - 26;
    float textY = buttonY1 + (buttonHeight / 2) - 5;

    glRasterPos2f(textX, textY);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)"  Clear");
}

void lineclearButton(void)
{
    glColor3f(0.3f, 0.35f, 0.25f);

    float buttonWidth = 0.08 * width;
    float buttonHeight = 0.08 * height;

    float buttonX1 = 0.005 * width;
    float buttonY1 = 0.42 * height;

    glBegin(GL_QUADS);
    glVertex2f(buttonX1, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight);
    glVertex2f(buttonX1, buttonY1 + buttonHeight);
    glEnd();

    glColor3f(0.3f, 0.35f, 0.25f);
    glLineWidth(2);

    glBegin(GL_LINE_LOOP);
    glVertex2f(buttonX1, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight);
    glVertex2f(buttonX1, buttonY1 + buttonHeight);
    glEnd();

    glColor3f(0.8f, 0.9f, 0.7f);

    float textX = buttonX1 + (buttonWidth / 2) - 26;
    float textY = buttonY1 + (buttonHeight / 2) - 5;

    glRasterPos2f(textX, textY);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)"Clearline");
}

void brushButton(void)
{
    glColor3f(0.72f, 0.6f, 0.5f);

    float buttonWidth = 0.08 * width;
    float buttonHeight = 0.08 * height;

    float buttonX1 = 0.005 * width;
    float buttonY1 = 0.52 * height;

    glBegin(GL_QUADS);
    glVertex2f(buttonX1, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight);
    glVertex2f(buttonX1, buttonY1 + buttonHeight);
    glEnd();

    glColor3f(0.72f, 0.6f, 0.5f);
    glLineWidth(2);

    glBegin(GL_LINE_LOOP);
    glVertex2f(buttonX1, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight);
    glVertex2f(buttonX1, buttonY1 + buttonHeight);
    glEnd();

    glColor3f(0.96f, 0.9f, 0.79f);

    float textX = buttonX1 + (buttonWidth / 2) - 26;
    float textY = buttonY1 + (buttonHeight / 2) - 5;

    glRasterPos2f(textX, textY);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)"  Brush");
}

void redButton(void)
{
    glColor3f(0.55f, 0.1f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(width - 70, 10);
    glVertex2f(width - 10, 10);
    glVertex2f(width - 10, 30);
    glVertex2f(width - 70, 30);
    glEnd();

    glColor3f(0.55f, 0.1f, 0.2f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(width - 70, 10);
    glVertex2f(width - 10, 10);
    glVertex2f(width - 10, 30);
    glVertex2f(width - 70, 30);
    glEnd();
}

void greenButton(void)
{
    glColor3f(0.31f, 0.78f, 0.47f);
    glBegin(GL_QUADS);
    glVertex2f(width - 70, 40);
    glVertex2f(width - 10, 40);
    glVertex2f(width - 10, 60);
    glVertex2f(width - 70, 60);
    glEnd();

    glColor3f(0.31f, 0.78f, 0.47f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(width - 70, 40);
    glVertex2f(width - 10, 40);
    glVertex2f(width - 10, 60);
    glVertex2f(width - 70, 60);
    glEnd();
}

void blueButton(void)
{
    glColor3f(0.27f, 0.51f, 0.71f);
    glBegin(GL_QUADS);
    glVertex2f(width - 70, 70);
    glVertex2f(width - 10, 70);
    glVertex2f(width - 10, 90);
    glVertex2f(width - 70, 90);
    glEnd();

    glColor3f(0.27f, 0.51f, 0.71f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(width - 70, 70);
    glVertex2f(width - 10, 70);
    glVertex2f(width - 10, 90);
    glVertex2f(width - 70, 90);
    glEnd();
}


void blackButton(void)
{
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_QUADS);
    glVertex2f(width - 70, 100);
    glVertex2f(width - 10, 100);
    glVertex2f(width - 10, 120);
    glVertex2f(width - 70, 120);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(width - 70, 100);
    glVertex2f(width - 10, 100);
    glVertex2f(width - 10, 120);
    glVertex2f(width - 70, 120);
    glEnd();
}

void yellowButton(void)
{
    glColor3f(0.95f, 0.75f, 0.1f);
    float buttonWidth = 60;
    float buttonHeight = 20;
    float buttonX = width - 70;
    float buttonY = 90 + 40;

    glBegin(GL_QUADS);
    glVertex2f(buttonX, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY + buttonHeight);
    glVertex2f(buttonX, buttonY + buttonHeight);
    glEnd();

    glColor3f(0.95f, 0.75f, 0.1f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(buttonX, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY + buttonHeight);
    glVertex2f(buttonX, buttonY + buttonHeight);
    glEnd();
}

void increaseButton(void)
{
    glColor3f(0.85f, 0.85f, 0.85f);
    float buttonWidth = 60;
    float buttonHeight = 20;
    float buttonX = width - 70;
    float buttonY = 90 + 190;

    glBegin(GL_QUADS);
    glVertex2f(buttonX, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY + buttonHeight);
    glVertex2f(buttonX, buttonY + buttonHeight);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(buttonX, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY + buttonHeight);
    glVertex2f(buttonX, buttonY + buttonHeight);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    float textX = buttonX + (buttonWidth / 2) - 5;
    float textY = buttonY + (buttonHeight / 2) - 5;

    glRasterPos2f(textX, textY);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)"+");
}

void decreaseButton(void)
{
    glColor3f(0.85f, 0.85f, 0.85f);
    float buttonWidth = 60;
    float buttonHeight = 20;
    float buttonX = width - 70;
    float buttonY = 90 + 160;

    glBegin(GL_QUADS);
    glVertex2f(buttonX, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY + buttonHeight);
    glVertex2f(buttonX, buttonY + buttonHeight);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(buttonX, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY + buttonHeight);
    glVertex2f(buttonX, buttonY + buttonHeight);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    float textX = buttonX + (buttonWidth / 2) - 5;
    float textY = buttonY + (buttonHeight / 2) - 5;

    glRasterPos2f(textX, textY);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)"-");
}

void saveButton(void)
{
    glColor3f(0.85f, 0.85f, 0.85f);
    float buttonWidth = 60;
    float buttonHeight = 20;
    float buttonX = width - 70;
    float buttonY = 90 + 100 + buttonHeight + 10;

    glBegin(GL_QUADS);
    glVertex2f(buttonX, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY + buttonHeight);
    glVertex2f(buttonX, buttonY + buttonHeight);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(buttonX, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY + buttonHeight);
    glVertex2f(buttonX, buttonY + buttonHeight);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    float textX = buttonX + (buttonWidth / 2) - 27;
    float textY = buttonY + (buttonHeight / 2) - 5;

    glRasterPos2f(textX, textY);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)"  Save");
}

void cyanButton(void)
{
    glColor3f(0.45f, 0.3f, 0.4f);
    float buttonWidth = 60;
    float buttonHeight = 20;
    float buttonX = width - 70;
    float buttonY = 90 + 100;

    glBegin(GL_QUADS);
    glVertex2f(buttonX, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY + buttonHeight);
    glVertex2f(buttonX, buttonY + buttonHeight);
    glEnd();

    glColor3f(0.45f, 0.3f, 0.4f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(buttonX, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY + buttonHeight);
    glVertex2f(buttonX, buttonY + buttonHeight);
    glEnd();
}

void orangeButton(void)
{
    glColor3f(0.96f, 0.55f, 0.28f);
    float buttonWidth = 60;
    float buttonHeight = 20;
    float buttonX = width - 70;
    float buttonY = 90 + 70;

    glBegin(GL_QUADS);
    glVertex2f(buttonX, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY + buttonHeight);
    glVertex2f(buttonX, buttonY + buttonHeight);
    glEnd();

    glColor3f(0.96f, 0.55f, 0.28f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(buttonX, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY + buttonHeight);
    glVertex2f(buttonX, buttonY + buttonHeight);
    glEnd();
}

class Point
{
public:
    Point(int x, int y)
    {
        xVal = x; yVal = y;
    }
    Point() {};
    void setCoords(int x, int y)
    {
        xVal = x; yVal = y;
    }
    void drawPoint(void);
    int getCoordsX()
    {
        return xVal;
    }
    int getCoordsY()
    {
        return yVal;
    }
   void setColor(float R, float G, float B)
    {
        this->R = R;
        this->G = G;
        this->B = B;
    }
private:
    int xVal, yVal;
    float R, G, B;
    static float size;
};
float Point::size = pointSize;


void Point::drawPoint(void)
{
    glColor3f(R, G, B);
    glPointSize(size);
    glBegin(GL_POINTS);
    glVertex3f(xVal, yVal, 0.0);
    glEnd();
}






class Line
{
public:
    Line(Point p1, Point p2)
    {
        this->p1 = p1;
        this->p2 = p2;
    }
    void setPoints(Point p1, Point p2) {
        this->p1 = p1;
        this->p2 = p2;
    }
    void setColor(float R, float G, float B)
    {
        this->R = R;
        this->G = G;
        this->B = B;
    }


    void drawLine(void) {
        glLineWidth(lineThickness);  
        glBegin(GL_LINES);
        glColor3f(R, G, B);
        glVertex2f(p1.getCoordsX(), p1.getCoordsY());
        glVertex2f(p2.getCoordsX(), p2.getCoordsY());
        glEnd();
        glFlush();
    }


private:
    Point p1;
    Point p2;
    float R, G, B = 0;
};




class Circle
{
public:
    Circle(Point p1, Point p2)
    {
        this->p1 = p1;
        this->p2 = p2;
    }

    void setPoints(Point p1, Point p2) {
        this->p1 = p1;
        this->p2 = p2;
    }
    void setColor(float R, float G, float B)
    {
        this->R = R;
        this->G = G;
        this->B = B;
    }
    void drawCircle(void)
    {
        float t = 0;
        glBegin(GL_LINE_LOOP);

        glColor3f(R, G, B);
        float R = sqrt(pow(p2.getCoordsX() - p1.getCoordsX(), 2) + pow(p2.getCoordsY() - p1.getCoordsY(), 2));
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < numVertices; ++i)
        {
            glVertex3f(p1.getCoordsX() + R * cos(t), p1.getCoordsY() + R * sin(t), 0.0);
            t += 2 * PI / numVertices;
        }
        glEnd();
        glFlush();
    }

private:
    Point p1;
    Point p2;
    float R, G, B = 0;
};




class Square
{
public:
    Square(Point p1, Point p2)
    {
        this->p1 = p1;
        this->p2 = p2;
    }

    void setPoints(Point p1, Point p2)
    {
        this->p1 = p1;
        this->p2 = p2;
    }
    void setColor(float R, float G, float B)
    {
        this->R = R;
        this->G = G;
        this->B = B;
    }
    void drawSquare(void)
    {
        glBegin(GL_LINE_LOOP);
        glColor3f(R, G, B);
        glVertex2f(p1.getCoordsX(), p1.getCoordsY());
        glVertex2f(p1.getCoordsX(), p2.getCoordsY());
        glVertex2f(p2.getCoordsX(), p2.getCoordsY());
        glVertex2f(p2.getCoordsX(), p1.getCoordsY());
        glEnd();
        glFlush();
    }

private:
    Point p1;
    Point p2;
    float R, G, B = 0;
};




class Triangle
{
public:
    Triangle(Point p1, Point p2, Point p3)
    {
        this->p1 = p1;
        this->p2 = p2;
        this->p3 = p3;
    }

    Triangle(Point p1, Point p2)
    {
        this->p1 = p1;
        this->p2 = p2;

        this->p3 = Point((p1.getCoordsX() + p2.getCoordsX()) / 2, p1.getCoordsY() + abs(p2.getCoordsX() - p1.getCoordsX()));
    }

    void setPoints(Point p1, Point p2, Point p3)
    {
        this->p1 = p1;
        this->p2 = p2;
        this->p3 = p3;
    }
    void setColor(float R, float G, float B)
    {
        this->R = R;
        this->G = G;
        this->B = B;
    }
    void drawTriangle(void)
    {
        glBegin(GL_LINE_LOOP);
        glColor3f(R, G, B);
        glVertex2f(p1.getCoordsX(), p1.getCoordsY());
        glVertex2f(p2.getCoordsX(), p2.getCoordsY());
        glVertex2f(p3.getCoordsX(), p3.getCoordsY());
        glEnd();
        glFlush();
    }

private:
    Point p1;
    Point p2;
    Point p3;
    float R, G, B = 0;
};


vector<Point> points;
vector<Point>::iterator pointsIterator;
Point currentPoint;
Point lastClickedPoint;
vector<Line> lines;
vector<Line>::iterator linesIterator;
vector<Circle> circles;
vector<Circle>::iterator circleIterator;
vector<Square> squares;
vector<Square>::iterator squareIterator;

vector<Triangle> triangles;
vector<Triangle>::iterator triangleIterator;







void drawScene(void)
{
    glClearColor(0.94f, 0.91f, 0.85f, 0.0); 
    glClear(GL_COLOR_BUFFER_BIT);     

    drawGrid(30);
    brushButton();
    gridButton();
    circleButton();
    squareButton();
    lineButton();
    redButton();
    greenButton();
    blueButton();
    blackButton();
    clearButton();  
    increaseButton();
    decreaseButton();
    lineclearButton();
    saveButton();
    EraserButton();
    TriangleButton();
    yellowButton();
    cyanButton();
    orangeButton();
    PatternButton();



    linesIterator = lines.begin();
    while (linesIterator != lines.end())
    {
        linesIterator->drawLine();  
        linesIterator++;
    }

    pointsIterator = points.begin();
    while (pointsIterator != points.end())
    {
        pointsIterator->drawPoint();  
        pointsIterator++;
    }

    circleIterator = circles.begin();
    while (circleIterator != circles.end())
    {
        circleIterator->drawCircle(); 
        circleIterator++;
    }

    squareIterator = squares.begin();
    while (squareIterator != squares.end())
    {
        squareIterator->drawSquare(); 
        squareIterator++;
    }



 triangleIterator =  triangles.begin();
    while ( triangleIterator !=  triangles.end())
    {
         triangleIterator->drawTriangle();  
         triangleIterator++;
    }
    
    switch (shape)
    {
    case 0:  
    {
        Line currentLine(lastClickedPoint, currentPoint);
        currentLine.setColor(R, G, B);
        currentLine.drawLine();
        break;
    }
    case 2:  
    {
        Circle currentCircle(lastClickedPoint, currentPoint);
        currentCircle.setColor(R, G, B);
        currentCircle.drawCircle();
        break;
    }
    case 3:  
    {
        Square currentSquare(lastClickedPoint, currentPoint);
        currentSquare.setColor(R, G, B);
        currentSquare.drawSquare();
        break;
    }
    case 4:  
    {
       Triangle currentTriangle(lastClickedPoint, currentPoint);
        currentTriangle.setColor(R, G, B);
        currentTriangle.drawTriangle();
        break;
    }
    }


   if (brushMode) {
    pointsIterator = points.begin();
    while (pointsIterator != points.end()) {
       
        currentPoint.setColor(R, G, B);
        pointsIterator->drawPoint();
        pointsIterator++;
    }

}
  if (EraserMode) {
    pointsIterator = points.begin();
    while (pointsIterator != points.end()) {
       
        currentPoint.setColor(1, 1, 1);
        pointsIterator->drawPoint();
        pointsIterator++;
    }
}

 
}



void clearShapes()
{
    points.clear();
    lines.clear();
    circles.clear();
    squares.clear();
    triangles.clear();
    glutPostRedisplay();  
}



void clearline()
{
   
    lines.clear();
   
    glutPostRedisplay();  
}





/*void keyInput(unsigned char key, int x, int y)
{
    switch (key)
    {
    case ' ':
        exit(0);
        break;
    case '1':
        shape = 0;
        lastClickedPoint.setCoords(0, 0);
        currentPoint.setCoords(0, 0);
        glutPostRedisplay();
        break;
    case '2':
        shape = 2;
        lastClickedPoint.setCoords(0, 0);
        currentPoint.setCoords(0, 0);
        glutPostRedisplay();
        break;
    case '3':
        shape = 3;
        lastClickedPoint.setCoords(0, 0);
        currentPoint.setCoords(0, 0);
        glutPostRedisplay();
        break;


    case 'r':
        R = 1;
        G = 0;
        B = 0;
        lastClickedPoint.setCoords(0, 0);
        currentPoint.setCoords(0, 0);
        glutPostRedisplay();
        break;
    case 'b':
        R = 0;
        G = 0;
        B = 1;
        lastClickedPoint.setCoords(0, 0);
        currentPoint.setCoords(0, 0);
        glutPostRedisplay();
        break;
        case 'e':
        R = 1;
        G = 1;
        B = 1;
        lastClickedPoint.setCoords(0, 0);
        currentPoint.setCoords(0, 0);
        glutPostRedisplay();
        break;
    case 'g':
        R = 0;
        G = 1;
        B = 0;
        lastClickedPoint.setCoords(0, 0);
        currentPoint.setCoords(0, 0);
        glutPostRedisplay();
        break;
    case 's':
        R = 0;
        G = 0;
        B = 0;
        lastClickedPoint.setCoords(0, 0);
        currentPoint.setCoords(0, 0);
        glutPostRedisplay();
        break;
    case 'c':
        clearShapes();
        break;
    case 'l':
        clearline();
        break;
    case 'a':  
        brushMode = !brushMode;
        if (brushMode) {
            std::cout << "Brush mode activated!" << std::endl;
            glutSetCursor(GLUT_CURSOR_CROSSHAIR);  
        }
        else {
            std::cout << "Brush mode deactivated!" << std::endl;
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);  
        }
        break;


case 'E':  
        EraserMode = !EraserMode;
        if (EraserMode) {
            std::cout << "eraser mode activated!" << std::endl;
            glutSetCursor(GLUT_CURSOR_CROSSHAIR);  
        }
        else {
            std::cout << "eraser mode deactivated!" << std::endl;
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);  
        }
        break;

    case 'w': 
        saveScreenshot("screenshot.png");
        glutPostRedisplay();
        break;
    case '+':  
        setLineThickness(lineThickness + 1.0f); 
        break;
    case '-':  
        setLineThickness(lineThickness - 1.0f); 
        break;


         

    default:
        break;
        
    }
}*/


void mouseControl(int button, int state, int x, int y)
{
    int viewportY = height - y;  


    
    if (x >= width - 70 && x <= width - 10 && viewportY >= 280 && viewportY <= 800) {
        setLineThickness(lineThickness + 1.0f); 
        glutPostRedisplay();
        return;
    }

    
    if (x >= width - 70 && x <= width - 10 && viewportY >= 250 && viewportY <= 270) {
        setLineThickness(lineThickness - 1.0f); 
        glutPostRedisplay();
        return;
    }

    
    if (x >= (width - 70) && x <= (width - 10) &&
        viewportY >= (90 + 100 + 20 + 10) &&
        viewportY <= (90 + 100 + 20 + 10 + 20)) {
        std::cout << "Save Screenshot button clicked!" << std::endl;
        saveScreenshot("Paint.jpg"); 
        glutPostRedisplay();
        return;
    }


    
    if (x >= 0.005 * width && x <= 0.08 * width &&
         viewportY >= 0.42 * height && viewportY <= 0.5 * height) {
        std::cout << "Lineclear button clicked!" << std::endl;
        clearline();
        glutPostRedisplay(); 
        return;
    }

if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

    if (x >= 0.005 * width && x <= 0.08 * width &&
         viewportY >= 0.52 * height && viewportY <= 0.6 * height) {
        std::cout << "Brush button clicked!" << std::endl;
        brushMode = !brushMode;  
        if (brushMode) {
            std::cout << "Brush mode activated!" << std::endl;
        }
        else {
            std::cout << "Brush mode deactivated!" << std::endl;
        }
        glutPostRedisplay();
        return;  
    }

}


    if (brushMode) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            
            lastClickedPoint.setCoords(x, viewportY);
            currentPoint = Point(x, viewportY);
            points.push_back(currentPoint);  
            return;
        }
    }


   

//Eraser

if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

    if (x >= 0.005 * width && x <= 0.08 * width &&
         viewportY >= 0.22 * height && viewportY <= 0.3 * height) {
        std::cout << "Brush button clicked!" << std::endl;
        EraserMode = !EraserMode;  
        if (EraserMode) {
            std::cout << "Eraser mode activated!" << std::endl;
        }
        else {
            std::cout << "Eraser mode deactivated!" << std::endl;
        }
        glutPostRedisplay();
        return;  
    }

}


    if (EraserMode) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
           
            lastClickedPoint.setCoords(x, viewportY);
            currentPoint = Point(x, viewportY);
            points.push_back(currentPoint);  
            return;
        }
    }






   
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

    //sprayyy
        if (x >= 0.005 * width && x <= 0.08 * width &&
         viewportY >= 0.12 * height && viewportY <= 0.2 * height){ 
    sprayMode = !sprayMode; 
    if (sprayMode) {
        std::cout << "Spray mode activated!" << std::endl;
        brushType = 1; 
    } else {
        std::cout << "Spray mode deactivated!" << std::endl;
        brushType = 0; 
    }

         }


//patternnnn
if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

    
        if (x >= 0.005 * width && x <= 0.08 * width &&
         viewportY >= 0.02 * height && viewportY <= 0.1 * height){ 
    patternMode = !patternMode; 
    if (patternMode) {
        std::cout << "pattern mode activated!" << std::endl;
        brushType = 2; 
    } else {
        std::cout << "pattern mode deactivated!" << std::endl;
        brushType = 0; 
    }

         }
}




    
        // Line
        if (x >= 0.005 * width && x <= 0.08 * width && viewportY >= 0.72 * height && viewportY <= 0.8 * height) {
            shape = 0;  
            lastClickedPoint.setCoords(0, 0);
            currentPoint.setCoords(0, 0);
            glutPostRedisplay();
            return;
        }

        // Circle
        if (x >= 0.005 * width && x <= 0.08 * width && viewportY >= 0.82 * height && viewportY <= 0.9 * height) {
            shape = 2; 
            lastClickedPoint.setCoords(0, 0);
            currentPoint.setCoords(0, 0);
            glutPostRedisplay();
            return;
        }

        // Square
        if (x >= 0.005 * width && x <= 0.08 * width && viewportY >= 0.92 * height && viewportY <= 1.0 * height) {
            shape = 3;  
            lastClickedPoint.setCoords(0, 0);
            currentPoint.setCoords(0, 0);
            glutPostRedisplay();
            return;
        }
        
//triangle
          if (x >= 0.005 * width && x <= 0.08 * width && viewportY >= 0.62 * height && viewportY <= 0.7 * height) {
            shape = 4; 
            lastClickedPoint.setCoords(0, 0);
            currentPoint.setCoords(0, 0);
            glutPostRedisplay();
            return;
        }

        //  Clear
        if (x >= 0.005 * width && x <= 0.08 * width && viewportY >= 0.32 * height && viewportY <= 0.4 * height) {
            clearShapes();  
            return;
        }

       
        if (x >= width - 70 && x <= width - 10 && viewportY >= 10 && viewportY <= 30) {
            R = 0.55f; G = 0.1f; B = 0.2f;  
            lastClickedPoint.setCoords(0, 0);
            currentPoint.setCoords(0, 0);
            glutPostRedisplay();
            return;
        }

        if (x >= width - 70 && x <= width - 10 && viewportY >= 40 && viewportY <= 60) {
            R = 0.31f; G = 0.78f; B = 0.47f; 
            lastClickedPoint.setCoords(0, 0);
            currentPoint.setCoords(0, 0);
            glutPostRedisplay();
            return;
        }

        if (x >= width - 70 && x <= width - 10 && viewportY >= 70 && viewportY <= 90) {
            R = 0.27f; G = 0.51f; B = 0.71f;  
            lastClickedPoint.setCoords(0, 0);
            currentPoint.setCoords(0, 0);
            glutPostRedisplay();
            return;
        }

        if (x >= width - 70 && x <= width - 10 && viewportY >= 100 && viewportY <= 120) {
            R = 0.0f; G = 0.0f; B = 0.0f;  
            lastClickedPoint.setCoords(0, 0);
            currentPoint.setCoords(0, 0);
            glutPostRedisplay();
            return;
        }

          if (x >= width - 70 && x <= width - 10 && viewportY >= 130 && viewportY <= 150) {
            R = 0.95f; G = 0.75f; B = 0.1f;  
            lastClickedPoint.setCoords(0, 0);
            currentPoint.setCoords(0, 0);
            glutPostRedisplay();
            return;
        }

         //cyan  
        if (x >= width - 70 && x <= width - 10 && viewportY >= 190 && viewportY <= 210) {
            R = 0.45f; G = 0.3f; B = 0.4f;  
            lastClickedPoint.setCoords(0, 0);
            currentPoint.setCoords(0, 0);
            glutPostRedisplay();
            return;
        }

        //orange  
          if (x >= width - 70 && x <= width - 10 && viewportY >= 160 && viewportY <= 180) {
           R = 0.96f; G = 0.55f; B = 0.28f;  
            lastClickedPoint.setCoords(0, 0);
            currentPoint.setCoords(0, 0);
            glutPostRedisplay();
            return;
    }
    }
    
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        lastClickedPoint.setCoords(x, viewportY);
        currentPoint = Point(x, viewportY);

      
        switch (shape) {
        case 0: 
        {
            Line line(lastClickedPoint, currentPoint);
            line.setColor(R, G, B);
            lines.push_back(line);
            break;
        }
        case 2: 
        {
            Circle circle(lastClickedPoint, currentPoint);
            circle.setColor(R, G, B);
            circles.push_back(circle);
            break;
        }
        case 3:  
        {
            Square square(lastClickedPoint, currentPoint);
            square.setColor(R, G, B);
            squares.push_back(square);
            break;
        }

         case 4: 
        {
            Triangle triangle(lastClickedPoint, currentPoint);
            triangle.setColor(R, G, B);
            triangles.push_back(triangle);
            break;
        }
        }
    

        glutPostRedisplay(); 
    }
 
    
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
       
        if (brushMode) return;

        switch (shape) {
        case 0: 
        {
            Line line(lastClickedPoint, currentPoint);
            line.setColor(R, G, B);
            lines.push_back(line);
            break;
        }
        case 2: 
        {
            Circle circle(lastClickedPoint, currentPoint);
            circle.setColor(R, G, B);
            circles.push_back(circle);
            break;
        }
        case 3: 
        {
            Square square(lastClickedPoint, currentPoint);
            square.setColor(R, G, B);
            squares.push_back(square);
            break;
        }
         case 4: 
        {
            Triangle triangle(lastClickedPoint, currentPoint);
            triangle.setColor(R, G, B);
            triangles.push_back(triangle);
            break;
        }
        }

        glutPostRedisplay();  
    }

 

}






void mouseMotion(int x, int y)
{

    currentPoint.setCoords(x, height - y);
    Line line(lastClickedPoint, currentPoint);



    if (brushMode) {
        int dx = currentPoint.getCoordsX() - lastClickedPoint.getCoordsX();
        int dy = currentPoint.getCoordsY() - lastClickedPoint.getCoordsY();

        int steps = std::max(abs(dx), abs(dy));

        // فاصله  
        float stepX = dx / (float)steps;
        float stepY = dy / (float)steps;

        for (int i = 0; i <= steps; i++) {
            int interpolatedX = lastClickedPoint.getCoordsX() + i * stepX;
            int interpolatedY = lastClickedPoint.getCoordsY() + i * stepY;

            Point interpolatedPoint(interpolatedX, interpolatedY);
            interpolatedPoint.setColor(R, G, B); 

            points.push_back(interpolatedPoint); 
        }

        lastClickedPoint = currentPoint; 
        points.push_back(currentPoint);
    }
    else {
        Line line(lastClickedPoint, currentPoint);
        line.setColor(R, G, B);
        line.drawLine();  
    }

if (EraserMode){
        int dx = currentPoint.getCoordsX() - lastClickedPoint.getCoordsX();
        int dy = currentPoint.getCoordsY() - lastClickedPoint.getCoordsY();

        int steps = std::max(abs(dx), abs(dy));

        float stepX = dx / (float)steps;
        float stepY = dy / (float)steps;

        for (int i = 0; i <= steps; i++) {
            int interpolatedX = lastClickedPoint.getCoordsX() + i * stepX;
            int interpolatedY = lastClickedPoint.getCoordsY() + i * stepY;

            Point interpolatedPoint(interpolatedX, interpolatedY);
            interpolatedPoint.setColor(0.94f, 0.91f, 0.85f); 

            points.push_back(interpolatedPoint); 
        }

        lastClickedPoint = currentPoint; 
        points.push_back(currentPoint);
    }
    else {
        Line line(lastClickedPoint, currentPoint);
        line.setColor(1, 1, 1);
        line.drawLine();  
    }

  
   if (sprayMode && brushMode) { 
    int sprayRadius = 3; 
    int sprayDensity = 17; 

    for (int i = 0; i < sprayDensity; i++) {
        float angle = static_cast<float>(rand()) / RAND_MAX * 2 * PI;
        float distance = static_cast<float>(rand()) / RAND_MAX * sprayRadius;
        int sprayX = currentPoint.getCoordsX() + cos(angle) * distance;
        int sprayY = currentPoint.getCoordsY() + sin(angle) * distance;

        Point sprayPoint(sprayX, sprayY);
        sprayPoint.setColor(R, G, B);
        points.push_back(sprayPoint); 
    }
} else if (brushMode) { 
    points.push_back(currentPoint);
}

if (brushMode && brushType == 2) { 
    int patternSize = 10; 
    glLineWidth(0.7f); 
    for (int i = -1; i <= 1; i++) { // محدود کردن تکرار افقی
        for (int j = -1; j <= 1; j++) { // محدود کردن تکرار عمودی
            if ((i + j) % 2 == 0) { // الگوی شطرنجی
                Point patternPoint(
                    currentPoint.getCoordsX() + i * patternSize,
                    currentPoint.getCoordsY() + j * patternSize
                );
                patternPoint.setColor(R, G, B);
                points.push_back(patternPoint);
            }
        }
    }
    glLineWidth(0.7f); 
}

        glutPostRedisplay();

    }




void setup(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glColor3f(0.0, 0.0, 0.0);
        glReadBuffer(GL_FRONT); // تنظیم برای خواندن پیکسل‌ها

        glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



}

void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();


    glOrtho(0.0, w, 0.0, h, -1.0, 1.0);


    width = w;
    height = h;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void printInteraction(void)
{
    cout << "Interaction:" << endl;
    cout << "Left mouse was clicked on the screen , hold it to draw" << endl
        << "click right mouse for going out" << endl;
}


int main(int argc, char** argv)
{
    printInteraction();
    glutInit(&argc, argv);

    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Paint");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    //glutKeyboardFunc(keyInput);

    glutMouseFunc(mouseControl);

    glutMotionFunc(mouseMotion);


    glewExperimental = GL_TRUE;
    glewInit();

    setup();

    glutMainLoop();
}

   

