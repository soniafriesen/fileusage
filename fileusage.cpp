/*
File: fileusage
purpose: To read and collect data from the hard drive based off of certain options
Coder: Sonia Friesen
Date: April 19,2019
*/
#include <deque>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include<filesystem>
#include <map>
#include <iomanip>
#include <regex>

using namespace std;
//this library is part C++17 but not completed in VS
//experimental means the library hasn't been finalized
using namespace std::experimental::filesystem;

//strct to get the filename, extension, number of extensions, and file sizes of those extensions.
struct Extension
{
	path::string_type ext;	
	size_t extCount;
	size_t fileSize;	
};
//struct to find the folder name and the total number of files, not the size.
struct File
{
	path::string_type folderName;
	size_t fileTotalSize;
	size_t extensionTotalCount;
	size_t allextCount;
};

/*
Method: operator << ();
Purpose: To overload it to read in the struct Extension
Agruments: THe ostream, and the Object extension
Returns: the ostream printout.
*/
ostream& operator << (ostream& os, Extension const& ex)
{
	os << right << setw(20) << ex.ext << right<< setw(2) << char(58) << right << setw(15) << ex.extCount << right << setw(2) << char(58) << right << setw(15) << ex.fileSize;
	return os;
}
/*
Method: operator << ();
Purpose: To overload it to read in the struct File
Agruments: THe ostream, and the Object extension
Returns: the ostream printout.
*/
ostream& operator << (ostream& os, File const& f)
{
	os << right << setw(20) << f.allextCount << right << setw(2) << char(58) << right << setw(15) << f.extensionTotalCount<< right << setw(2) << char(58) << right << setw(15) << f.fileTotalSize;
	return os;
}

/*
Method: operator <();
Purpose: To overload the compare operator extension names using Alpha
Arugments: Extension object
Returns: Object of Extension to whoes name is first alpha
*/
bool operator < (Extension const& lhs, Extension const& rhs)
{
	return lhs.ext < rhs.ext;		
}

/*
Method: sortDesc();
Purpose: To overload the compare extension sorting desc
Arugments: Extension object
Returns: Object of Extension desc
*/
bool sortDesc(Extension const& lhs, Extension const& rhs)
{
	return lhs.ext > rhs.ext;
}
/*
Method: sortFileSize();
Purpose: To sort the structs by thier file size
Arugments: Extension object
Returns: Object of Extension to whoes file size is less than the other
*/
bool sortFileSize(Extension const& lhs, Extension const& rhs)
{
	return lhs.fileSize < rhs.fileSize;
}

