# Max Brizzio
# I pledge my honor that I have abided by the Stevens Honor System.
import hashlib
import secrets

def keyGeneration(): # Generates list of 512 key-pairs kept in tuple
    secretKeys = ([], [])
    publicKeys = ([], [])
    for i in range(0, 256):
        #Create 2 256-bit tokens using secrets library... use both to fill up list of secret keys
        token0 = secrets.token_bytes(32)
        token1 = secrets.token_bytes(32)
        secretKeys[0].append(token0)
        secretKeys[1].append(token1)
        # Hash each secret key and put in list corresponding to its original token
        publicKeys[0].append(hashlib.sha256(token0).hexdigest())
        publicKeys[1].append(hashlib.sha256(token1).hexdigest())
    return (secretKeys, publicKeys)
    
def signatureGeneration(digest, keys): # Sign message using hash of original message and list of 256 key-pairs (Lamport's Method)
    #Convert digest to binary for Lamport's Method
    bin_digest = bin(int(digest, base=16))
    # Convert binary num to string so we can iterate through easily
    strdig = str(bin_digest)
    strdig = strdig[2::]
    signature = []
    # Pad binary number with 0's until it is atleast 256-bits
    strdig = strdig.zfill(256)
    for i in range(256):
        if(strdig[i] == '0'): # If current bit is a 0, append corresponding key to signature (token from 0 list) if not append key from 1 list
            signature.append(keys[0][0][i])
        else:
            signature.append(keys[0][1][i])
    return signature

def signatureVerification(m, signature, keys): # Verify signature
    #Convert message into hashed binary string of 256-bits
    digest = hashlib.sha256(m.encode("utf-8")).hexdigest()
    bin_digest = bin(int(digest, base=16)).zfill(8)
    strdig = str(bin_digest)[2::].zfill(256)
    print("{}{}".format("\n\nmessage digest (padded to 256 characters): ", strdig))
    print()
    for i in range(len(strdig)):
        currBit = strdig[i]
        if(currBit == '0'): #If ith bit in digest is 0, check if ith public key is equal to hashed ith signature block
            if(hashlib.sha256(signature[i]).hexdigest() != keys[1][0][i]):
                print("Signature could not be verified...\n")
                return False
        else:
            if(hashlib.sha256(signature[i]).hexdigest() != keys[1][1][i]):
                print("Signature could not be verified...\n")
                return False

    print("Signature verified successfully\n")
    return True

m = input("Input a message you would like to sign: ")
digest = hashlib.sha256(m.encode("utf-8")).hexdigest()
bin_digest = bin(int(digest, base=16)).zfill(8)
strdig = str(bin_digest)[2::]
print("{}{}".format("\n\nmessage digest (not padded to 256 characters): ", strdig))
keys = keyGeneration()
sig = signatureGeneration(digest, keys)
signatureVerification(m, sig, keys)

