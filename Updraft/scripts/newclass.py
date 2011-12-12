#!/usr/bin/python2

import argparse
import os
import string
from os import path

parser = argparse.ArgumentParser (
  description = "Creates the class header and source file in the correct format"
)
parser.add_argument('-c', '--classname', type=str)
parser.add_argument('-d', '--directory', type=str)

args = parser.parse_args()

if (args.directory == None):
  print "You should specify a directory with -d flag"
  exit(1)

if (args.classname == None):
  print "You should specify a class name with -c flag"
  exit(1)

if (not os.path.exists(args.directory)):
  try:
    os.mkdir(args.directory)
  except OSError as e:
    print "Could not create directory:"
    print e.message
    exit(1)
elif (not os.path.isdir(args.directory)):
  print "Specifed path exists and is not a directory"
  exit(1)

os.chdir(args.directory)
directory_transl = os.path.normpath(args.directory).upper().translate(string.maketrans("/\\","__"))
guard = "UPDRAFT_"+directory_transl+"_"+args.classname.upper()+"_H_"

hfilename = args.classname.lower()+".h"
cppfilename = args.classname.lower()+".cpp"

hlines = [
  "#ifndef "+guard,
  "#define "+guard,
  "",
  "namespace Updraft {",
  "namespace Core {",
  "",
  "class "+args.classname+" {",
  " public:",
  " private:",
  "};",
  "",
  "}  // End namespace Core",
  "}  // End namespace Updraft",
  "",
  "#endif  // "+guard,
  ""
]
cpplines = [
  "#include \""+hfilename+"\"",
  "",
  "namespace Updraft {",
  "namespace Core {",
  "",
  "}  // End namespace Core",
  "}  // End namespace Updraft",
  ""
]

for i in range(0, len(hlines)):
  hlines[i] = hlines[i]+os.linesep
for i in range(0, len(cpplines)):
  cpplines[i] = cpplines[i]+os.linesep

header_file = None
try:
  header_file = open(hfilename, "w")
except IOError as e:
  print "Could not create header file for writing:"
  print e.message
  exit(1)
header_file.writelines(hlines)
header_file.close()

source_file = None
try:
  source_file = open(cppfilename, "w")
except IOError as e:
  print "Could not create source file for writing:"
  print e.message
  exit(1)
source_file.writelines(cpplines)
source_file.close()

print "Created "+hfilename+" and "+cppfilename+" in directory "+args.directory
