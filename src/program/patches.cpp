#include "patches.hpp"

namespace patch = exl::patch;
namespace inst = exl::armv8::inst;
namespace reg = exl::armv8::reg;

void costumeRoomPatches() {
    patch::CodePatcher p(0x262850);
    p.WriteInst(inst::Movz(reg::W0, 0));
    p.Seek(0x2609B4);
    p.WriteInst(inst::Movz(reg::W0, 0));

    p.Seek(0x25FF74);
    p.WriteInst(inst::Movz(reg::W0, 1));
    p.Seek(0x25FF74);
    p.WriteInst(inst::Movz(reg::W0, 0));
}

void stubSocketInit() {
    patch::CodePatcher p(0x95C498);
    p.WriteInst(inst::Nop());
}

void disableDepthShadow() {
    patch::CodePatcher p(0xA6E5E4);
    p.Write(0xD65F03C0); /* RET instruction*/
}

void runCodePatches() {
    costumeRoomPatches();
    stubSocketInit();
    //disableDepthShadow();

    //patch::CodePatcher d(0x87F180);
    //d.WriteInst(inst::Nop());

    //patch::CodePatcher o(0x87F71C);
    //o.Write(0xD2800019); /* MOV X25, 0*/

    //patch::CodePatcher o(0x009B22A8);
    //o.WriteInst(inst::Nop());
    
}