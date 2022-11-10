#pragma once
#include "DebugMenu.h"

/*
#define Category_Impl(name, index, amountOfPages)   \
auto name = new dc::name(index, #name);             \
name->pages.tryAllocBuffer(amountOfPages, nullptr); \
categories.pushBack(name);                         \

#define Page_Impl(category, name, index)                        \
category->pages.pushBack(new dp::category##name(index, #name)); \


#define Category_Header(name) \
namespace dc { class name : public smol::Category { \
    public:                                         \
    using smol::Category::Category; \
};}                                                 \

#define Page_Header(category, name) \
namespace dp { class category##name : public smol::Page { \
    public: \
    using smol::Page::Page; \
    virtual void draw() override; \
};} \
*/

#define CreateCategory(index, maxPages, name) \
auto name = new smol::Category(index, #name); \
name->pages.tryAllocBuffer(maxPages, nullptr); \
categories.pushBack(name); \

#define page(index, category, name, type) \
category->pages.pushBack(new type(index, #name)); \

sead::PtrArray<smol::Category> CreatePages();






namespace p {
    class Template : public smol::Page {
        public:
        using smol::Page::Page;
        void draw();
        

    };

    class InfoInfo : public smol::Page {
        public:
        using smol::Page::Page;
        void draw();
        
    };

    class InfoStats : public smol::Page {
        public:
        using smol::Page::Page;
        void draw();
        
    };

    class WorldActors : public smol::Page {
        public:
        using smol::Page::Page;
        void draw();
        void drawActorInfo(al::LiveActor *actor);
    };
}