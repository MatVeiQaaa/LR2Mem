# LR2Mem
Definitions and bindings for major memory structures of Lunatic Rave 2.

You would want to link LR2Mem.lib, which you can build with provided .vcxproj file or acquire from the release, and include LR2Bindings.hpp somewhere in your project and call LR2::Init() once at DllMain to avoid conflicting with other projects. Init() sets a hook on main scene switch loop to acquire the offset to 'game' structure on stack. bool LR2::isInit indicates that the pointers at LR2::pGame and LR2::pSqlite are initialized and can be used to access game memory.
