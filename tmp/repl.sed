s/typedef .*(ULONG)[^;]*;//;
s/ULONG/unsigned long/g;
s/typedef .*(PLONG)[^;]*;//;
s/PLONG/long */g;
s/typedef .*(PULONG)[^;]*;//;
s/PULONG/unsigned long */g;
s/typedef .*(LONG)[^;]*;//;
s/LONG/long/g;

s/typedef .*(USHORT)[^;]*;//;
s/USHORT/unsigned short/g;
s/typedef .*(PSHORT)[^;]*;//;
s/USHORT/short */g;
s/typedef .*(PUSHORT)[^;]*;//;
s/PUSHORT/unsigned short */g;
s/typedef .*(SHORT)[^;]*;//;
s/SHORT/short/g;

s/typedef .*(void)[^;]*;//;
s/(\\(|\\*|^|,|<|[[:space:]]|\\*\\/\\)|[ULP]|LP)VOID/\\1void/g;

s/wincompat.hpp/types.hpp/g;

s/typedef L?P?BOOL [^;]*;//;
s/(\\(|\\*|^|,|<|[[:space:]]|\\*\\/\\)BOOL\\1bool/g;
s/(\\(|\\*|^|,|<|[[:space:]]|\\*\\/\\)L?PBOOL\\1bool */g;

s/unsigned[[:space:]]__int64/std::uint64_t/g;

s/unsigned[[:space:]]__int32/std::uint32_t/g;

s/unsigned[[:space:]]__int16/std::uint16_t/g;

s/unsigned[[:space:]]__int8/std::uint8_t/g;

s/MAKEWORD/make_word/g;

s/HIBYTE/high_part/g;

s/LOBYTE/low_part/g;

s/unsigned std::int/std::uint/g;

s/GetTickCount/SDL_GetTicks/g;

s/MoveMemory/memmove/g;

s/FillMemory/memset/g;

s/EqualMemory/memcmp/g;

s/CopyMemory/memcpy/g;

s/ZeroMemory\\(([^,]+),/memset(\\1,0,/g;

s/LPOVERLAPPED/OVERLAPPED */g;

s/LPCVOID/const void */g;

s/_tcs([a-z])/str\\1/g;

s/TEXT\\("([^"]*?)"\\)/"\\1"/g;

s/TEXT\\(([^)]+)\\)/\\1/g;

s/_ASSERT/assert/g;

