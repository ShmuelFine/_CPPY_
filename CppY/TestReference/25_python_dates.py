#Python Datetime

import datetime
x = datetime.datetime.now()
print(x)

#Date Output
print(x.year)
print(x.strftime("%A"))

#Creating Date Objects
x = datetime.datetime(2020, 5, 17)
print(x)

#The strftime() Method
x = datetime.datetime(2018, 6, 1)
print(x.strftime("%B"))