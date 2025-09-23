#include "text_processor.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <codecvt>  // 用于UTF-8与宽字符转换
#include <locale>   // 用于本地化支持

// 构造函数：初始化分词器并加载停用词
TextProcessor::TextProcessor(const std::string& dictPath,
    const std::string& hmmPath,
    const std::string& userDictPath,
    const std::string& idfPath,
    const std::string& stopwordsPath) {
    // 初始化Jieba分词器
    jieba_ = std::make_unique<cppjieba::Jieba>(
        dictPath,
        hmmPath,
        userDictPath,
        idfPath,
        stopwordsPath
    );

    // 加载停用词
    if (!loadStopwords(stopwordsPath)) {
        std::cerr << "警告: 无法加载停用词文件，将不会进行停用词过滤" << std::endl;
    }
}

// 析构函数
TextProcessor::~TextProcessor() {}

// 辅助函数：判断宽字符是否为空白字符（支持中文全角空格等）
bool isWideSpace(wchar_t c) {
    // 包含ASCII空白和中文全角空格等Unicode空白字符
    return iswspace(c) || c == L'　'; // L'　'是全角空格
}

// 加载停用词（使用UTF-8兼容处理）
bool TextProcessor::loadStopwords(const std::string& stopwordsPath) {
    std::ifstream file(stopwordsPath, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    // UTF-8到宽字符的转换器
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string line;

    while (std::getline(file, line)) {
        try {
            // 将UTF-8字符串转换为宽字符串处理
            std::wstring wideLine = converter.from_bytes(line);

            // 过滤宽字符中的空白
            auto newEnd = std::remove_if(wideLine.begin(), wideLine.end(), isWideSpace);
            wideLine.erase(newEnd, wideLine.end());

            // 转换回UTF-8字符串
            std::string word = converter.to_bytes(wideLine);

            if (!word.empty()) {
                stopwords_.insert(word);
            }
        }
        catch (const std::range_error& e) {
            // 忽略无效的UTF-8字符
            continue;
        }
    }

    return true;
}

// 文本预处理（兼容UTF-8）
std::string TextProcessor::preprocessText(const std::string& text) {
    std::string processed;
    processed.reserve(text.size()); // 预分配空间提升性能

    for (size_t i = 0; i < text.size();) {
        unsigned char c = static_cast<unsigned char>(text[i]);

        // 处理单字节ASCII字符（0-127）
        if (c <= 0x7F) {
            // 字母转换为小写
            if ((c >= 'A' && c <= 'Z')) {
                processed += tolower(c);
            }
            // 保留数字和空格
            else if ((c >= '0' && c <= '9') || c == ' ') {
                processed += c;
            }
            // 忽略其他ASCII字符（标点符号等）
            i++;
        }
        // 处理多字节UTF-8字符（中文等）
        else {
            // UTF-8编码规则：
            // 110xxxxx 表示2字节序列
            // 1110xxxx 表示3字节序列
            // 11110xxx 表示4字节序列
            if ((c & 0xE0) == 0xC0) { // 2字节UTF-8
                if (i + 1 < text.size()) {
                    processed += text[i];
                    processed += text[i + 1];
                    i += 2;
                }
                else {
                    i++; // 无效序列，跳过
                }
            }
            else if ((c & 0xF0) == 0xE0) { // 3字节UTF-8（中文常用）
                if (i + 2 < text.size()) {
                    processed += text[i];
                    processed += text[i + 1];
                    processed += text[i + 2];
                    i += 3;
                }
                else {
                    i++; // 无效序列，跳过
                }
            }
            else if ((c & 0xF8) == 0xF0) { // 4字节UTF-8
                if (i + 3 < text.size()) {
                    processed += text[i];
                    processed += text[i + 1];
                    processed += text[i + 2];
                    processed += text[i + 3];
                    i += 4;
                }
                else {
                    i++; // 无效序列，跳过
                }
            }
            else {
                i++; // 无效UTF-8字节，跳过
            }
        }
    }

    return processed;
}

// 分词
std::vector<std::string> TextProcessor::segment(const std::string& text) {
    if (!jieba_) {
        return {};
    }

    // 先预处理文本
    std::string processedText = preprocessText(text);
    if (processedText.empty()) {
        return {};
    }

    // 执行分词
    std::vector<std::string> words;
    jieba_->Cut(processedText, words, true); // 精确模式

    return words;
}

// 分词并过滤停用词
std::vector<std::string> TextProcessor::segmentAndFilter(const std::string& text) {
    std::vector<std::string> words = segment(text);
    std::vector<std::string> filteredWords;
    filteredWords.reserve(words.size());

    for (const std::string& word : words) {
        // 过滤掉停用词和单字符（中文单字可能有意义，这里保留长度>1的词）
        if (stopwords_.find(word) == stopwords_.end() && word.length() > 1) {
            filteredWords.push_back(word);
        }
    }

    return filteredWords;
}
