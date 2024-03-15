#ifndef GUIVERSION
#define GUIVERSION

#define GUI_VERSION_STR "8.1.1"
/*
   GUI_VERSION is (major << 16) + (minor << 8) + patch.
*/
#define GUI_VERSION 0x080101
/*
   can be used like #if (GUI_VERSION >= GUI_VERSION_CHECK(1, 0, 1))
*/
#define GUI_VERSION_CHECK(major, minor, patch) ((major<<16)|(minor<<8)|(patch))

#endif // GUIVERSION

