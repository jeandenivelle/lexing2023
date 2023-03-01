

template< typename C, typename T >
void print_table(lexing::classifier<C, T>& cl,
	std::string dec1, std::string dec2, std::ostream& file,
	const std::function< void(std::ostream& out, const C& c) >& printC,
	const std::function< void(std::ostream& out, const T& t) >& printT);

template< typename C, typename T >
void print_code(lexing::classifier<C, T>& cl, T error, std::ostream& file,
	const std::function< void(std::ostream& out, const C& t)>& printC,
	const std::function< void(std::ostream& out, const T& t)>& printT);

template< typename C, typename S >
void simple(std::map<S, std::set<C>>& f, unsigned int s, int blocked, std::ostream& file,
	const std::function< void(std::ostream& out, const C& t)>& printC);

template< typename C, typename S, typename T >
void isolated(lexing::flatmap<C, S>& delta, unsigned int s, int blocked, std::ostream& file,
	const std::function< void(std::ostream& out, const C& t)>& printC,
	const std::function< void(std::ostream& out, const T& t)>& printT);

template< typename C, typename S, typename T>
	void linear(C c1, size_t i, size_t j, lexing::flatmap<C, S>& delta,
		unsigned int s, int blocked, int ntabs, std::ostream& file,
		const std::function< void(std::ostream& out, const C& t)>& printC,
		const std::function< void(std::ostream& out, const T& t)>& printT);

template< typename C, typename S, typename T >
void binary(C c1, size_t i, size_t j, lexing::flatmap<C, S>& delta,
	unsigned int s, int blocked, int ntabs, std::ostream& file,
	const std::function< void(std::ostream& out, const C& t)>& printC,
	const std::function< void(std::ostream& out, const T& t)>& printT);

#define build_mode 0

std::string test("< > [ ] ( ) <> [] var true ~ & | -> <-> // comment\n");

int main(int argc, char* argv[])
{
	/*
	varstore< form > vs;

	tokenizer tok(filereader(&std::cin, "stdin"));
	std::cout << tok << "\n";

	parser prs(tok, vs);

	prs.debug = 2;
	auto res = prs.parse(sym_Session);

	std::cout << "parser returned " << res << "\n";*/

	tokenizer t;
	auto cls = t.getclassifier();
	std::cout << cls << std::endl;
	test_table(cls);
	test_code(cls);
}

void printchar(std::ostream& out, int c)
{
	//out << "'" << c << "'";
	out << c;
}

void printsymbol(std::ostream& out, symboltype t)
{
	out << "sym_" << t;
}

void printstring(std::ostream& out, std::string& s) {
	out << "\"" << s << "\"";
}


void test_table(lexing::classifier<char, symboltype>& cls)
{
#if build_mode
	std::ofstream file;
	file.open("table.h");
	print_table<char, symboltype>(cls,
		"std::vector< lexing::deterministic<char, symboltype> ::state_info > st",
		"std::vector< lexing::deterministic<char, symboltype> ::trans > tr",
		file, printchar, printsymbol);
	file.close();
#else
#include "table.h"
	lexing::deterministic<char, symboltype> det(st, tr);

	std::istringstream teststr(test);
	std::istream& input(teststr);
	filereader testread(&input, "test");

	auto start = std::chrono::steady_clock::now();

	while (true)
	{
		if (!testread.has(1))
			break;

		auto p = det.readandclassify(testread);
		std::cout << "get = " << p.first << "/" << p.second << std::endl;
		if (p.second == 0)
			p.second = 1;
		testread.commit(p.second);
	}

	auto end = std::chrono::steady_clock::now();
	auto ns = end - start;
	std::cout << "with table: " << ns.count() << " ns = " << (float)ns.count() / 1000000 << " ms \n\n";
#endif
}


