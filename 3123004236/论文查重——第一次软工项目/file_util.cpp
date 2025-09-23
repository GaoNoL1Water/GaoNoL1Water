//实现文件读取
#include "file_util.h"
#include<fstream>//提供对文件的输入输出流
#include<sstream>//提供对字符串的输入输出流，方便对字符串的一系列操作
//什么是流操作：先与缓冲区交互，再考虑其他


//读取文件内的全部内容，并以字符串形式返回
string FileUtil::readFileToString(const string& filePath) {
	ifstream file;

	//不写的话默认生效ios::in | ios::text，in能够以只读方式打开文件，text是以文本模式打开
	//binary能够以二进制模式打开文件，不进行任何自动转换，防止乱码
	file.open(filePath, ios::in | ios::binary);

	//检查文件是否成功打开
	if (!file.is_open()) {

		//throw不是变量名，而是主动触发一个异常，充当报警信号
		//runtime_error是一个c++提供的异常类，表示运行时才会出现的通用错误
		//这里调用了runtime_error的构造函数，创建了一个runtime_error对象，随后被throw抛出
		throw runtime_error("无法打开文件：" + filePath);
	}

	//创建一个字符串流对象，用于暂存文件内容
	stringstream buffer;

	//rdbuf()的作用是返回当前流对象的缓冲区指针
	//这里rdbuf()是返回了文件流的内存缓冲区的指针，这个指针又被字符串流的内存缓冲区接收
	//从而完成了一次缓冲区到缓冲区的直接拷贝
	buffer << file.rdbuf();

	file.close();

	//str()不带参数时能将字符串流中的数据取出来，转换成一个string类型的对象
	return buffer.str();
}


//将指定的字符串写入目标文件中，返回是否成功的布尔值
bool FileUtil::writeStringToFile(const string& content, const string& filePath) {
	ofstream file;

	//不写的话默认生效ios::out | ios::text，in能够以只写方式打开文件，text是以文本模式打开
	//binary能够以二进制模式打开文件，不进行任何自动转换，防止乱码
	file.open(filePath, ios::out | ios::binary);

	if (!file.is_open()) {
		return false;
	}

	//<<操作符被专门重载，用以支持string类型能直接写入流的缓冲区
	file << content;
	file.close();
	return true;
}


//检查指定路径的文件是否存在，返回一个布尔值表示结果
bool FileUtil::fileExists(const string& filePath) {
	ifstream file;
	file.open(filePath, ios::in | ios::binary);

	//good()的作用是检查流的状态是否正常，并返回一个布尔值表示结果
	bool exists = file.good();

	file.close();
	return exists;
}