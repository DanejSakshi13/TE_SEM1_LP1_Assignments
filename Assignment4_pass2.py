import nltk
# you will have to download nltk library before running this file
# using comand = pip instal nltk

nltk.download('punkt')
intermediateCodeFile = open("intermediate.txt").readlines()
machineCode = open("machine.txt","w")

# Initialize an empty list to store tokens
tokens = []

tempIndex = 0
# Iterate through each line in the file till we encounter line 'end of code\n' because after it we have written literals, literal address, symbols ans symbol address 
for line in intermediateCodeFile: 
    if line == "end of code\n":
        tempIndex = intermediateCodeFile.index(line)
        tempIndex = tempIndex + 1
        break

    # Tokenize each line into words
    line_tokens = nltk.word_tokenize(line)
    # Add the words from this line to the list of tokens
    tokens.extend(line_tokens)
    # Add '\n' to indicate a new line
    tokens.append('\n')

literals = []
line = intermediateCodeFile[tempIndex]
tempIndex = tempIndex + 1
literals = nltk.word_tokenize(line)
print(literals)

litAdr = []
line = intermediateCodeFile[tempIndex]
tempIndex = tempIndex + 1
litAdr = nltk.word_tokenize(line)
print(litAdr)

symbols = []
line = intermediateCodeFile[tempIndex]
tempIndex = tempIndex + 1
symbols = nltk.word_tokenize(line)
print(symbols)

symAdr = []
line = intermediateCodeFile[tempIndex]
tempIndex = tempIndex + 1
symAdr = nltk.word_tokenize(line)
print(symAdr)

for i in range(len(tokens)):
    tokens[i] = tokens[i].upper()

print("\nintermediate tokens ")
print(tokens)

def is_integer(s):
    try:
        int(s)
        return True
    except ValueError:
        return False


reg = ["R1","R2","R3","R4","R5"]
comand = ""
previous = tokens[0]
bracketCount = 0

# Similar concept is applied as previous one
for i in tokens:
    if i == ")" and bracketCount == 0:
        bracketCount = bracketCount + 1
        if is_integer(previous):
            comand = comand + previous + ") "
        continue

    
    if i == ")" and bracketCount == 1:
        bracketCount = bracketCount + 1

    if i == "(" and bracketCount == 2:
        if previous in reg :
            comand = comand + str((reg.index(previous) + 1)) + " "
        else :
            comand = comand + "_ "

    if bracketCount == 3:
        bracketCount = 0

    if "AD" in i or "IS" in i:
        temp = ""
        for j in i:
            temp = temp + j
            if j == ",":
                temp = ""
        
        if "AD" in i and temp == "5":
            comand = comand + "_ "
        else :
            comand = comand + temp + " "
        
        continue

    if is_integer(i) and "_" in previous:
        comand = comand + "_ " + i + " "

    if "S" in i or "L" in i:
        temp = ""
        for j in i:
            temp = temp + j
            if j == ",":
                temp = ""
        
        if "S" in i:
            tempVar = int(litAdr[int(temp) - 1]) + 1
            comand = comand + str(tempVar) + " "

        if "L" in i:
            tempVar = int(litAdr[int(temp) - 1]) + 1
            comand = comand + str(tempVar) + " "

    if "C" in i:
        temp = ""
        for j in i:
            temp = temp + j
            if j == ",":
                temp = ""
        
        comand = comand + temp + " "
    
    if i == "\n":
        if bracketCount == 2:
            if previous in reg:
                comand = comand + str(reg.index(previous) + 1)
        bracketCount = 0
        print(comand)
        comand = comand + "\n"
        machineCode.write(comand)
        comand = ""

    previous = i