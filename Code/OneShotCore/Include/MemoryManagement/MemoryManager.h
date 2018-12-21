#pragma once
#ifndef ONE_SHOT_CORE_MEMORY_MANAGER_H
#define ONE_SHOT_CORE_MEMORY_MANAGER_H

#include "Interfaces/IHeap.h"

class ONE_SHOT_CORE_DLL CMemoryManager
{
	public:
		static CMemoryManager* GetInstance();

		template<class T>
		IHeap* CreateHeap(std::string sHeapName, size_t szAlignment = 1, IHeap* pParentHeap = nullptr, size_t szHeapSize = -1)
		{
			std::transform(sHeapName.begin(), sHeapName.end(), sHeapName.begin(), ::tolower);
			T* pHeap = new T();
			if (!pHeap->Initialize(sHeapName, szAlignment, pParentHeap, szHeapSize))
				return nullptr;

			auto [iter, added] = m_pHeaps.try_emplace(sHeapName, pHeap);
			if (!added)
				return m_pHeaps[sHeapName];

			return pHeap;
		}

		IHeap* GetHeap(std::string sHeapName) const;
		IHeap* GetRootHeap() const { return m_pRootHeap; }
		void DumpHeapDetails();

	private:
		CMemoryManager() : m_pRootHeap(nullptr) { return; }
		~CMemoryManager() { return; }

		IHeap* m_pRootHeap;
		std::map<std::string, IHeap*> m_pHeaps;
		static CMemoryManager* sm_pInstance;
};

#pragma region Memory Management Macros and Overloads
#define EXPAND(x) x
#define GET_GE_NEW_MACRO(_1,_2,NAME,...) NAME
#define GET_GE_NEW_ARRAY_MACRO(_1,_2,_3,NAME,...) NAME

#ifdef _DEBUG
	//forces a class to be allocated on a specific heap
	#define ALLOCATE_ON_HEAP(heapName) \
		void* operator new(size_t szSize, const uint32_t iLine, const char* pFile) \
		{ \
			IHeap* pHeap = CMemoryManager::GetInstance()->GetHeap(heapName); \
			if(pHeap == nullptr) \
				throw; \
			return pHeap->Allocate(szSize, iLine, pFile); \
		} \
		void* operator new[](size_t szSize, const uint32_t iLine, const char* pFile) \
		{\
			IHeap* pHeap = CMemoryManager::GetInstance()->GetHeap(heapName); \
			if(pHeap == nullptr) \
				throw; \
			return pHeap->Allocate(szSize, iLine, pFile); \
		}
	#define GE_NEW_1(x) new(__LINE__, __FILE__) x
	#define GE_NEW_2(x, heap) new(__LINE__, __FILE__, heap) x
	#define GE_NEW(...) EXPAND(GET_GE_NEW_MACRO(__VA_ARGS__, GE_NEW_2, GE_NEW_1)(__VA_ARGS__))	
	#define GE_NEW_ARRAY_2(x, count) new(__LINE__, __FILE__) x[count]
	#define GE_NEW_ARRAY_3(x, count, heap) new(__LINE__, __FILE__, heap) x[count]
	#define GE_NEW_ARRAY(...) EXPAND(GET_GE_NEW_ARRAY_MACRO(__VA_ARGS__, GE_NEW_ARRAY_3, GE_NEW_ARRAY_2)(__VA_ARGS__))
	//this is literally only there for consistency
	#define GE_DELETE(x) delete x
	#define GE_DELETE_ARRAY(x) delete[] x
#else
	//currently does nothing in non debug mode	
	#define ALLOCATE_ON_HEAP(heapName)
	#define GE_NEW_1(x) new x
	#define GE_NEW_2(x, heap) new(heap) x
	#define GE_NEW(...) EXPAND(GET_GE_NEW_MACRO(__VA_ARGS__, GE_NEW_2, GE_NEW_1)(__VA_ARGS__))
	#define GE_NEW_ARRAY_2(x, count) new x[count]
	#define GE_NEW_ARRAY_3(x, count, heap) new(heap) x[count]
	#define GE_NEW_ARRAY(...) EXPAND(GET_GE_NEW_ARRAY_MACRO(__VA_ARGS__, GE_NEW_ARRAY_3, GE_NEW_ARRAY_2)(__VA_ARGS__))
	//this is literally only there for consistency
	#define GE_DELETE(x) delete x
	#define GE_DELETE_ARRAY(x) delete[] x
#endif

//Overloads declarations
ONE_SHOT_CORE_DLL inline void* operator new(size_t szSize, std::string sHeapName);
ONE_SHOT_CORE_DLL inline void* operator new[](size_t szSize, std::string sHeapName);
ONE_SHOT_CORE_DLL inline void* operator new(size_t szSize, const uint32_t iLine, const char* pFileName, std::string sHeapName = "Root");
ONE_SHOT_CORE_DLL inline void* operator new[](size_t szSize, const uint32_t iLine, const char* pFileName, std::string sHeapName = "Root");
ONE_SHOT_CORE_DLL inline void DeleteOperator(void* ptr)
{
	IHeap::AllocationFooter* pAllocFooter = (IHeap::AllocationFooter*)((char*)ptr - sizeof(IHeap::AllocationFooter));
	if (pAllocFooter->ulSignature == 0xA110C000)
	{
		pAllocFooter->pHeap->Free(ptr);
	}
	else
	{
		free(ptr);
	}
}
ONE_SHOT_CORE_DLL inline void DeleteArrayOperator(void* ptr)
{
	IHeap::AllocationFooter* pAllocFooter = (IHeap::AllocationFooter*)((char*)ptr - sizeof(IHeap::AllocationFooter));
	if (pAllocFooter->ulSignature == 0xA110C000)
	{
		pAllocFooter->pHeap->Free(ptr);
	}
	else
	{
		free(ptr);
	}
}
inline void operator delete(void* ptr)
{
	DeleteOperator(ptr);
}
inline void operator delete[](void* ptr)
{
	DeleteArrayOperator(ptr);
}
#pragma endregion 

#endif