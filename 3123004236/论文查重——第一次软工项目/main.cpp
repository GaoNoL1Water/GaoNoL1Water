#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include "file_util.h"
#include "text_processor.h"
#include "sim_hash.h"

// 词典路径：每个字符串严格单行，无任何换行
const std::string DICT_PATH = "D:/ThirdParty/Jieba/dict/jieba.dict.utf8";
const std::string HMM_PATH = "D:/ThirdParty/Jieba/dict/hmm_model.utf8";
const std::string USER_DICT_PATH = "D:/ThirdParty/Jieba/dict/user.dict.utf8";
const std::string IDF_PATH = "D:/ThirdParty/Jieba/dict/idf.utf8";
const std::string STOPWORDS_PATH = "D:/ThirdParty/Jieba/dict/stop_words.utf8";

// 打印使用说明：所有输出字符串严格单行
void printUsage(const std::string& programName) {
    std::cout << "论文查重工具使用方法:" << std::endl;
    std::cout << programName << " [原文路径] [抄袭版路径] [输出路径]" << std::endl;
    std::cout << "示例:" << std::endl;
    std::cout << programName << " orig.txt orig_add.txt ans.txt" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "错误：参数数量不正确！" << std::endl;
        printUsage(argv[0]);
        return 1;
    }

    std::string origFilePath = argv[1];
    std::string plagiarizedFilePath = argv[2];
    std::string outputFilePath = argv[3];

    try {
        if (!FileUtil::fileExists(origFilePath)) { throw std::runtime_error("原文文件不存在: " + origFilePath); }
        if (!FileUtil::fileExists(plagiarizedFilePath)) { throw std::runtime_error("抄袭版文件不存在: " + plagiarizedFilePath); }

        std::string origContent = FileUtil::readFileToString(origFilePath);
        std::string plagiarizedContent = FileUtil::readFileToString(plagiarizedFilePath);

        if (origContent.empty()) { throw std::runtime_error("原文内容为空: " + origFilePath); }
        if (plagiarizedContent.empty()) { throw std::runtime_error("抄袭版内容为空: " + plagiarizedFilePath); }

        TextProcessor textProcessor(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOPWORDS_PATH);

        std::vector<std::string> origWords = textProcessor.segmentAndFilter(origContent);
        std::vector<std::string> plagiarizedWords = textProcessor.segmentAndFilter(plagiarizedContent);

        if (origWords.empty()) { throw std::runtime_error("原文分词无有效内容"); }
        if (plagiarizedWords.empty()) { throw std::runtime_error("抄袭版分词无有效内容"); }

        SimHash simHash;
        uint64_t origHash = simHash.computeSimHash(origWords);
        uint64_t plagiarizedHash = simHash.computeSimHash(plagiarizedWords);

        double similarity = simHash.calculateSimilarity(origHash, plagiarizedHash);

        std::stringstream resultStream;
        resultStream << std::fixed << std::setprecision(2) << similarity;
        std::string result = resultStream.str();

        if (!FileUtil::writeStringToFile(result, outputFilePath)) { throw std::runtime_error("无法写入结果: " + outputFilePath); }

        std::cout << "查重完成，相似度: " << result << std::endl;
        std::cout << "结果保存至: " << outputFilePath << std::endl;

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "未知错误！" << std::endl;
        return 1;
    }
}
