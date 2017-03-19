#include "PhysicsHelpers.h"

#include "BoxShape.h"
#include "CircleShape.h"
#include "PolyShape.h"

namespace AABBCalculator
{
	PhysicsDefs::AABB RectangleAABB(glm::vec2 center, double w, double h, int angle)
	{
		PhysicsDefs::AABB result;

		double radAngle = angle * M_PI / 180;

		result.width = w * cos(radAngle) + h * sin(radAngle);
		result.height = w * sin(radAngle) + h * cos(radAngle);
		result.center = center;

		return result;
	}

	PhysicsDefs::AABB CircleAABB(glm::vec2 center, double radius)
	{
		PhysicsDefs::AABB result;

		result.width = radius * 2;
		result.height = radius * 2;
		result.center = center;

		return result;
	}

	PhysicsDefs::AABB PolylineAABB(const std::vector<glm::vec2>& points)
	{
		PhysicsDefs::AABB result;

		auto xExtremes = std::minmax_element(points.begin(), points.end(),
			[](const glm::vec2& lhs, const glm::vec2& rhs) {
			return lhs.x < rhs.x;
		});

		auto yExtremes = std::minmax_element(points.begin(), points.end(),
			[](const glm::vec2& lhs, const glm::vec2& rhs) {
			return lhs.y < rhs.y;
		});

		result.width = xExtremes.second->x - xExtremes.first->x;
		result.height = yExtremes.second->y - yExtremes.first->y;
		result.center.x = xExtremes.first->x + result.width / 2;
		result.center.y = xExtremes.first->y + result.height / 2;
		return result;
	}
}

namespace PolylineHelper
{
	glm::vec2 CalcPolylineCentroid(const std::vector<glm::vec2>& points)
	{
		int nextIndex;
		glm::vec2 centroid;
		double area = 0;

		for (int i = 0; i < points.size(); ++i)
		{
			nextIndex = (i + 1) % points.size();
			area += (points[i].x*points[nextIndex].y - points[nextIndex].x*points[i].y);
		}
		area /= 2;

		for (int i = 0; i < points.size(); ++i)
		{
			nextIndex = (i + 1) % points.size();
			centroid.x += (points[i].x + points[nextIndex].x)*(points[i].x*points[nextIndex].y - points[nextIndex].x*points[i].y);
			centroid.y += (points[i].y + points[nextIndex].y)*(points[i].x*points[nextIndex].y - points[nextIndex].x*points[i].y);

		}

		centroid.x /= (6 * area);
		centroid.y /= (6 * area);

		return centroid;
	}
}

namespace CollisionFunctions
{
	PhysicsDefs::MTV SATTest(const ICollisionShape* shape1, const ICollisionShape* shape2)
	{
		PhysicsDefs::MTV result;
		result.didCollide = false;

		glm::vec2 position1 = shape1->GetCentroid();
		glm::vec2 position2 = shape2->GetCentroid();
		std::vector<glm::vec2> p1Points = shape1->GetBounds();
		std::vector<glm::vec2> p2Points = shape2->GetBounds();

		//Minimum translation axis.
		glm::vec2 mtAxis;
		//Minimum translation value. When united with mtAxis we get the minimum translation vector
		double mtValue = DBL_MAX;
		double temp;

		//If no points return
		if (p1Points.size() < 1 || p2Points.size() < 1)
			return result;

		//Generate axes to check for each shape
		std::vector<glm::vec2> p1Axes = GetAxes(p1Points), p2Axes = GetAxes(p2Points);

		PhysicsDefs::Projection p1Proj, p2Proj;

		//After calculating the axes, we can begin checking for overlaps
		for (int i = 0; i < p1Axes.size(); ++i)
		{

			//Get projections
			p1Proj = GetProjection(p1Points, p1Axes[i]);
			p2Proj = GetProjection(p2Points, p1Axes[i]);

			//Check for overlap. If no overlap, then no interection between polygons so we return
			//Otherwise we compare to mtValue
			if (!DoProjectionsOverlap(p1Proj, p2Proj))
				return result;
			else
			{
				//Get value of overlap
				temp = CalculateOverlap(p1Proj, p2Proj);

				//If it is less than pervious, change mtValue and mtAxis accordingly
				if (temp < mtValue)
				{
					mtValue = temp;
					mtAxis = p1Axes[i];
				}
			}
		}

		//After calculating the axes, we can begin checking for overlaps
		for (int i = 0; i < p2Axes.size(); ++i)
		{

			//Get projections
			p1Proj = GetProjection(p1Points, p2Axes[i]);
			p2Proj = GetProjection(p2Points, p2Axes[i]);

			//Check for overlap. If no overlap, then no interection between polygons so we return
			//Otherwise we compare to mtValue
			if (!DoProjectionsOverlap(p1Proj, p2Proj))
				return result;
			else
			{
				//Get value of overlap
				temp = CalculateOverlap(p1Proj, p2Proj);

				//If it is less than pervious, change mtValue and mtAxis accordingly
				if (temp < mtValue)
				{
					mtValue = temp;
					mtAxis = p2Axes[i];
				}
			}
		}
		//If we got this far then the objects are intersecting and we have an MTV.
		//We use this MTV to push objects out of on another, in this case p2 out of p1
		glm::vec2 directionVector, xyInc;

		//Calculate direction vector
		directionVector = glm::vec2(position2.x - position1.x,
			position2.y - position1.y);

		//Get MTV
		xyInc = CalculateMTV(mtAxis, mtValue, directionVector);

		result.xyIncrement = xyInc;
		result.didCollide = true;

		return result;
	}

