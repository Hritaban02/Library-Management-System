**********README**********

A library of books contains many books. Each book is stored in a text file in a given directory,
which contains the library. Each book has a title, an author, a release date, and a Language. These
can be read from the Project Gutenberg file header, see for example the novel Mobi Dick by
Herman Hesse:
https://www.gutenberg.org/files/2701/2701-0.txt
The file header is given below for reference.

A book can contain many types of content, e.g. a novel, a play, etc. For this assignment, you need
to implement a novel and play, but your software design should be such that other types of books
can be easily added. A novel can contain many chapters. Each chapter has a title followed by
many paragraphs. Each paragraph contains many sentences, and each sentence is a sequence of
words. A play has a optional prologue followed by many acts. Each act has many scenes, and each
scene is a sequence of dialogues. Additionally, a play also has a list of characters or personae,
where each dialogue is spoken by a character.

Use case 1:
You have to create a Library management system (LMS) which manages all the books in a given
directory. The program will start with a directory as input, and will manage the books in that
directory. The LMS will maintain the type of each book in the library, in a file called “index.txt”.
It should automatically detect all files in the directory and be able to detect the newly added
books. It should then ask the user for type of each of the newly added books and add them to
“index.txt”.

Use case 2:
The LMS should have the following functionality:
1. List all books along with their filename, title, and author name.
2. Search for a book by its title or author name, and show the matching books as a list.
3. Display a book from any of the above two lists.

Use case 3:
The LMS should be able to perform the following analytics tasks:
1. If the type of the selected book is novel, given an input word, the LMS should be able to
show the top 5 chapters, or top 5 paragraphs, where this word appears most of the times.
2. If the type of the selected book is play, then given an input character, the LMS should
show a list of all other characters which appear in at least one scene with the input
character.

Example Project Gutenberg file header:
The Project Gutenberg EBook of Moby Dick; or The Whale, by Herman
Melville
This eBook is for the use of anyone anywhere at no cost and with almost
no restrictions whatsoever. You may copy it, give it away or re-use it
under the terms of the Project Gutenberg License included with this
eBook or online at www.gutenberg.org
Title: Moby Dick; or The Whale
Author: Herman Melville
Release Date: December 25, 2008 [EBook #2701] Last Updated: December 3,
2017
Language: English
Character set encoding: UTF-8
*** START OF THIS PROJECT GUTENBERG EBOOK MOBY DICK; OR THE WHALE ***

In the terminal, go to the directory in which you have saved the source files

Execute command: make

Execute command: ./a.out "Path to Library"
