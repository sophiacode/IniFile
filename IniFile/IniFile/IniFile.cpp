#include<fstream>
#include<iostream>
#include<cstdlib>
#include<string>
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
	IniFile emptyfile;

	if (this != &rhs)
	{
		delete[] _FileName;
		_FileName = new(std::nothrow) char[strlen(rhs._FileName)];
		if (_FileName == nullptr)
		{
			std::cerr << "Fail to allocate memory." << std::endl;
			return emptyfile;
		}
		std::copy(rhs._FileName, rhs._FileName + strlen(_FileName), _FileName);

		_FileSize = rhs._FileSize;

		delete[] _FileContainer;
		_FileContainer = new(std::nothrow) char[_FileSize];
		if (_FileContainer == nullptr)
		{
			std::cerr << "Fail to allocate memory." << std::endl;
			return emptyfile;
		}
		std::copy(rhs._FileContainer, rhs._FileContainer + rhs._FileSize, _FileContainer);
	}
	return *this;
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
	std::copy(filename, filename + strlen(filename) + 1, _FileName);
	
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

bool IniFile::getIntegerValue(const char * section, const char * key, int & container)
{
	char *value;
	int size;
	unsigned int sec_start, sec_end, key_start, key_end, value_start, value_end;
	findPosition(section, key, sec_start, sec_end, key_start, key_end, value_start, value_end);
	if (sec_start == 0 || sec_end == 0 || key_start == 0 
		|| key_end == 0 || value_start == 0 || value_end == 0)
	{
		std::cerr << "Fail to find the key." << std::endl;
		return false;
	}
	else
	{
		size = value_end - value_start + 1;
		value = new char[size];
		std::copy(_FileContainer + value_start, _FileContainer + value_end, value);
		value[size] = '\0';
		container = atoi(value);
		return true;
	}
}

bool IniFile::getDoubleValue(const char * section, const char * key, double & container)
{
	char *value;
	int size;
	unsigned int sec_start, sec_end, key_start, key_end, value_start, value_end;
	findPosition(section, key, sec_start, sec_end, key_start, key_end, value_start, value_end);
	if (sec_start == 0 || sec_end == 0 || key_start == 0
		|| key_end == 0 || value_start == 0 || value_end == 0)
	{
		std::cerr << "Fail to find the key." << std::endl;
		return false;
	}
	else
	{
		size = value_end - value_start + 1;
		value = new char[size];
		std::copy(_FileContainer + value_start, _FileContainer + value_end + 1, value);
		value[size] = '\0';
		container = atof(value);
		return true;
	}
}

bool IniFile::getStringValue(const char * section, const char * key, char * container)
{
	int size;
	unsigned int sec_start, sec_end, key_start, key_end, value_start, value_end;
	findPosition(section, key, sec_start, sec_end, key_start, key_end, value_start, value_end);
	if (sec_start == 0 || sec_end == 0 || key_start == 0
		|| key_end == 0 || value_start == 0 || value_end == 0)
	{
		std::cerr << "Fail to find the key." << std::endl;
		return false;
	}
	else
	{
		size = value_end - value_start + 1;
		std::copy(_FileContainer + value_start, _FileContainer + value_end, container);
		container[size] = '\0';
		return true;
	}
}

bool IniFile::setIntegerValue(const char * section, const char * key, const int value)
{
	char * valueOther;
	valueOther = new char[6];
	_itoa_s(value, valueOther, 6, 10);
	if (setStringValue(section, key, valueOther))
		return true;
	else
		return false;
}

bool IniFile::setDoubleValue(const char * section, const char * key, const double value)
{
	char *valueOther;
	int len = 20;
	valueOther = new char[len];
	_gcvt_s(valueOther, len, value, len);
	if (setStringValue(section, key, valueOther))
		return true;
	else
		return false;
}

