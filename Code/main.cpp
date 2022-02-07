#include <iostream>
#include <iomanip>
#include <cstdio>
#include <string>
#include <sstream>
#include <locale>
#include <fstream>
#include <dirent.h>
#include <algorithm>
#include <regex>
#include <list>
#include <set>
#include <queue>

#include "Book.h"

void Read_From_Index(std::fstream &,std::list <Book> &);
bool Check_Index(std::string &,std::list <Book> &);
void Add_To_Index(std::string &,std::string &,std::list <Book> &);
void Removing_Books_Not_Present_In_Library_from_Index(std::string &,std::list <std::string> &,std::list <Book> &);
void Display_Books(std::list <Book> &);
void Search_For_Books(std::list <Book> &,std::list <Book> &);
void Search_For_Books_By_Title(std::string &,std::list <Book> &,std::list <Book> &);
void Search_For_Books_By_Name(std::string &,std::list <Book> &,std::list <Book> &);
int Select_A_Book(std::string &,std::list <Book> &);
void Display_Whole_Book(std::string &,std::string &);
void What_To_Do_With_Book(std::string &,std::list <Book> &,int);
void First_K_Things(std::string &,std::string);
void First_K_Paragraph(std::string &,std::string,std::string &);
int Count_Keyword_In_Para(std::list <std::string> &,std::string &);
void First_K_Chapter(std::string &,std::string,std::string &);
void Character_in_the_scene(std::string &,std::string &,std::string,std::set <std::string> &);
void remove_punct(std::string &);


std::locale loc("en_US.UTF-8");

int main(int argc,char** args)
{
	if(argc > 2)
	{
		std::cout << "The limit of arguments have been exceeded. Please enter only directory path in the command line." << std::endl;
		return(1);
	}
	else if(argc < 2)
	{
		std::cout << "Too less arguments. Please enter the directory path in the command line." << std::endl;
		return(2);
	}

    
	
	std::cout << "*****WELCOME TO THE LIBRARY MANAGEMENT SYSTEM*****" << std::endl;
	std::cout << std::endl;
	struct dirent *directory_entry;  // Pointer for directory entry 
  
    // opendir() returns a pointer of DIR type.  
    DIR *dr = opendir(args[1]); 
  
    if (dr == NULL)  // opendir returns NULL if couldn't open directory 
    { 
        std::cout << "Could not open current directory" << std::endl; 
        return 0; 
    }

	
	std::string directory=args[1];

 
	std::fstream index_file;
	index_file.open(directory+"/"+std::string("index.txt"));
	index_file.imbue(loc);
	if(!index_file)
	{
		std::cerr << "Error while opening Index File" << std::endl;
		return (3);
	}
	
	std::list <Book> Book_List;
	Read_From_Index(index_file,Book_List);
	
	index_file.close();
	
	
	
	std::list <std::string> Books_In_Library_At_Present;
	
	
  	int count{0};
    while ((directory_entry = readdir(dr)) != NULL) 
    {
		std::string file_name=directory_entry->d_name; 
		
        if(file_name != "." && file_name!=".." && file_name!="index.txt")
        {
			Books_In_Library_At_Present.emplace_back(file_name);
        	count++;
        	if(!Check_Index(file_name,Book_List))
			{
				std::cout << "\nHOLY MOLY!!! There's a new book in the library!!!" << std::endl;
				std::cout << std::endl;
				Add_To_Index(directory,file_name,Book_List);
			}
        	
        }
            
    }
	
	Removing_Books_Not_Present_In_Library_from_Index(directory,Books_In_Library_At_Present,Book_List);
	
	
	if(count==0)
    {
    	std::cout << "There are no books in the Library...Add some books to the Library first." << std::endl;
		return 0;
    }
	
	
	
	std::list <Book> Search_List;
	
	
	char choice;
	do
	{
		std::cout << "\n\n*****LIBRARY MANAGEMENT SYSTEM MENU*****\n" << std::endl;
		std::cout << "1)To display all the books in the library press '1'.\n" << std::endl;
		std::cout << "2)To search for books by title or author's name press '2'.\n" << std::endl;
		std::cout << "Enter your choice: " << std::endl;
		std::cin >> choice;
		switch(choice)
		{
			case '1':
			{
				std::cout << "\n___List Of All The Books In The Library___\n" << std::endl;
 
				Display_Books(Book_List);
				Search_List=Book_List;
				break;
			}
			case '2':
			{
				Search_For_Books(Book_List,Search_List);
				
				break;
			}
			default :
				std::cout << "\nWrong User Input!!!Please enter a correct choice!!!\n" << std::endl;
				choice='y';
		}
		if(choice=='1' || choice=='2')
		{
			std::cout << "\nWould you like to display another list\?\?(If yes press 'y' else any other key)\n";
			std::cin >> choice;
			std::cout << std::endl;
		}
	}while(choice=='y');
	
	int index=Select_A_Book(directory,Search_List);
	
	What_To_Do_With_Book(directory,Search_List,index);
	
	
    closedir(dr);     
    return 0; 
} 