// Generates table
template< typename C, typename T >
void print_table(lexing::classifier<C, T>& cls,
	std::string dec1, std::string dec2, std::ostream& file,
	const std::function< void(std::ostream& out, const C& c) >& printC,
	const std::function< void(std::ostream& out, const T& t) >& printT)
{
	// Table info
	std::vector<lexing::deterministic<char, symboltype>::state_info>  st;
	std::vector<lexing::deterministic<char, symboltype>::trans> tr;

	for (size_t i = 0; i < cls.nrstates(); i++)
	{
		st.push_back(lexing::deterministic<char, symboltype>::state_info(tr.size(), cls.classifications[i]));
		for (auto it = cls.delta[i].noneps.begin(); it != cls.delta[i].noneps.end(); ++it) {
			if (it->second == lexing::deterministic<C, T>::blocked)
				tr.push_back(lexing::deterministic<char, symboltype>::trans(it->first, it->second));
			else
				tr.push_back(lexing::deterministic<char, symboltype>::trans(it->first, i + it->second));
		}
	}

	file << dec1 << " = ";
	file << "{";

	for (size_t i = 0; i < st.size(); i++)
	{
		auto s = st[i];
		file << "{" << s.start << ", ";
		printT(file, s.token);
		file << "}";
		if (i != st.size() - 1)
			file << ", ";
	}
	file << "};\n";

	file << dec2 << " = ";
	file << "{";
	for (size_t i = 0; i < tr.size(); i++)
	{
		auto t = tr[i];
		file << "{";
		printC(file, t.c);
		file << ", " << t.start << "}";
		if (i != tr.size() - 1)
			file << ", ";
	}
	file << "};\n";
}


void test_code(lexing::classifier<char, symboltype>& cls)
{
#if build_mode
	std::ofstream file;
	file.open("code.h");
	print_code<char, symboltype>(cls, cls.classifications[0], file, printchar, printsymbol);
	file.close();
#else
	std::istringstream teststr(test);
	std::istream& input(teststr);
	filereader testread(&input, "test");

	auto start = std::chrono::steady_clock::now();
	while (true)
	{
		if (!testread.has(1))
			break;

		auto p = simulate(cls.classifications[0], testread);

		std::cout << "get = " << p.first << "/" << p.second << std::endl;
		if (p.second == 0)
			p.second = 1;
		testread.commit(p.second);
	}

	auto end = std::chrono::steady_clock::now();
	auto ns = end - start;

	std::cout << "with code: " << ns.count() << " ns = " << (float)ns.count() / 1000000 << " ms \n\n";
#endif
}


// This function generates C++ code for a working classifier.
// The resulting program has the same interface as readandclassify
// Function printC prints a C, while
// function printT prints a T.

template< typename C, typename T >
void print_code( const lexing::classifier<C,T> & cl, std::ostream& file,
   const std::function< void( std::ostream& out, const C& t ) > & printC,
   const std::function< void( std::ostream& out, const T& t ) > & printT )
{
   int blocked = lexing::deterministic<C,T> ::blocked;

   file << "template< typename T >\n";
   file << "std::pair< T, size_t > simulate(T error, filereader& read)\n";
   file << "{\n";
   file << "\tstd::pair<T, size_t> latest = std::make_pair(error, 0);\n";
   file << "\tif (!read.has(1))\n\t\tthrow std::logic_error(\"end of file must be checked in advance\"); \n";
   file << "\tchar c; size_t i = 0;\n";

	for (size_t i = 0; i < cl.nrstates(); i++)
	{
		file << "\nQ" << i << ":\n";

		if (cl.classifications[i] != error)
		{
			file << "\tlatest.first = ";
			printT(file, cl.classifications[i]);
			file << "; latest.second = i;\n";
		}

		if (!(cl.delta[i].noneps.size() == 1 && cl.delta[i].noneps.begin()->second == blocked))
		{
			file << "\tif (!read.has(i + 1))\n\t\treturn latest;\n";
			file << "\n\tc = read.peek(i);\n";
			//file << "\tstd::cout << \" state" << i << ": \" << i << \" \" << c << \" \" << std::endl;\n\n";
		}

		//linear(cl.delta[i].noneps.begin()->first, 0, cl.delta[i].noneps.size(), cl.delta[i].noneps, i, blocked, 1, file, printC, printT);
		isolated(cl.delta[i].noneps, i, blocked, file, printC, printT);
	}

	file << "\n\treturn latest;\n}\n";
}

