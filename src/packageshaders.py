import os

print("running shader package script")

f = open("shaders.h", "w")
f.write("#include <string>\n")
f.write("using std::string;\n\n")

for filename in os.listdir(os.getcwd()):
	filenameSplit = filename.split(".")
	if len(filenameSplit) > 1 and filenameSplit[1] == "glsl":
		print(filename)
		s = open(filename, "r");
		#f.write('const GLchar* ' + filenameSplit[0] + '_source = \n')
		f.write('string ' + filenameSplit[0] + '_source = \n')
		i = 0;
		shaderLines = s.readlines();
		for line in shaderLines:
			line = line[:len(line)-1] #chop off newline character
			if len(line) > 0: #omit empty lines
				f.write('\t')
				f.write('"' + line) 
				if i == 0:
					f.write('\\n');
				f.write('"')
				if i < len(shaderLines) - 2:
					f.write('\n')
			i = i + 1
		f.write(';\n\n')
		s.close()

f.close()