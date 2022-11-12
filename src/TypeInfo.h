#include <cstdint>
#include <type_traits>

class TypeInfo {
    virtual void dummy() { }

    const char* const mName;
    const union {
        const TypeInfo* mFirstBase;
        const struct {
            const unsigned int mFlags;
            const unsigned int mBaseCount;
        };
    };

public:
    TypeInfo() = delete;
    TypeInfo(TypeInfo&) = delete;
    TypeInfo(TypeInfo&&) = delete;

    struct BaseEntry {
        const TypeInfo* entry;
        const void* offsetFlags;
    };

    struct ConstIterator {
        constexpr ConstIterator(const BaseEntry* ptr)
            : mPtr(ptr)
        {
        }

        const TypeInfo* operator*() { return mPtr->entry; }

        const BaseEntry* getEntry() { return mPtr; }
        const TypeInfo* getTypeInfo() { return mPtr->entry; }

        ConstIterator& operator++()
        {
            mPtr++;
            return *this;
        }

        friend bool operator==(const ConstIterator& a, const ConstIterator& b) { return a.mPtr == b.mPtr; };
        friend bool operator!=(const ConstIterator& a, const ConstIterator& b) { return a.mPtr != b.mPtr; };

    private:
        const BaseEntry* mPtr;
    };

    bool isMultipleEntry() const
    {
        return mFlags < 255;
    }

    bool isSingleEntry() const { return !isMultipleEntry(); }

    ConstIterator begin() const
    {
        if (isSingleEntry())
            return ConstIterator(reinterpret_cast<const BaseEntry*>(&mFirstBase));
        return ConstIterator(reinterpret_cast<const BaseEntry*>(reinterpret_cast<const uint8_t*>(this) + sizeof(*this)));
    }

    ConstIterator end() const
    {
        if (isSingleEntry())
            return ConstIterator(reinterpret_cast<const BaseEntry*>(&mFirstBase));
        return ConstIterator(reinterpret_cast<const BaseEntry*>(reinterpret_cast<const uint8_t*>(this) + sizeof(*this) + sizeof(BaseEntry) * mBaseCount));
    }

    const TypeInfo* getBase() const { return begin().getTypeInfo(); }
    unsigned int getBaseCount() const { return isMultipleEntry() ? mBaseCount : 1; }

    bool isDirectBase(const TypeInfo* base) const
    {
        for (const TypeInfo* info : *this)
            if (info == base)
                return true;
        return false;
    }

    const char* getName() const { return mName; }

    template <typename T>
    static const TypeInfo* getClassTypeInfo(const T* instance)
    {
        static_assert(std::is_polymorphic<T>::value, "class must have a vtable");

        struct vtable {
            virtual void dummy() { }

            const TypeInfo* info;
        };

        return reinterpret_cast<const vtable*>(*(const uint8_t**)instance - sizeof(vtable))->info;
    }

    static const TypeInfo* getClassTypeInfoFromVtable(const void* vtablePtr)
    {
        struct vtable {
            virtual void dummy() { }

            const TypeInfo* info;
        };

        return reinterpret_cast<const vtable*>(vtablePtr)->info;
    }
};
