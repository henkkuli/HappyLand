#pragma once

template <class T>
class manager;
class entityManager;

#include <vector>
#include <algorithm>

template <class T>
class renderManager;
template <class T>
class manager {
public:
	virtual ~manager() {}

	virtual void removeAt(unsigned int i) {
		delete data[i];					// Deconstruct the item
		data[i] = data.back();			// Move the last to its place
		data.pop_back();				// Remove the last and shorten the vector by one
	}
	virtual void add(T *element) {
		data.push_back(element);		// Add the new element just to the end of the vector
	}

	virtual void update(double delta) = 0;

protected:
	// Constructor for inheritance
	manager() {
	}

	typedef typename std::vector<T*>::iterator iterator;
	iterator begin() {
		return data.begin();
	}
	iterator end() {
		return data.end();
	}

private:
	// Prevent copying
	manager(const manager&);
	manager& operator=(const manager&);

	std::vector<T*> data;

	friend class renderManager<T>;
};

// Specific managers

#include "entity.h"
class entityManager : public manager<entity> {
public:
	entityManager() {
	}

	virtual ~entityManager() {
	}

	virtual void update(double delta) {
		// Update every entity one by one
		for (iterator it = begin(); it < end(); ++it) {
			(*it)->update(delta);
		}
	}
};