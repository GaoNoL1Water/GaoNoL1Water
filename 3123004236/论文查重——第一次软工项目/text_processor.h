#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_set>
#include "cppjieba/Jieba.hpp"

// 文本处理器类，用于中文分词和文本预处理
class TextProcessor {
private:
    // Jieba分词器实例
    std::unique_ptr<cppjieba::Jieba> jieba_;

    // 停用词集合
    std::unordered_set<std::string> stopwords_;

    // 加载停用词
    bool loadStopwords(const std::string& stopwordsPath);


public:
    // 构造函数，需要指定词典路径
    TextProcessor(const std::string& dictPath,
        const std::string& hmmPath,
        const std::string& userDictPath,
        const std::string& idfPath,
        const std::string& stopwordsPath);

    // 析构函数
    ~TextProcessor();

    // 分词函数，返回分词结果
    std::vector<std::string> segment(const std::string& text);

    // 分词并过滤停用词
    std::vector<std::string> segmentAndFilter(const std::string& text);

    // 文本预处理：去除标点符号、转换为小写等
    std::string preprocessText(const std::string& text);
};