void Read_From_Index(std::fstream &index_file,std::list <Book> &Book_List)
{
	std::string line;
	while(!index_file.eof())
	{
		std::getline(index_file,line);
		if(line.length()>0)
		{
			line.erase(remove(line.begin(), line.end(), '\t'), line.end());
			std::stringstream ISS{line};
			std::string file_name;
			std::string type;
			std::string title;
			std::string author_name;
			std::getline(ISS,file_name,'|');
			
			std::getline(ISS,type,'|');

			std::getline(ISS,title,'|');

			std::getline(ISS,author_name);
			Book_List.emplace_back(file_name,type,title,author_name);
		}
	}
}

bool Check_Index(std::string &file_name,std::list <Book> &Book_List)
{
	auto it=Book_List.begin();
	for(;it!=Book_List.end();it++)
	{
		if(it->get_file_name()==file_name)
		{
			return true;
		}
	}
	return false;
}

void Add_To_Index(std::string &directory,std::string &file_name_val,std::list <Book> &Book_List)
{
	std::fstream curr_file;
	curr_file.open(directory+"/"+file_name_val);
	curr_file.imbue(loc);
	
	if(!curr_file)
	{
		std::cerr << "Error while opening "<< file_name_val << std::endl;
		return;
	}
	
	std::string file_name=file_name_val;
	
	std::string type;
	std::cout << "Enter the type of the book " << file_name << " is?" << std::endl;
	std::cin >> type;
	
	std::string title;
	std::string author_name;
	
	std::string line;
	while(!curr_file.eof())
	{
		std::getline(curr_file,line);
		
		if(line.find("Title: ")!=std::string::npos)
		{
			
			line.erase(remove(line.begin(), line.end(), '\n'), line.end());
			line.erase(remove(line.begin(), line.end(), '\r'), line.end());
			title=line.substr(line.find("Title: ")+7);
			
			break;
		}
		if(line.find("***")!=std::string::npos)
		{
			title="Title Not Found!!!";
			break;
		}
	}
	
	curr_file.clear();
	
	curr_file.seekg(0,std::ios::beg);
	
	while(!curr_file.eof())
	{
		std::getline(curr_file,line);
		;
		if(line.find("Author: ")!=std::string::npos)
		{
			line.erase(remove(line.begin(), line.end(), '\n'), line.end());
			line.erase(remove(line.begin(), line.end(), '\r'), line.end());
			author_name=line.substr(line.find("Author: ")+8);
			break;
		}
		if(line.find("***")!=std::string::npos)
		{
			author_name="Author Name Not Found!!!";
			break;
		}
	}
	
	
	std::fstream index_file;
	index_file.open(directory+"/"+std::string("index.txt"),std::ios_base::app);
	index_file.imbue(loc);
	if(!index_file)
	{
		std::cerr << "Error while opening Index File" << std::endl;
		return;
	}
	

	index_file << file_name << "\t|\t" << type << "\t|\t" << title << "\t|\t" << author_name << std::endl;
	
	
	
	Book_List.emplace_back(file_name,type,title,author_name);
	
	
	
	curr_file.close();
	index_file.close();
}

