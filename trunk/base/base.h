// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� BASE_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// BASE_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef BASE_EXPORTS
#define BASE_API __declspec(dllexport)
#else
#define BASE_API __declspec(dllimport)
#endif

// �����Ǵ� base.dll ������
class BASE_API Cbase {
public:
	Cbase(void);
	// TODO: �ڴ�������ķ�����
};

extern BASE_API int nbase;

BASE_API int fnbase(void);


_declspec(dllexport)
std::wstring GetPluinName();

_declspec(dllexport)
void DataFormat(std::string& text,const std::string& host,boost::mutex& mutex,boost::unordered_map<std::wstring/*url*/,std::wstring/*title*/>& datas);

