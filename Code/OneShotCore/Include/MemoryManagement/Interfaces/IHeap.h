#pragma once
#ifndef ONE_SHOT_CORE_I_HEAP_H
#define ONE_SHOT_CORE_I_HEAP_H

#include <vector>
#include <string>

class ONE_SHOT_CORE_DLL IHeap
{
	public:
		#pragma pack(push, 1)
		struct AllocationFooter
		{
			uint32_t ulSignature = 0xA110C000;
			IHeap* pHeap;
		};
		#pragma pack(pop)

	public:
		virtual ~IHeap() {  }

		virtual bool Initialize(std::string sHeapName, size_t szAlignment = 1, IHeap* pParentHeap = nullptr, size_t szHeapSize = -1) = 0;
		virtual void AddChild(IHeap* pHeap) = 0;
		virtual void RemoveChild(IHeap* pHeap) = 0;
		virtual std::vector<IHeap*> GetChildren() const = 0;
		virtual void Clear() = 0;
		virtual void Shutdown() = 0;

		virtual void* Allocate(size_t szSize) = 0;
		virtual void* Allocate(size_t szSize, const uint32_t iLine, const char* pFile) = 0;
		virtual void Free(void* pPtr) = 0;
		virtual std::string WriteHeapOverviewToString(bool bPrintChildren = false) const = 0;
		virtual std::string WriteHeapDetailsToString() const = 0;
		virtual std::string WriteAllocationsToString() const = 0;

		virtual std::string GetHeapName() const = 0;
		virtual size_t GetHeapSize() const = 0;
		virtual size_t GetCurrentAllocationCount() const = 0;
		virtual size_t GetAmountAllocated() const = 0;
		virtual size_t GetAmountFree() const = 0;
		virtual size_t GetPeakAmountAllocated() const = 0;
		virtual size_t GetPeakAllocationCount() const = 0;
};

#endif