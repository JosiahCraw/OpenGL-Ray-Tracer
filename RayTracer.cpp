#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "SceneObject.h"
#include "Ray.h"
#include <GL/glut.h>
#include "Plane.h"
#include "TextureBMP.h"
#include "Cylinder.h"
#include "Cone.h"
#include <thread>
using namespace std;

const bool AA = false; // Warning due to the number of divisions AA takes approx 10mins
const float WIDTH = 40.0;
const float HEIGHT = 40.0;
const float EDIST = 20.0;
const int NUMDIV = 600;
const int MAX_STEPS = 5;
const float XMIN = -WIDTH * 0.5;
const float XMAX =  WIDTH * 0.5;
const float YMIN = -HEIGHT * 0.5;
const float YMAX =  HEIGHT * 0.5;

vector<SceneObject*> sceneObjects;  //A global list containing pointers to objects in the scene
vector<glm::vec3> lights;
vector<TextureBMP*> textures;

//---The most important function in a ray tracer! ---------------------------------- 
//   Computes the colour value obtained by tracing a ray and finding its 
//     closest point of intersection with objects in the scene.
//----------------------------------------------------------------------------------
glm::vec3 trace(Ray ray, int step)
{
    float phong = 100;               //TODO put into object variables
	glm::vec3 backgroundCol(0.25,0.25,0.5);
	glm::vec3 ambientCol(0.2);   //Ambient color of light
	glm::vec3 specColour(1);   //TODO incorporate into object description

    ray.closestPt(sceneObjects);		//Compute the closest point of intersetion of objects with the ray

    if(ray.xindex == -1) return backgroundCol;      //If there is no intersection return background colour

    glm::vec3 materialCol;

    if (sceneObjects[ray.xindex]->isTextured()) {
        vector<float> texturePos = sceneObjects[ray.xindex]->texCoord(ray.xpt);
        materialCol = textures[sceneObjects[ray.xindex]->getTexture()]->getColorAt(texturePos[0], texturePos[1]);
    } else if (sceneObjects[ray.xindex]->isProcedural()) {
        materialCol = sceneObjects[ray.xindex]->procedCol(ray.xpt);
    } else {
        materialCol = sceneObjects[ray.xindex]->getColor(); //else return object's colour
    }
    // Init Colour Sum
    glm::vec3 colourSum(0);

    glm::vec3 normalVector = sceneObjects[ray.xindex]->normal(ray.xpt); //Compute the normal vector

    for (int i=0;i<lights.size();i++) {
        // Init variables for computation of base colour

        glm::vec3 lightVector(lights[i] - ray.xpt); //Compute light vector
        lightVector = glm::normalize(lightVector); //Normailize the light vector
        float lDotn = glm::dot(lightVector, normalVector); //Dot product the light and normal vector

        // Variables for specular reflections
        glm::vec3 reflVector = glm::reflect(-lightVector, normalVector); //Compute refection vector
        reflVector = glm::normalize(reflVector);
        float rDotv = glm::dot(reflVector, -ray.dir);



        // Create and check shadow ray in direction of light
        Ray shadow(ray.xpt, lightVector);
        shadow.closestPt(sceneObjects);

        // Check if in shadow first so colourSum is unchanged
        if (shadow.xindex != -1 && shadow.xdist < glm::distance(lights[i], ray.xpt)) { //TODO check transparancy
            colourSum += ambientCol * materialCol *
                         (1 - sceneObjects[ray.xindex]->getTransparency()); // Return base colour as pixel is in shadow
        } else if (lDotn < 0) {
            colourSum += ambientCol * materialCol;
        } else if (rDotv < 0) {
            colourSum += ambientCol * materialCol + lDotn * materialCol; // Add light to the output colour
        } else {
            colourSum += ambientCol * materialCol + lDotn * materialCol + specColour * pow(rDotv, phong);
        }
    }
    // Reflections
    if (sceneObjects[ray.xindex]->isReflective() && step < MAX_STEPS) {
        glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVector);
        reflectedDir = glm::normalize(reflectedDir);
        Ray reflectedRay(ray.xpt, reflectedDir);
        glm::vec3 reflectedCol = trace(reflectedRay, step+1);
        colourSum += (sceneObjects[ray.xindex]->getReflectivity()*reflectedCol);
    }

    // Refraction
    if (sceneObjects[ray.xindex]->isRefractive() && step < MAX_STEPS) {
        if (sceneObjects[ray.xindex]->isTransparent()) {
            Ray internalRefractRay(ray.xpt, ray.dir);
            internalRefractRay.closestPt(sceneObjects);
            Ray refractRay(internalRefractRay.xpt, ray.dir);
            refractRay.closestPt(sceneObjects);
            glm::vec3 refractCol = trace(refractRay, step+1);
            colourSum += refractCol;
        } else {
            glm::vec3 internalVec = glm::refract(ray.dir, normalVector, sceneObjects[ray.xindex]->getRefacIndex());
            internalVec = glm::normalize(internalVec);
            Ray internalRefractRay(ray.xpt, internalVec);
            internalRefractRay.closestPt(sceneObjects);
            glm::vec3 internalNormal = sceneObjects[ray.xindex]->normal(internalRefractRay.xpt);
            glm::vec3 outVector = glm::refract(internalVec, -internalNormal,
                                               1.0f / sceneObjects[ray.xindex]->getRefacIndex());
            Ray refractRay(internalRefractRay.xpt, outVector);
            glm::vec3 refractCol = trace(refractRay, step + 1);
            colourSum += refractCol; //TODO transpar = sceneObjects[ray.xindex]->getTranspar();
        }
    }

    return colourSum;
}


