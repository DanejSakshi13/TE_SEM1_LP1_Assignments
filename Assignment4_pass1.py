import nltk
# you will have to download nltk library before running this file
# using comand = pip instal nltk

nltk.download('punkt')

# Read the file content and split it into lines
file_content = open("asm.txt").readlines()
intermediateCodeFile = open("intermediate.txt","w")

# Initialize an empty list to store tokens
tokens = []

# Iterate through each line in the file
for line in file_content:
    # Tokenize each line into words
    line_tokens = nltk.word_tokenize(line)
    # Add the words from this line to the list of tokens
    tokens.extend(line_tokens)
    # Add '\n' to indicate a new line
    tokens.append('\n')

for i in range(len(tokens)):
    tokens[i] = tokens[i].upper()

#Declaring mot,pot,dl,reg
mot = ["MOVER","MOVEM","ADD","SUB","MULT","DIV","BC","COMP","PRINT","READ"]
pot = ["START","END","EQU","ORIGIN","LTROG"]
dl = ["DS","DC"]
reg = ["R1","R2","R3","R4","R5"]


#literle table
literals = []
litAdr = []

#Symbol table
symbols = []
symAdr = []

baseAdr = int(tokens[1]) 
currentlocation = baseAdr - 1 #keep the address of current instruction

ltrogCount = 0 #This variable keep the count of unaddresed literals
previous = tokens[0] #It contains previous token

#function to check if the given string is integer
def is_integer(s):
    try:
        int(s)
        return True
    except ValueError:
        return False

comand = ""

print("\nIntermediate code : ")
#Initializing literal and symbole table and generating intermediate code
for i in tokens:
	#Here i is a token(eg. =3') in the array of tokens(=['START','200',.....])
	
	
	if('=' in i): #if the token contains i in it then it will execute (eg =3')
		literals.append(i)
		ltrogCount = ltrogCount + 1   


	if('DS' in i or 'DC' in i or ':' in i): # ':' is for label
		symbols.append(previous)
		symAdr.append(currentlocation) 

	if('ORIGIN' in previous):
		currentlocation = int(i) 

	if('\n' in i):
		currentlocation = currentlocation + 1

	if('LTROG' in i):
		litIndex = len(litAdr)
		for j in range(ltrogCount): #This for loop will execute for ltrogCount times where ltrogCount is total number of un initialized variables
			comand = str(currentlocation) + ")"
			litAdr.append(currentlocation) #adding address to literals
			currentlocation = currentlocation + 1
			tempLiteral = ""
			for h in literals[litIndex]:
				if(h == "="):
					tempLiteral = ""
				else :
					tempLiteral = tempLiteral + h

			literals[litIndex] = tempLiteral
			comand = comand + "(AD,5) _ " + tempLiteral
			print(comand)
			comand = comand + "\n"
			intermediateCodeFile.write(comand) #Writing the code in the txt file
			comand = ""
			litIndex = litIndex + 1
			
		ltrogCount = 0

	#intermediate code	
	if(is_integer(i)):
		comand = comand + "(C," + i + ")"

	if(i in pot):
		if(i != "LTROG"):
			index = pot.index(i) + 1
			comand = comand + "(AD," + str(index) + ")"

	if(i in reg):
		comand = comand + i    

	if(i in mot):
		index = mot.index(i) + 1
		comand = comand + "(IS," + str(index) + ")"

	if(i in dl):
		index = dl.index(i) + 1
		comand = comand + "(DL," + str(index) + ")"

	if(i in symbols):
		index = symbols.index(i) + 1
		comand = comand + "(S," + str(index) + ")"

	if(i in literals):
		index = literals.index(i) + 1
		comand = comand + "(L," + str(index) + ")"

	if('\n' in i):
		print(comand)
		comand = comand + "\n"
		intermediateCodeFile.write(comand)	#Writing the code in the txt file
		comand = ""
		comand = comand + str(currentlocation) + ")"

	previous = i


intermediateCodeFile.write("end of code\n")
for i in literals:
	intermediateCodeFile.write(i + " ") #Writing literals in the txt file
intermediateCodeFile.write("\n") #For new line

for i in litAdr:
	intermediateCodeFile.write(str(i) + " ")  #Writing literal address in the txt file
intermediateCodeFile.write("\n")

for i in symbols:
	intermediateCodeFile.write(i + " ")
intermediateCodeFile.write("\n")

for i in symAdr:
	intermediateCodeFile.write(str(i) + " ")
intermediateCodeFile.write("\n")

print("\nLiteral table : ")   
print(literals)
print(litAdr)
print("\nSymbol Table : ")
print(symbols)
print(symAdr) 

intermediateCodeFile.close()
