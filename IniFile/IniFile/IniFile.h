/*********************************************************************
*
*	@file		IniFile.h
*	@brief		read and write .ini file
*
*	Date        Name            Description
*	18/10/31	HQWY/Sophia		Creation
*
*********************************************************************/

#pragma once
#include<string>
#include<map>

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
	void createIniFile(const char * filename);

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
	*         maxlen      字符串最大容量
	*
	*  \return 获取成功则返回true
	*          获取失败则返回false
	*/
	bool getStringValue(const char * section, const char * key, char * container,int maxlen);

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
	void setIntegerValue(const char * section, const char * key, const int value);

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
	void setDoubleValue(const char * section, const char * key, const double value);

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
	void setStringValue(const char * section, const char * key, const char * value);

private:
	std::string  _FileName;                         /**< .ini文件名      */
	std::string  _FileContainer;                    /**< .ini文件内容    */
	std::map<std::string, std::string>  _FileMap;   /**< 储存.ini文件的value值 */

	/**
	 *  \brief  将文件内容载入_FileContainer中
	 *
	 *  \return 操作成功返回true，否则返回false
	 */
	void loadIniFile();

	/**
	 *  \brief  创建红黑树储存.ini文件的key-value值
	 */
	void createMap();

	/**
	 *  \brief  判断当前位置是否在注释行中
	 *
	 *  \param  pos 当前位置
	 *
	 *  \return 在注释行中则返回true,否则返回false
	 */
	bool isInComment(int pos);

	/**
	 *  \brief  获取value
	 *
	 *  \param  section section名
	 *          key     key名
	 *
	 *  \return value值
	 */
	std::string getValue(const char * section, const char * key);
};
