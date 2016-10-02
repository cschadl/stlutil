#include "workspace/stlutil/move_accumulate.h"

#include <iostream>
#include <string>
#include <vector>
#include <numeric>

class thing
{
private:
	std::string m_string;

public:
	thing() = default;

	static size_t copy_count;

	thing(const std::string& str) : m_string(str) { }
	thing(const char* cstr) : m_string(cstr) { }

	thing(size_t n, char c) : m_string(n, c) { }

	thing(const thing& rhs)
		: m_string(rhs.m_string)
	{
		std::cout << "Copying thing: " << rhs.m_string << std::endl;

		copy_count++;
	}

	thing& operator=(const thing& rhs)
	{
		std::cout << "Copy-assigning thing: " << rhs.m_string << std::endl;
		m_string = rhs.m_string;

		return *this;

		copy_count++;
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
};

size_t thing::copy_count = 0;

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

	auto out_thing = stlutil::move_accumulate(things.begin(), things.end(), thing(),
	//auto out_thing = std::accumulate(things.begin(), things.end(), thing(),
		[](thing out, const thing& in_thing)
		{
			out.concat(in_thing);

			return out;
		});

	std::cout << out_thing.str() << std::endl;

	std::cout << "Made " << thing::copy_count << " copies" << std::endl;

	for (const thing& t : things)
		std::cout << t.str() << std::endl;

	return 0;
}
