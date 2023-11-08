# Max Brizzio
# I pledge my honor that I have abided by the Stevens Honor System

import hashlib
import random
# from primeGen import *
# from BrizzioMaxLab1 import *
#from BrizzioMaxLab3 import *

def gcd(x, y):
    while(y):
       x, y = y, x % y
    return abs(x)
def euclideanHelper(a, b):
    # Base Case
    if a == 0:
        return (b, 0, 1)
    # tuple to hold data from last recursive call
    info = euclideanHelper(b % a, a)
    # x = y - q * x
    x = info[2] - b // a * info[1]
    # y = x
    y = info[1]

    return (info[0], x, y)

def getInverse(y, a):
    if y < 0:
        return y + a
    return y

def eea(a, b): # Usage: eea(modulo, integer) to find multiplicative inverse 
    info = euclideanHelper(a, b)
    return getInverse(info[2], a)

primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, # Sample primes
                     31, 37, 41, 43, 47, 53, 59, 61, 67,
                     71, 73, 79, 83, 89, 97, 101, 103,
                     107, 109, 113, 127, 131, 137, 139,
                     149, 151, 157, 163, 167, 173, 179,
                     181, 191, 193, 197, 199, 211, 223,
                     227, 229, 233, 239, 241, 251, 257,
                     263, 269, 271, 277, 281, 283, 293,
                     307, 311, 313, 317, 331, 337, 347, 349]
 
def smallPrime(n): # Generate small prime number
    while True:
        num = random.randrange(2**(32-1)+1, 2**32 - 1) #Generate random 32-bit number
 
        for divisor in primes:
            if num % divisor == 0 and divisor**2 <= num: #check if number has reasonable chance to be prime
                break
        return num
 
def isMillerRabinPassed(n): #Check if number is passed by a Miller Rabin primality check
    x = 0 
    ec = n - 1
    while ec % 2 == 0:
        ec >>= 1
        x += 1
    assert(2**x * ec == n - 1)
 
    def notPrime(temp): #Helper function for isMillerRobinPassed
        if pow(temp, ec, n) == 1:
            return False
        for i in range(x):
            if pow(temp, 2**i * ec, n) == n - 1:
                return False
        return True
 
    # Set number of trials here
    count = 20
    for i in range(count):
        temp = random.randrange(2, n)
        if notPrime(temp):
            return False
    return True
def generateKey(): # Generate Key
    #Find primes P and Q in between range(2^31, 2^32)
    p = 0
    q = 0
    isPrimeP = False # Bools to represent if p and q are prime
    isPrimeQ = False
    while(True):
        if(not isPrimeP):
            temp_p = smallPrime(32) #Generate a 32-bit prime number
        if(not isPrimeQ):
            temp_q = smallPrime(32)
        if(isMillerRabinPassed(temp_p)): #Check if P or Q passes Miller Rabin Primality chec
            p = temp_p
            isPrimeP = True
        if(isMillerRabinPassed(temp_q)):
            q = temp_q
            isPrimeQ = True
        if(isPrimeP & isPrimeQ):
            break
    print("{}{}".format("\nPrime P found: ", p))
    print("{}{}".format("\nPrime Q found: ", q))
    # N = P*Q
    n = p * q
    print("{}{}".format("\nN = ", n))
    #Find Euler Totient phi = (p-1)(q-1)
    phi = (p - 1) * (q - 1)
    print("{}{}".format("\nphi = ", phi))
    # Select e randomly s.t. 1 < e < phi and gcd(e, phi) = 1
    e = -1
    while(True):
        rand = random.randrange(2, phi)
        if(gcd(rand, phi) == 1):
            e = rand
            break
    print("{}{}".format("\ne = ", e))
    #Calculate d s.t. e*d = 1(mod phi) using extended euclidean
    d = eea(phi, e)
    print("{}{}".format("\nKey has been generated: d = ", d))    
    return (d, n, e)

def DigSig(N, d): # Given key, input Message M and sign M using key
    m = input("\nEnter a message to send: ")
    #Outputs 64-bit digest
    digest = hashlib.sha256(m.encode("utf-8")).hexdigest()
    #We only need first 15 hexdigits (60 bits)
    digest_15 = ""
    for i in range(15):
        digest_15 += digest[i]
    print("{}{}".format("\nMessage hashed and substring from digest grabbed... first 60 bits of digest = ", digest_15))
    #Generate Digital Signature with digest_15^d modN
    # signature = (int(digest_15, base=16)**d) % N
    signature = pow(int(digest_15, base=16), d, N)
    print("{}{}{}{}{}".format("\nMessage Signed... Sending message '", m, "' and signature '", signature, "' to receiver..."))
    return (m, signature)

def verifySig(m, sig, e, N):
    digest = (hashlib.sha256(m.encode("utf-8")).hexdigest())
    digest_15 = ""
    for i in range(15):
        digest_15 += digest[i]
    message = pow(sig, e, N)
    print("{}{}".format("\n\n\tReceived message hashed: ", hex(message)))
    print("{}{}".format("\tDecryption of received signature: ", hex(int(digest_15, base=16))))
    if(hex(message) == hex(int(digest_15, base=16))):
        print("\n\nSignature verified successfully... ")
    else:
        print("\nSignature could not be verified...")
    
info = generateKey()
key = info[0]
N = info[1]
e = info[2]

info2 = DigSig(N, key)
m = info2[0]
sig = info2[1]
verifySig(m, sig, e, N)



