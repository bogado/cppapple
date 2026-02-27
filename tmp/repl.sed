s/TEXT\\("([^"]*?)"\\)/"\\1"/g;

s/TEXT\\(([^)]+)\\)/\\1/g;

s/wincompat.hpp/types.hpp/g;
s/MAKEWORD/make_word/g;
s/HIBYTE/high_part/g;
s/LOBYTE/low_part/g;
s/ZeroMemory\\(([^,]+),/memset(\\1,0,/g;

s/GetTickCount/SDL_GetTicks/g;

s/MoveMemory/memmove/g;

s/FillMemory/memset/g;

s/EqualMemory/memcmp/g;

s/CopyMemory/memcpy/g;

s/_tcs([a-z])/str\\1/g;

s/_ASSERT/assert/g;

