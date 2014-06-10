// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 BASE_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// BASE_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef BASE_EXPORTS
#define BASE_API __declspec(dllexport)
#else
#define BASE_API __declspec(dllimport)
#endif

// 此类是从 base.dll 导出的
class BASE_API Cbase {
public:
	Cbase(void);
	// TODO: 在此添加您的方法。
};

extern BASE_API int nbase;

BASE_API int fnbase(void);


_declspec(dllexport)
std::wstring GetPluinName();

_declspec(dllexport)
void DataFormat(std::string& text,const std::string& host,boost::mutex& mutex,boost::unordered_map<std::wstring/*url*/,std::wstring/*title*/>& datas);

