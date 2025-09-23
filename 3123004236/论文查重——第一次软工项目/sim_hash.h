#pragma once
//����SimHash������غ���

//hash������һ��ӳ�亯�������ܹ��������С���������ݣ����ַ������ṹ�壬�ļ��ȣ�
//ת��Ϊ�̶���С�����ֵ��ͨ������������������ֵ���ǹ�ϣֵ
//���磺��ϣ��unordered_map����ͨ����ϣ��������ӳ��ɹ�ϣֵ���Դ˶�λ���ݴ洢λ��
//��Simhash���Ǹ��ݴʵ���Ҫ�Լ�Ȩ�����ó����µĹ�ϣֵ��ͨ����������һ�ԱȾ��ܵó��ı����ƶ�

//�����������������������ȳ����������Ĳ����ָ��
//��Simhashֵ����64λ������������������ı���Simhashֵ�ĺ�������ԽС���ı�Խ����

#include<iostream>
using namespace std;
#include<string>
#include<vector>
#include<cstdint>//�ṩ�������̶�λ������������ȷ���Ƶ�λ����С����ֹ����������
#include<functional>//�ṩͨ�õ��㷨��������أ�(��hash����)����algorithm�������ݴ������

class SimHash {
public:
	SimHash();
	~SimHash();

	//uint64_t��64λ�޷����������ͣ���Simhash����Ҫ���ϣֵ��64λ�����ʹ��uint64_t

	//���ݴ�����ı��ķִʣ������ı���Simhashֵ
	uint64_t computeSimHash(const vector<string>& words);

	//��������SimHashֵ֮��ĺ�������
	static int hammingDistance(uint64_t hash1, uint64_t hash2);

	//����������ת��Ϊ���ƶ�
	static double calculateSimilarity(uint64_t hash1, uint64_t hash2);

private:

	//���㵥���ʵĹ�ϣֵ
	uint64_t computeWordHash(const string& word);

	//ͨ����ÿһλ��Ȩ�ؼ��㣬���Թ�ϣֵ���м�Ȩ
	void applyWeighting(vector<int>& featureVector, uint64_t hash, double weight);
};