	PhysicsDefs::Projection GetProjection(std::vector<glm::vec2> points, glm::vec2 axis)
	{
		PhysicsDefs::Projection result;

		//Set min/max to the first point for a base case
		result.min = points[0].x * axis.x + points[0].y * axis.y;
		result.max = result.min;

		float newProj;

		for (int i = 1; i < points.size(); ++i)
		{
			//Get projection of the each point, change min/max if needed
			newProj = glm::dot(points[i], axis);

			if (newProj < result.min)
				result.min = newProj;
			else if (newProj > result.max)
				result.max = newProj;
		}
		return result;
	}

	bool DoProjectionsOverlap(const PhysicsDefs::Projection& proj1, const PhysicsDefs::Projection& proj2)
	{
		bool result = true;

		//If there is a gap, then no overlap
		if ((proj1.max <= proj2.min) || (proj2.max <= proj1.min))
			result = false;

		return result;
	}

	double CalculateOverlap(const PhysicsDefs::Projection& proj1, const PhysicsDefs::Projection& proj2)
	{
		return std::max(0.f, std::min(proj1.max, proj2.max) - std::max(proj1.min, proj2.min));
	}

	glm::vec2 CalculateMTV(glm::vec2 axis, double magnitude, glm::vec2 directionVector)
	{
		//Calculate slope of axis
		double angle = atan(axis.y / axis.x);

		//Calculate change along x and y axes
		glm::vec2 result(cos(angle)*magnitude, sin(angle)*magnitude);

		//Check to make sure MTV isn't pointing towards a polygon
		if (result.x*directionVector.x + result.y * directionVector.y < 0)
		{
			result.x = -result.x;
			result.y = -result.y;
		}

		return result;
	}

	std::vector<glm::vec2> GetAxes(std::vector<glm::vec2> points)
	{
		std::vector<glm::vec2> result;

		double length;
		glm::vec2 axis, edge;
		for (int i = 0; i < points.size(); ++i)
		{
			//Calculate the edge between each point and its neighbor
			edge.x = points[(i + 1) % points.size()].x - points[i].x;
			edge.y = points[(i + 1) % points.size()].y - points[i].y;

			//Get length of edge
			length = sqrt(edge.x * edge.x + edge.y * edge.y);

			//Normalize
			edge.x /= length;
			edge.y /= length;

			//Push the pependiular vector to edge into the axes vector
			result.push_back(glm::vec2(-edge.y, edge.x));
		}

		return result;
	}
}

ICollisionShape* ShapeFactory::CreateShape(PhysicsDefs::IShapeData* data)
{
	auto cs = data->GetShape();

	switch (cs)
	{
		case PhysicsDefs::Rectangle:
		{
			PhysicsDefs::RectangleData* rectData = dynamic_cast<PhysicsDefs::RectangleData*>(data);
			if (!rectData)
				return 0;
			return CreateBox(rectData);
		}
			
		case PhysicsDefs::Circle:
		{
			PhysicsDefs::CircleData* circleData = dynamic_cast<PhysicsDefs::CircleData*>(data);
			if (!circleData)
				return 0;
			return CreateCircle(circleData);
		}
		case PhysicsDefs::Polygon:
		{
			PhysicsDefs::PolygonData* polyData = dynamic_cast<PhysicsDefs::PolygonData*>(data);
			if (!polyData)
				return 0;
			return CreatePolyShape(polyData);
		}
	}
}

CircleShape* ShapeFactory::CreateCircle(PhysicsDefs::CircleData* data)
{
	//if (data->radius == 0)
	//	return 0;

	return new CircleShape(data->center.x, data->center.y, data->radius);
}

BoxShape* ShapeFactory::CreateBox(PhysicsDefs::RectangleData* data)
{
	//if (data->width == 0 || data->height == 0)
	//	return 0;

	return new BoxShape(data->center.x, data->center.y, data->width, data->height, data->angle);
}

PolyShape* ShapeFactory::CreatePolyShape(PhysicsDefs::PolygonData* data)
{
	//if (data->points.size() == 0)
	//	return 0;

	return new PolyShape(data->points);
}