/*Method Name: scan();
Arguments: Takes in the path/directory
Purpose: Scan the folder once.
Returns: void*/
void scan(path const& f) //-r
{
	cout << "fileusage(c) April 19, 2019, Sonia Friesen" << endl;
	deque<Extension> extFiles;
	vector<File> folders;
	Extension files;
	File folder;
	
	//use iterators to walk through the folder, like the iterators traverse a container 
	//create a beginning directory iterator passing it the folder "path object", which represents the root of the search
	directory_iterator dir(f);

	//create an end iterator, which points to the end of the search, the end of any folder
	directory_iterator end; //default points to the end of the folder

	//begin the search
	//continuous reading until end of folder
	while (dir != end)
	{		

		if (is_directory(dir->status())) //folders name for the verbose
		{
			folder.folderName = dir->path();
		}
		else if (is_regular_file(dir->status()))
		{
			files.ext = dir->path().extension();//100 percent yes we need this	
			files.extCount = 1;
			try
			{
				files.fileSize = file_size(f);
			}
			catch (std::exception ex)
			{
				cerr << "Exception has orrured = " << ex.what() << endl;

			}			
		}
		extFiles.push_back(files);
		folders.push_back(folder);
		++dir;
	}
	extFiles.pop_front();
	map <path::string_type, int> extension;
	for (size_t i = 0; i < extFiles.size(); ++i)
	{
		++extension[extFiles[i].ext];
	}
	vector<Extension> finalExt;
	Extension final;

	//put back into vectorto get the filesize
	for (auto x : extension)
	{
		final.ext = x.first;
		final.extCount = x.second;
		finalExt.push_back(final);
	}

	size_t fileSizeCounter = 0;
	//get the filesize from the original vector of extentsion object
	for (size_t i = 0; i < finalExt.size(); ++i)
	{
		fileSizeCounter = 0;
		for (size_t j = 0; j < extFiles.size(); ++j)
		{
			if (extFiles[j].ext == finalExt[i].ext)
			{
				fileSizeCounter += extFiles[j].fileSize;
			}
			finalExt[i].fileSize = fileSizeCounter;
		}
	}
	size_t extCounter = finalExt.size();
	size_t totalFileCounter = 0;
	size_t totalSizeFileCounter = 0;
	vector<File> folderObjInfo;
	File folderInfo;
	//get the total of the files
	for (size_t i = 0; i < finalExt.size(); ++i)
	{
		totalFileCounter += finalExt[i].extCount;
		totalSizeFileCounter += finalExt[i].fileSize;
	}

	folderInfo.allextCount = extCounter;
	folderInfo.extensionTotalCount = totalFileCounter;
	folderInfo.fileTotalSize = totalSizeFileCounter;

	folderObjInfo.push_back(folderInfo);

	cout << "All Files: C:\\fileusage" << endl;
	cout << "\n";
	cout << right << setw(20) << "Ext" << right << setw(2) << char(58) << right << setw(15) << "#" << right << setw(2) << char(58) << right << setw(15) << "Total " << endl;
	cout << right<<setw(15)<< char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << endl;
	
	//print out the output
	sort(extFiles.begin(), extFiles.end());
	for (size_t i = 0; i < extFiles.size(); ++i)
	{
		cout << extFiles[i] << endl;		
	}
	cout << right << setw(15) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << endl;
	//print out the output
	for (size_t i = 0; i < folderObjInfo.size(); ++i)
	{
		cout << folderObjInfo[i] << endl;
	}
	return exit(0);
}

/*Method Name: rscan();
	Purpose: recursivly scan a path/folder and its subfolders
	Arguments: Takes in the path/directory
	Returns: void*/
void rscan(path const& f)
{
	cout << "fileusage(c) April 19, 2019, Sonia Friesen" << endl;
	//create the struct objects to hold the scan resutls
	deque<Extension> extObjInfo;	
	Extension extFile;	

	//start scanning recursivly through the folder 
	//use ranged for to do a recursive iterator
	for (auto dir : recursive_directory_iterator(f))
	{
		if (is_regular_file(dir.status()))
		{
			//get the extensions and add them to the object vector
			extFile.ext = dir.path().extension();		
			extFile.extCount = 1;
		}
		try
		{
			//research on filesizes and print filesize, returns file in bytes.
			//incrementing the extension count.
			//accumlating filesizecount(adding the file size of the same extension name)
			extFile.fileSize = file_size(dir);

		}
		catch (std::exception ex)
		{
			cerr << "Exception has orrured = " << ex.what() << endl;

		}
		extObjInfo.push_back(extFile);						
	}
	
	//sort
	sort(extObjInfo.begin(), extObjInfo.end());	
	extObjInfo.pop_front();

	//use map to get the number of each extension
	map <path::string_type, int> extension;
	for (size_t i = 0; i < extObjInfo.size(); ++i)
	{
		++extension[extObjInfo[i].ext];
	}
	vector<Extension> finalExt;
	Extension final;

	//put back into vectorto get the filesize
	for (auto x : extension)
	{
		final.ext = x.first;
		final.extCount = x.second;
		finalExt.push_back(final);		
	}

	size_t fileSizeCounter = 0;
	//get the filesize from the original vector of extentsion object
	for (size_t i = 0; i < finalExt.size(); ++i)
	{
		fileSizeCounter = 0;
		for (size_t j = 0; j < extObjInfo.size(); ++j)
		{
			if (extObjInfo[j].ext == finalExt[i].ext)
			{
				fileSizeCounter += extObjInfo[j].fileSize;
			}
			finalExt[i].fileSize = fileSizeCounter;			
		}
	}
	size_t extCounter = finalExt.size();
	size_t totalFileCounter = 0;
	size_t totalSizeFileCounter = 0;
	vector<File> folderObjInfo;
	File folderInfo;
	//get the total of the files
	for (size_t i = 0; i < finalExt.size(); ++i)
	{
		totalFileCounter += finalExt[i].extCount;
		totalSizeFileCounter += finalExt[i].fileSize;
	}

	folderInfo.allextCount = extCounter;
	folderInfo.extensionTotalCount = totalFileCounter;
	folderInfo.fileTotalSize = totalSizeFileCounter;

	folderObjInfo.push_back(folderInfo);
	
	cout << "All Files: C:\\fileusage" << endl;
	cout << "\n";
	cout << right << setw(20) << "Ext" << right << setw(2) << char(58) << right << setw(15) << "#" << right << setw(2) << char(58) << right << setw(15) << "Total " << endl;
	cout << right << setw(15) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << endl;

	for (size_t i = 0; i < finalExt.size(); ++i)
	{
		cout << finalExt[i] << endl;
	}

	cout << right << setw(15) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << endl;

	for (size_t i = 0; i < folderObjInfo.size(); ++i)
	{
		cout << folderObjInfo[i] << endl;
	}
	return exit(0);

}

