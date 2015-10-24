/*********************************************************************
*
*	@file		IniFile.h
*	@brief		read and write .ini file
*
*	Date        Name            Description
*	18/10/15	HQWY/Sophia		Creation
*
*********************************************************************/

#pragma once

class IniFile
{
public:
	/**
	*  \brief 构造函数
	*/
	IniFile();

	/**
	*  \brief 重载构造函数
	*
	*  \param filename .ini文件名
	*/
	IniFile(const char * filename);

	/**
	*  \brief 拷贝构造函数
	*
	*  \param rhs 拷贝对象
	*/
	IniFile(const IniFile & rhs);

	/**
	*  \brief 析构函数
	*/
	~IniFile();

	/**
	*  \brief 重载等号
	*
	*  \param rhs 拷贝对象
	*
	*  \return    自身的引用
	*/
	IniFile & operator = (const IniFile & rhs);

	/**
	*  \brief 创建函数
	* 
	*  \param filename .ini文件名
	*
	*  \return 创建成功则返回true
	*          创建失败则返回false
	*/
	bool createIniFile(const char * filename);

	/**
	*  \brief 获取整数型变量
	*
	*  \param section     .ini文件的section名
	*         key         .ini文件的key名
	*    	  container   赋值对象
	*
	*  \return 获取成功则返回true
	*          获取失败则返回false
	*/
	bool getIntegerValue(const char * section, const char * key, int & container);

	/**
	*  \brief 获取实数型变量
	*
	*  \param section     .ini文件的section名
	*         key         .ini文件的key名
	*    	  container   赋值对象
	*
	*  \return 获取成功则返回true
	*          获取失败则返回false
	*/
	bool getDoubleValue(const char * section, const char * key, double & container);

	/**
	*  \brief 获取字符串型变量
	*
	*  \param section     .ini文件的section名
	*         key         .ini文件的key名
	*    	  container   赋值对象
	*
	*  \return 获取成功则返回true
	*          获取失败则返回false
	*/
	bool getStringValue(const char * section, const char * key, char * container);

	/**
	*  \brief 设置变量整数型数值
	*
	*  \param section .ini文件的section名
	*         key     .ini文件的key名
	*    	  value   需设置的数值
	*
	*  \return 设置成功则返回true
	*          设置失败则返回false
	*/
	bool setIntegerValue(const char * section, const char * key, const int value);

	/**
	*  \brief 设置变量实数型数值
	*
	*  \param section .ini文件的section名
	*         key     .ini文件的key名
	*    	  value   需设置的数值
	*
	*  \return 设置成功则返回true
	*          设置失败则返回false
	*/
	bool setDoubleValue(const char * section, const char * key, const double value);

	/**
	*  \brief 设置变量字符串型数值
	*
	*  \param section .ini文件的section名
	*         key     .ini文件的key名
	*    	  value   需设置的数值
	*
	*  \return 设置成功则返回true
	*          设置失败则返回false
	*/
	bool setStringValue(const char * section, const char * key, const char * value);

private:
	char * _FileName;       /* .ini文件名      */
	char * _FileContainer;  /* 储存文件内容 */
//	bool   _IsFileExist;    /* 标记文件是否存在 */
	int    _FileSize;       /* .ini文件大小 */

	bool loadIniFile();
	bool getFileSize();
	bool findSection(int &sec_start, int &sec_end);
	bool findKey(int &key_start,int &key_end)
};
