#Python Modules

#Create a Module
f = open("../mymodule.py", "w")
f.write('def greeting(name):\n'
        '    print("Hello, " + name)')
f.close()

#Use a Module
import mymodule
mymodule.greeting("Jonathan")

#Variables in Module
f = open("../mymodule.py", "w")
f.write('person1 = {"name": "John","age": 36,"country": "Norway"}')
f.close()

import mymodule
a = mymodule.person1["age"]
print(a)