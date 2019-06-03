/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The object class
*  This is a generic type for storing objects in the scene.
*  Being an abstract class, this class cannot be instantiated.
*  Sphere, Plane etc, must be defined as subclasses of Object
*      and provide implementations for the virtual functions
*      intersect()  and normal().
-------------------------------------------------------------*/

#ifndef H_SOBJECT
#define H_SOBJECT
#include <glm/glm.hpp>
#include <string>
#include <vector>


class SceneObject
{
protected:
	glm::vec3 color;
	float refracIndex = 0;
	float reflectivity = 0;
	bool refractive = false;
	bool reflective = false;
	float transparancy = 0;
	bool transparent;
	bool textured = false;
	int texture, texX, texY;
	glm::vec3 proCol;
	bool procedural;
public:
	SceneObject() {}
	virtual glm::vec3 procedCol(glm::vec3 pt) = 0;
    virtual float intersect(glm::vec3 pos, glm::vec3 dir) = 0;
	virtual std::vector<float> texCoord(glm::vec3 pt) = 0;
	virtual glm::vec3 normal(glm::vec3 pos) = 0;
	virtual ~SceneObject() {}
	glm::vec3 getColor();
	void setColor(glm::vec3 col);
	bool isRefractive(void);
	bool isReflective(void);
	bool isTextured(void);
	float getRefacIndex(void);
	float getReflectivity(void);
	float getTransparency(void);
	void setReflective(float reflec);
	void setRefractive(float refrac, float transp);
	void setTexture(int tex, int x, int y);
	int getTexture(void);
	bool isTransparent(void);
	bool isProcedural(void);
	void setProcedural(glm::vec3 col);
};

#endif
