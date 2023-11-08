#Max Brizzio
#I pledge my honor that I have abided by the Stevens Honor System
import math
from multiprocessing.sharedctypes import Value
import random
import json 
import time
import sys
import itertools
import threading

def isPrime(n):
    # given positive input, return bool describing whether n is prime
    if n==2 or n==3: return True
    if n%2==0 or n<2: return False
    for i in range(3, int(n**0.5)+1, 2):   # only odd numbers
        if n%i==0:
            return False    

    return True

def generateSGP(n):
    #Given limit, generate Sophie Germain Primes in a dictionary to n
    dictSGP = {}
    randSGP = {}
    primes = []
    for i in range(2, n):
        #Generate primes 1 to n, append to array
        if(isPrime(i)):
            primes.append(i)
    for i in primes:
        #Iterate through primes array finding SGP
        p = (2 * i) + 1
        if(isPrime(p)):
            dictSGP[i] = p

    key, val = random.choice(list(dictSGP.items()))
    randSGP = {"p": key, "q": val, "g": 0}
    return (randSGP, dictSGP)

def generator(n):
    s = set(range(1, n))
    results = []
    for a in s:
        g = set()
        for x in s:
            g.add((a**x) % n)
        if g == s:
            return a
    return results


done = False
#here is the animation
def animate():
    for c in itertools.cycle(['|', '/', '-', '']):
        if done:
            break
        sys.stdout.write('\rGenerating generators ' + c)
        sys.stdout.flush()
        time.sleep(0.1)

def animate2():
    for c in itertools.cycle(['|', '/', '-', '']):
        if done:
            break
        sys.stdout.write('\rDumping Data ' + c)
        sys.stdout.flush()
        time.sleep(0.1)
t = threading.Thread(target=animate)
t2 = threading.Thread(target=animate2)

def calculate(p, q, g, mod):
    print("Let's begin the calculations (remember every calculation is done with modP)\n\n")
    time.sleep(.5)
    a = random.randrange(2, q)
    b = random.randrange(2, q)
    print("{}{}{}{}".format("\n\ta = ", a, ",\t b = ", b))
    x = pow(g, a, mod)
    y = pow(g, b, mod)
    time.sleep(.5)
    print("{}{}{}{}".format("\n\tx = ", x, ",\t y = ", y))
    c = pow(x, b, mod)
    d = pow(y, a, mod)
    time.sleep(.5)
    print("{}{}{}{}".format("\n\tx^b = ", c, ",   y^a = ", d))
    if c == d:
        time.sleep(.25)
        print("\nx^b = y^a so now we can send shared key")
        return (a,b,c,d)
    print("Something went wrong...")
    return (0,0,0,0)

def printSGP(d):
    d = dict(d)
    print("{}{}{}{}{}{}".format("P = ", d["p"], ",\tQ = ", d["q"], ",\tg = ", "?"))

n = input("In what range would you like to search for Sophie-Germain primes?: ")

K = 0
try:
    int(n)
    currSGP, sgp = generateSGP(int(n))
    time.sleep(.5)
    print("{}{}{}\n".format("All Sophie-Germain primes from 2 to ", n, ": \n"))
    for key in sgp:
        if(int(n) > 100000):
            break
        else: print("{}{}{}".format(key, " : ", sgp[key]))

    time.sleep(.75)

    print("\nSelecting random Sophie-Germain pair... \n")
    time.sleep(1.25)
    print("\nSophie-Germain pair selected! \n \n\t\t")
    time.sleep(.5)
    printSGP(currSGP)

    t.start()
    currSGP["g"] = generator(currSGP["p"])
    time.sleep(.5)
    done = True
    print("{}{}{}".format("\n\n\tGenerator found! Generator set to: ", currSGP["g"], "\n"))
    public_values = currSGP

    print("Selecting random a and b from 2 to Q...")
    time.sleep(1.5)
    bool = calculate(currSGP["p"], currSGP["q"], currSGP["g"], currSGP["p"])
    if bool == (0,0,0,0):
        print("Something went wrong...")
    else:
        A = bool[0]
        B = bool[1]
        C = bool[2]
        D = bool[3]


    jfile = open("public.json","w")
    json.dump(public_values, jfile)
    time.sleep(1.5)
    done = False

    t2.start()
    time.sleep(1.5)
    done = True
    print("\n\nData dumped to public.json...\n\n")
    time.sleep(1.5)

    with open("public.json", "r") as jfile:
        data = json.load(jfile)
    data = dict(data)
    data["a"] = A
    data["b"] = B
    data["x"] = C
    data["y"] = D

    with open("public.json", "w") as f:
        json.dump(data, f)
    print("Calculations complete: Writing data back to public.json \n\n////////////////////////////(END OF PROCESS)////////////////////////////")
except ValueError:
    print()






