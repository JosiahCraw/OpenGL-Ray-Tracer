/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The object class
*  This is a generic type for storing objects in the scene
*  Sphere, Plane etc. must be defined as subclasses of Object.
*  Being an abstract class, this class cannot be instantiated.
-------------------------------------------------------------*/

#include "SceneObject.h"

glm::vec3 SceneObject::getColor()
{
	return color;
}


void SceneObject::setReflective(float reflec) {
	reflective = true;
	reflectivity = reflec;
}


void SceneObject::setRefractive(float refrac, float transp) {
	if (refrac == 1.0) {
		transparent = true;
	}
	refractive = true;
	transparancy = transp;
	refracIndex = refrac;
}


float SceneObject::getTransparency(void) {
	return transparancy;
}


void SceneObject::setColor(glm::vec3 col)
{
	color = col;
}


bool SceneObject::isRefractive(void) {
	return refractive;
}


bool SceneObject::isReflective(void) {
	return reflective;
}


float SceneObject::getRefacIndex(void) {
	return refracIndex;
}


float SceneObject::getReflectivity(void) {
	return reflectivity;
}

void SceneObject::setTexture(int tex, int x, int y) {
    textured = true;
    texture = tex;
    texX = x;
    texY = y;
}

bool SceneObject::isTextured(void) {
	return textured;
}

int SceneObject::getTexture() {
	return texture;
}

bool SceneObject::isTransparent(void) {
	return transparent;
}

bool SceneObject::isProcedural() {
    return procedural;
}

void SceneObject::setProcedural(glm::vec3 col) {
    procedural = true;
    proCol = col;
}