// Generates switch case
// to be able to group characters that lead to the same state,
// f consits of pairs <state number, character> ordered by state number
template< typename C, typename S >
void simple(std::map<S, std::set<C>>& f, unsigned int s, int blocked, std::ostream& file,
	const std::function< void(std::ostream& out, const C& t)>& printC)
{
	// if only one character requires equality check,
	// print one if statement
	if (f.size() == 1 && f.begin()->second.size() == 1)
	{
		file << "\tif (c == ";
		printC(file, *(f.begin()->second.begin()));
		file << ")";
		if (f.begin()->first == blocked)
			file << " return latest;\n";
		else
			file << " { ++i; goto Q" << s + f.begin()->first << "; }\n";

		return;
	}

	file << "\tswitch(c) {\n";

	for (auto p = f.begin(); p != f.end(); ++p)
	{
		for (auto c : p->second)
		{
			file << "\t\tcase ";
			printC(file, c);
			file << ":\n";
		}

		file << "\t\t\t";
		if (p->first == blocked)
			file << "return latest;\n";
		else
			file << "++i; goto Q" << s + p->first << ";\n";
	}

	file << "\t}\n";


	/*
	for (auto p : f)
	{
		file << "\tif (c == ";
		printC(file, p.first);
		file << ")";
		if (p.second == blocked)
			file << " return latest;\n";
		else
			file << " { ++i; goto Q" << s + p.second << "; }\n";
	}
	*/
}

template< typename C, typename S, typename T >
void isolated(lexing::flatmap<C, S>& delta, unsigned int s, int blocked, std::ostream& file,
	const std::function< void(std::ostream& out, const C& t)>& printC,
	const std::function< void(std::ostream& out, const T& t)>& printT)
{
	lexing::flatmap<C, S> delta_reduced;
	std::map<S, std::set<C>> f; // the map will be passed to simple, if not empty

	for (auto p = delta.begin(); p != delta.end(); ++p)
	{
		// if the beginning of the next interval is the next character
		// after the beginning of the current interval, add it to map
		// otherwise, it stays in the border function
		if (p != delta.end() - 1 && p->first + 1 == (p + 1)->first)
		{
			if (f.count(p->second))
				f[p->second].insert(p->first);
			else
				f[p->second] = { p->first };
		}
		else
		{
			delta_reduced.append(p->first, p->second);
		}
	}

	if (!f.empty())
		simple(f, s, blocked, file, printC);

	binary(delta.begin()->first, 0, delta_reduced.size(), delta_reduced, s, blocked, 1, file, printC, printT);
}

template< typename C, typename S, typename T >
	void linear(C c1, size_t i, size_t j, lexing::flatmap<C, S>& delta,
		unsigned int s, int blocked, int ntabs, std::ostream& file,
		const std::function< void(std::ostream& out, const C& t)>& printC,
		const std::function< void(std::ostream& out, const T& t)>& printT)
{
	// print intervals from j to i in decreasing order
	auto a = delta.begin() + i, b = delta.begin() + j - 1;
	a--;
	if (a == b && b->second == blocked) {
		file << "return latest;\n";
		return;
	}

	for (auto p = b; p != a; --p)
	{
		for (size_t n = 0; n < ntabs; n++)	file << "\t";

		if (p->first != c1) {
			file << "if (c >= ";
			printC(file, p->first);
			file << ")";

			if (p->second == blocked)
				file << " return latest;\n";
			else
				file << " { ++i; goto Q" << s + p->second << "; }\n";
		}
		else
		{
			if (p->second == blocked)
				file << "return latest;\n";
			else
				file << "++i; goto Q" << s + p->second << ";\n";
		}
	}
}

// i - start index
// j - end index
template< typename C, typename S, typename T >
void binary(C c1, size_t i, size_t j, lexing::flatmap<C, S>& delta,
	unsigned int s, int blocked, int ntabs, std::ostream& file,
	const std::function< void(std::ostream& out, const C& t)>& printC,
	const std::function< void(std::ostream& out, const T& t)>& printT)
{
	// print a newline before the first nested if
	if (ntabs == 1 && j != i + 1)
		file << '\n';

	// if length is smaller than 5, do linear print
	if (j - i + 1 < 5)
	{
		linear(c1, i, j, delta, s, blocked, ntabs, file, printC, printT);
		return;
	}

	size_t k = i + (j - i) / 2; // index that splits delta into two
	auto p = delta.begin() + k; // pointer to the first element of the upper part

	for (size_t n = 0; n < ntabs; n++)	file << "\t";
	file << "if (c >= ";
	printC(file, p->first);
	file << ") {\n";

	// print the upper part of the border function
	binary(p->first, k, j, delta, s, blocked, ntabs + 1, file, printC, printT);

	for (size_t n = 0; n < ntabs; n++)	file << "\t";

	file << "} else {\n";
	// print the lower part of the border function
	binary((delta.begin() + i)->first, i, k, delta, s, blocked, ntabs + 1, file, printC, printT);

	for (size_t n = 0; n < ntabs; n++)	file << "\t";
	file << "}\n";
}
