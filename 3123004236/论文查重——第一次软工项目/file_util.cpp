//ʵ���ļ���ȡ
#include "file_util.h"
#include<fstream>//�ṩ���ļ������������
#include<sstream>//�ṩ���ַ����������������������ַ�����һϵ�в���
//ʲô�������������뻺�����������ٿ�������


//��ȡ�ļ��ڵ�ȫ�����ݣ������ַ�����ʽ����
string FileUtil::readFileToString(const string& filePath) {
	ifstream file;

	//��д�Ļ�Ĭ����Чios::in | ios::text��in�ܹ���ֻ����ʽ���ļ���text�����ı�ģʽ��
	//binary�ܹ��Զ�����ģʽ���ļ����������κ��Զ�ת������ֹ����
	file.open(filePath, ios::in | ios::binary);

	//����ļ��Ƿ�ɹ���
	if (!file.is_open()) {

		//throw���Ǳ�������������������һ���쳣���䵱�����ź�
		//runtime_error��һ��c++�ṩ���쳣�࣬��ʾ����ʱ�Ż���ֵ�ͨ�ô���
		//���������runtime_error�Ĺ��캯����������һ��runtime_error�������throw�׳�
		throw runtime_error("�޷����ļ���" + filePath);
	}

	//����һ���ַ��������������ݴ��ļ�����
	stringstream buffer;

	//rdbuf()�������Ƿ��ص�ǰ������Ļ�����ָ��
	//����rdbuf()�Ƿ������ļ������ڴ滺������ָ�룬���ָ���ֱ��ַ��������ڴ滺��������
	//�Ӷ������һ�λ���������������ֱ�ӿ���
	buffer << file.rdbuf();

	file.close();

	//str()��������ʱ�ܽ��ַ������е�����ȡ������ת����һ��string���͵Ķ���
	return buffer.str();
}


//��ָ�����ַ���д��Ŀ���ļ��У������Ƿ�ɹ��Ĳ���ֵ
bool FileUtil::writeStringToFile(const string& content, const string& filePath) {
	ofstream file;

	//��д�Ļ�Ĭ����Чios::out | ios::text��in�ܹ���ֻд��ʽ���ļ���text�����ı�ģʽ��
	//binary�ܹ��Զ�����ģʽ���ļ����������κ��Զ�ת������ֹ����
	file.open(filePath, ios::out | ios::binary);

	if (!file.is_open()) {
		return false;
	}

	//<<��������ר�����أ�����֧��string������ֱ��д�����Ļ�����
	file << content;
	file.close();
	return true;
}


//���ָ��·�����ļ��Ƿ���ڣ�����һ������ֵ��ʾ���
bool FileUtil::fileExists(const string& filePath) {
	ifstream file;
	file.open(filePath, ios::in | ios::binary);

	//good()�������Ǽ������״̬�Ƿ�������������һ������ֵ��ʾ���
	bool exists = file.good();

	file.close();
	return exists;
}