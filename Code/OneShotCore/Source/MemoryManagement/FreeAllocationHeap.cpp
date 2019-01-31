#include "CoreHeaders.h"
#include "MemoryManagement/FreeAllocationHeap.h"
#include <math.h>

CFreeAllocationHeap::CFreeAllocationHeap() :
m_bInitialized(false),
m_pParentHeap(nullptr),
m_pFirstAllocation(nullptr),
m_pLastAllocation(nullptr),
m_szAlignment(1),
m_szNextAllocationNumber(0),
m_szCurrentAllocationCount(0),
m_szMemoryAllocated(0),
m_szPeakMemoryAllocated(0),
m_szPeakAllocationCount(0)
{
}

CFreeAllocationHeap::~CFreeAllocationHeap()
{
	Shutdown();
}

bool CFreeAllocationHeap::Initialize(std::string sHeapName, size_t szAlignment, IHeap* pParentHeap, size_t szHeapSize)
{
	if(!m_bInitialized)
	{
		if (szAlignment == 0)
			szAlignment = 1;

		m_sHeapName = sHeapName;
		m_szAlignment = szAlignment;
		m_pParentHeap = pParentHeap;

		m_bInitialized = true;
		return true;
	}

	return false;
}

void CFreeAllocationHeap::AddChild(IHeap* pHeap)
{
	if(m_pChildren.size() > 0)
	{
		for (IHeap* pChild : m_pChildren)
		{
			if (pHeap == pChild)
				return;
		}
	}

	m_pChildren.push_back(pHeap);
}

void CFreeAllocationHeap::RemoveChild(IHeap* pHeap)
{
	if (m_pChildren.size() > 0)
	{
		for(UINT i = 0; i < m_pChildren.size(); i++)
		{
			if(m_pChildren[i] == pHeap)
			{
				delete pHeap;
				pHeap = nullptr;
				m_pChildren.erase(m_pChildren.begin() + i);
			}
		}
	}
}

std::vector<IHeap*> CFreeAllocationHeap::GetChildren() const
{
	return m_pChildren;
}

void CFreeAllocationHeap::Clear()
{
	DebugAllocation* pAlloc = m_pFirstAllocation;
	while (pAlloc != nullptr)
	{
		pAlloc = pAlloc->pNextAllocation;
		Free(pAlloc);
		pAlloc = nullptr;
	}
}

void CFreeAllocationHeap::Shutdown()
{
	if(m_bInitialized)
	{
		Clear();

		if (m_pParentHeap != nullptr)
		{
			m_pParentHeap->RemoveChild(this);
		}

		m_pParentHeap = nullptr;
		m_pFirstAllocation = nullptr;
		m_pLastAllocation = nullptr;
		m_szMemoryAllocated = 0;
		m_szNextAllocationNumber = 0;
		m_szCurrentAllocationCount = 0;
		m_bInitialized = false;
	}
}

void* CFreeAllocationHeap::Allocate(size_t szSize, const uint32_t iLine, const char* pFile)
{
	if (m_bInitialized)
	{
		size_t szRequestSize = szSize + sizeof(DebugAllocation);

		DebugAllocation* pAllocation = (DebugAllocation*)malloc(szRequestSize);
		pAllocation->ullSignature = 0x00C0FFEE;
		pAllocation->pNextAllocation = nullptr;
		pAllocation->pPrevAllocation = nullptr;
		pAllocation->szAllocationNumber = m_szNextAllocationNumber;
		pAllocation->pDataPtr = (char*)pAllocation + sizeof(DebugAllocation);
		pAllocation->ulFileLine = iLine;
		pAllocation->pFile = pFile;
		m_szNextAllocationNumber++;
		pAllocation->szAllocationSize = szSize;
		m_szMemoryAllocated += szSize;
		pAllocation->allocationFooter.pHeap = this;
		pAllocation->allocationFooter.ulSignature = 0xA110C000;

		if (m_szMemoryAllocated > m_szPeakMemoryAllocated)
			m_szPeakMemoryAllocated = m_szMemoryAllocated;

		if (m_pFirstAllocation == nullptr)
		{ 
			m_pFirstAllocation = pAllocation;
			m_pLastAllocation = pAllocation;
		}
		else
		{
			m_pLastAllocation->pNextAllocation = pAllocation;
			pAllocation->pPrevAllocation = m_pLastAllocation;
			m_pLastAllocation = pAllocation;
		}

		m_szCurrentAllocationCount++;
		if (m_szCurrentAllocationCount > m_szPeakAllocationCount)
			m_szPeakAllocationCount = m_szCurrentAllocationCount;

		return ((char*)pAllocation) + sizeof(DebugAllocation);
	}

	OSE_LOG_ERROR("Memory", "Free Allocation Heap couldn't allocated memory as it wasn't initialized! Allocation occured in File: % on Line: %", pFile, iLine);
	return nullptr;
}

