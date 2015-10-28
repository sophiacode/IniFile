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
	bool createIniFile(const char * filename);

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
	*
	*  \return ��ȡ�ɹ��򷵻�true
	*          ��ȡʧ���򷵻�false
	*/
	bool getStringValue(const char * section, const char * key, char * container);

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
	bool setIntegerValue(const char * section, const char * key, const int value);

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
	bool setDoubleValue(const char * section, const char * key, const double value);

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
	bool setStringValue(const char * section, const char * key, const char * value);

private:
	char        * _FileName;                  /* .ini�ļ���      */
	char        * _FileContainer;             /* .ini�ļ����� */
	unsigned int  _FileSize;                  /* .ini�ļ���С */
	const unsigned int _MAXFILESIZE = 65535;  /* �ļ�����С */

	/**
	 *  \brief  ���ļ���������_FileContainer��
	 *
	 *  \return �����ɹ�����true�����򷵻�false
	 */
	bool loadIniFile();

	/**
	 *  \brief  ������Ӧsection/key/value��λ��
	 *          (��δ���ҵ�����Ӧ������Ϊ0)
	 *
	 *  \param  section      ����ҵ�section��
	 *          key          ����ҵ�key��
	 *			sec_end      section�����������һ���ַ�λ�� 
	 *			key_end      section����һ���ַ�����λ��
	 *			value_start  value��һ���ַ�����λ��
	 *			value_end    value���һ���ַ�����λ��
	 */
	void findPosition(const char * section, const char * key,
		unsigned int &sec_end, unsigned int &key_end,
		unsigned int &value_start, unsigned int &value_end);

	/**
	 *  \brief ����ע����
	 *
	 *  \param position ��ǰλ��
	 */
	void ignoreComment(unsigned int &position);
};
