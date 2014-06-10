#ifndef _DARK_DLL_
#define _DARK_DLL_
namespace dark
{
	namespace dll
	{
		class SetResourceHandle
		{
		public:
			HINSTANCE old;
			//構造設置資源句柄
			SetResourceHandle()
			{
				old	=	AfxGetResourceHandle();
			}
			SetResourceHandle(HINSTANCE hinstance)
			{
				old	=	AfxGetResourceHandle();
				AfxSetResourceHandle(hinstance);
			}
			//析構恢復資源句柄
			~SetResourceHandle()
			{
				AfxSetResourceHandle(old);
			}
		};
	}
}
#endif	//dll
