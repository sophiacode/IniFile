#include<fstream>
#include<iostream>
#include"inifile.h"

IniFile::IniFile() :
	_FileContainer(nullptr),
	_FileName(nullptr),
	_FileSize(0)
{
	
}

IniFile::IniFile(const char * filename)
{
	_FileName = new(std::nothrow) char[strlen(filename)];
	if (_FileName == nullptr)
	{
		std::cerr << "Fail to allocate memory."<< std::endl;
		return;
	}
	std::copy(filename, filename + strlen(filename), _FileName);

	if (!loadIniFile())
	{
		delete[] _FileName;
		_FileName = nullptr;
		_FileContainer = nullptr;
		_FileSize = 0;
		std::cerr << "Fail to create the IniFile:" << filename << std::endl;
		return;
	}
}

IniFile::IniFile(const IniFile & rhs)
{
	_FileName = new(std::nothrow) char[strlen(rhs._FileName)];
	if (_FileName == nullptr)
	{
		std::cerr << "Fail to allocate memory." << std::endl;
		return;
	}
	std::copy(rhs._FileName, rhs._FileName + strlen(_FileName), _FileName);

	_FileSize = rhs._FileSize;

	_FileContainer = new(std::nothrow) char[_FileSize];
	if (_FileContainer == nullptr)
	{
		std::cerr << "Fail to allocate memory." << std::endl;
		return;
	}
	std::copy(rhs._FileContainer, rhs._FileContainer + rhs._FileSize, _FileContainer);
}

IniFile::~IniFile()
{
	if (!_FileName)
	{
		delete[] _FileName;
		_FileName = nullptr;
	}
	if (!_FileContainer)
	{
		delete[] _FileContainer;
		_FileContainer = nullptr;
	}
	_FileSize = 0;
}

IniFile & IniFile::operator = (const IniFile & rhs)
{
	if (this != &rhs)
	{
		delete[] _FileName;
		_FileName = new(std::nothrow) char[strlen(rhs._FileName)];
		if (_FileName == nullptr)
		{
			std::cerr << "Fail to allocate memory." << std::endl;
			return;
		}
		std::copy(rhs._FileName, rhs._FileName + strlen(_FileName), _FileName);

		_FileSize = rhs._FileSize;

		delete[] _FileContainer;
		_FileContainer = new(std::nothrow) char[_FileSize];
		if (_FileContainer == nullptr)
		{
			std::cerr << "Fail to allocate memory." << std::endl;
			return;
		}
		std::copy(rhs._FileContainer, rhs._FileContainer + rhs._FileSize, _FileContainer);
	}
	return *this;
}

bool IniFile::getIntegerValue(const char * section, const char * key, int & container)
{

}

bool IniFile::getDoubleValue(const char * section, const char * key, double & container)
{

}

bool IniFile::getStringValue(const char * section, const char * key, char * container)
{

}

bool IniFile::createIniFile(const char * filename)
{
	delete[] _FileName;
	_FileName = nullptr;
	delete[] _FileContainer;
	_FileContainer = nullptr;

	_FileName = new(std::nothrow) char[strlen(filename)];
	if (_FileName == nullptr)
	{
		std::cerr << "Fail to allocate memory." << std::endl;
		return false;
	}
	std::copy(filename, filename + strlen(filename), _FileName);

	if (!loadIniFile())
	{
		delete[] _FileName;
		_FileName = nullptr;
		_FileContainer = nullptr;
		_FileSize = 0;
		std::cerr << "Fail to create the IniFile:" << filename << std::endl;
		return false;
	}

	return true;
}

bool IniFile::setIntegerValue(const char * section, const char * key, const int value)
{

}

bool IniFile::setDoubleValue(const char * section, const char * key, const double value)
{

}

bool IniFile::setStringValue(const char * section, const char * key, const char * value)
{
	char * buf;
	unsigned int sec_start,sec_end, key_start,key_end,value_start,value_end;
	findPosition(section, key, sec_start, sec_end, key_start, key_end, value_start, value_end);
	if (sec_end == 0)
	{
		unsigned int newsize = _FileSize + strlen(section) + strlen(key) + strlen(value) + 5;
		if (newsize > _MAXFILESIZE)
		{
			std::cerr << "File space is not enough." << std::endl;
			return false;
		}
		sec_end = _FileSize;

	}
	
}

bool IniFile::loadIniFile()
{
	std::ifstream in;
	in.open(_FileName, std::ios::binary);
	if (!in.is_open())
	{
		std::cerr << "Fail to open the file:" << std::endl;
		return false;
	}

	in.seekg(0, std::ios::end);
	_FileSize = in.tellg();
	if (_FileSize > _MAXFILESIZE)
	{
		std::cerr << "File:" << _FileName << " is too large.\n" << std::endl;
		return false;
	}
	
	_FileContainer = new(std::nothrow) char[_FileSize];
	if (!_FileContainer)
	{
		std::cerr << "Fail to allocate memeory." << std::endl;
		return false;
	}
	in.seekg(std::ios::beg);
	in.read(_FileContainer, sizeof(char)*_FileSize);

	return true;
}

void IniFile::findPosition(const char * section, const char * key, 
	unsigned int &sec_start,unsigned int &sec_end, 
	unsigned int &key_start,unsigned int &key_end,
	unsigned int &value_start,unsigned int &value_end)
{
	unsigned int i = 0;
	
	/*find section position*/
	while (_FileContainer[i])
	{
		if (_FileContainer[i] == '[')
		{
			unsigned int j = i + 1;
			while (_FileContainer[j] && _FileContainer[j] != ']')
			{
				j++;
			}
			if (strncmp(_FileContainer + i, section, j - i) == 0)
			{
				sec_end = j - 1;
				break;
			}
			else
			{
				i = j;
			}
		}
		i++;
	}
	if (!_FileContainer[i])
	{
		sec_start = sec_end = key_start = key_end = value_start = value_end = 0;
		return;
	}

	/*find key position*/
	i = sec_end;
	unsigned int temp;
	while (_FileContainer[i])
	{   
		if (_FileContainer[i] == '=')
		{
			temp = i;
			i--;
			while (_FileContainer[i] == ' ')
			{
				i--;
			}
			unsigned int j = i - 1;
			while (j > sec_end && _FileContainer[j - 1] != '\n')
			{
				j--;
			}
			if (strncpy(_FileContainer + j, key, i - j) == 0)
			{
				key_end = i;
				break;
			}
			else
			{
				i = temp;
			}
		}
		i++;
	}
	if (!_FileContainer[i])
	{
		key_start = key_end = value_start = value_end = 0;
		return;
	}

	/*find value position*/
	i = temp + 1;
	if (!_FileContainer[i])
	{
		value_start = value_end = 0;
		return;
	}
	while (_FileContainer[i] == ' ')
	{
		i++;
	}
	value_start = i;
	i++;
	while (_FileContainer[i+1]!='\0' && _FileContainer[i+1] != '\n')
	{
		i++;
	}
	value_end = i;

	return;
}