void Removing_Books_Not_Present_In_Library_from_Index(std::string &directory,std::list <std::string> &Books_In_Library_At_Present,std::list <Book> &Book_List)
{
	auto it=Book_List.begin();
	int count=0;
	for(;it!=Book_List.end();)
	{
		if(std::find(Books_In_Library_At_Present.begin(), Books_In_Library_At_Present.end(), it->get_file_name()) != Books_In_Library_At_Present.end())
		{
			it++;
		}
		else
		{
			it=Book_List.erase(it);
			count++;
		}
	}
	
	
	if(count>0)
	{
		std::fstream index_file;
		index_file.open(directory+"/"+std::string("index.txt"),std::ios::out | std::ios::trunc);
		
		if(!index_file)
		{
			std::cerr << "Error while opening Index File" << std::endl;
			return;
		}
		it=Book_List.begin();
		for(;it!=Book_List.end();it++)
		{
			index_file << it->get_file_name() << "\t|\t" << it->get_type() << "\t\\t" << it->get_title() << "\t|\t" << it->get_author_name() << std::endl;
		}
		index_file.close();
	}
}


void Display_Books(std::list <Book> &Book_List)
{
	
	auto it=Book_List.begin();
	int i=0;
	std::cout << "\n\t" << std::setw(30) << std::left << "FILENAME" << std::setw(30) << std::left << "TITLE" << std::setw(30) << std::left << "AUTHOR" << std::endl;
	for(;it!=Book_List.end();it++)
	{
		std::cout << ++i << ")\t" << std::setw(30) << std::left << it->get_file_name() << std::setw(30) << std::left  << it->get_title() << std::setw(30) << std::left  << it->get_author_name() << std::endl;
	}
}


void Search_For_Books(std::list <Book> &Book_List,std::list <Book> &Search_List)
{
	std::string keyword;
	char choice;
	
	do
	{
		std::cout << "\n\n*****SEARCH MENU*****" << std::endl;
		std::cout << "T)To search for books by title press 'T'." << std::endl;
		std::cout << "A)To search for books by author's name press 'A'." << std::endl;
		std::cout << "q)To exit the menu." << std::endl << std::endl;
		std::cout << "Enter your choice: " << std::endl ;
		std::cin >> choice;
		switch(choice)
		{
			case 'T':
			{
				std::cout << "Enter the keyword you want to search for in the titles: " << std::endl;
				std::cin >> keyword;
				std::cout << std::endl << "\n___Searches that match with \"" << keyword << "\"___\n" << std::endl;
				Search_For_Books_By_Title(keyword,Book_List,Search_List);
				if(Search_List.size()==0)
				{
					std::cout << "\n### No Match Found!!!!Please Try Again ###\n" << std::endl;
					
				}
				break;
			}
			case 'A':
			{
				std::cout << "Enter the keyword you want to search for in the author's name: " << std::endl;
				std::cin >> keyword;
				std::cout << std::endl << "\n___Searches that match with \"" << keyword << "\"___\n" << std::endl;
				Search_For_Books_By_Name(keyword,Book_List,Search_List);
				if(Search_List.size()==0)
				{
					std::cout << "\n### No Match Found!!!!Please Try Again ###\n" << std::endl;
				}
				break;
			}
			case 'q':
				std::cout << "Bye, Bye!!" << std::endl;
				break;
			default :
				std::cout << "Wrong User Input!!!Please enter a correct choice!!!" << std::endl;
		}
	}while(choice!='q');
}


void Search_For_Books_By_Title(std::string &keyword,std::list <Book> &Book_List,std::list <Book> &Search_List)
{
	
	std::transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);
	
	std::string title_val;
	
	auto it=Book_List.begin();
	for(;it!=Book_List.end();it++)
	{
		title_val=it->get_title();
		std::transform(title_val.begin(),title_val.end(), title_val.begin(), ::tolower);
		if(title_val.find(keyword)!=std::string::npos)
		{
			Search_List.push_back(*it);
		}
	}
	if(Search_List.size()!=0)
	{
		Display_Books(Search_List);
	}
	
}


void Search_For_Books_By_Name(std::string &keyword,std::list <Book> &Book_List,std::list <Book> &Search_List)
{
	std::transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);
	
	std::string name_val;
	
	auto it=Book_List.begin();
	for(;it!=Book_List.end();it++)
	{
		name_val=it->get_author_name();
		std::transform(name_val.begin(),name_val.end(), name_val.begin(), ::tolower);
		if(name_val.find(keyword)!=std::string::npos)
		{
			Search_List.push_back(*it);
		}
	}
	if(Search_List.size()!=0)
	{
		Display_Books(Search_List);
	}
}

