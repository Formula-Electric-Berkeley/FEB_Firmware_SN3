import ctypes

# a = ctypes.c_uint8(0xFF)
# b = ctypes.c_uint8(0x9C)
c = ctypes.c_int16(-100)          # -100
d = ctypes.c_int16(0)             # -100

arr = (ctypes.c_uint8 * 2)()
dest = arr
src = ctypes.pointer(c)
ctypes.memmove(dest, src, 2 * ctypes.sizeof(ctypes.c_uint8))
print(arr, [x for x in arr])

dest = ctypes.pointer(d)
src = arr
ctypes.memmove(dest, src, 2 * ctypes.sizeof(ctypes.c_uint8))
print(dest, [x for x in src], d)

# arr = [b, a]
# arr = (ctypes.c_uint8 * len(arr))(*arr)
# dest = ctypes.pointer(c)
# src = arr
# ctypes.memmove(dest, src, 2 * ctypes.sizeof(ctypes.c_uint8))
# print(c)
# print(arr[0], arr[1])

# dest = ctypes.pointer(c)
# src = ctypes.pointer(d)
# ctypes.memmove(dest, src, 2 * ctypes.sizeof(ctypes.c_uint8))
# print(c)