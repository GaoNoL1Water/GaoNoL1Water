#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_set>
#include "cppjieba/Jieba.hpp"

// �ı��������࣬�������ķִʺ��ı�Ԥ����
class TextProcessor {
private:
    // Jieba�ִ���ʵ��
    std::unique_ptr<cppjieba::Jieba> jieba_;

    // ͣ�ôʼ���
    std::unordered_set<std::string> stopwords_;

    // ����ͣ�ô�
    bool loadStopwords(const std::string& stopwordsPath);


public:
    // ���캯������Ҫָ���ʵ�·��
    TextProcessor(const std::string& dictPath,
        const std::string& hmmPath,
        const std::string& userDictPath,
        const std::string& idfPath,
        const std::string& stopwordsPath);

    // ��������
    ~TextProcessor();

    // �ִʺ��������طִʽ��
    std::vector<std::string> segment(const std::string& text);

    // �ִʲ�����ͣ�ô�
    std::vector<std::string> segmentAndFilter(const std::string& text);

    // �ı�Ԥ����ȥ�������š�ת��ΪСд��
    std::string preprocessText(const std::string& text);
};
