//ʵ��Simhash�㷨
#include "sim_hash.h"
#include<unordered_map>

SimHash::SimHash()
{
}

SimHash::~SimHash()
{
}


//���㵥���ʵĹ�ϣֵ
uint64_t SimHash::computeWordHash(const string& word)
{

	//����һ����ϣ����functional���ṩ���ࣩ�����ڽ�stringת��Ϊ��ϣֵ
	hash<string> hasher;

	return hasher(word);
}

//ͨ����ÿһλ��Ȩ�ؼ��㣬���Թ�ϣֵ���м�Ȩ�ϲ�
void SimHash::applyWeighting(vector<int>& featureVector, uint64_t hash, double weight)
{
	//featureVector���������洢��ϣֵÿһλ��Ȩ��

	//����64λ��ϣֵ��ÿһλ
	for (int i = 0; i < 64; i++) {

		//ͨ�����������ϣֵ�ĵ�iλ�Ƿ�Ϊ1
		// ��Ϊ��ͨ��int���͵�1��32λ���������������1ULL
		//1��ʾֵΪ1��U��ʾ�޷���unsigned������ֵ���Ǹ�����LL��ʾ������long long���պ�64λ
		//<<i��ʾ����iλ��ʹ�õ�iλΪ1������λΪ0����ͨ�������㣨ȫ1Ϊ1�������
		if (hash & (1ULL << i)) {
			featureVector[i] += weight;//��ǰλ��Ȩ
		}
		else {
			featureVector[i] -= weight;//��ǰλ��Ȩ
		}

	}
}


//���ݴ�����ı��ķִʣ������ı���Simhashֵ
uint64_t SimHash::computeSimHash(const vector<string>& words)
{
	//��ʼ��featureVector���飬�ܹ�64���ҳ�ʼֵΪ0�������洢��ϣֵÿһλ��Ȩ��
	vector<int> featureVector(64, 0);

	//�ù�ϣ��洢ÿ���ʵĴ�Ƶ(���ִ���)������Ȩ
	unordered_map<string, int>wordFreq;

	//����ÿ���ʵĴ�Ƶ
	//ѭ������words����word����ÿ��ѭ����������Ԫ�أ�auto�Զ��Ƶ�word�����ͣ�������
	//const *& Ϊ���ô��ݣ���ʾword��ֻ���ģ�������ѭ�����޸�
	for (const auto& word : words) {

		//unordered_map�Ƕ�̬��ϣ����С�Զ����ݣ���������ָ����С
		//��key �Ѿ������ڹ�ϣ���У�wordFreq[word] ���ظü���Ӧ��ֵ��
		//��� key ������,���Զ�����һ���µļ�ֵ��(��Ϊ word,ֵΪ�����͵�Ĭ��ֵ),Ȼ�󷵻��²����ֵ
		//�����൱���ڹ�ϣ���д�����һ��(word,0)�ļ�ֵ�ԣ�++����(word,1)
		wordFreq[word]++;
	}

	//��ÿ���ʽ��д���
	for (const auto& pair : wordFreq) { // pair��(��, ��Ƶ)
		const string& word = pair.first;
		int freq = pair.second; // Ȩ��Ϊ��Ƶ

		uint64_t wordHash = computeWordHash(word);
		applyWeighting(featureVector, wordHash, freq); // �ô�Ƶ��ΪȨ��
	}


	//�������SimHashֵ
	uint64_t simHash = 0;
	for (int i = 0; i < 64; i++) {

		//���ù�ϣֵ�ĵ�iλ�ļ�ȨΪ������ͨ�������㽫SimHashֵ�ĵ�iλ��Ϊ1������Ϊ0
		if (featureVector[i] > 0) {
			simHash |= (1ULL << i);//�Ȼ�����|���ٸ�ֵ=
		}
	}

	return simHash;
}

//��������SimHashֵ֮��ĺ�������
int SimHash::hammingDistance(uint64_t hash1, uint64_t hash2)
{
	// ����������ϣֵ�����
	uint64_t xorResult = hash1 ^ hash2;

	// �����������1�ĸ������������룩
	int distance = 0;
	while (xorResult) {
		distance += xorResult & 1;
		xorResult >>= 1;
	}

	return distance;
}

//����������ת��Ϊ���ƶ�
double SimHash::calculateSimilarity(uint64_t hash1, uint64_t hash2)
{
	// ���㺣������
	int distance = hammingDistance(hash1, hash2);

	// �������ƶȣ�1 - ��һ���ĺ������룩
	// 64�ǹ�ϣֵ��λ��
	return 1.0 - static_cast<double>(distance) / 64.0;
}




