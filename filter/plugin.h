#pragma once

typedef std::wstring (*GetPluinName_T)();

typedef void (*DataFormat_T)(std::string& text,const std::string& host,boost::mutex& mutex,boost::unordered_map<std::wstring/*url*/,std::wstring/*title*/>& datas);