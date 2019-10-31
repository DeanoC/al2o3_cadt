#pragma once

#include "al2o3_cadt/freelist.h"
#include <type_traits>

namespace Cadt {

	// currently BaseFreeList and FreeList c++ only handle POD types (no ctor or dtor will be called)
	struct BaseFreeList {
		template<typename T>
		[[nodiscard]] static auto Create(size_t capacity) -> BaseFreeList* {
				return (BaseFreeList*) CADT_FreeListCreate(sizeof(T), capacity);
		}

		[[nodiscard]] static auto From(CADT_FreeListHandle fl) -> BaseFreeList* {
				return (BaseFreeList*) fl;
		}

		static auto Destroy(BaseFreeList* v) -> void {
			CADT_FreeListDestroy((CADT_FreeListHandle)v);
		}

		[[nodiscard]] static auto Clone(BaseFreeList const* src) -> BaseFreeList* {
				return (BaseFreeList*) CADT_FreeListClone((CADT_FreeListHandle)src);
		}

		operator CADT_FreeListHandle() { return (CADT_FreeListHandle)this; }

		[[nodiscard]] void* alloc() { return CADT_FreeListAlloc((CADT_FreeListHandle) this); };
		void release(void* ptr) { return CADT_FreeListRelease((CADT_FreeListHandle) this, ptr); };

		BaseFreeList() = delete;
		~BaseFreeList() = delete;
	};

	template<typename T, bool ispod = std::is_pod<T>()> struct FreeList;

	template<typename T>
	struct FreeList<T, true> : private BaseFreeList {
		[[nodiscard]] static auto Create(size_t capacity) -> FreeList* {
			return (FreeList*) BaseFreeList::Create<T>(capacity);
		}

		[[nodiscard]] static auto From(CADT_FreeListHandle fl) ->FreeList* {
				ASSERT(CADT_FreeListElementSize(fl) == sizeof(T));
				return (FreeList*) BaseFreeList::From(fl);
		}

		static auto Destroy(FreeList* v) -> void {
			BaseFreeList::Destroy(v);
		}

		[[nodiscard]] static auto Clone(FreeList const* src) -> FreeList* {
				return (FreeList*) BaseFreeList::Clone(src);
		}

		operator CADT_FreeListHandle() { return (CADT_FreeListHandle)this; }

		auto clone() { return Clone(this); }
		auto destroy() { Destroy(this); }

		[[nodiscard]] T* alloc() { return (T*)BaseFreeList::alloc(); };
		void release(T* ptr) { return BaseFreeList::release(ptr); };

		FreeList() = delete;
		~FreeList() = delete;
	};
};