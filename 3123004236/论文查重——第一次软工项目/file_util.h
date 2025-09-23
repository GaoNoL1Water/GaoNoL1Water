#pragma once
//声明文件读取函数
#include<iostream>
using namespace std;
#include<string>



class FileUtil {
	//Fileutil类的成员函数都是静态的，所以这个类是工具类，不需要创建对象
	//所以构造函数Fileutil(){}和析构函数~Fileutil(){}不能写

public:

	//static的作用：被修饰的成员函数属于整个类，而非某个对象，变成了静态成员函数
	//因此调用这些函数无需先创建对象

	//读取文件内的全部内容，并以字符串形式返回
	static string readFileToString(const string& filePath);

	//将指定的字符串写入目标文件中，返回是否成功的布尔值
	static bool writeStringToFile(const string& content, const string& filePath);

	//检查指定路径的文件是否存在，返回一个布尔值表示结果
	static bool fileExists(const string& filePath);

};


