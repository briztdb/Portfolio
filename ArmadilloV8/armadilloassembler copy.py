#Max Brizzio
# I pledge my honor that I have abided by the Stevens Honor System.
import json
import random

def getInstructions(): # Function to comb text file, grab all instructions (assuming syntax is correct) and generate machine code
    instructions = []
    print("Type your code below. If you are done with inputting instructions, input 'exit' on it's own line: ")
    lineN = 1 # Counter to display line number
    while(True): # Loop to fetch all instructions inputted until exit is inputted, all instructions are saved to a list
        curr = input("{}{}".format(lineN, ": \t"))
        if(curr.lower() == 'exit'):
            break
        instructions.append(curr)
        lineN += 1
    print(instructions)
    instructionsF = [] # List to hold binary encoded instructions
    
    for instruction in instructions:
        uncombed = instruction.split() #split each instruction by their spaces to parse mnemonics and each argument
        destReg = uncombed[0] # Grab destination register
        operation = uncombed[1] # Grab instruction mnemonic
        if(operation == 'subtract'): # Convert to binary encoding based on mnemonic given
            source1 = uncombed[2]
            source2 = uncombed[3]
            instructionsF.append(generateOpcode(destReg, operation, source1, source2))
        if(operation == 'add'):
            source1 = uncombed[2]
            source2 = uncombed[3]
            instructionsF.append(generateOpcode(destReg, operation, source1, source2))
        if(operation == 'please'):
            source1 = uncombed[2]
            instructionsF.append(generateOpcode(destReg, operation, source1, source1))
    return instructionsF

def generateOpcode(destReg, operation, source1, source2): #Function where given operands and mnemonic, corresponding opcode will be generated
    instruction = [] #create string of 18-bits all 0 to start
    for i in range(18): # Fill instruction with 0's
        instruction.append('0')
    instruction[15] = '1' # All instructions have a 1 at the 14th bit
    if(operation != "please"): # If not loading, use ALU
        instruction[17] = '1' 
        if(operation == "add"):
            instruction[16] = '1'
        if(destReg == 'earth'):
            instruction[13] = '0'
            instruction[14] = '0'
        if(destReg == 'wind'):
            instruction[13] = '0'
            instruction[14] = '1'
        if(destReg == 'fire'):
            instruction[13] = '1'
            instruction[14] = '0'
        if(destReg == 'water'):
            instruction[13] = '1'
            instruction[14] = '1'
        if(source1 == 'earth'):
            instruction[11] = '0'
            instruction[12] = '0'
        if(source1 == 'wind'):
            instruction[11] = '0'
            instruction[12] = '1'
        if(source1 == 'fire'):
            instruction[11] = '1'
            instruction[12] = '0'
        if(source1 == 'water'):
            instruction[11] = '1'
            instruction[12] = '1'
        if(source2 == 'earth'):
            instruction[9] = '0'
            instruction[10] = '0'
        if(source2 == 'wind'):
            instruction[9] = '0'
            instruction[10] = '1'
        if(source2 == 'fire'):
            instruction[9] = '1'
            instruction[10] = '0'
        if(source2 == 'water'):
            instruction[9] = '1'
            instruction[10] = '1'
    else: # Else if loading, binary encoding is different 
        instruction[0] = '1'
        i = 1
        for char in source1:
            instruction[i] = char
            i += 1
        if(destReg == 'earth'):
            instruction[13] = '0'
            instruction[14] = '0'
        if(destReg == 'wind'):
            instruction[13] = '0'
            instruction[14] = '1'
        if(destReg == 'fire'):
            instruction[13] = '1'
            instruction[14] = '0'
        if(destReg == 'water'):
            instruction[13] = '1'
            instruction[14] = '1'
    string = ''.join(instruction) # Put binary encoding into a string
    num = int(string, 2) #Convert string to binary number
    instruction = hex(num) # Convert binary number to hexadecimal
    return instruction

def getJSON(filename): # Function to retrieve instructions from JSON file given filename
    with open("{}{}".format(filename, ".json"), "r") as jfile:
        data = json.load(jfile)
    instructionsF = []
    for instruction in data:
        uncombed = instruction.split() #split each instruction by their spaces to parse mnemonics and each argument
        destReg = uncombed[0] # Grab destination register
        operation = uncombed[1] # Grab instruction mnemonic
        if(operation == 'subtract'): # Convert to binary encoding based on mnemonic given
            source1 = uncombed[2]
            source2 = uncombed[3]
            instructionsF.append(generateOpcode(destReg, operation, source1, source2))
        if(operation == 'add'):
            source1 = uncombed[2]
            source2 = uncombed[3]
            instructionsF.append(generateOpcode(destReg, operation, source1, source2))
        if(operation == 'please'):
            source1 = uncombed[2]
            instructionsF.append(generateOpcode(destReg, operation, source1, source1))
    return instructionsF

def generateInstructionImage(instructions): #Function to generate image files to be loaded into Logisim-Evolution (instructional memory)
    file = open("InstructionImageFile.txt", "w") # Open file "imagefile.txt" to write to or create if it does not exist
    file.write("v3.0 hex words addressed\n") # First line in image file
    lineNumber = 0 # counter to track line number
    instructionNumber = 0 # Counter to track how many instructions left
    for line in range(32): # For each line
        hexLineNumber = hex(lineNumber)[2::] # Formatted line number
        file.write("{}{}".format(hexLineNumber.zfill(2), ":"))
        for hexString in range(8):
            if instructionNumber >= len(instructions): # if there are no more instructions, fill rest of file with 00000
                file.write(" 00000")
            else:
                instructionF = instructions[instructionNumber][2::].zfill(5) #Format instruction to exclude 0x and fill to 5 digits
                file.write("{}{}".format(" ", instructionF))
                instructionNumber += 1
        file.write("\n")
        lineNumber += 8

def generateDataImage():
    file = open("DataImageFile.txt", "w")
    file.write("v3.0 hex words addressed\n")
    lineNumber = 0
    for line in range(16):
        hexLineNumber = hex(lineNumber)[2::]
        file.write("{}{}".format(hexLineNumber.zfill(2), ":"))
        for hexString in range(16):
            randomNumber = hex(random.randint(0, 255))[2::].zfill(2) #generate random hex number with size 2
            file.write("{}{}".format(" ", randomNumber))
        file.write("\n")
        lineNumber += 8
            
def doEverything(json, filename):
    if json:
        instructions = getJSON(filename)
        generateInstructionImage(instructions)
        generateDataImage()
    else:
        instructions = getInstructions()
        generateInstructionImage(instructions)
        generateDataImage()

doEverything(True, "sampleProgram")
