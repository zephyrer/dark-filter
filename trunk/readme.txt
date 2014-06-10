程序功能
	從草榴社區 獲取帖子 標題 url
	(每次 傳入界面中 url 設置 以及此為第多少此請求 到 lua腳本 由腳本函數url_format返回請求頁面)
	
	之後 用戶可以 輸入 一個 filter 作為 正則表達式 匹配需要尋找的帖子(不需要等待 搜索完所有數據)
	
	import export 按鈕 導出導入 工作狀態 以便可以在下次 打開程序時 繼續上次的工作
	
	once more 按鈕 對所有超時請求 重新發出一次請求
	
	雙擊列表數據 將使用默認瀏覽器 打開 列表中記錄的url
	按下ctrl 雙擊 列表數據 複製 列表中記錄的 帖子標題 到 剪貼板
	
	
	
	此程序只從 草榴獲取數據 然通過 插件 可獲取其他網站數據 詳見 插件

編譯 
	1	程序使用Poco庫進行http通信 故需要自行配置Poco環境(調試使用版本 1.46 )
	2	程序使用boost進行 正則表達式 xml 智能指針 ... 操作 故需要自行配置boost環境(調試使用版本 1.55.0 )
	3	程序使用了 lua腳本 故需要自行配置lua環境(調試使用版本 5.1 )
	4	include目錄為 餘封裝的庫 部分代碼 此程序中調用了這些代碼 需要將include加入到 編譯器include目錄中
	
	
插件
	通過 改寫 lua 腳本 url_format 函數 指定 請求網站url
	
	插件只需要實現一個dll 為其導出 
	typedef std::wstring (*GetPluinName_T)();	//返回插件名 多個插件 不能重名
	
	typedef void (*DataFormat_T)(std::string& text	//網站返回的 響應內容
		,const std::string& host	//網站域名
		,boost::mutex& mutex		//同步鎖
		,boost::unordered_map<std::wstring/*url*/,std::wstring/*title*/>& datas	//數據緩存 應該將獲取的 數據 添加到 此散列中
		);
	兩個函數
	之後將dll放入 plugin目錄 即可
	
	