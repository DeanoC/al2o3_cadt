#pragma once

#include "al2o3_cadt/vector.h"
#include <type_traits>

namespace Cadt {

	// currently BaseVector and Vector c++ only handle POD types (no ctor or dtor will be called)

	struct BaseVector {
		template<typename T>
		[[nodiscard]] static auto Create() -> BaseVector* {
			return (BaseVector*) CADT_VectorCreate(sizeof(T));
		}
		template<typename T>
		[[nodiscard]] static auto CreateWithAllocator(Memory_Allocator* allocator) -> BaseVector* {
			return (BaseVector*) CADT_VectorCreateWithAllocator(sizeof(T), allocator);
		}

		[[nodiscard]] static auto From(CADT_VectorHandle vec) -> BaseVector* {
			return (BaseVector*) vec;
		}

		static auto Destroy(BaseVector* v) -> void {
			CADT_VectorDestroy((CADT_VectorHandle)v);
		}

		[[nodiscard]] static auto Clone(BaseVector const* src) -> BaseVector* {
			return (BaseVector*) CADT_VectorClone((CADT_VectorHandle)src);
		}

		operator CADT_VectorHandle() { return (CADT_VectorHandle)this; }

		[[nodiscard]] auto clone() const { return Clone(this); }
		[[nodiscard]] auto elementSize() const { return CADT_VectorElementSize((CADT_VectorHandle)this); }
		[[nodiscard]] auto size() const { return CADT_VectorSize((CADT_VectorHandle)this); }
		[[nodiscard]] auto empty() const { return CADT_VectorIsEmpty((CADT_VectorHandle)this); }
		[[nodiscard]] auto capacity() const { return CADT_VectorCapacity((CADT_VectorHandle)this); }
		[[nodiscard]] void* at(size_t index) { return CADT_VectorAt((CADT_VectorHandle)this, index); }
		[[nodiscard]] void const* at(size_t index) const { return CADT_VectorAt((CADT_VectorHandle)this, index); }
		[[nodiscard]] void * peek() { return CADT_VectorPeekElement((CADT_VectorHandle)this); }
		[[nodiscard]] void const * peek() const { return CADT_VectorPeekElement((CADT_VectorHandle)this); }
		[[nodiscard]] void * data() { return CADT_VectorData((CADT_VectorHandle)this); }
		[[nodiscard]] void const * data() const { return CADT_VectorData((CADT_VectorHandle)this); }

		void resize(size_t size) { CADT_VectorResize((CADT_VectorHandle)this, size); }
		void resizeWithDefault(size_t size, void* def) { CADT_VectorResizeWithDefault((CADT_VectorHandle)this, size, def); }
		void reserve(size_t size) const { CADT_VectorReserve((CADT_VectorHandle)this, size); }
		size_t push(void const * element) { return CADT_VectorPushElement((CADT_VectorHandle)this, element); }
		size_t pushCloneToEnd(size_t index) { return CADT_VectorPushElementCloneToEnd((CADT_VectorHandle)this, index); }
		void replace(size_t srcIndex,	size_t dstIndex) { CADT_VectorReplace((CADT_VectorHandle)this, srcIndex, dstIndex); }
		void swap(size_t srcIndex,	size_t dstIndex) { CADT_VectorSwap((CADT_VectorHandle)this, srcIndex, dstIndex); }
		void pop(void* out) { CADT_VectorPopElement((CADT_VectorHandle)this, out); }
		void popAndDiscard() { CADT_VectorPopAndDiscardElement((CADT_VectorHandle)this); }
		void remove(size_t index) { CADT_VectorRemove((CADT_VectorHandle)this, index); }
		void swapRemove(size_t index) { CADT_VectorSwapRemove((CADT_VectorHandle)this, index); }

		BaseVector() = delete;
		~BaseVector() = delete;
	};

	template<typename T, bool ispod = std::is_pod<T>()> struct Vector;

	template<typename T>
	struct Vector<T, true> : private BaseVector {
		[[nodiscard]] static auto Create() ->Vector* {
			return (Vector*) BaseVector::Create<T>();
		}
		[[nodiscard]] static auto CreateWithAllocator(Memory_Allocator* allocator) -> Vector* {
			return (Vector*) BaseVector::CreateWithAllocator<T>();
		}
		static auto Destroy(Vector* v) -> void {
			BaseVector::Destroy(v);
		}
		[[nodiscard]] static auto Clone(Vector const* src) -> Vector* {
			ASSERT(CADT_VectorElementSize(src) == sizeof(T));
			return BaseVector::Clone(src);
		}

		[[nodiscard]] static auto From(CADT_VectorHandle vec) -> Vector* {
			ASSERT(CADT_VectorElementSize(vec) == sizeof(T));
			return (Vector*) vec;
		}

		using BaseVector::clone;
		using BaseVector::elementSize;
		using BaseVector::size;
		using BaseVector::empty;
		using BaseVector::capacity;
		using BaseVector::reserve;
		using BaseVector::pushCloneToEnd;
		using BaseVector::replace;
		using BaseVector::swap;
		using BaseVector::popAndDiscard;
		using BaseVector::remove;
		using BaseVector::swapRemove;

		auto destroy() { Destroy(this); }

		auto resize(size_t size) {
			static const T defaultT{};
			CADT_VectorResizeWithDefault((CADT_VectorHandle)this, size, &defaultT);
		}

		auto resizeNoInit(size_t size) {
			CADT_VectorResize((CADT_VectorHandle)this, size);
		}
		[[nodiscard]] T & at(size_t index) { return *(T*)BaseVector::at(index); }
		[[nodiscard]] T const& at(size_t index) const { return *(T const*)BaseVector::at(index); }
		[[nodiscard]] T & peek() { return *(T*)BaseVector::peek(); }
		[[nodiscard]] T const & peek() const { return *(T const*)BaseVector::peek(); }
		[[nodiscard]] T * data() { return (T*)BaseVector::data(); }
		[[nodiscard]] T const * data() const { return (T*)BaseVector::data(); }
		auto push(T const & val) { return BaseVector::push(&val); }
		[[nodiscard]] auto pop() -> T { T v; BaseVector::pop(&v); return v; }

		Vector() = delete;
		~Vector() = delete;
	};

	static_assert( sizeof(BaseVector*) == sizeof(CADT_VectorHandle));
};