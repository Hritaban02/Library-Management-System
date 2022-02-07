#include "Book.h"
#include <string>
#include <locale>

Book::Book(std::string file_name_val,std::string type_val,std::string title_val,std::string author_name_val)
: file_name{file_name_val} , type{type_val} , title{title_val} , author_name{author_name_val}
{
}

Book::~Book()
{
}

std::string Book::get_file_name() const{
	return this->file_name;
}

std::string Book::get_type() const{
	return this->type;
}

std::string Book::get_title() const{
	return this->title;
}

std::string Book::get_author_name() const{
	return this->author_name;
}
