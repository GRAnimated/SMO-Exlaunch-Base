// File consisting of bad macros to make quicker hooks

#include "hook/trampoline_hook.hpp"

/* QUICK_TOGGLE
    Quick_TOGGLE(HookName, bool)
    Disables a function if a boolean is true, otherwise keep running */
#define Quick_TOGGLE(HookName, toggle)          \
    HOOK_DEFINE_TRAMPOLINE(HookName) {          \
        static void Callback(void* original) {  \
            if (toggle) return;                 \
            Orig(original);                     \
        }                                       \ 
    };

#define Quick_TOGGLE_install(HookName, Symbol)  \
    HookName::InstallAtSymbol(Symbol);

/* Quick_RET
    Quick_RET(HookName, 0xAAAAAAAA)
    Returns at a specified ASM location */
#define Quick_RET(HookName, HookLoc)            \
    patch::CodePatcher HookName(HookLoc);       \
    p.Write(0xD65F03C0); /* RET instruction*/   \

