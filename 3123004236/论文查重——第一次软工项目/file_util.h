#pragma once
//�����ļ���ȡ����
#include<iostream>
using namespace std;
#include<string>



class FileUtil {
	//Fileutil��ĳ�Ա�������Ǿ�̬�ģ�����������ǹ����࣬����Ҫ��������
	//���Թ��캯��Fileutil(){}����������~Fileutil(){}����д

public:

	//static�����ã������εĳ�Ա�������������࣬����ĳ�����󣬱���˾�̬��Ա����
	//��˵�����Щ���������ȴ�������

	//��ȡ�ļ��ڵ�ȫ�����ݣ������ַ�����ʽ����
	static string readFileToString(const string& filePath);

	//��ָ�����ַ���д��Ŀ���ļ��У������Ƿ�ɹ��Ĳ���ֵ
	static bool writeStringToFile(const string& content, const string& filePath);

	//���ָ��·�����ļ��Ƿ���ڣ�����һ������ֵ��ʾ���
	static bool fileExists(const string& filePath);

};


