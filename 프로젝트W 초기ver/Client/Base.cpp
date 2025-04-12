#include "pch.h"
#include "Base.h"

UINT Base::m_GlobalID = 0;

Base::Base()
	: m_ID(m_GlobalID++)
{
}

Base::Base(const Base& _Other)
	: m_ID(m_GlobalID++)
	, m_Name(_Other.m_Name)
{
}

Base::~Base()
{
}