bool isCharacter(std::string &val)
{
	if(val.find("ACT")!=std::string::npos || val.size()==0)
	{
		return false;
	}
	char c;
	bool flag=true;
	for(size_t i=0;i<val.size();i++)
	{
		c=val[i];
		if(!(c>=65 && c<=90) && c!=32 && !(c>=48 && c<=57))
		{
			flag=false;
			return flag;
		}
	}
	return flag;
}

int Select_A_Book(std::string &directory,std::list <Book> &Search_List)
{
	int index;
	
	std::cout << "\nEnter the index number of the book you want to display from the above list: " << std::endl;
	std::cin >> index;
		
	int count=0;
	auto it=Search_List.begin();
	while(it!=Search_List.end())
	{
		count++;
		if(count==index)
		{
			std::string file_name_to_display=it->get_file_name();
			std::cout << "***********************************************************************" << std::endl;
			Display_Whole_Book(directory,file_name_to_display);
			break;
		}
		it++;
	}
	return index;
}

void Display_Whole_Book(std::string &directory,std::string &file_name)
{
	std::fstream curr_file;
	curr_file.open(directory+"/"+file_name);
	curr_file.imbue(loc);
	
	int count=0;
	char choice='y';
	std::string line;
	do
	{
		while(!curr_file.eof() && count<=50)
		{
			std::getline(curr_file,line);
			std::cout << line << std::endl;
			count++;
		}
		
		if(count==51)
		{
			std::cout << "\n\nDo you want to display the next 50 lines of " << file_name << " \?\?(If yes press 'y' else press any other key)" << std::endl;
			std::cin >> choice;
			count=0;
		}
		
	}while(choice=='y');
	curr_file.close();
}

void What_To_Do_With_Book(std::string& directory,std::list <Book> &Search_List,int index)
{
	int count=0;
	auto it=Search_List.begin();
	while(it!=Search_List.end())
	{
		count++;
		if(count==index)
		{
			std::string type_val=it->get_type();
			std::transform(type_val.begin(),type_val.end(), type_val.begin(), ::tolower);
			if(type_val=="novel")
			{
				char choice;
				std::cout << "\n*******" << it->get_title() << " Is A NOVEL!!!!*******" << std::endl;
				do{
					First_K_Things(directory,it->get_file_name());
					std::cout << "\nWould you like to enter another keyword for search\?\?(If yes press 'y' else any other key)" << std::endl;
					std::cin >> choice;
				}while(choice=='y');
			}
			else if(type_val=="play")
			{
				char choice;
				std::cout << "\n*******" << it->get_title() << " Is A PLAY!!!!*******" << std::endl;
				do{
					std::string Character;
					std::set <std::string> Char_list;
					std::cout << "\nEnter a valid character name to get the list of characters who have at least one scene with yours: " << std::endl;
					std::cin.ignore();
					std::getline(std::cin,Character);
					
					std::transform(Character.begin(),Character.end(), Character.begin(), ::toupper);
					
					Character_in_the_scene(Character,directory,it->get_file_name(),Char_list);
					std::cout << "*****************" << std::endl;
					std::cout << "\nList Of Characters in Scene with " << Character << " are:" << std::endl;
					if(Char_list.size()==0)
					{
						std::cout << "\n### No Characters are in the play with your character.\n### Your character might be dead or your character does not exist in the play. \n### Please check your spelling, enter the exact name that was used in the play and try again!!"<< std::endl; 
					}
					else
					{
						auto it=Char_list.begin();
						int i=0;
						while(it!=Char_list.end())
						{
							std::cout << ++i << ")" << *it << std::endl;
							it++;
						}
					}
					std::cout << "\nWould you like to enter another Character\?\?(If yes press 'y' else any other key)" << std::endl;
					std::cin >> choice;
				}while(choice=='y');
			}
			break;
		}
		it++;
	}
}

