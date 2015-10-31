#include<assert.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include"inifile.h"

IniFile::IniFile() :
	_FileName(nullptr)
{
	
}

IniFile::IniFile(const char * filename)
{
	_FileName = new(std::nothrow) char[strlen(filename) + 1];
	assert(_FileName != nullptr);
	std::copy(filename, filename + strlen(filename) + 1, _FileName);

	loadIniFile();

	createMap();
}

IniFile::IniFile(const IniFile & rhs)
{
	_FileName = new(std::nothrow) char[strlen(rhs._FileName) + 1];
	assert(_FileName != nullptr);
	std::copy(rhs._FileName, rhs._FileName + strlen(rhs._FileName) + 1, _FileName);

	_FileContainer = rhs._FileContainer;

	_FileMap = rhs._FileMap;
}

IniFile::~IniFile()
{
	if (!_FileName)
	{
		delete[] _FileName;
		_FileName = nullptr;
	}
}

IniFile & IniFile::operator = (const IniFile & rhs)
{
	if (this != &rhs)
	{
		if(!_FileName)
			delete[] _FileName;
		_FileName = new(std::nothrow) char[strlen(rhs._FileName) + 1];
		assert(_FileName != nullptr);
		std::copy(rhs._FileName, rhs._FileName + strlen(rhs._FileName) + 1, _FileName);

		_FileContainer = rhs._FileContainer;

		_FileMap = rhs._FileMap;
	}
	return *this;
}

void IniFile::createIniFile(const char * filename)
{
	if(!_FileName)
		delete[] _FileName;
	
	_FileName = new(std::nothrow) char[strlen(filename) + 1];	
	assert(_FileName != nullptr);
	std::copy(filename, filename + strlen(filename) + 1, _FileName);
	
	loadIniFile();

	createMap();
}

bool IniFile::getIntegerValue(const char * section, const char * key, int & container)
{
	std::string value = getValue(section, key);
	if (value.empty())
	{
		std::cerr << "Fail to find the key" << std::endl;
		container = 0;
		return false;
	}
	else
	{
		std::stringstream buffer;
		buffer << value;
		buffer >> container;
		return true;
	}
}

bool IniFile::getDoubleValue(const char * section, const char * key, double & container)
{
	std::string value = getValue(section, key);
	if (value.empty())
	{
		std::cerr << "Fail to find the key" << std::endl;
		container = 0.0;
		return false;
	}
	else
	{
		std::stringstream buffer;
		buffer << value;
		buffer >> container;
		return true;
	}
}

bool IniFile::getStringValue(const char * section, const char * key, char * container)
{
	std::string value = getValue(section, key);
	if (value.empty())
	{
		std::cerr << "Fail to find the key" << std::endl;
		container[0] = '\0';
		return false;
	}
	else
	{
		value.copy(container, value.size());
		container[value.size()] = '\0';
		return true;
	}
}

void IniFile::setIntegerValue(const char * section, const char * key, const int value)
{
	std::stringstream buffer;
	std::string valuestr;

	buffer << value;
	buffer >> valuestr;

	setStringValue(section, key, valuestr.c_str());
}

void IniFile::setDoubleValue(const char * section, const char * key, const double value)
{
	std::stringstream buffer;
	std::string valuestr;
	
	buffer << value;
	buffer >> valuestr;
	
	setStringValue(section, key, valuestr.c_str());
}

