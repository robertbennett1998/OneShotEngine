#include "pch.h"
#include "Geometry.h"

using namespace OneShotRenderer;

CGeometry::CGeometry() :
	m_pVertexBuffer(nullptr),
	m_ullVertexCount(0),
	m_ullIndicieCount(0),
	m_sGeomName("UNDEFINED")
{
}

CGeometry::~CGeometry()
{
}

bool CGeometry::Create(IVertexBuffer* pVertexBuffer, std::vector<SubGeometry*> pSubGeometries, uint64_t ullVertexCount, uint64_t ullIndicieCount, std::string sGeomName)
{
	m_pSubGeometries = pSubGeometries;
	m_pVertexBuffer = pVertexBuffer;
	m_ullIndicieCount = ullIndicieCount;
	m_ullVertexCount = ullVertexCount;
	m_sGeomName = sGeomName;

	return true;
}

void CGeometry::Bind() const
{
	m_pVertexBuffer->Bind();
}

void CGeometry::Shutdown()
{
	for (uint16_t i = 0; i < m_pSubGeometries.size(); i++)
	{
		OSE_DELETE(m_pSubGeometries[i]);
		m_pSubGeometries[i] = nullptr;
	}

	OSE_SAFE_SHUTDOWN(m_pVertexBuffer);
}