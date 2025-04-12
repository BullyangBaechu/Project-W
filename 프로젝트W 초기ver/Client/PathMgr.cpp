#include "pch.h"
#include "PathMgr.h"

PathMgr::PathMgr()
	: m_SolutionPath{}
	, m_ContentPath{}
	, m_BinPath{}
{
}

PathMgr::~PathMgr()
{
}

void PathMgr::Init()
{
	// 현재 경로	
	// 프로그램이 실행된 경로
	// VisualStudio 로 실행할 경우 프로젝트 속성 '작업 디렉터리' 에 영향을 받는다.
	// 실제 실행파일 경로와 작업 디렉터리를 일치시켜서 동일한 동작을 수행할 수 있도록 한다.
	GetCurrentDirectory(255, m_BinPath);

	// Bin 상위폴더 경로를 ContentPath 에 복사한다.
	int Length = (int)wcslen(m_BinPath);
	int i = Length;
	for ( ; i > 0; --i)
	{
		if ('\\' == m_BinPath[i])
			break;
	}		
	wmemcpy_s(m_ContentPath, 255, m_BinPath, i);

	// \\Content\\ 를 붙여준다.
	wcscat_s(m_ContentPath, L"\\Content\\");

	// SolutionPath 계산하기
	--i;
	for (; i > 0; --i)
	{
		if ('\\' == m_BinPath[i])
			break;
	}
	wmemcpy_s(m_SolutionPath, 255, m_BinPath, i + 1);
}
