#include<iostream>
#include<fstream>
#include"inifile.h"

IniFile::IniFile() :
	_FileContainer(nullptr),
	_FileName(nullptr),
	_FileSize(0)
{
	
}

IniFile::IniFile(const char * filename)
{
	_FileName = new char[strlen(filename)];
	if (!getFileSize()||!loadIniFile())
	{
		_FileName = nullptr;
		_FileContainer = nullptr;
		_FileSize = 0;
		std::cerr << "Fail to create an IniFile.\n" << std::endl;
	}
}

bool IniFile::setIntegerValue(const char * section, const char * key, const int value)
{
}

bool IniFile::getFileSize()
{
	std::fstream in;

	in.open(_FileName, std::ios::in);
	if (!in.is_open())
	{
		std::cerr << "Fail to open the file.\n" << std::endl;
		return false;
	}

	in.seekg(0,std::ios::end);
	_FileSize = in.tellg();
	in.close();
	return true;
}

bool IniFile::loadIniFile()
{
	std::fstream in;

	in.open(_FileName, std::ios::in);
	_FileContainer = new char[_FileSize];


}