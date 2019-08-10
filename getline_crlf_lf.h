// STL Utilities
// Helpers for dealing with DOS CRLF or Unix LF
#ifndef _STLUTIL_GETLINE_CRLF_LF_H
#define _STLUTIL_GETLINE_CRLF_LF_H

#include <istream>
#include <string>

namespace stlutil
{

// Helper template for newline chars
template <typename char_t>
struct newline_traits { };

template <>
struct newline_traits<char>
{
	static constexpr char cr() { return '\r'; }
	static constexpr char lf() { return '\n'; }
};

template <>
struct newline_traits<wchar_t>
{
	static constexpr wchar_t cr() { return L'\r'; }
	static constexpr wchar_t lf() { return L'\n'; }
};

/// Function for reading from either DOS CRLF or Unix LF.  Use instead of std::getline()
/// Adapted from http://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf
/// But extended for UNICODE (maybe this isn't necessarry?)
template <typename char_t>
std::basic_istream<char_t>& getline_crlf_lf(std::basic_istream<char_t>& is, std::basic_string<char_t>& t)
{
	t.clear();

	// The characters in the stream are read one-by-one using a std::streambuf.
	// That is faster than reading them one-by-one using the std::istream.
	// Code that uses streambuf this way must be guarded by a sentry object.
	// The sentry object performs various tasks,
	// such as thread synchronization and updating the stream state.

	typename std::basic_istream<char_t>::sentry se(is, true /* dont skip whitespace */);
	std::basic_streambuf<char_t>* sb = is.rdbuf();

	for(;;)
	{
		int c = sb->sbumpc();
		switch (c)
		{
		case newline_traits<char_t>::lf():
			return is;
		case newline_traits<char_t>::cr():
			if(sb->sgetc() == newline_traits<char_t>::lf())
				sb->sbumpc();
			return is;
		case EOF:
			// Also handle the case when the last line has no line ending
			if(t.empty())
				is.setstate(std::ios::eofbit);
			return is;
		default:
			t += (char_t) c;
		}
	}

	return is;
}

};	// namespace stlutil

#endif // _STLUTIL_GETLINE_CRLF_LF_H