void IniFile::setStringValue(const char * section, const char * key, const char * value)
{
	int sec_start, sec_end, key_start, key_end, value_start, value_end;
	std::string sec(section);
	std::string k(key);
	std::string val(value);

	loadIniFile();

	sec_start = 0;
	int leftbrace, rightbrace, lastnewline, nextnewline;
	do {
		sec_start = _FileContainer.find(section, sec_start + 1);
		while (isInComment(sec_start))
		{
			sec_start = _FileContainer.find(section, sec_start + 1);
		}
		if (sec_start < 0)
			break;
		leftbrace = _FileContainer.rfind('[', sec_start);
		rightbrace = _FileContainer.find(']', sec_start);
		lastnewline = _FileContainer.rfind('\n', sec_start);
		nextnewline = _FileContainer.find('\n', sec_start);
		if (rightbrace < 0)
		{
			sec_start = -1;
			break;
		}
		if (nextnewline < 0)
			nextnewline = _FileContainer.size();
	} while (leftbrace < lastnewline || rightbrace > nextnewline);
	
	/* not find the section */
	if (sec_start < 0)
	{
		_FileContainer += "\n[" + sec + "]\n" + k + "=" + val + "\n";

		_FileMap.insert(std::map<std::string, std::string>::value_type(sec + "_" + k, val));
	}
	else{
		sec_end = sec_start + strlen(section);

		int bound = _FileContainer.find('[', sec_start);
		key_start = _FileContainer.find(key, sec_end);
		while (isInComment(key_start))
		{
			key_start = _FileContainer.find(key, key_start + 1);
		}

		/* not find the key */
		if (key_start < 0 || (bound > 0 && key_start > bound))
		{
			int pos = _FileContainer.find('\n', sec_end) + 1;
			_FileContainer.insert(pos, key);
			pos += strlen(key);
			_FileContainer.insert(pos, "=");
			pos++;
			_FileContainer.insert(pos, value);
			pos += strlen(value);
			_FileContainer.insert(pos, "\n");

			_FileMap.insert(std::map<std::string, std::string>::value_type(sec + "_" + k, val));
		}

		else {
			key_end = key_start + strlen(key);
			value_start = _FileContainer.find('=', key_end) + 1;
			while (_FileContainer[value_start] == ' ')
				value_start++;
			value_end = _FileContainer.find('\n', value_start) - 1;
			if (value_end < 0)
				value_end = _FileContainer.size() - 1;
			while (_FileContainer[value_end] == ' ')
				value_end--;

			_FileContainer.replace(value_start, value_end - value_start + 1, value);

			auto iter = _FileMap.find(sec + "_" + k);
			iter->second = val;
		}
	}

	std::fstream file(_FileName, std::ios::out);
	file << _FileContainer;
}

void IniFile::loadIniFile()
{
	std::stringstream buffer;
	std::fstream file(_FileName,std::ios::in);

	buffer << file.rdbuf();
	_FileContainer = buffer.str();
}

void IniFile::createMap()
{
	std::string section;
	std::string key;
	std::string value;

	int pos = 0;
	int sec_start, sec_end, key_start, key_end, value_start, value_end;

	while (pos < _FileContainer.size())
	{
		sec_start = _FileContainer.find('[', pos) + 1;
		if (isInComment(sec_start))
		{
			pos = _FileContainer.find('\n',sec_start) + 1;
			if (pos < 1)
				pos = _FileContainer.size();
			continue;
		}
		while (_FileContainer[sec_start] == ' ')
			sec_start++;
		sec_end = _FileContainer.find(']', sec_start) - 1;
		while (_FileContainer[sec_end] == ' ')
			sec_end--;
		section = _FileContainer.substr(sec_start, sec_end - sec_start + 1);

		pos = sec_end;
		int end = _FileContainer.find('[', pos);
		if (end < 0)
			end = _FileContainer.size();

		while (pos < end)
		{
			key_end = _FileContainer.find('=', pos) - 1;
			if (isInComment(key_end))
			{
				pos = _FileContainer.find('\n', key_end) + 1;
				if (pos < 1)
					pos = _FileContainer.size();
				continue;
			}
			while (_FileContainer[key_end] == ' ')
				key_end--;
			key_start = _FileContainer.rfind('\n', key_end) + 1;
			while (_FileContainer[key_start] == ' ')
				key_start++;
			key = _FileContainer.substr(key_start, key_end - key_start + 1);

			value_start = _FileContainer.find('=', key_end) + 1;
			while (_FileContainer[value_start] == ' ')
				value_start++;
			value_end = _FileContainer.find('\n', value_start) - 1;
			if (value_end < 0)
				value_end = _FileContainer.size() - 1;
			while (_FileContainer[value_end] == ' ')
				value_end--;
			value = _FileContainer.substr(value_start, value_end - value_start + 1);

			_FileMap.insert(std::map<std::string, std::string>::value_type(section + "_" + key, value));

			pos = _FileContainer.find('=', value_end);
			if (pos < 0)
				pos = _FileContainer.size();
		}
		if (pos == _FileContainer.size())
			break;
		pos = end;
	}
}

bool IniFile::isInComment(int pos)
{
	int comment_start = _FileContainer.rfind(';', pos);
	int newline = _FileContainer.rfind('\n', pos);
	if (comment_start < 0)
		return false;
	if (comment_start < newline)
		return false;
	return true;
}

std::string IniFile::getValue(const char * section, const char * key)
{
	std::string s(section);
	std::string k(key);
	std::string value;

	auto iter = _FileMap.find(s + "_" + k);
	if (iter != _FileMap.end())
	{
		value = iter->second;
	}
	return value;
}