void First_K_Things(std::string &directory,std::string file_name_val)
{
	std::string keyword;
	std::cin.ignore();
	std::cout << "\n\nEnter the keyword you want to search for: " << std::endl;
	std::cin >> keyword;
	
	
	char choice;
	do
	{
		std::cout << "\nDo you want to display the first k paragraphs or chapters\?\?" << std::endl;
		std::cout << "1)Paragraphs" << std::endl;
		std::cout << "2)Chapters" << std::endl;
		std::cout << "Enter your choice(1 or 2):" << std::endl;
		std::cin >> choice;
		switch(choice)
		{
			case '1':
				First_K_Paragraph(directory,file_name_val,keyword);
				break;
			case '2':
				First_K_Chapter(directory,file_name_val,keyword);
				break;
			default:
				std::cout << "Wrong Input!!! Please try again!!!" << std::endl;
		}
	}while(choice !='1' && choice!='2');
}


void First_K_Paragraph(std::string &directory,std::string file_name_val,std::string& keyword)
{
	size_t k;
	std::cout << "\nEnter the number of Paragraphs to be displayed:" << std::endl;
	std::cin >> k;
	
	std::fstream curr_file;
	curr_file.open(directory+"/"+file_name_val);
	curr_file.imbue(loc);
	
	std::priority_queue< std::pair <int, std::list<std::string> > > pq; 
	
	std::string line;
	while(!curr_file.eof())
	{
		std::getline(curr_file,line);
		line.erase(remove(line.begin(), line.end(), '\n'), line.end());
		line.erase(remove(line.begin(), line.end(), '\r'), line.end());
		
		if(line.find("CHAPTER")!=std::string::npos)
		{
			break;
		}
	}
	do
	{
		
		if(line.find("CHAPTER")!=std::string::npos || line.size()==0)
		{
			
			std::getline(curr_file,line);
			
			line.erase(remove(line.begin(), line.end(), '\n'), line.end());
			line.erase(remove(line.begin(), line.end(), '\r'), line.end());
			
			std::list <std::string> Para;
			do
			{
				if(line.size()!=0 && line.find("CHAPTER")==std::string::npos && !curr_file.eof())
				{
					Para.push_back(line);
					
				}
					
				std::getline(curr_file,line);
				
				line.erase(remove(line.begin(), line.end(), '\n'), line.end());
				line.erase(remove(line.begin(), line.end(), '\r'), line.end());
				
			}while(line.size()!=0 && line.find("CHAPTER")==std::string::npos && !curr_file.eof());
			
			int count=Count_Keyword_In_Para(Para,keyword);
			
			pq.push(std::pair<int,std::list <std::string>>(count, Para));
			Para.clear();
			
		}
		
	}while(!curr_file.eof());
	
	
	std::cout << "\n***********************************************************************\n" ;
	if(pq.size()>=k)
	{
		std::cout << "\nThe first " << k << " Paragraphs to have the keyword \"" << keyword << "\" with maximum count are: \n" << std::endl;
		size_t i=1;
		while(i<=k)
		{
			auto it=pq.top().second.begin();
			std::cout << "Number of times the word \"" << keyword << "\" occurs in the Paragraph below is: " << pq.top().first << "\n" << std::endl;
			while(it!=pq.top().second.end())
			{
				std::cout << *it << std::endl;
				it++;
			}
			pq.pop();
			i++;
			std::cout << "\n***********************************************************************\n" ;
		}
	}
	else
	{
		std::cout << "There are only " << pq.size() << " Paragraphs!!!" << std::endl;
		while(!pq.empty())
		{
			auto it=pq.top().second.begin();
			std::cout << "Number of times the word \"" << keyword << "\" occurs in the Paragraph below is: " << pq.top().first << "\n" << std::endl;
			while(it!=pq.top().second.end())
			{
				std::cout << *it << std::endl;
				it++;
			}
			pq.pop();
			
			std::cout << "\n***********************************************************************\n" ;
		}
	}
}

int Count_Keyword_In_Para(std::list <std::string> &Para,std::string &keyword)
{
	int count=0;
	
	
	auto it=Para.begin();
	while(it!=Para.end())
	{
		std::string line=*it;
		remove_punct(line);
		std::istringstream ISS{line};
		std::string word;
		while(ISS>>word)
		{
			if(word==keyword)
			{
				count++;
			}
		}
		it++;
	}
	return count;
}

