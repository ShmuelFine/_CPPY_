#Python RegEx

#RegEx Module
import re

#RegEx in Python
#Check if the string starts with "The" and ends with "Spain":
txt = "The rain in Spain"
x = re.search("^The.*Spain$", txt)
if (x):
  print("YES! We have a match!")
else:
  print("No match")

#The findall() Function
#Return a list containing every occurrence of "ai":
txt = "The rain in Spain"
x = re.findall("ai", txt)
print(x)
#empty list returned
txt = "The rain in Spain"
#Check if "Portugal" is in the string:
x = re.findall("Portugal", txt)
print(x)
if (x):
  print("Yes, there is at least one match!")
else:
  print("No match")

#The search() Function
txt = "The rain in Spain"
x = re.search("\s", txt)
print("The first white-space character is located in position:", x.start())
#None returned
txt = "The rain in Spain"
x = re.search("Portugal", txt)
print(x)

#The split() Function
txt = "The rain in Spain"
x = re.split("\s", txt)
print(x)
#specifying the maxsplit parameter
x = re.split("\s", txt, 1)
print(x)

#The sub() Function
txt = "The rain in Spain"
x = re.sub("\s", "9", txt)
print(x)
#specifying the count parameter
txt = "The rain in Spain"
x = re.sub("\s", "9", txt, 2)
print(x)

#Match Object
txt = "The rain in Spain"
x = re.search(r"\bS\w+", txt)
print(x)
#Print the position
txt = "The rain in Spain"
x = re.search(r"\bS\w+", txt)
print(x.span())
#Print the string
print(x.string)
#Print the part of the string where there was a match
print(x.group())