int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		//scan the current folder("." current directory)		
		path currentFile = ".";
		scan(currentFile);
		return EXIT_SUCCESS;
	}

	//its only the executable and the the folder being inputed
	if (argc == 2)
	{
		string folder = argv[1];
		path directory = folder;
		rscan(folder);
		return EXIT_SUCCESS;
	}


	//}
	//simple title for program
	cout << "fileusage(c) April 19, 2019, Sonia Friesen" << endl;

	// Using a deque lets you eliminate ('eat') arguments as you process them
	// A deque provides perform pop_front and pop_back efficiently, while giving up little to vector in general performance
	deque<string> dqArgs(argv, argv + argc);

	//removes the executable argument
	dqArgs.pop_front();

	//use a map to store  all the switch options
	map<char, bool>defaultSwitches;
	char cplus = '+';
	char c = 'c';
	char java = 'j';
	char csharp = '#';
	char web = 'w';
	char regularex = 'x';
	char summary = 's';
	char recursive = 'r';
	char reverse = 'R';
	char sortSize = 'S';
	char detailExtReport = 'v';
	char help = 'h';

	//add them to the map with default of false because no switches are currently active
	defaultSwitches.insert(pair<char, bool>(cplus, false));
	defaultSwitches.insert(pair<char, bool>(c, false));
	defaultSwitches.insert(pair<char, bool>(java, false));
	defaultSwitches.insert(pair<char, bool>(csharp, false));
	defaultSwitches.insert(pair<char, bool>(web, false));
	defaultSwitches.insert(pair<char, bool>(regularex, false));
	defaultSwitches.insert(pair<char, bool>(summary, false));
	defaultSwitches.insert(pair<char, bool>(recursive, false));
	defaultSwitches.insert(pair<char, bool>(reverse, false));
	defaultSwitches.insert(pair<char, bool>(sortSize, false));
	defaultSwitches.insert(pair<char, bool>(detailExtReport, false));
	defaultSwitches.insert(pair<char, bool>(help, false));

	// parse a command line argument
	//take off the next element and put it ina string
	string parse = dqArgs[0];
	dqArgs.pop_front();

	//parse the string, look for a switch and compare it to the map	
	size_t length = parse.length();
	vector<char> userSwitches;
	for (size_t i = length - 1; i > 0; --i)
	{
		userSwitches.push_back(parse[i]);
	}

	int switchIndex = 0;
	//find the "-" character which starts the switches
	vector<char>::iterator switchStarter;
	for (size_t i = 0; i < userSwitches.size(); ++i)
	{
		if (userSwitches[i] == '-')
		{
			switchIndex = i;
			break;
		}
	}

	//compare the arg input to the map to set true if that switch exists
	for (size_t i = switchIndex; i < userSwitches.size(); ++i)
	{
		map<char, bool> ::iterator it;
		it = defaultSwitches.find(userSwitches[i]);
		if (it != defaultSwitches.end())
		{
			defaultSwitches[userSwitches[i]] = true;
		}
		else
		{
			defaultSwitches[userSwitches[i]] = false;
		}
	}

	if (defaultSwitches.at('h') == true)
	{
		cout << "Usage: fileusage[-hrRsSvc + #jw(x regularexpression)][folder]" << endl;
		return EXIT_SUCCESS;
	}

	//find if there is a regex expression
	regex regexArg;
	if (defaultSwitches.at(regularex) == true)
	{
		try
		{
			regexArg = dqArgs[0];

		}
		catch (std::regex_error er)
		{
			cerr << "Not valid regex" << endl;
			return EXIT_FAILURE;
		}

		dqArgs.pop_front();
	}

	//get the folder from the deque.. shouold be at(0) because be pop_front all other elemenets which could contain the regex or folder
	string folder = dqArgs[0];
	path directory = folder;
	deque<Extension> allExts;
	vector<File> allFiles;
	Extension extInfo;
	File fileInfo;

	//scan the folder
	//recursive iterators
	for (recursive_directory_iterator dir(directory), end; dir != end; ++dir)
	{
		if (is_directory(dir->status()))
		{
			fileInfo.folderName = dir->path(); //for the verbose
		}
		else if (is_regular_file(dir->status()))
		{
			//does the ext exist in container
			//incremnt count and filesize count
			// if it doesn't create a filedata object ext, count 1, filesiez.
			//FileData d{dir->path().extension(). 1, size
			//collect the extension, increment count of extension, accumulate filesizes
			extInfo.ext = dir->path().extension();
			try
			{
				//research on filesizes and print filesize, returns file in bytes.
				//incrementing the extension count.
				//accumlating filesizecount(adding the file size of the same extension name)
				extInfo.fileSize = file_size(directory);

			}
			catch (std::exception ex)
			{
				cerr << "Exception has orrured" << endl;

			}
			//research on filesizes and print filesize, returns file in bytes.
		}
		allExts.push_back(extInfo);
		allFiles.push_back(fileInfo);
	}
	//sort		
	sort(allExts.begin(), allExts.end());
	allExts.pop_front();

	//use map to get the number of each extension
	map <path::string_type, int> extension;
	for (size_t i = 0; i < allExts.size(); ++i)
	{
		++extension[allExts[i].ext];
	}
	vector<Extension> finalExt;
	Extension final;

	//put back into vectorto get the filesize
	for (auto x : extension)
	{
		final.ext = x.first;
		final.extCount = x.second;
		finalExt.push_back(final);
	}

	size_t fileSizeCounter = 0;
	//get the filesize from the original vector of extentsion object
	for (size_t i = 0; i < finalExt.size(); ++i)
	{
		fileSizeCounter = 0;
		for (size_t j = 0; j < allExts.size(); ++j)
		{
			if (allExts[j].ext == finalExt[i].ext)
			{
				fileSizeCounter += allExts[j].fileSize;
			}
			finalExt[i].fileSize = fileSizeCounter;
		}
	}
	size_t extCounter = finalExt.size();
	size_t totalFileCounter = 0;
	size_t totalSizeFileCounter = 0;
	vector<File> folderObjInfo;
	File folderInfo;
	//get the total of the files
	for (size_t i = 0; i < finalExt.size(); ++i)
	{
		totalFileCounter += finalExt[i].extCount;
		totalSizeFileCounter += finalExt[i].fileSize;
	}

	folderInfo.allextCount = extCounter;
	folderInfo.extensionTotalCount = totalFileCounter;
	folderInfo.fileTotalSize = totalSizeFileCounter;

	folderObjInfo.push_back(folderInfo);

	cout << "\n";
	
	//start looking for simple switches
	if (defaultSwitches.at(reverse) == true || defaultSwitches.at(sortSize) == true)
	{
		cout << "All files: " << dqArgs[0] << endl;
		cout << "\n";
		cout << right << setw(20) << "Ext" << right << setw(2) << char(58) << right << setw(15) << "#" << right << setw(2) << char(58) << right << setw(15) << "Total " << endl;
		cout << right << setw(15) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << endl;
		{
			sort(finalExt.begin(), finalExt.end(), sortDesc);
		}

		if (defaultSwitches.at(sortSize) == true)
		{
			sort(finalExt.begin(), finalExt.end(), sortFileSize);
		}
		for (size_t i = 0; i < finalExt.size(); ++i)
		{
			cout << finalExt[i] << endl;
		}	
		
		cout << right << setw(15) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << endl;

		for (size_t i = 0; i < folderObjInfo.size(); ++i)
		{
			cout << folderObjInfo[i] << endl;
		}			
	}

	//if summary switch is called
	if (defaultSwitches.at(summary) == true)
	{
		cout << "All files: " << dqArgs[0] << endl;
		cout << "\n";
		cout << right << setw(20) << "Ext" << right << setw(2) << char(58) << right << setw(15) << "#" << right << setw(2) << char(58) << right << setw(15) << "Total " << endl;
		cout << right << setw(15) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << endl;
		
		for (size_t i = 0; i < finalExt.size(); ++i)
		{
			cout << finalExt[i] << endl;
		}

		cout << right << setw(15) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << endl;

		for (size_t i = 0; i < folderObjInfo.size(); ++i)
		{
			cout << folderObjInfo[i] << endl;
		}
	}

	//if the verbose switch is called
	if (defaultSwitches.at(detailExtReport) == true)
	{
		cout << "All files: " << dqArgs[0] << endl;
		cout << "\n";
		cout << right << setw(20) << "Ext" << right << setw(2) << char(58) << right << setw(15) << "#" << right << setw(2) << char(58) << right << setw(15) << "Total " << endl;
		cout << right << setw(15) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << endl;

		sort(allExts.begin(), allExts.end());
		for (size_t i = 0; i <finalExt .size(); ++i)
		{
			cout << finalExt[i].ext << "( " << finalExt[i].extCount << " folders " <<  endl;			
			cout << allFiles[i].folderName << endl;
			cout << "\n";			
		}

		for (size_t i = 0; i < finalExt.size(); ++i)
		{
			cout << finalExt[i] << endl;
		}

		cout << right << setw(15) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << endl;

		for (size_t i = 0; i < folderObjInfo.size(); ++i)
		{
			cout << folderObjInfo[i] << endl;
		}
		cout << "This was a challenge!!!" << endl;
	}

	//if supress recursive is called
	if (defaultSwitches.at(recursive) == true)
	{
		scan(directory);
	}

