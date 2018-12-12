#include <iostream>
#include <vector>
#include <utility>

class gadget
{
public:
	gadget(size_t len = 0, int const * data = nullptr) :
		len(len),
		data(new int[len])
	{
		std::cout << "Constructor" << std::endl;
		if (data) {
			std::copy(data, data + len, this->data);
		}
	}

	// Rule of Three
	~gadget() 
	{ 
		std::cout << "Destructor" << std::endl;
		delete [] data;
	}
	gadget(const gadget &rh) :
		data(nullptr),
		len(0)
	{
		std::cout << "Copy Constructor" << std::endl;
		*this = rh; // use copy assignment operator
	}
	gadget& operator= (const gadget &rh)
	{ 
		// this is the only member that actually copies the data
		std::cout << "Copy Assignment Operator" << std::endl;
		if (this != &rh)
		{
			// optimization, do not need to realloc if the arrays are the same size
			if (len != rh.len)
			{
				delete[] data;
				len = rh.len;
				data = new int[len];
			}
			std::cout << "copying all elements of array..." << std::endl;
			std::copy(rh.data, rh.data + rh.len, data);
		}
		return *this;
	}

	// rule of Five
	gadget(gadget &&rh) :
		data(std::exchange(rh.data, nullptr)),
		len(std::exchange(rh.len, 0))
	{
		std::cout << "Move Constructor" << std::endl;
	}	
	gadget& operator= (gadget &&rh) &
	{
		std::cout << "Move Assignment Operator" << std::endl;
		// cheap pointer swap. rh destructor cleans up.
		swap(rh);
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& os, const gadget& g);

	size_t len;
	int *data;

private:
	// need this for the move assignment operator
	void swap(gadget& g)
	{
		std::cout << "internal swap function" << std::endl;
		std::swap(len, g.len);
		std::swap(data, g.data);
	}
};

std::ostream& operator<<(std::ostream& os, const gadget& g)
{
	os << "[";
	if (g.len > 0)
		os << g.data[0];
	for (size_t i = 1; i < g.len; i++)
		os << ", " << g.data[i];
	os << "]";
	return os;
}

gadget f(gadget i) 
{
	return i;
}

int main()
{
	gadget g(20);
	for(size_t i=0; i<g.len; i++)
		g.data[i] = i;
	std::cout << "g: " << g << std::endl;

	gadget h(5);
	for(size_t i=0; i<h.len; i++)
		h.data[i] = h.len - 1 - i;
	std::cout << "h: " << h << std::endl;

	std::cout << std::endl << "std::swap(g, h)" << std::endl;
	std::swap(g, h);
	std::cout << "h: " << h << std::endl;
	std::cout << "g: " << g << std::endl;

	std::cout << std::endl << "h = g" << std::endl;
	h = g;
	std::cout << "h: " << h << std::endl;
	std::cout << "g: " << g << std::endl;

	std::cout << std::endl << "h.data[1] = 111" << std::endl;
	h.data[1] = 111;
	std::cout << "h: " << h << std::endl;
	std::cout << "g: " << g << std::endl;

	std::cout << std::endl << "h = gadget(10)" << std::endl;
	h = gadget(10);
	std::cout << "h: " << h << std::endl;
	std::cout << "g: " << g << std::endl;

	std::cout << std::endl << "g = gadget(g)" << std::endl;
	g = gadget(g);
	std::cout << "h: " << h << std::endl;
	std::cout << "g: " << g << std::endl;

	std::cout << std::endl << "g = gadget(std::move(g))" << std::endl;
	g = gadget(std::move(g));
	std::cout << "h: " << h << std::endl;
	std::cout << "g: " << g << std::endl;

	std::cout << std::endl << "g = g" << std::endl;
	g = g;
	std::cout << "h: " << h << std::endl;
	std::cout << "g: " << g << std::endl;

	std::cout << std::endl << "h = f(h)" << std::endl;
	h = f(h);
	std::cout << "h: " << h << std::endl;
	std::cout << "g: " << g << std::endl;
	return 0;
}
