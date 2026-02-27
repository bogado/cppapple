
s/GetTickCount/SDL_GetTicks/g;

s/ZeroMemory\\(([^,]+),/memset(\\1,0,/g;
s/MoveMemory/memmove/g;
s/EqualMemory/memcmp/g;
s/CopyMemory/memcpy/g;
s/FillMemory/memset/g;

s/ZeroMemory\\(([^,]+),/memset(\\1,0,/g;
s/MoveMemory/memmove/g;
s/EqualMemory/memcmp/g;
s/CopyMemory/memcpy/g;
s/FillMemory/memset/g;

s/wincompat.hpp/types.hpp/g;
s/MAKEWORD/make_word/g;
s/HIBYTE/high_part/g;
s/LOBYTE/low_part/g;
s/_tcs([a-z])/str\\1/g;

s/_ASSERT/assert/g;

