#include "text_processor.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <codecvt>  // ����UTF-8����ַ�ת��
#include <locale>   // ���ڱ��ػ�֧��

// ���캯������ʼ���ִ���������ͣ�ô�
TextProcessor::TextProcessor(const std::string& dictPath,
    const std::string& hmmPath,
    const std::string& userDictPath,
    const std::string& idfPath,
    const std::string& stopwordsPath) {
    // ��ʼ��Jieba�ִ���
    jieba_ = std::make_unique<cppjieba::Jieba>(
        dictPath,
        hmmPath,
        userDictPath,
        idfPath,
        stopwordsPath
    );

    // ����ͣ�ô�
    if (!loadStopwords(stopwordsPath)) {
        std::cerr << "����: �޷�����ͣ�ô��ļ������������ͣ�ôʹ���" << std::endl;
    }
}

// ��������
TextProcessor::~TextProcessor() {}

// �����������жϿ��ַ��Ƿ�Ϊ�հ��ַ���֧������ȫ�ǿո�ȣ�
bool isWideSpace(wchar_t c) {
    // ����ASCII�հ׺�����ȫ�ǿո��Unicode�հ��ַ�
    return iswspace(c) || c == L'��'; // L'��'��ȫ�ǿո�
}

// ����ͣ�ôʣ�ʹ��UTF-8���ݴ���
bool TextProcessor::loadStopwords(const std::string& stopwordsPath) {
    std::ifstream file(stopwordsPath, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    // UTF-8�����ַ���ת����
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string line;

    while (std::getline(file, line)) {
        try {
            // ��UTF-8�ַ���ת��Ϊ���ַ�������
            std::wstring wideLine = converter.from_bytes(line);

            // ���˿��ַ��еĿհ�
            auto newEnd = std::remove_if(wideLine.begin(), wideLine.end(), isWideSpace);
            wideLine.erase(newEnd, wideLine.end());

            // ת����UTF-8�ַ���
            std::string word = converter.to_bytes(wideLine);

            if (!word.empty()) {
                stopwords_.insert(word);
            }
        }
        catch (const std::range_error& e) {
            // ������Ч��UTF-8�ַ�
            continue;
        }
    }

    return true;
}

// �ı�Ԥ��������UTF-8��
std::string TextProcessor::preprocessText(const std::string& text) {
    std::string processed;
    processed.reserve(text.size()); // Ԥ����ռ���������

    for (size_t i = 0; i < text.size();) {
        unsigned char c = static_cast<unsigned char>(text[i]);

        // �����ֽ�ASCII�ַ���0-127��
        if (c <= 0x7F) {
            // ��ĸת��ΪСд
            if ((c >= 'A' && c <= 'Z')) {
                processed += tolower(c);
            }
            // �������ֺͿո�
            else if ((c >= '0' && c <= '9') || c == ' ') {
                processed += c;
            }
            // ��������ASCII�ַ��������ŵȣ�
            i++;
        }
        // ������ֽ�UTF-8�ַ������ĵȣ�
        else {
            // UTF-8�������
            // 110xxxxx ��ʾ2�ֽ�����
            // 1110xxxx ��ʾ3�ֽ�����
            // 11110xxx ��ʾ4�ֽ�����
            if ((c & 0xE0) == 0xC0) { // 2�ֽ�UTF-8
                if (i + 1 < text.size()) {
                    processed += text[i];
                    processed += text[i + 1];
                    i += 2;
                }
                else {
                    i++; // ��Ч���У�����
                }
            }
            else if ((c & 0xF0) == 0xE0) { // 3�ֽ�UTF-8�����ĳ��ã�
                if (i + 2 < text.size()) {
                    processed += text[i];
                    processed += text[i + 1];
                    processed += text[i + 2];
                    i += 3;
                }
                else {
                    i++; // ��Ч���У�����
                }
            }
            else if ((c & 0xF8) == 0xF0) { // 4�ֽ�UTF-8
                if (i + 3 < text.size()) {
                    processed += text[i];
                    processed += text[i + 1];
                    processed += text[i + 2];
                    processed += text[i + 3];
                    i += 4;
                }
                else {
                    i++; // ��Ч���У�����
                }
            }
            else {
                i++; // ��ЧUTF-8�ֽڣ�����
            }
        }
    }

    return processed;
}

// �ִ�
std::vector<std::string> TextProcessor::segment(const std::string& text) {
    if (!jieba_) {
        return {};
    }

    // ��Ԥ�����ı�
    std::string processedText = preprocessText(text);
    if (processedText.empty()) {
        return {};
    }

    // ִ�зִ�
    std::vector<std::string> words;
    jieba_->Cut(processedText, words, true); // ��ȷģʽ

    return words;
}

// �ִʲ�����ͣ�ô�
std::vector<std::string> TextProcessor::segmentAndFilter(const std::string& text) {
    std::vector<std::string> words = segment(text);
    std::vector<std::string> filteredWords;
    filteredWords.reserve(words.size());

    for (const std::string& word : words) {
        // ���˵�ͣ�ôʺ͵��ַ������ĵ��ֿ��������壬���ﱣ������>1�Ĵʣ�
        if (stopwords_.find(word) == stopwords_.end() && word.length() > 1) {
            filteredWords.push_back(word);
        }
    }

    return filteredWords;
}
