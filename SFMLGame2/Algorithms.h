#include <vector>

template<class T>

class Algorithms
{
public:
	static std::vector<T> Algorithms<T>::MergeSort(std::vector<T>& list, bool (*comparator)(T&, T&))
	{
		if (list.size() <= 1)
		{
			return list;
		}

		std::vector<T> left, right, result;
		int middle = floor((list.size() + 1) / 2);

		for (int i = 0; i < middle; ++i)
		{
			left.push_back(list[i]);
		}

		for (int i = middle; i < list.size(); ++i)
		{
			right.push_back(list[i]);
		}

		left = MergeSort(left, comparator);
		right = MergeSort(right, comparator);
		result = Merge(left, right, comparator);
		return result;
	};
private:
	Algorithms();
	static std::vector<T> Algorithms<T>::Merge(std::vector<T>& left, std::vector<T>& right, bool (*comparator)(T&, T&))
	{
		std::vector<T> result;
		while (left.size() > 0 || right.size() > 0)
		{
			if (left.size() > 0 && right.size() > 0)
			{
				if (comparator(left.front(), right.front()))
				{
					result.push_back(left.front());
					left.erase(left.begin());
				}
				else
				{
					result.push_back(right.front());
					right.erase(right.begin());
				}
			}
			else if (left.size() > 0)
			{
				for (int i = 0; i < left.size(); i++)
					result.push_back(left[i]);
				break;
			}
			else if (right.size() > 0)
			{
				for (int i = 0; i < right.size(); i++)
					result.push_back(right[i]);
				break;
			}
		}

		return result;
	}
};

