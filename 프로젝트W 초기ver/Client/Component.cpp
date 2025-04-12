#include "pch.h"
#include "Component.h"

Component::Component(COMPONENT_TYPE _Type)
	: m_Type(_Type)
	, m_Actor(nullptr)
{
}

Component::~Component()
{
}
