# LR2Mem
Definitions and bindings for major memory structures of Lunatic Rave 2.

You would want to include LR2Bindings.hpp somewhere in your project and call LR2::Init() once. Init() sets a hook on main scene switch loop to acquire the offset to 'game' structure on stack. bool LR2::isInit indicates that the pointer at LR2::pGame is initialized and can be used to access game memory.
