import pickle
import module
import pyABCserializer


print("Creating instance of module.PyDerived")
b = module.PyDerived()
print()

print("Setting data in module.PyDerived")
b.data = "blah"
print()

print("Demonstrating that data python function can be called from c++, and is accessing data")
b.call()
print()

print("Getting hex dump from pickle directly")
hex_b1 = pickle.dumps(b).hex()
print(hex_b1)
print()

print("Getting hex dump through c++ function that internally calls pickle")
hex_b2 = pyABCserializer.save(b)
print(hex_b2)
print()

print("Checking that the two hex dumps are equal")
print(hex_b1 == hex_b2)
assert(hex_b1 == hex_b2)
print()

print("Converting hex dump to bytes object")
bytes_b = bytes.fromhex(hex_b1)
print()

print("Attempting to unpickle bytes object")
unpacked = pickle.loads(bytes_b)
print()

print("Checking that unpickled object is the correct type and has the data")
print("Type:", type(unpacked))
unpacked.call()
print()

print("Attempting to create object from hex dump in c++ function call")
c = pyABCserializer.load(hex_b2)
print()

print("Checking that object is the correct type and has the data")
print("Type:", type(c))
c.call()
print()
