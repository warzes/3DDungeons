#pragma once

#include "Atomics.h"
#include <cassert>

// Forward declares
template <class T> class Ref;
template <class T> class RefConst;

template <class T>
class RefTarget
{
public:
	RefTarget() = default;
	RefTarget(const RefTarget&) { /* Do not copy refcount */ }
	~RefTarget()
	{
#if defined(_DEBUG) // только для отладки
		uint32_t value = m_refCount.load(std::memory_order_relaxed);
		assert(value == 0 || value == c_embedded);
#endif
	}

	RefTarget& operator=(const RefTarget &) { /* Don't copy refcount */ return *this; }

	// Mark this class as embedded, this means the type can be used in a compound or constructed on the stack.
	// The Release function will never destruct the object, it is assumed the destructor will be called by whoever allocated
	// the object and at that point in time it is checked that no references are left to the structure.
	void SetEmbedded() const 
	{ 
		uint32_t old = m_refCount.fetch_add(c_embedded, std::memory_order_relaxed);
		assert(old < c_embedded);
	}

	// Get current refcount of this object
	uint32_t GetRefCount() const { return m_refCount.load(std::memory_order_relaxed); }

	/// Add or release a reference to this object
	void AddRef() const
	{
		// Adding a reference can use relaxed memory ordering
		m_refCount.fetch_add(1, std::memory_order_relaxed);
	}

	void Release() const
	{
		// Releasing a reference must use release semantics...
		if( m_refCount.fetch_sub(1, std::memory_order_release) == 1 )
		{
			// ... so that we can use aquire to ensure that we see any updates from other threads that released a ref before deleting the object
			std::atomic_thread_fence(std::memory_order_acquire);
			delete static_cast<const T*>(this);
		}
	}

	//static int sInternalGetRefCountOffset() { return offsetof(T, m_refCount); }

protected:
	static constexpr uint32_t c_embedded = 0x0ebedded; // A large value that gets added to the refcount to mark the object as embedded
	mutable std::atomic<uint32_t> m_refCount = 0; // Current reference count
};

// Pure virtual version of RefTarget
class RefTargetVirtual
{
public:
	virtual ~RefTargetVirtual() = default;

	virtual void AddRef() = 0;
	virtual void Release() = 0;
};

template <class T>
class Ref
{
public:
	Ref() = default;
	Ref(T* inRHS) : m_ptr(inRHS) { addRef(); }
	Ref(const Ref<T>& inRHS) : m_ptr(inRHS.m_ptr) { addRef(); }
	Ref(Ref<T>&& inRHS) noexcept : m_ptr(inRHS.m_ptr) { inRHS.m_ptr = nullptr; }
	~Ref() { release(); }

	// Assignment operators
	Ref<T>& operator=(T* inRHS) { if( m_ptr != inRHS ) { release(); m_ptr = inRHS; addRef(); } return *this; }
	Ref<T>& operator=(const Ref<T>& inRHS) { if( m_ptr != inRHS.m_ptr ) { release(); m_ptr = inRHS.m_ptr; addRef(); } return *this; }
	Ref<T>& operator=(Ref<T>&& inRHS) noexcept { if( m_ptr != inRHS.m_ptr ) { release(); m_ptr = inRHS.m_ptr; inRHS.m_ptr = nullptr; } return *this; }

	// Casting operators
	operator T* const() const { return m_ptr; }
	operator T*() { return m_ptr; }

	// Access like a normal pointer
	T* const operator->() const { return m_ptr; }
	T* operator->() { return m_ptr; }
	T& operator*() const { return *m_ptr; }

	// Comparison
	bool operator==(const T* inRHS) const { return m_ptr == inRHS; }
	bool operator==(const Ref<T>& inRHS) const { return m_ptr == inRHS.m_ptr; }
	bool operator!=(const T* inRHS) const { return m_ptr != inRHS; }
	bool operator!=(const Ref<T>& inRHS) const { return m_ptr != inRHS.m_ptr; }

	// Get pointer
	T* GetPtr() const { return m_ptr; }
	T* GetPtr() { return m_ptr; }

