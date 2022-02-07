#ifndef _BOOK_H_
#define _BOOK_H_

#include <string>
#include <locale>

class Book
{
private:
	std::string file_name;
	std::string type;
	std::string title;
	std::string author_name;
public:
	Book(std::string file_name,std::string type,std::string title,std::string author_name);
	~Book();
	std::string get_file_name() const;
	std::string get_type() const;
	std::string get_title() const;
	std::string get_author_name() const;
};

#endif // _BOOK_H_