//	//searching for certain fiels 
	vector<path::string_type> exts;
	for (size_t i = 0; i < finalExt.size(); ++i)
	{
		exts.push_back(finalExt[i].ext);
	}
	cout << "\n";
	//search fro c
	if (defaultSwitches.at(c) == true)
	{
		
		vector<File> totalFiles;
		File file;				
		string c = ".c";
		string h = ".h";
		size_t fileCounter =  0;
		size_t totalFileCount = 0 ;
		size_t fileSizecounter = 0;
		
		for (size_t i = 0; i < finalExt.size(); ++i)
		{
			if (finalExt[i].ext == c || finalExt[i].ext == h)
			{
				++fileCounter;
				totalFileCount += finalExt[i].extCount;
				fileSizecounter += finalExt[i].fileSize;
			}			
		}
		file.allextCount = fileCounter;
		file.extensionTotalCount = totalFileCount;
		file.fileTotalSize = fileSizeCounter;
		totalFiles.push_back(file);
		
		cout << "C files: " << dqArgs[0] << endl;
		cout << "\n";
		cout << right << setw(20) << "Ext" << right << setw(2) << char(58) << right << setw(15) << "#" << right << setw(2) << char(58) << right << setw(15) << "Total " << endl;
		cout << right << setw(15) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << endl;
		
		for (size_t i = 0; i < finalExt.size(); ++i)
		{
			if (finalExt[i].ext == c || finalExt[i].ext == h)
			cout << finalExt[i] << endl;
		}

		cout << right << setw(15) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << endl;

		for (size_t i = 0; i < totalFiles.size(); ++i)
		{
			cout << totalFiles[i] << endl;
		}
	}
	cout << "\n";
	//search for cplus
	if (defaultSwitches.at(cplus) == true)
	{
		vector<File> totalFiles;
		File file;
		string cpls = ".cc";
		string cp = ".cp";
		string cpp = ".cpp";
		string cxx = ".cxx";
		string cplusplus = ".c++";
		string hpp = ".hpp";
		string hxx = ".xx";
		size_t fileCounter = 0;
		size_t totalFileCount = 0;
		size_t fileSizecounter = 0;

		for (size_t i = 0; i < finalExt.size(); ++i)
		{
			if (finalExt[i].ext == cpls || finalExt[i].ext == cp || finalExt[i].ext == cpp || finalExt[i].ext == cxx || finalExt[i].ext == cplusplus || finalExt[i].ext == hpp || finalExt[i].ext == hxx)
			{
				++fileCounter;
				totalFileCount += finalExt[i].extCount;
				fileSizecounter += finalExt[i].fileSize;
			}
		}
		file.allextCount = fileCounter;
		file.extensionTotalCount = totalFileCount;
		file.fileTotalSize = fileSizeCounter;
		totalFiles.push_back(file);
		

		cout << "C++ files: " << dqArgs[0] << endl;
		cout << "\n";
		cout << right << setw(20) << "Ext" << right << setw(2) << char(58) << right << setw(15) << "#" << right << setw(2) << char(58) << right << setw(15) << "Total " << endl;
		cout << right << setw(15) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << endl;
		for (size_t i = 0; i < finalExt.size(); ++i)
		{
			if (finalExt[i].ext == cpls || finalExt[i].ext == cp || finalExt[i].ext == cpp || finalExt[i].ext == cxx || finalExt[i].ext == cplusplus || finalExt[i].ext == hpp || finalExt[i].ext == hxx)
				cout << finalExt[i] << endl;
		}
		cout << right << setw(15) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << endl;
		
		for (size_t i = 0; i < totalFiles.size(); ++i)
		{
			cout << totalFiles[i] << endl;
		}
	}
	cout << "\n";
	//search for java
	if (defaultSwitches.at(java) == true)
	{
		vector<File> totalFiles;
		File file;
		size_t fileCounter = 0;
		size_t totalFileCount = 0;
		size_t fileSizecounter = 0;		
		string javacls = ".class";
		string j = ".j ";
		string jad = ".jad";
		string jar = ".jar";
		string java = ".java";
		string jsp = ".jsp";
		string ser = ".ser";
		
		for (size_t i = 0; i < finalExt.size(); ++i)
		{
			
			if (finalExt[i].ext == javacls || finalExt[i].ext == j || finalExt[i].ext == jad || finalExt[i].ext == jar || finalExt[i].ext == java || finalExt[i].ext == jsp || finalExt[i].ext == ser)
			{
				++fileCounter;
				totalFileCount += finalExt[i].extCount;
				fileSizecounter += finalExt[i].fileSize;
			}			
				
		}
		file.allextCount = fileCounter;
		file.extensionTotalCount = totalFileCount;
		file.fileTotalSize = fileSizeCounter;
		totalFiles.push_back(file);

		cout << "Java files: " << dqArgs[0] << endl;
		cout << "\n";
		cout << right << setw(20) << "Ext" << right << setw(2) << char(58) << right << setw(15) << "#" << right << setw(2) << char(58) << right << setw(15) << "Total " << endl;
		cout << right << setw(15) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << endl;
		for (size_t i = 0; i < finalExt.size(); ++i)
		{

			if (finalExt[i].ext == javacls || finalExt[i].ext == j || finalExt[i].ext == jad || finalExt[i].ext == jar || finalExt[i].ext == java || finalExt[i].ext == jsp || finalExt[i].ext == ser)
			cout << finalExt[i] << endl;
		}
		cout << right << setw(15) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << endl;
		
		for (size_t i = 0; i < totalFiles.size(); ++i)
		{
			cout << totalFiles[i] << endl;
		}
	}
	cout << "\n";
	//search for csharp
	if (defaultSwitches.at(csharp) == true)
	{
		vector<File> totalFiles;
		File file;
		size_t fileCounter = 0;
		size_t totalFileCount = 0;
		size_t fileSizecounter = 0;		
		string cs= ".cs";
		string vb = ".vb ";
		string jsl = ".jsl";	

		for (size_t i = 0; i < finalExt.size(); ++i)
		{
			if (finalExt[i].ext == cs || finalExt[i].ext == vb || finalExt[i].ext == jsl)
			{
				++fileCounter;
				totalFileCount += finalExt[i].extCount;
				fileSizecounter += finalExt[i].fileSize;
			}
		}
		file.allextCount = fileCounter;
		file.extensionTotalCount = totalFileCount;
		file.fileTotalSize = fileSizeCounter;
		totalFiles.push_back(file);

		cout << "C# files: " << dqArgs[0] << endl;
		cout << "\n";
		cout << right << setw(20) << "Ext" << right << setw(2) << char(58) << right << setw(15) << "#" << right << setw(2) << char(58) << right << setw(15) << "Total " << endl;
		cout << right << setw(15) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << endl;
		
		for (size_t i = 0; i < finalExt.size(); ++i)
		{
			if (finalExt[i].ext == cs || finalExt[i].ext == vb || finalExt[i].ext == jsl)
				cout << finalExt[i] << endl;
		}
		cout << right << setw(15) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << endl;

		for (size_t i = 0; i < totalFiles.size(); ++i)
		{
			cout << totalFiles[i] << endl;
		}
	}
	cout << "\n";
	//search for web
	if (defaultSwitches.at(web) == true)
	{
		vector<File> totalFiles;
		File file;
		size_t fileCounter = 0;
		size_t totalFileCount = 0;
		size_t fileSizecounter = 0;
		string htm = ".htm";
		string html = ".html ";
		string html5 = ".html5";
		string js = ".js";
		string jse = ".jse ";
		string jsc = ".jsc";

		for (size_t i = 0; i < finalExt.size(); ++i)
		{
			if (finalExt[i].ext == htm || finalExt[i].ext == html || finalExt[i].ext == html5 || finalExt[i].ext == js || finalExt[i].ext == jse || finalExt[i].ext == jsc)
			{
				++fileCounter;
				totalFileCount += finalExt[i].extCount;
				fileSizecounter += finalExt[i].fileSize;
			}
		}
		file.allextCount = fileCounter;
		file.extensionTotalCount = totalFileCount;
		file.fileTotalSize = fileSizeCounter;
		totalFiles.push_back(file);

		cout << "Web files: " << dqArgs[0] << endl;
		cout << "\n";
		cout << right << setw(20)<< "Ext" << right << setw(2) << char(58) << right << setw(15) << "#" << right << setw(2) << char(58) << right << setw(15) << "Total " << endl;
		cout << right << setw(15) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << endl;

		for (size_t i = 0; i < finalExt.size(); ++i)
		{
			if (finalExt[i].ext == htm || finalExt[i].ext == html || finalExt[i].ext == html5 || finalExt[i].ext == js || finalExt[i].ext == jse || finalExt[i].ext == jsc)
				cout << finalExt[i] << endl;
		}
		cout << right << setw(15) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << char(45) << endl;
		for (size_t i = 0; i < totalFiles.size(); ++i)
		{
			cout << totalFiles[i] << endl;
		}
	}
	return EXIT_SUCCESS;
}