bool IniFile::setStringValue(const char * section, const char * key, const char * value)
{
	char *buf;
	unsigned int sec_start,sec_end, key_start,key_end,value_start,value_end;

	findPosition(section, key, sec_start, sec_end, 
		key_start, key_end, value_start, value_end);

	/* not find the section */
	if (sec_end == 0)
	{
		unsigned int newsize = _FileSize + strlen(section) + strlen(key) + strlen(value) + 7;
		if (newsize > _MAXFILESIZE)
		{
			std::cerr << "File space is not enough." << std::endl;
			return false;
		}
		else
		{
			buf = new char[newsize];
			std::copy(_FileContainer, _FileContainer + _FileSize, buf);
			sprintf(buf + _FileSize, "[%s]\r\n%s=%s\r\n", section, key, value);
			_FileSize = newsize;
			delete[] _FileContainer; 
			_FileContainer = new char[_FileSize];
			std::copy(buf, buf + _FileSize, _FileContainer);
			_FileContainer[_FileSize]='\0';
		}
	}

	/* not find the key */
	else if (key_end == 0)
	{
		unsigned int newsize = _FileSize + strlen(key) + strlen(value) + 3;
		if (newsize > _MAXFILESIZE)
		{
			std::cerr << "File space is not enough." << std::endl;
			return false;
		}
		else
		{
			buf = new char[newsize];
			std::copy(_FileContainer, _FileContainer + sec_end + 2, buf);
			sprintf(buf + sec_end + 3, "\r\n%s=%s", key, value);
			std::copy(_FileContainer + sec_end + 3,
				_FileContainer + _FileSize, 
				buf + sec_end + strlen(key) + strlen(value) + 5);
			_FileSize = newsize;
			_FileContainer = new char[newsize];
			std::copy(buf, buf + newsize, _FileContainer);
			_FileContainer[_FileSize] = '\0';
		}
	}

	/* not find the value or change the value */
	else
	{
		unsigned int newsize = _FileSize - (value_end - value_start + 1) + strlen(value);
		if (newsize > _MAXFILESIZE)
		{
			std::cerr << "File space is not enough." << std::endl;
			return false;
		}
		else {
			if (value_start == 0)
			{
				buf = new char[newsize];
				std::copy(_FileContainer, _FileContainer + key_end + 1, buf);
				sprintf(buf + key_end + 1, "%s\r\n", value);
				_FileSize = newsize;
				_FileContainer = new char[_FileSize];
				std::copy(buf, buf + newsize, _FileContainer);
				_FileContainer[_FileSize] = '\0';		
			}
			else
			{
				buf = new char[newsize];
				std::copy(_FileContainer, _FileContainer + value_start, buf);
				sprintf(buf + value_start, "%s",value);
				std::copy(_FileContainer + value_end + 1, _FileContainer + _FileSize,
					buf+ value_start + strlen(value));
				_FileSize = newsize;
				_FileContainer = new char[_FileSize];
				std::copy(buf, buf + _FileSize, _FileContainer);
				_FileContainer[_FileSize] = '\0';
			}
		}
	}

	std::ofstream out(_FileName,std::ios::binary);
	if (!out.is_open())
	{
		std::cerr << "Fail to open the file:" << _FileName << std::endl;
		return false;
	}
	out.write(_FileContainer, sizeof(char)*_FileSize);
	out.close();
	
	return true;
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
	_FileContainer[_FileSize] = '\0';

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
			if (strncmp(_FileContainer + i + 1, section, j - i - 1) == 0)
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
			if (strncmp(_FileContainer + j, key, i - j) == 0)
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
	if (_FileContainer[i] == '\0' || _FileContainer[i] == '\n')
	{
		value_start = value_end = 0;
		return;
	}
	while (_FileContainer[i] == ' ')
	{
		i++;
	}
	value_start = i;
	while (_FileContainer[i+1]!='\0' && _FileContainer[i+1] != '\n')
	{
		i++;
	}
	value_end = i - 1;

	return;
}

