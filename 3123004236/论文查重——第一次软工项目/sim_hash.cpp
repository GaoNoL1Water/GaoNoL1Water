//实现Simhash算法
#include "sim_hash.h"
#include<unordered_map>

SimHash::SimHash()
{
}

SimHash::~SimHash()
{
}


//计算单个词的哈希值
uint64_t SimHash::computeWordHash(const string& word)
{

	//创建一个哈希对象（functional库提供的类），用于将string转化为哈希值
	hash<string> hasher;

	return hasher(word);
}

//通过对每一位的权重计算，来对哈希值进行加权合并
void SimHash::applyWeighting(vector<int>& featureVector, uint64_t hash, double weight)
{
	//featureVector数组用来存储哈希值每一位的权重

	//遍历64位哈希值的每一位
	for (int i = 0; i < 64; i++) {

		//通过与运算检查哈希值的第i位是否为1
		// 因为普通的int类型的1是32位，会溢出，所以用1ULL
		//1表示值为1；U表示无符号unsigned，即该值不是负数；LL表示长整型long long，刚好64位
		//<<i表示左移i位，使得第i位为1，其余位为0，再通过与运算（全1为1）来检查
		if (hash & (1ULL << i)) {
			featureVector[i] += weight;//当前位加权
		}
		else {
			featureVector[i] -= weight;//当前位减权
		}

	}
}


//根据传入的文本的分词，计算文本的Simhash值
uint64_t SimHash::computeSimHash(const vector<string>& words)
{
	//初始化featureVector数组，总共64格且初始值为0，用来存储哈希值每一位的权重
	vector<int> featureVector(64, 0);

	//用哈希表存储每个词的词频(出现次数)用来加权
	unordered_map<string, int>wordFreq;

	//计算每个词的词频
	//循环遍历words，用word代替每次循环遍历到的元素，auto自动推导word的类型，方便快捷
	//const *& 为引用传递，表示word是只读的，不允许循环内修改
	for (const auto& word : words) {

		//unordered_map是动态哈希表，大小自动扩容，不用事先指定大小
		//若key 已经存在于哈希表中，wordFreq[word] 返回该键对应的值；
		//如果 key 不存在,会自动插入一个新的键值对(键为 word,值为该类型的默认值),然后返回新插入的值
		//这里相当于在哈希表中创建了一个(word,0)的键值对，++后是(word,1)
		wordFreq[word]++;
	}

	//对每个词进行处理
	for (const auto& pair : wordFreq) { // pair是(词, 词频)
		const string& word = pair.first;
		int freq = pair.second; // 权重为词频

		uint64_t wordHash = computeWordHash(word);
		applyWeighting(featureVector, wordHash, freq); // 用词频作为权重
	}


	//最后生成SimHash值
	uint64_t simHash = 0;
	for (int i = 0; i < 64; i++) {

		//若该哈希值的第i位的加权为正，则通过或运算将SimHash值的第i位设为1，否则为0
		if (featureVector[i] > 0) {
			simHash |= (1ULL << i);//先或运算|，再赋值=
		}
	}

	return simHash;
}

//计算两个SimHash值之间的海明距离
int SimHash::hammingDistance(uint64_t hash1, uint64_t hash2)
{
	// 计算两个哈希值的异或
	uint64_t xorResult = hash1 ^ hash2;

	// 计算异或结果中1的个数（海明距离）
	int distance = 0;
	while (xorResult) {
		distance += xorResult & 1;
		xorResult >>= 1;
	}

	return distance;
}

//将海明距离转换为相似度
double SimHash::calculateSimilarity(uint64_t hash1, uint64_t hash2)
{
	// 计算海明距离
	int distance = hammingDistance(hash1, hash2);

	// 计算相似度（1 - 归一化的海明距离）
	// 64是哈希值的位数
	return 1.0 - static_cast<double>(distance) / 64.0;
}




