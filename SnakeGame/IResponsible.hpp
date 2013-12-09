#pragma once

#include <vector>
#include "GameObject.hpp"

class IResponsible
{
public:
	IResponsible() : changed(true) 
	{ 
		minPrio = INT_MAX;
		maxPrio = INT_MIN;
	}
	~IResponsible() { }
	void AddObject(GameObject* obj, int prio = 0)
	{
		if (!obj) return;

		minPrio = std::min(minPrio, prio);
		maxPrio = std::max(maxPrio, prio);

		objects[prio].push_back(obj);

		//if (objects.find(prio) == objects.end())
		//	objects[prio] = 

		changed = true;
	}
	void RemoveObject(GameObject* obj)
	{
		//auto it = objects.find(obj);
		for (int i = minPrio; i <= maxPrio; i++)
		{
			auto it = std::find(objects[i].begin(), objects[i].end(), obj);
			if (it != objects[i].end())
			{
				delete obj;
				obj = 0;
				objects[i].erase(it);
				break;
			}
		}

		changed = true;
	}

	std::vector<GameObject*> GetFlat()
	{
		if (changed)
		{
			std::vector<GameObject*> objs;
			for (auto& vec : objects)
			{
				for (auto& obj : vec.second)
				{
					objs.push_back(obj);
				}
			}

			cachedFlat = objs;
			changed = false;

			return objs;
		}
		else
		{
			return cachedFlat;
		}
	}
protected:
	std::map<int, std::vector<GameObject*>> objects;
private:
	int minPrio;
	int maxPrio;
	std::vector<GameObject*> cachedFlat;
	bool changed;
};

