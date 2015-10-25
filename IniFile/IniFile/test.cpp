#include<fstream>
#include<iostream>
#include"IniFile.h"

int main(void)
{
	IniFile file;
	//IniFile file2("test2.ini");
	//IniFile file3(file2);
	//IniFile file4;
	file.createIniFile("test.ini");
	//file4 = file;

	int v_int;
	double v_double;
	char v_char[10];
	file.getIntegerValue("General", "Age", v_int);
	std::cout << v_int << std::endl;
	file.getDoubleValue("Info", "Cost", v_double);
	std::cout << v_double << std::endl;
	file.getStringValue("General", "Language", v_char);
	std::cout << v_char << std::endl;

	file.setStringValue("General", "Language", "Chinese");
	file.setStringValue("Another", "Test", "Succeed");
	file.setStringValue("Another", "Test2", "Succeed");
	//file.setIntegerValue("Another", "Number", 30);
	//file.setDoubleValue("Info", "Get", 234.123);

	system("pause");
	return 0;
}