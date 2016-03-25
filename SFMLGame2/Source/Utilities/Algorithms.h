#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#define _USE_MATH_DEFINES

#include <math.h>
#include <vector>
#include <algorithm>

template<class T>
class Algorithms
{
public:
	static std::vector<T> Algorithms<T>::MergeSortVector(std::vector<T>& list, bool (*comparator)(T&, T&))
	{
		if (list.size() <= 1)
		{
			return list;
		}

		auto middle = list.begin() + list.size() / 2;

		std::vector<T> left (list.begin(), middle), right(middle, list.end()), result;

		left = MergeSortVector(left, comparator);
		right = MergeSortVector(right, comparator);
		result = Merge(left, right, comparator);
		return result;
	};
private:
	Algorithms();
	static std::vector<T> Algorithms<T>::Merge(std::vector<T>& left, std::vector<T>& right, bool(*comparator)(T&, T&))
	{
		std::vector<T> result;
		unsigned int leftIndex = 0, rightIndex = 0;
		while (leftIndex < left.size() && rightIndex < right.size())
		{
			if (comparator(left[leftIndex], right[rightIndex]))
			{
				result.push_back(left[leftIndex]);
				leftIndex++;
			}
			else
			{
				result.push_back(right[rightIndex]);
				rightIndex++;
			}
		}
		while (leftIndex < left.size())
		{
			result.push_back(left[leftIndex]);
			leftIndex++;
		}
		while (rightIndex < right.size())
		{
			result.push_back(right[rightIndex]);
			rightIndex++;
		}
		return result;
	}
};

#endif