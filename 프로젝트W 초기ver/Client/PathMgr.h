#pragma once

#include "global.h"

class PathMgr
{
	SINGLE(PathMgr);

private:
	wchar_t m_SolutionPath[255];	// 솔루션 경로
	wchar_t m_ContentPath[255];		// 리소스들이 들어있는 Content 폴더 경로
	wchar_t m_BinPath[255];			// 실행파일이 있는 Bin 폴더 경로
	
public:
	void Init();

	wchar_t* GetSolutionPath() { return m_SolutionPath; }
	wchar_t* GetBinPath() { return m_BinPath; }
	wchar_t* GetContentPath() { return m_ContentPath; }
};

