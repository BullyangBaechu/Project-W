#pragma once

class Texture;

class RenderMgr
{
	SINGLE(RenderMgr)
private:
	Vec2				m_Resolution;

	HDC					m_DC;			// DeviceContext 핸들		
	Texture*			m_BBTex;		// 백버퍼용 텍스쳐

	// 자주 사용할 펜, 브러쉬
	HPEN				m_Pen[(int)PEN_TYPE::END];
	HBRUSH				m_Brush[(int)BRUSH_TYPE::END];

	// DebugShape 정보
	list<tDebugShape>	m_ShapeList;

	// 디버깅 토글 용
	bool				m_bDebugRenderToggle;

public:
	int Init(Vec2 _Resoution);
	void Progress();

	HDC GetMainDC() { return m_DC; }
	HPEN GetPen(PEN_TYPE _Type) { return m_Pen[(int)_Type]; }
	HBRUSH GetBrush(BRUSH_TYPE _Type) { return m_Brush[(int)_Type]; }

	void AddDebugShape(tDebugShape _shape) { m_ShapeList.push_back(_shape); }

private:
	void DebugRender();
	int CreateBuffer();
	void CreateGDIObject();
};

