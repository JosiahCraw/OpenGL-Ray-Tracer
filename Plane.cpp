#include "Plane.h"
#include "TextureBMP.h"
#include <math.h>
#include <vector>



bool Plane::isInside(glm::vec3 pt)
{

	glm::vec3 ua, ub, uc, ud, va, vb, vc, vd;

	ua = b - a;
	ub = c - b;
	uc = d - c;
	ud = a - d;

	va = pt - a;
	vb = pt - b;
	vc = pt - c;
	vd = pt - d;

	float aCrAn, bCrBn, cCrCn, dCrDn;

	aCrAn = glm::dot(cross(ua, va), normal(pt));
    bCrBn = glm::dot(cross(ub, vb), normal(pt));
    cCrCn = glm::dot(cross(uc, vc), normal(pt));
    dCrDn = glm::dot(cross(ud, vd), normal(pt));

    if (aCrAn > 0 && bCrBn > 0 && cCrCn > 0 && dCrDn > 0) {
        return true;
    }
	
	return false;
}



float Plane::intersect(glm::vec3 posn, glm::vec3 dir)
{
	glm::vec3 n = normal(posn);
	glm::vec3 vdif = a - posn;
	float vdotn = glm::dot(dir, n);
	if(fabs(vdotn) < 1.e-4) return -1;
    float t = glm::dot(vdif, n)/vdotn;
	if(fabs(t) < 0.0001) return -1;
	glm::vec3 q = posn + dir*t;
	if(isInside(q)) return t;
    else return -1;
}



glm::vec3 Plane::normal(glm::vec3 pt)
{
	glm::vec3 n = glm::vec3(0);

    n = glm::cross((b - a),(d - a));
    n = glm::normalize(n);

    return n;
}


vector<float> Plane::texCoord(glm::vec3 pt) {
	float texS, texT;
	vector<float> out;

	texS = (pt.x - a.x) / (b.x - a.x);
	texT = (pt.z - a.z) / (d.z - a.z);

	texS = texS * texX;
	texT = texT * texY;

	texS = texS - (long) texS;
	texT = texT - (long) texT;
	out.push_back(texS);
	out.push_back(texT);
	return out;


}

glm::vec3 Plane::procedCol (glm::vec3 pt)  {
	return glm::vec3(0);
}