void First_K_Chapter(std::string& directory,std::string file_name_val,std::string& keyword)
{
	size_t k;
	std::cout << "\nEnter the number of Chapters to be displayed:" << std::endl;
	std::cin >> k;
	
	std::fstream curr_file;
	curr_file.open(directory+"/"+file_name_val);
	curr_file.imbue(loc);
	
	std::priority_queue< std::pair <int, std::string > > pq; 
	
	std::string line;
	while(!curr_file.eof())
	{
		std::getline(curr_file,line);
		line.erase(remove(line.begin(), line.end(), '\n'), line.end());
		line.erase(remove(line.begin(), line.end(), '\r'), line.end());
		
		if(line.find("CHAPTER")!=std::string::npos)
		{
			break;
		}
	}
	do
	{
		
		if(line.find("CHAPTER")!=std::string::npos)
		{
			std::string Chapter=line;
			int count=0;
			
			std::getline(curr_file,line);
			line.erase(remove(line.begin(), line.end(), '\n'), line.end());
			line.erase(remove(line.begin(), line.end(), '\r'), line.end());
			
			do
			{
				if(line.find("CHAPTER")==std::string::npos && !curr_file.eof())
				{
					remove_punct(line);
					std::istringstream ISS{line};
					std::string word;
					while(ISS>>word)
					{
						
						if(word==keyword)
						{
							count++;
						}
					}
				}
					
				std::getline(curr_file,line);
				line.erase(remove(line.begin(), line.end(), '\n'), line.end());
				line.erase(remove(line.begin(), line.end(), '\r'), line.end());
				
			}while(line.find("CHAPTER")==std::string::npos && !curr_file.eof());
			
			pq.push(std::pair<int, std::string>(count, Chapter));
			
		}
		
	}while(!curr_file.eof());
	
	
	std::cout << "\n***********************************************************************\n" ;
	if(pq.size()>=k)
	{
		std::cout << "\nThe first " << k << " Chapters to have the keyword \"" << keyword << "\" with maximum count are: \n" << std::endl;
		size_t i=1;
		while(i<=k)
		{
			std::cout << "Number of times the word \"" << keyword << "\" occurs in the Chapter below is: " << pq.top().first << "\n" << std::endl;
			std::cout << pq.top().second << std::endl;
			pq.pop();
			i++;
			std::cout << "\n***********************************************************************\n" ;
		}
	}
	else
	{
		std::cout << "There are only " << pq.size() << " Paragraphs!!!" << std::endl;
		while(!pq.empty())
		{
			
			std::cout << "Number of times the word \"" << keyword << "\" occurs in the Chapter below is: " << pq.top().first << "\n" << std::endl;
			std::cout << pq.top().second << std::endl;
			pq.pop();
			
			std::cout << "\n***********************************************************************\n" ;
		}
	}
}

void Character_in_the_scene(std::string& Character,std::string& directory,std::string file_name_val,std::set <std::string> &Char_list)
{
	std::fstream curr_file;
	curr_file.open(directory+"/"+file_name_val);
	curr_file.imbue(loc);
	
	std::set <std::string> Scene_list;
	
	std::string line;
	while(!curr_file.eof())
	{
		std::getline(curr_file,line);
		
		if(line.find("SCENE")!=std::string::npos)
		{
			
			std::string curr_line;
			
			while(curr_line.find("SCENE")==std::string::npos && !curr_file.eof())
			{
				
				std::istringstream ISS{curr_line};
				std::string curr_string;
				std::getline(ISS,curr_string,'.');
				
				if(isCharacter(curr_string))
				{
					//std::cout << curr_string << std::endl;
					Scene_list.insert(curr_string);
				}
				
				std::getline(curr_file,curr_line);
			}
			
			if(Scene_list.find(Character)!=Scene_list.end())
			{
				std::set<std::string>::iterator it = Scene_list.begin();
				while(it!=Scene_list.end())
				{
					Char_list.insert(*it);
					it++;
				}
			}
			Scene_list.clear();
			
		}
	}
	curr_file.close();
}

void remove_punct(std::string &str)
{
	
	for (int i = 0, len = str.size(); i < len; i++) 
    { 
        // check whether parsing character is punctuation or not 
        if (ispunct(str[i])) 
        { 
            str.erase(i--, 1); 
            len = str.size(); 
        } 
    } 
}
