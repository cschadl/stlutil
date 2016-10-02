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
	//static size_t copy_count;

	thing(const std::string& str)
		: m_copy_count(new size_t) 
		, m_string(str)
	{
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
		std::cout << "Copying thing: " << rhs.m_string << std::endl;

		(*m_copy_count)++;
	}

	thing& operator=(const thing& rhs)
	{
		std::cout << "Copy-assigning thing: " << rhs.m_string << std::endl;
		m_copy_count = rhs.m_copy_count;
		m_string = rhs.m_string;

		return *this;

		(*m_copy_count)++;
	}

//	thing(thing&& rhs)
//		: m_string(std::move(rhs.m_string))
//	{
//		//std::cout << "Moved thing " << m_string << std::endl;
//	}
//
//	thing& operator=(thing&& rhs)
//	{
//		m_string = std::move(rhs.m_string);
//		//std::cout << "Move-assigned thing " << m_string << std::endl;
//
//		return *this;
//	}

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
	// This makes lots of copies ...
	//std::vector<thing> things = { "a" , "bb", "ccc" };
	//
	
	// But, this doesn't...
	std::vector<thing> things;
	things.reserve(26);

	char c = 'a';
	for (int i = 0 ; i < 26 ; )
		things.emplace_back(++i, c++);

	//auto out_thing = stlutil::move_accumulate(things.begin(), things.end(), thing(),
	auto out_thing = std::accumulate(things.begin(), things.end(), thing(),
		[](thing out, const thing& in_thing)
		{
			out.concat(in_thing);

			return out;
		});

	std::cout << out_thing.str() << std::endl;

	std::cout << "Made " << out_thing.copy_count() << " copies" << std::endl;

	for (const thing& t : things)
		std::cout << t.str() << std::endl;

	return 0;
}
