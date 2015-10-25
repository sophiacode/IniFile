#include<fstream>
#include<iostream>
#include"IniFile.h"

int main(void)
{
	IniFile file;
	IniFile file2("test2.ini");
	IniFile file3(file2);
	IniFile file4;

	file.createIniFile("test.ini");
	file4 = file;

	int value;
	file.getIntegerValue("General", "Age", value);

	

	system("pause");
	return 0;
}