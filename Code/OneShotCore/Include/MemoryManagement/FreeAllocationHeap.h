#pragma once
#ifndef ONE_SHOT_CORE_FREE_ALLOCATION_HEAP_H
#define ONE_SHOT_CORE_FREE_ALLOCATION_HEAP_H

#include "MemoryManagement/Interfaces/IHeap.h"

class ONE_SHOT_CORE_DLL CFreeAllocationHeap : public IHeap
{
	private:
		#pragma pack(push, 1)
		struct DebugAllocation
		{
			uint32_t ullSignature = 0x00C0FFEE;
			DebugAllocation* pNextAllocation;
			DebugAllocation* pPrevAllocation;
			size_t szAllocationNumber;
			size_t szAllocationSize;
			const char* pFile;
			uint32_t ulFileLine;
			void* pDataPtr;
			AllocationFooter allocationFooter;
		};
		#pragma pack(pop)

	public:
		CFreeAllocationHeap();
		virtual ~CFreeAllocationHeap();

		bool Initialize(std::string sHeapName, size_t szAlignment = 1, IHeap* pParentHeap = nullptr, size_t szHeapSize = -1) override;
		void AddChild(IHeap* pHeap) override;
		void RemoveChild(IHeap* pHeap) override;
		std::vector<IHeap*> GetChildren() const override;
		void Clear() override;
		void Shutdown() override;

		void* Allocate(size_t szSize) override { throw; }
		void* Allocate(size_t szSize, const uint32_t iLine, const char* pFile) override;
		void Free(void* pPtr) override;
		std::string WriteHeapOverviewToString(bool bPrintChildren = false) const override;
		std::string WriteHeapDetailsToString() const override;
		std::string WriteAllocationsToString() const override;
		std::string GetHeapName() const override
		{
			return m_sHeapName;
		}

		size_t GetHeapSize() const override
		{
			return -1;
		}

		size_t GetCurrentAllocationCount() const override
		{
			return m_szCurrentAllocationCount;
		}

		size_t GetAmountAllocated() const override
		{
			return m_szMemoryAllocated;
		}

		size_t GetAmountFree() const override
		{
			return -1;
		}

		size_t GetPeakAmountAllocated() const override
		{
			return m_szPeakMemoryAllocated;
		}

		size_t GetPeakAllocationCount() const override
		{
			return m_szPeakAllocationCount;
		}

	private:
		bool m_bInitialized;
		IHeap* m_pParentHeap;
		std::string m_sHeapName;
		std::vector<IHeap*> m_pChildren;
		DebugAllocation* m_pFirstAllocation;
		DebugAllocation* m_pLastAllocation;
		size_t m_szAlignment;
		size_t m_szNextAllocationNumber;
		size_t m_szCurrentAllocationCount;
		size_t m_szMemoryAllocated;
		size_t m_szPeakMemoryAllocated;
		size_t m_szPeakAllocationCount;
};

#endif