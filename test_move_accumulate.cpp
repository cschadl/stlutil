#include "move_accumulate.h"

#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <memory>

class thing
{
private:
	std::shared_ptr<size_t>	m_copy_count;
	std::string 				m_string;

public:
	thing(const std::string& str)
		: m_copy_count(new size_t) 
		, m_string(str)
	{
		*m_copy_count = 0;
	}

	thing(const char* cstr)
		: thing(std::string(cstr))
	{
	}

	thing(size_t n, char c)
		: thing(std::string(n, c))
	{
	}

	thing() : thing(std::string("")) { }

	thing(const thing& rhs)
		: m_copy_count(rhs.m_copy_count)
		, m_string(rhs.m_string)
	{
		//std::cout << "Copying thing: " << rhs.m_string << std::endl;

		(*m_copy_count)++;
	}

	thing& operator=(const thing& rhs)
	{
		//std::cout << "Copy-assigning thing: " << rhs.m_string << std::endl;
		m_copy_count = rhs.m_copy_count;
		m_string = rhs.m_string;

		(*m_copy_count)++;

		return *this;
	}

	// If these aren't explicitly specified (or, implemented as above)
	// then we'll get lots of copies with accumulate or move_accumulate
	thing(thing&&) = default;
	thing& operator=(thing&&) = default; 

	const std::string& str() const { return m_string; }

	void concat(const thing& t)
	{
		m_string += t.str();
	}

	size_t copy_count() const { return *m_copy_count; }
};

int main(int argc, char** argv)
{
	std::vector<thing> things;
	things.reserve(26);

	char c = 'a';
	for (int i = 0 ; i < 26 ; )
		things.emplace_back(++i, c++);

	auto thing_concat = 	
		[](thing out, const thing& in_thing)
		{
			out.concat(in_thing);

			return out;
		};

	auto out_thing1 = stlutil::move_accumulate(things.begin(), things.end(), thing(), thing_concat);
	auto out_thing2 = std::accumulate(things.begin(), things.end(), thing(), thing_concat);

	std::cout << "out_thing1: " << std::endl << out_thing1.str() << std::endl;
	std::cout << "out_thing2: " << std::endl << out_thing2.str() << std::endl;
	std::cout << "Made " << out_thing1.copy_count() << " copies for out_thing1 (move_accumulate)" << std::endl;
	std::cout << "Made " << out_thing2.copy_count() << " copies for out_thing2 (std::accumulate)" << std::endl;

	return 0;
}
