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
	_FileName = new(std::nothrow) char[strlen(filename) + 1];
	if (_FileName == nullptr)
	{
		std::cerr << "Fail to allocate memory."<< std::endl;
		return;
	}
	std::copy(filename, filename + strlen(filename) + 1, _FileName);

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
	_FileName = new(std::nothrow) char[strlen(rhs._FileName) + 1];
	if (_FileName == nullptr)
	{
		std::cerr << "Fail to allocate memory." << std::endl;
		return;
	}
	std::copy(rhs._FileName, rhs._FileName + strlen(rhs._FileName) + 1, _FileName);

	_FileSize = rhs._FileSize;

	_FileContainer = new(std::nothrow) char[_FileSize + 1];
	if (_FileContainer == nullptr)
	{
		std::cerr << "Fail to allocate memory." << std::endl;
		return;
	}
	std::copy(rhs._FileContainer, rhs._FileContainer + rhs._FileSize + 1, _FileContainer);
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
		if(!_FileName)
			delete[] _FileName;
		_FileName = new(std::nothrow) char[strlen(rhs._FileName) + 1];
		if (_FileName == nullptr)
		{
			std::cerr << "Fail to allocate memory." << std::endl;
			return emptyfile;
		}
		std::copy(rhs._FileName, rhs._FileName + strlen(rhs._FileName) + 1, _FileName);

		_FileSize = rhs._FileSize;

		if(!_FileContainer)
			delete[] _FileContainer;
		_FileContainer = new(std::nothrow) char[_FileSize + 1];
		if (_FileContainer == nullptr)
		{
			std::cerr << "Fail to allocate memory." << std::endl;
			return emptyfile;
		}
		std::copy(rhs._FileContainer, rhs._FileContainer + rhs._FileSize + 1, _FileContainer);
	}
	return *this;
}

bool IniFile::createIniFile(const char * filename)
{
	if(!_FileName)
		delete[] _FileName;
	if(!_FileContainer)
		delete[] _FileContainer;
	
	_FileName = new(std::nothrow) char[strlen(filename) + 1];	

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
	char value[10];
	
	if (getStringValue(section, key, value))
	{
		container = atoi(value);
		return true;
	}
	else
	{
		container = 0;
		return false;
	}
}

bool IniFile::getDoubleValue(const char * section, const char * key, double & container)
{
	char value[30];

	if (getStringValue(section, key, value))
	{
		container = atof(value);
		return true;
	}
	else
	{
		container = 0;
		return false;
	}
}