	//void** InternalGetPointer() { return reinterpret_cast<void **>(&m_ptr); }

private:
	template <class T2> friend class RefConst;

	// Use "variable = nullptr;" to release an object, do not call these functions
	void addRef() { if( m_ptr != nullptr ) m_ptr->AddRef(); }
	void release() { if( m_ptr != nullptr ) m_ptr->Release(); }

	T* m_ptr; // Pointer to object that we are reference counting
};

template <class T>
class RefConst
{
public:
	RefConst() : m_ptr(nullptr) {}
	RefConst(const T* inRHS) : m_ptr(inRHS) { addRef(); }
	RefConst(const RefConst<T>& inRHS) : m_ptr(inRHS.m_ptr) { addRef(); }
	RefConst(RefConst<T>&& inRHS) noexcept : m_ptr(inRHS.m_ptr) { inRHS.m_ptr = nullptr; }
	RefConst(const Ref<T>& inRHS) : m_ptr(inRHS.m_ptr) { addRef(); }
	RefConst(Ref<T>&& inRHS) noexcept : m_ptr(inRHS.m_ptr) { inRHS.m_ptr = nullptr; }
	~RefConst() { release(); }

	/// Assignment operators
	RefConst<T>& operator=(const T* inRHS) { if( m_ptr != inRHS ) { release(); m_ptr = inRHS; addRef(); } return *this; }
	RefConst<T>& operator=(const RefConst<T>& inRHS) { if( m_ptr != inRHS.m_ptr ) { release(); m_ptr = inRHS.m_ptr; addRef(); } return *this; }
	RefConst<T>& operator=(RefConst<T>&& inRHS) noexcept { if( m_ptr != inRHS.m_ptr ) { release(); m_ptr = inRHS.m_ptr; inRHS.m_ptr = nullptr; } return *this; }
	RefConst<T>& operator=(const Ref<T>& inRHS) { if( m_ptr != inRHS.m_ptr ) { release(); m_ptr = inRHS.m_ptr; addRef(); } return *this; }
	RefConst<T>& operator=(Ref<T>&& inRHS) noexcept { if( m_ptr != inRHS.m_ptr ) { release(); m_ptr = inRHS.m_ptr; inRHS.m_ptr = nullptr; } return *this; }

	// Casting operators
	operator const T*() const { return m_ptr; }

	// Access like a normal pointer
	const T* operator->() const { return m_ptr; }
	const T& operator*() const { return *m_ptr; }

	// Comparison
	bool operator==(const T* inRHS) const { return m_ptr == inRHS; }
	bool operator==(const RefConst<T>& inRHS) const { return m_ptr == inRHS.m_ptr; }
	bool operator==(const Ref<T>& inRHS) const { return m_ptr == inRHS.m_ptr; }
	bool operator!=(const T* inRHS) const { return m_ptr != inRHS; }
	bool operator!=(const RefConst<T>& inRHS) const { return m_ptr != inRHS.m_ptr; }
	bool operator!=(const Ref<T>& inRHS) const { return m_ptr != inRHS.m_ptr; }

	// Get pointer
	const T* GetPtr() const { return m_ptr; }

	//void** InternalGetPointer() { return const_cast<void **>(reinterpret_cast<const void **>(&m_ptr)); }

private:
	// Use "variable = nullptr;" to release an object, do not call these functions
	void addRef() { if( m_ptr != nullptr ) m_ptr->AddRef(); }
	void release() { if( m_ptr != nullptr ) m_ptr->Release(); }

	const T* m_ptr; ///< Pointer to object that we are reference counting
};

namespace std
{
	/// Declare std::hash for Ref
	template <class T>
	struct hash<Ref<T>>
	{
		size_t operator()(const Ref<T>& inRHS) const
		{
			return hash<T*>{ }(inRHS.GetPtr());
		}
	};

	/// Declare std::hash for RefConst
	template <class T>
	struct hash<RefConst<T>>
	{
		size_t operator()(const RefConst<T>& inRHS) const
		{
			return hash<const T *> { }(inRHS.GetPtr());
		}
	};
}