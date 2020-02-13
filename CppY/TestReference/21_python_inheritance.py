# Python Inheritance

# Create a Parent Class
class Person:
  def __init__(self, fname, lname):
    self.firstname = fname
    self.lastname = lname

  def printname(self):
    print(self.firstname, self.lastname)

#Use the Person class to create an object, and then execute the printname method:
x = Person("John", "Doe")
x.printname()

#Create a Child Class
class Student(Person):
# Add the __init__() Function
  def __init__(self, fname, lname,year):
# Use the super() Function
    super().__init__(fname, lname)

#Add Properties
    self.graduationyear = year

#Add Methods
  def welcome(self):
      print("Welcome", self.firstname, self.lastname, "to the class of", self.graduationyear)

x = Student("Mike", "Olsen",2019)
x.printname()
x.welcome()