void CFreeAllocationHeap::Free(void* pPtr)
{
	if (m_bInitialized)
	{
		DebugAllocation* pAllocation = (DebugAllocation*)(((char*)pPtr) - sizeof(DebugAllocation));
		if (pAllocation->ullSignature == 0x00C0FFEE)
		{
			if(pAllocation->pPrevAllocation != nullptr)
				pAllocation->pPrevAllocation->pNextAllocation = pAllocation->pNextAllocation;
			
			if (pAllocation->pNextAllocation != nullptr)
				pAllocation->pNextAllocation->pPrevAllocation = pAllocation->pPrevAllocation;

			if (pAllocation == m_pLastAllocation)
				m_pLastAllocation = pAllocation->pPrevAllocation;

			if (pAllocation == m_pFirstAllocation)
				m_pFirstAllocation = pAllocation->pNextAllocation;

			m_szMemoryAllocated -= pAllocation->szAllocationSize;
			ZeroMemory(pAllocation, sizeof(DebugAllocation) + pAllocation->szAllocationSize);
			free(pAllocation);
			m_szCurrentAllocationCount--;
		}
		else
		{
			OSE_LOG_WARNING("Memory", "Attempt to deallocate memory with an incorrect signature, will use free! This may have undesidred side affects such as an object not been deconstructed! It is also a sign of heap corruption!");
			free(pPtr);
		}
	}
}

std::string CFreeAllocationHeap::WriteHeapOverviewToString(bool bPrintChildren) const
{
	if (bPrintChildren)
	{
		std::stringstream ss;
		ss << WriteHeapOverviewToString();

		if (m_pChildren.size() > 0)
		{
			for (IHeap* pChild : m_pChildren)
				ss << "\t" << pChild->WriteHeapOverviewToString(true);
		}

		ss << "\n";

		OutputDebugStringA(ss.str().c_str());
		return ss.str();
	}
	else
	{
		std::stringstream ss;
		ss << m_sHeapName << " Heap Overview - Alignment: " << m_szAlignment << "\n";
		ss << "\tNumber of allocations: " << m_szCurrentAllocationCount << "\n";
		ss << "\tPeak number of allocations: " << m_szPeakAllocationCount << "\n";
		ss << "\tMemory allocated: " << m_szMemoryAllocated << "\n";
		ss << "\tPeak memory allocated: " << m_szPeakMemoryAllocated << "\n";

		return ss.str();
	}
	return "NULL";
}

std::string CFreeAllocationHeap::WriteHeapDetailsToString() const
{
	std::stringstream ss;
	ss << WriteHeapOverviewToString();
	
	if(m_pFirstAllocation != nullptr)
		ss << WriteAllocationsToString();

	return ss.str();
}

std::string CFreeAllocationHeap::WriteAllocationsToString() const
{
	std::stringstream ss;
	ss << "Allocations:\n";
	
	if(m_pFirstAllocation == nullptr)
	{
		ss << "No allocations!\n";
		return ss.str();
	}

	DebugAllocation allocation = *m_pFirstAllocation;
	while(true)
	{
		if (allocation.ullSignature == 0x00C0FFEE)
		{
			ss << allocation.szAllocationNumber << ": " << allocation.pDataPtr << " with " << allocation.szAllocationSize << " bytes of memory. It was allocated on line " << allocation.ulFileLine << " of " << allocation.pFile << "\n";
			if (allocation.pNextAllocation != nullptr)
				allocation = *allocation.pNextAllocation;
			else
				break;
		}
		else
		{
			break;
		}
	}

	return ss.str();
}