bool IniFile::getStringValue(const char * section, const char * key, char * container)
{
	unsigned int sec_end, key_end, value_start, value_end;

	findPosition(section, key, sec_end, key_end, value_start, value_end);

	if (key_end == 0)
	{
		std::cerr << "Fail to find the key." << std::endl;
		container[0] = '\0';
		return false;
	}
	else
	{
		unsigned int size = value_end - value_start + 2;
		std::copy(_FileContainer + value_start, _FileContainer + value_end + 1, container);
		container[size - 1] = '\0';
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

	findPosition(section, key, sec_end, key_end, value_start, value_end);

	/* not find the section */
	if (sec_end == 0)
	{
		unsigned int newsize = _FileSize + strlen(section) + strlen(key) + strlen(value) + 9;
		if (newsize >= _MAXFILESIZE)
		{
			std::cerr << "File space is not enough." << std::endl;
			return false;
		}
		else
		{
			buf = new char[newsize + 1];
			std::copy(_FileContainer, _FileContainer + _FileSize, buf);
			sprintf(buf + _FileSize, "\r\n[%s]\r\n%s=%s\r\n", section, key, value);
			_FileSize = newsize;
			delete[] _FileContainer; 
			_FileContainer = new char[_FileSize + 1];
			std::copy(buf, buf + _FileSize, _FileContainer);
			_FileContainer[_FileSize] = '\0';
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
			buf = new char[newsize + 1];
			std::copy(_FileContainer, _FileContainer + sec_end + 1, buf);
			sprintf(buf + sec_end + 1, "%s=%s\r\n", key, value);
			std::copy(_FileContainer + sec_end + 1,
				_FileContainer + _FileSize, 
				buf + sec_end + strlen(key) + strlen(value) + 4);
			delete[] _FileContainer;
			_FileSize = newsize;
			_FileContainer = new char[newsize + 1];
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
			buf = new char[newsize + 1];
			std::copy(_FileContainer, _FileContainer + value_start, buf);
			sprintf(buf + value_start, "%s", value);
			std::copy(_FileContainer + value_end + 1, _FileContainer + _FileSize,
				buf + value_start + strlen(value));
			_FileSize = newsize;
			//delete[] _FileContainer;
			_FileContainer = new char[_FileSize + 1];
			std::copy(buf, buf + _FileSize, _FileContainer);
			_FileContainer[_FileSize] = '\0';
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
	if (_FileSize >= _MAXFILESIZE)
	{
		std::cerr << "File:" << _FileName << " is too large.\n" << std::endl;
		return false;
	}
	
	_FileContainer = new(std::nothrow) char[_FileSize + 1];
	if (!_FileContainer)
	{
		std::cerr << "Fail to allocate memeory." << std::endl;
		return false;
	}
	in.seekg(std::ios::beg);
	in.read(_FileContainer, sizeof(char)*_FileSize);
	_FileContainer[_FileSize] = '\0';
	in.close();
	
	return true;
}

void IniFile::findPosition(const char * section, const char * key,
	unsigned int &sec_end, unsigned int &key_end,
	unsigned int &value_start, unsigned int &value_end)
{
	unsigned int i = 0;
	
	/*find section position*/
	while (_FileContainer[i])
	{
		ignoreComment(i);
		if (_FileContainer[i] == '[')
		{
			i++;
			while (_FileContainer[i] == ' ')
			{
				i++;
			}

			unsigned int j = i + 1;
			unsigned int k;
			while (_FileContainer[j] && _FileContainer[j] != ']' 
				&& _FileContainer[j] != ' ')
			{
				j++;
			}
			k = j;
			while (_FileContainer[k] && _FileContainer[k] != ']')
			{
				k++;
			}
			if (strncmp(_FileContainer + i, section, j - i) == 0)
			{
				sec_end = k + 2;
				break;
			}
			else
			{
				i = k;
			}
		}
		i++;
	}
	if (!_FileContainer[i])
	{
		sec_end = key_end = value_start = value_end = 0;
		return;
	}

	/*find key position*/
	i = sec_end;
	unsigned int temp;
	while (_FileContainer[i])
	{   
		ignoreComment(i);
		if (_FileContainer[i] == '=')
		{
			temp = i;
			i--;
			while (_FileContainer[i] == ' ')
			{
				i--;
			}
			unsigned int j = i - 1;
			while (j > sec_end && _FileContainer[j] != '\n' && _FileContainer[j] != ' ')
			{
				j--;
			}
			j++;

			if (strncmp(_FileContainer + j, key, i - j + 1) == 0)
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
		key_end = value_start = value_end = 0;
		return;
	}

	/*find value position*/
	i = temp + 1;
	if (_FileContainer[i] == '\0' || _FileContainer[i] == '\r')
	{
		value_start = value_end = 0;
		return;
	}
	while (_FileContainer[i] == ' ')
	{
		i++;
	}
	value_start = i;
	while (_FileContainer[i] != '\0' && _FileContainer[i] != '\r' && _FileContainer[i] != ' ')
	{
		i++;
	}
	value_end = i - 1;

	return;
}

void IniFile::ignoreComment(unsigned int &position)
{
	unsigned int i = position;
	if (_FileContainer[i] == ';' 
		&& (i == 0 || _FileContainer[i - 1] == '\n'))
	{
		while (_FileContainer[i] != '\n')
		{
			i++;
		}
	}
	position = i;
}

