from deep_translator import GoogleTranslator


filename = "temp.py"
tempname = "main.py.temp"

fout = open(tempname, "w", encoding="utf-8", newline="\n")
def write(line):
    if '\n' in line:
        fout.write(line)
    else:
        fout.write(line + '\n')



fin = open(filename, "r", encoding="utf8", newline="\n")
lines = fin.readlines()


nulpo = 0
for line in lines:
    if line == None:
        pass

    bicom = line.find("\"\"\"")
    if bicom != -1:
        if nulpo:
            nulpo = 0
        else:
            nulpo = 1

    if nulpo:
        line = GoogleTranslator(source='auto', target='ru').translate(line[bicom : len(line)])
        if line != None:
            write(line)
        continue




    comment = line.find('#')
    if comment != -1:

        pos = line.rfind("\"", comment)
        if pos != -1:
            if line != None:
                write(line)
            pass
        pos = line.rfind("'", comment)
        if pos != -1:
            if line != None:
                write(line)
            pass
        
        
        

        else:    
            lt = line[0:comment]
            line = GoogleTranslator(source='auto', target='ru').translate(line[comment:len(line)])
            if line != None:
                write(lt + line)

                
    else:
        if line != None:
            write(line)

fin.close()
fout.close()
import os
os.rename(tempname, "filename.py")
