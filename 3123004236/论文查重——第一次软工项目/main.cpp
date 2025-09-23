#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include "file_util.h"
#include "text_processor.h"
#include "sim_hash.h"

// �ʵ�·����ÿ���ַ����ϸ��У����κλ���
const std::string DICT_PATH = "D:/ThirdParty/Jieba/dict/jieba.dict.utf8";
const std::string HMM_PATH = "D:/ThirdParty/Jieba/dict/hmm_model.utf8";
const std::string USER_DICT_PATH = "D:/ThirdParty/Jieba/dict/user.dict.utf8";
const std::string IDF_PATH = "D:/ThirdParty/Jieba/dict/idf.utf8";
const std::string STOPWORDS_PATH = "D:/ThirdParty/Jieba/dict/stop_words.utf8";

// ��ӡʹ��˵������������ַ����ϸ���
void printUsage(const std::string& programName) {
    std::cout << "���Ĳ��ع���ʹ�÷���:" << std::endl;
    std::cout << programName << " [ԭ��·��] [��Ϯ��·��] [���·��]" << std::endl;
    std::cout << "ʾ��:" << std::endl;
    std::cout << programName << " orig.txt orig_add.txt ans.txt" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "���󣺲�����������ȷ��" << std::endl;
        printUsage(argv[0]);
        return 1;
    }

    std::string origFilePath = argv[1];
    std::string plagiarizedFilePath = argv[2];
    std::string outputFilePath = argv[3];

    try {
        if (!FileUtil::fileExists(origFilePath)) { throw std::runtime_error("ԭ���ļ�������: " + origFilePath); }
        if (!FileUtil::fileExists(plagiarizedFilePath)) { throw std::runtime_error("��Ϯ���ļ�������: " + plagiarizedFilePath); }

        std::string origContent = FileUtil::readFileToString(origFilePath);
        std::string plagiarizedContent = FileUtil::readFileToString(plagiarizedFilePath);

        if (origContent.empty()) { throw std::runtime_error("ԭ������Ϊ��: " + origFilePath); }
        if (plagiarizedContent.empty()) { throw std::runtime_error("��Ϯ������Ϊ��: " + plagiarizedFilePath); }

        TextProcessor textProcessor(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOPWORDS_PATH);

        std::vector<std::string> origWords = textProcessor.segmentAndFilter(origContent);
        std::vector<std::string> plagiarizedWords = textProcessor.segmentAndFilter(plagiarizedContent);

        if (origWords.empty()) { throw std::runtime_error("ԭ�ķִ�����Ч����"); }
        if (plagiarizedWords.empty()) { throw std::runtime_error("��Ϯ��ִ�����Ч����"); }

        SimHash simHash;
        uint64_t origHash = simHash.computeSimHash(origWords);
        uint64_t plagiarizedHash = simHash.computeSimHash(plagiarizedWords);

        double similarity = simHash.calculateSimilarity(origHash, plagiarizedHash);

        std::stringstream resultStream;
        resultStream << std::fixed << std::setprecision(2) << similarity;
        std::string result = resultStream.str();

        if (!FileUtil::writeStringToFile(result, outputFilePath)) { throw std::runtime_error("�޷�д����: " + outputFilePath); }

        std::cout << "������ɣ����ƶ�: " << result << std::endl;
        std::cout << "���������: " << outputFilePath << std::endl;

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "����: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "δ֪����" << std::endl;
        return 1;
    }
}