void traceVoid(Ray ray, glm::vec3 &col) {
    col = trace(ray, 1);
}


//---The main display module -----------------------------------------------------------
// In a ray tracing application, it just displays the ray traced image by drawing
// each cell as a quad.
//---------------------------------------------------------------------------------------
void display()
{
	float xp, yp;  //grid point
	float cellX = (XMAX-XMIN)/NUMDIV;  //cell width
	float cellY = (YMAX-YMIN)/NUMDIV;  //cell height

	glm::vec3 eye(0., 0., 10.);  //The eye position (source of primary rays) is the origin

	glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glBegin(GL_QUADS);  //Each cell is a quad.

	for(int i = 0; i < NUMDIV; i++)  	//For each grid point xp, yp
	{
		xp = XMIN + i*cellX;
		for(int j = 0; j < NUMDIV; j++)
		{
			yp = YMIN + j*cellY;

		    glm::vec3 dir(xp+0.5*cellX, yp+0.5*cellY, -EDIST);	//direction of the primary ray
            glm::vec3 col;

            // Check if AntiAliasing is on
		    if (AA) {
                glm::vec3 dir0(xp+0.25*cellX, yp+0.25*cellY, -EDIST);
                glm::vec3 dir1(xp+0.75*cellX, yp+0.25*cellY, -EDIST);
                glm::vec3 dir2(xp+0.25*cellX, yp+0.75*cellY, -EDIST);
                glm::vec3 dir3(xp+0.75*cellX, yp+0.75*cellY, -EDIST);

                Ray ray0 = Ray(eye, dir0);
                Ray ray1 = Ray(eye, dir1);
                Ray ray2 = Ray(eye, dir2);
                Ray ray3 = Ray(eye, dir3);

                ray0.normalize();
                ray1.normalize();
                ray2.normalize();
                ray3.normalize();

                glm::vec3 col0;
                glm::vec3 col1;
                glm::vec3 col2;
                glm::vec3 col3;

                col0 = trace(ray0, 1);
                col1 = trace(ray1, 1);
                col2 = trace(ray2, 1);
                col3 = trace(ray3, 1);

                col = (col0 + col1 + col2 + col3) * 0.25f;

		    } else {
                Ray ray = Ray(eye, dir);        //Create a ray originating from the camera in the direction 'dir'
                ray.normalize();                //Normalize the direction of the ray to a unit vector
                col = trace(ray, 1); //Trace the primary ray and get the colour value
            }

			glColor3f(col.r, col.g, col.b);
			glVertex2f(xp, yp);				//Draw each cell with its color value
			glVertex2f(xp+cellX, yp);
			glVertex2f(xp+cellX, yp+cellY);
			glVertex2f(xp, yp+cellY);
        }
    }

    glEnd();
    glFlush();
}


