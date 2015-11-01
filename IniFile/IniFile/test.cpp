#include<iostream>
#include"IniFile.h"

int main(void)
{
	IniFile file2("test.ini");
	IniFile file3(file2);
	IniFile file4 = file3;

	int v_int;
	double v_double;
	char v_char[10];

	for (int i = 0;i < 1000;i++)
	{
		IniFile file;
		file.createIniFile("test.ini");

		file.setStringValue("Another", "Test", "Succeed");
		file.setIntegerValue("Another", "Count", i);
		int count;
		file.getIntegerValue("Another", "Count", count);
		std::cout << count << std::endl;
	}

	file2.getStringValue("General", "Language", v_char,10);
	file3.getIntegerValue("General", "Age", v_int);
	file4.getDoubleValue("Info", "Cost", v_double);
	std::cout << v_char << std::endl;
	std::cout << v_int << std::endl;
	std::cout << v_double << std::endl;

	file2.setDoubleValue("Test", "Double", 478.1231);
	
	system("pause");
	return 0;
}

