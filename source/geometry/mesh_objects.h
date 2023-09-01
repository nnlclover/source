#ifndef __MESH_OBJECTS_H_
#define __MESH_OBJECTS_H_

#include <geometry/Mesh.h>

Mesh generateConeMesh(float height, float radius, int num_segments);
Mesh generateSphere(float radius, unsigned int numRings, unsigned int numSegments);
Mesh generateTorus(float outerRadius, float innerRadius, unsigned int numSegments, unsigned int numRings);
Mesh generateTorusPart(float outerRadius, float innerRadius, float length, unsigned int numSegments, unsigned int numRings);
Mesh generateTube(float outerRadius, float innerRadius, float length, unsigned int numSegments);
Mesh generatePlane(float width, float height, unsigned int numSegments);


#endif //!__MESH_OBJECTS_H_