s/INT([0-9]+)/std::int\\1_t/g;

s/LPOVERLAPPED/OVERLAPPED */g;

s/LPCVOID/const void */g;

s/wincompat.hpp/types.hpp/g;

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

s/_tcs([a-z])/str\\1/g;

s/TEXT\\("([^"]*?)"\\)/"\\1"/g;

s/TEXT\\(([^)]+)\\)/\\1/g;

s/_ASSERT/assert/g;

