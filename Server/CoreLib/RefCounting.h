#pragma once

#ifndef __REFCOUNTABLE__
#define __REFCOUNTABLE__

class RefCountable {
public:
	RefCountable() : m_iRefCount(1) {}
	virtual ~RefCountable() {}

	inline int32 GetRefCount() { return m_iRefCount; }
	inline int32 ADdRef() { return ++m_iRefCount; }
	int32 ReleaseRef() {
		int32 iRefCount = --m_iRefCount;
		if (iRefCount == 0)
			delete this;
		return iRefCount;
	}
protected:
	atomic<int32> m_iRefCount;
};



// SharedPoint
template<typename T>
class TSharedPtr {
public:
	TSharedPtr() {}
	TSharedPtr(T* p_pPtr) { Set(p_pPtr); }

	// ����
	TSharedPtr(const TSharedPtr& p_cRhs) { Set(p_cRhs.m_pPtr); }
	// �̵�
	TSharedPtr(TShardPtr&& p_pRhs) {
		m_pPtr = p_pRhs.m_pPtr;
		p_pRhs.m_pPtr = nullptr; 
	}
	// ��� ���� ����
	template<typename U>
	TSharedPtr(const TSharedPtr<U>& p_pRhs) {
		Set(static_cast<T*>(p_pRhs.m_pPtr));
	}

	~TSharedPtr() { Release(); }

public:
	// ���� ������
	TSharedPtr& operator=(const TSharedPtr& p_pRhs) {
		if (m_pPtr != p_pRhs) {
			Release();
			Set(p_pRhs.m_pPtr);
		}
		return *this;
	}

	// �̵� ������
	TSharedPtr& operator=(TSharedPtr&& p_pRhs) {
		Release();
		m_pPtr = p_pRhs.m_pPtr;
		p_pRhs.m_pPtr = nullptr;
		return *this;
	}

	bool operator==(const TSharedPtr& p_pRhs) const { return m_pPtr == p_pRhs.m_pPtr; }
	bool operator=(T* p_pRhs) const { return m_pPtr == p_pRhs; }
	bool operator!=(const TSharedPtr& p_pRhs) const { return m_pPtr != p_pRhs.m_pPtr; }
	bool operator!=(T* p_pRhs) const { return m_pPtr != p_pRhs; }
	bool operator<(const TSharedPtr& p_pRhs) const { return m_pPtr < p_pRhs.m_pPtr; }
	T* operator*() { return m_pPtr; }
	const T* operator*() const { return m_pPtr; }
	operator T* () const { return m_pPtr; }
	T* operator->() { return m_pPtr; }
	const T* operator->() const { return m_pPtr; }

	inline bool IsNull() { return m_pPtr == nullptr; }
private:
	inline void Set(T* p_pPtr) {
		m_pPtr = p_pPtr;
		if (p_pPtr)
			p_pPtr->AddRef();
	}

	inline void Release() {
		if (m_pPtr != nullptr) {
			m_pPtr->ReleaseRef();
			m_pPtr = nullptr;
		}
	}
private:
	T* m_pPtr = nullptr;
};

#endif
