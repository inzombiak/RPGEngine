#ifndef PHYSICS_HELPERS_H
#define PHYSICS_HELPERS_H

#define _USE_MATH_DEFINES

#include <math.h>
#include <vector>
#include <algorithm>

#include "..\Utilities\GameDefs.h"
class ICollisionShape;
class CircleShape;
class BoxShape;
class PolyShape;

namespace PhysicsDefs
{
	typedef struct AABB
	{
		glm::vec2 center;
		double width;
		double height;
	};

	//Struct for polygon projection
	struct Projection
	{
		float min, max;
	};

	struct MTV
	{
		bool didCollide;
		glm::vec2 xyIncrement;
	};

	enum CollisionShape
	{
		Polygon,
		Rectangle,
		Circle,
	};

	static std::map<string, CollisionShape> collisionShapeStringToEnumMap =
	{
		{ "Polygon", CollisionShape::Polygon },
		{ "Rectangle", CollisionShape::Rectangle },
		{ "Circle", CollisionShape::Circle },
	};

	struct IShapeData
	{
		IShapeData() {};
		virtual ~IShapeData() {};
		virtual CollisionShape GetShape()
		{
			return shape;
		}
	protected:
		CollisionShape shape;
	};

	struct PolygonData : public IShapeData
	{
		PolygonData()
		{
			shape = Polygon;
		}
		std::vector<glm::vec2> points;
	};

	struct RectangleData : public IShapeData
	{
		RectangleData()
		{
			shape = Rectangle;
			angle = 0;
		}
		glm::vec2 center;
		double width;
		double height;
		int angle;
	};

	struct CircleData : public IShapeData
	{
		CircleData()
		{
			shape = Circle;
		}
		glm::vec2 center;
		double radius;
	};
}

namespace AABBCalculator
{
	PhysicsDefs::AABB RectangleAABB(glm::vec2 center, double w, double h, int angle);
	PhysicsDefs::AABB CircleAABB(glm::vec2 center, double radius);
	PhysicsDefs::AABB PolylineAABB(const std::vector<glm::vec2>& points);
}

namespace PolylineHelper
{
	glm::vec2 CalcPolylineCentroid(const std::vector<glm::vec2>& points);
}

namespace CollisionFunctions
{
	//Methods for seperating axis theorem
	PhysicsDefs::MTV SATTest(const ICollisionShape* shape1, const ICollisionShape* shape2);
	//Returns vector of SAT axes. Arguement is the points of the shape
	std::vector<glm::vec2> GetAxes(std::vector<glm::vec2> points);
	//Get projection of a polygon on an axis
	PhysicsDefs::Projection GetProjection(std::vector<glm::vec2> poly, glm::vec2 axis);
	//Checks if projections overlap.
	bool DoProjectionsOverlap(const PhysicsDefs::Projection& proj1, const PhysicsDefs::Projection& proj2);
	//Returns overlap of two projections
	double CalculateOverlap(const PhysicsDefs::Projection& proj1, const PhysicsDefs::Projection& proj2);
	//Returns the MTV. Takes MTV axis, magnitude and the direction vector between the two polygons
	glm::vec2 CalculateMTV(glm::vec2 axis, double magnitude, glm::vec2 directionVec);
}

class ShapeFactory
{
public:
	static ICollisionShape* CreateShape(PhysicsDefs::IShapeData* data);

private:
	static CircleShape* CreateCircle(PhysicsDefs::CircleData* data);
	static BoxShape* CreateBox(PhysicsDefs::RectangleData* data);
	static PolyShape* CreatePolyShape(PhysicsDefs::PolygonData* data);
};		  

#endif