s/#define UINT64 unsigned __int64//
s/UINT64/std::uint64_t/g
s/typedef unsigned int UINT4;//
s/UINT4/std::uint32_t/g 

s/(\(|\*|^|,|<|[[:space:]]|\*\/)N?L?PCT?(STR|CH|CHAR)/\1const char */g
s/(\(|\*|^|,|<|[[:space:]]|\*\/)N?L?PT?(STR|CH|CHAR)/\1char */g
s/(\(|\*|^|,|<|[[:space:]]|\*\/)L?CPBYTE/\1const std::uint8_t */g
s/(\(|\*|^|,|<|[[:space:]]|\*\/)L?PBYTE/\1std::uint8_t */g
s/(\(|\*|^|,|<|[[:space:]]|\*\/)TCHAR/\1char/g

s/NULL/nullptr/g
s/TRUE/true/g
s/FALSE/false/g

s/(\(|\*|^|,|<|[[:space:]]|\*\/)LPDWORD/\1std::uint32_t */g
s/(\(|\*|^|,|<|[[:space:]]|\*\/)LPWORD/\1std::int32_t */g
s/(\(|\*|^|,|<|[[:space:]]|\*\/)DWORD/\1std::uint32_t/g
s/(\(|\*|^|,|<|[[:space:]]|\*\/)WORD/\1std::uint16_t/g
s/(\(|\*|^|,|<|[[:space:]]|\*\/)DBYTE/\1std::uint8_t */g
s/(\(|\*|^|,|<|[[:space:]]|\*\/)BYTE/\1std::uint8_t/g
s/(\(|\*|^|,|<|[[:space:]]|\*\/)TCHAR/\1std::uint8_t/g

s/(\(|\*|^|,|<|[[:space:]]|\*\/|[ULP]|LP)BOOL/\1bool/g
s/(\(|\*|^|,|<|[[:space:]]|\*\/|[ULP]|LP)FLOAT/\1float/g
s/(\(|\*|^|,|<|[[:space:]]|\*\/|[ULP]|LP)U(INT|int)16/\1std::uint16_t/g
s/(\(|\*|^|,|<|[[:space:]]|\*\/|[ULP]|LP)U(INT|int)32/\1std::uint32_t/g
s/(\(|\*|^|,|<|[[:space:]]|\*\/|[ULP]|LP)U(INT|int)8/\1std::uint8_t/g
s/(\(|\*|^|,|<|[[:space:]]|\*\/|[ULP]|LP)(INT|int)16/\1std::int16_t/g
s/(\(|\*|^|,|<|[[:space:]]|\*\/|[ULP]|LP)(INT|int)32/\1std::int32_t/g
s/(\(|\*|^|,|<|[[:space:]]|\*\/|[ULP]|LP)(INT|int)8/\1std::int8_t/g
s/(\(|\*|^|,|<|[[:space:]]|\*\/|[ULP]|LP)INT/\1int/g
s/(\(|\*|^|,|<|[[:space:]]|\*\/|[ULP]|LP)LONG/\1long/g
s/(\(|\*|^|,|<|[[:space:]]|\*\/|[ULP]|LP)SHORT/\1short/g
s/(\(|\*|^|,|<|[[:space:]]|\*\/|[ULP]|LP)VOID/\1void/g

s/(\(|\*|^|,|<|[[:space:]]|\*\/)L?P(std|int|long|short|void|char)/\1\2 */g
s/(\(|\*|^|,|<|[[:space:]]|\*\/|U)L(std|int|long|short|void|char)/\1long \2/g
s/(\(|\*|^|,|<|[[:space:]]|\*\/)U(std|int|long|short|void|char)/\1unsigned \2/g

s/(\(|\*|^|,|<|[[:space:]]|\*\/)PSZ/\1char */g

s/wincompat.hpp/types.hpp/

s/unsigned[[:space:]]__int64/std::uint64_t/g
s/unsigned[[:space:]]__int32/std::uint32_t/g
s/unsigned[[:space:]]__int16/std::uint16_t/g
s/unsigned[[:space:]]__int8/std::uint8_t/g

s/MAKEWORD/make_word/g
s/HIBYTE/high_part/g
s/LOBYTE/low_part/g

s/unsigned std::int/std::uint/g
s/GetTickCount/SDL_GetTicks/g
s/MoveMemory/memmove/g
s/FillMemory/memset/g
s/EqualMemory/memcmp/g
s/CopyMemory/memcpy/g
s/ZeroMemory\(([^,]+),/memset(\1,0,/g
s/LPOVERLAPPED/OVERLAPPED */g
s/LPCVOID/const void */g
s/_tcs([a-z])/str\1/g
s/TEXT\("([^"]*?)"\)/"\1"/g
s/TEXT\(([^)]+)\)/\1/g
s/_ASSERT/assert/g

# this is a revert back
s/intR/INTR/g
