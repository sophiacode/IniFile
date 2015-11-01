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
	*  \brief ���캯��
	*/
	IniFile();

	/**
	*  \brief ���ع��캯��
	*
	*  \param filename .ini�ļ���
	*/
	IniFile(const char * filename);

	/**
	*  \brief �������캯��
	*
	*  \param rhs ��������
	*/
	IniFile(const IniFile & rhs);

	/**
	*  \brief ��������
	*/
	~IniFile();

	/**
	*  \brief ���صȺ�
	*
	*  \param rhs ��������
	*
	*  \return    ���������
	*/
	IniFile & operator = (const IniFile & rhs);

	/**
	*  \brief ��������
	* 
	*  \param filename .ini�ļ���
	*
	*  \return �����ɹ��򷵻�true
	*          ����ʧ���򷵻�false
	*/
	void createIniFile(const char * filename);

	/**
	*  \brief ��ȡ�����ͱ���
	*
	*  \param section     .ini�ļ���section��
	*         key         .ini�ļ���key��
	*    	  container   ��ֵ����
	*
	*  \return ��ȡ�ɹ��򷵻�true
	*          ��ȡʧ���򷵻�false
	*/
	bool getIntegerValue(const char * section, const char * key, int & container);

	/**
	*  \brief ��ȡʵ���ͱ���
	*
	*  \param section     .ini�ļ���section��
	*         key         .ini�ļ���key��
	*    	  container   ��ֵ����
	*
	*  \return ��ȡ�ɹ��򷵻�true
	*          ��ȡʧ���򷵻�false
	*/
	bool getDoubleValue(const char * section, const char * key, double & container);

	/**
	*  \brief ��ȡ�ַ����ͱ���
	*
	*  \param section     .ini�ļ���section��
	*         key         .ini�ļ���key��
	*    	  container   ��ֵ����
	*         maxlen      �ַ����������
	*
	*  \return ��ȡ�ɹ��򷵻�true
	*          ��ȡʧ���򷵻�false
	*/
	bool getStringValue(const char * section, const char * key, char * container,int maxlen);

	/**
	*  \brief ���ñ�����������ֵ
	*
	*  \param section .ini�ļ���section��
	*         key     .ini�ļ���key��
	*    	  value   �����õ���ֵ
	*
	*  \return ���óɹ��򷵻�true
	*          ����ʧ���򷵻�false
	*/
	void setIntegerValue(const char * section, const char * key, const int value);

	/**
	*  \brief ���ñ���ʵ������ֵ
	*
	*  \param section .ini�ļ���section��
	*         key     .ini�ļ���key��
	*    	  value   �����õ���ֵ
	*
	*  \return ���óɹ��򷵻�true
	*          ����ʧ���򷵻�false
	*/
	void setDoubleValue(const char * section, const char * key, const double value);

	/**
	*  \brief ���ñ����ַ�������ֵ
	*
	*  \param section .ini�ļ���section��
	*         key     .ini�ļ���key��
	*    	  value   �����õ���ֵ
	*
	*  \return ���óɹ��򷵻�true
	*          ����ʧ���򷵻�false
	*/
	void setStringValue(const char * section, const char * key, const char * value);

private:
	std::string  _FileName;                         /**< .ini�ļ���      */
	std::string  _FileContainer;                    /**< .ini�ļ�����    */
	std::map<std::string, std::string>  _FileMap;   /**< ����.ini�ļ���valueֵ */

	/**
	 *  \brief  ���ļ���������_FileContainer��
	 *
	 *  \return �����ɹ�����true�����򷵻�false
	 */
	void loadIniFile();

	/**
	 *  \brief  �������������.ini�ļ���key-valueֵ
	 */
	void createMap();

	/**
	 *  \brief  �жϵ�ǰλ���Ƿ���ע������
	 *
	 *  \param  pos ��ǰλ��
	 *
	 *  \return ��ע�������򷵻�true,���򷵻�false
	 */
	bool isInComment(int pos);

	/**
	 *  \brief  ��ȡvalue
	 *
	 *  \param  section section��
	 *          key     key��
	 *
	 *  \return valueֵ
	 */
	std::string getValue(const char * section, const char * key);
};