//---This function initializes the scene ------------------------------------------- 
//   Specifically, it creates scene objects (spheres, planes, cones, cylinders etc)
//     and add them to the list of scene objects.
//   It also initializes the OpenGL orthographc projection matrix for drawing the
//     the ray traced image.
//----------------------------------------------------------------------------------
void initialize()
{
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);
    glClearColor(0, 0, 0, 1);

	//-- Create a pointer to a sphere object
 	Sphere *sphere1 = new Sphere(glm::vec3(-5.0, 5.0, -90.0), 15.0, glm::vec3(0, 0, 1));
 	sphere1->setReflective(0.8);

    Sphere *sphere2 = new Sphere(glm::vec3(0.0, 0.0, -20.0), 10.0, glm::vec3(0.1, 0.1, 0.1));
    sphere2->setRefractive(1.1, 0.9);
    sphere2->setReflective(0.2);

    Sphere *sphere3 = new Sphere(glm::vec3(10.0, 15.0, -70.0), 7.0, glm::vec3(1, 0, 0));

    Sphere *sphere4 = new Sphere(glm::vec3(-10, 0, -10), 4, glm::vec3(0.0, 0.0, 0.0));
    sphere4->setRefractive(1, 1);
    sphere4->setReflective(0.1);

    Cylinder *cylinder1 = new Cylinder(glm::vec3(-20, -10, -15), 1.6, 10, glm::vec3(0.4, 0.4, 0.4));
    cylinder1->setTexture(1, 1, 1);
    cylinder1->setReflective(0.3);

    Cylinder *cylinder2 = new Cylinder(glm::vec3(20, -10, -10), 3, 10, glm::vec3(0.3, 0.3, 0.3));
    cylinder2->setProcedural(glm::vec3(1,0,0));

    //-- Create Plane
    Plane *plane = new Plane (glm::vec3(-300., -10, 200),
                              glm::vec3(300., -10, 200),
                              glm::vec3(300., -10, -200),
                              glm::vec3(-300., -10, -200),
                              glm::vec3(0.5, 0.5, 0));
    plane->setReflective(0.3);
    plane->setTexture(0, 60, 40);

    // Cube
    glm::vec3 origin = glm::vec3(-15, -10, -30);
    float height = 3;
    float width = 3;
    float depth = 3;
    glm::vec3 col = glm::vec3(0.5,0,0);

    Plane *plane0 = new Plane (glm::vec3(origin.x, origin.y+height, origin.z),
                               glm::vec3(origin.x, origin.y, origin.z),
                               glm::vec3(origin.x+width, origin.y, origin.z),
                               glm::vec3(origin.x+width, origin.y+height, origin.z),
                               col);
    Plane *plane1 = new Plane (glm::vec3(origin.x, origin.y+height, origin.z+depth),
                               glm::vec3(origin.x, origin.y, origin.z+depth),
                               glm::vec3(origin.x, origin.y, origin.z),
                               glm::vec3(origin.x, origin.y+height, origin.z),
                               col);
    Plane *plane2 = new Plane (glm::vec3(origin.x+width, origin.y+height, origin.z),
                               glm::vec3(origin.x+width, origin.y, origin.z),
                               glm::vec3(origin.x+width, origin.y, origin.z+depth),
                               glm::vec3(origin.x+width, origin.y+height, origin.z+depth),
                               col);
    Plane *plane3 = new Plane (glm::vec3(origin.x, origin.y+height, origin.z+depth),
                               glm::vec3(origin.x, origin.y+height, origin.z),
                               glm::vec3(origin.x+width, origin.y+height, origin.z),
                               glm::vec3(origin.x+width, origin.y+height, origin.z+depth),
                               col);
    Plane *plane4 = new Plane (glm::vec3(origin.x+width, origin.y+height, origin.z+depth),
                               glm::vec3(origin.x+width, origin.y, origin.z+depth),
                               glm::vec3(origin.x, origin.y, origin.z+depth),
                               glm::vec3(origin.x, origin.y+height, origin.z+depth),
                               col);

    Cone *cone1 = new Cone(glm::vec3(-20, 0, -15), 1.6, 6, glm::vec3(0.3, 0.3, 0.3));
    cone1->setReflective(0.6);

    // Textures
    TextureBMP *bricks = new TextureBMP("Brick.bmp");
    TextureBMP *cylind = new TextureBMP("cylindTex.bmp");
    textures.push_back(bricks);
    textures.push_back(cylind);

	//--Add the above to the list of scene objects
	sceneObjects.push_back(sphere1);
    sceneObjects.push_back(sphere2);
    sceneObjects.push_back(sphere3);
    sceneObjects.push_back(sphere4);
    sceneObjects.push_back(cylinder1);
    sceneObjects.push_back(cylinder2);
    sceneObjects.push_back(plane);
    sceneObjects.push_back(plane0);
    sceneObjects.push_back(plane1);
    sceneObjects.push_back(plane2);
    sceneObjects.push_back(plane3);
    sceneObjects.push_back(plane4);
    sceneObjects.push_back(cone1);

    // Lights
    glm::vec3 light1(-50, 40, -90);
    glm::vec3 light2(50, 40, 90);
    lights.push_back(light1);
    lights.push_back(light2);
}


int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("My Very Special Raytracer");

    glutDisplayFunc(display);
    initialize();

    glutMainLoop();
    return 0;
}
