#pragma once
//声明SimHash计算相关函数

//hash函数是一种映射函数，它能够将任意大小的输入数据（如字符串，结构体，文件等）
//转换为固定大小的输出值（通常是整数），这个输出值就是哈希值
//例如：哈希表unordered_map就是通过哈希函数将键映射成哈希值，以此定位数据存储位置
//而Simhash则是根据词的重要性加权，最后得出文章的哈希值，通过海明距离一对比就能得出文本相似度

//海明距离是用来衡量两个等长二进制数的差异的指标
//而Simhash值都是64位二进制数，因此两个文本的Simhash值的海明距离越小，文本越相似

#include<iostream>
using namespace std;
#include<string>
#include<vector>
#include<cstdint>//提供整数（固定位数），用来精确控制的位数大小，防止兼容性问题
#include<functional>//提供通用的算法（函数相关）(如hash函数)，而algorithm则是数据处理相关

class SimHash {
public:
	SimHash();
	~SimHash();

	//uint64_t是64位无符号整数类型，而Simhash常常要求哈希值是64位，因此使用uint64_t

	//根据传入的文本的分词，计算文本的Simhash值
	uint64_t computeSimHash(const vector<string>& words);

	//计算两个SimHash值之间的海明距离
	static int hammingDistance(uint64_t hash1, uint64_t hash2);

	//将海明距离转换为相似度
	static double calculateSimilarity(uint64_t hash1, uint64_t hash2);

private:

	//计算单个词的哈希值
	uint64_t computeWordHash(const string& word);

	//通过对每一位的权重计算，来对哈希值进行加权
	void applyWeighting(vector<int>& featureVector, uint64_t hash, double weight);
};