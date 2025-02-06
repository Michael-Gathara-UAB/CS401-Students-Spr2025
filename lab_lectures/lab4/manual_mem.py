"""
This file showcases how to write Python code that calls the garbage collector in order to free up memory when a programmer wants it freed.
"""

import gc

# Disable garbage collector
gc.disable()

class MemoryHungryObject:
    def __init__(self, name, data):
        self.name = name
        # Create a large list to consume memory
        self.data = [data] * 1000000

def create_objects():
    # Dictionary to store our objects
    objects = {}
    
    # Create several large objects
    for i in range(5):
        objects[f"object_{i}"] = MemoryHungryObject(f"object_{i}", i)
        print(f"Created {objects[f'object_{i}'].name}")
    
    return objects

def main():
    print("Garbage collector enabled:", gc.isenabled())    
    print("Creating objects...")
    
    # Create objects and store them
    objects = create_objects()
    
    # Let's see the amount of memory each object takes
    print("\nObjects memory usage")
    for obj_name, obj in objects.items():
        print(f"{obj_name}: {format(obj.data.__sizeof__(), ',')} Bytes")
    
    # Let's see the objects we've created
    print("\nBefore manual cleanup:")
    print("Current objects:", list(objects.keys()))
    
    # Manual cleanup of specific objects
    print("\nPerforming manual cleanup...")
    del objects["object_1"]
    del objects["object_3"]
    
    # Force garbage collection despite it being disabled
    gc.collect()
    
    print("\nRemaining objects memory usage:")
    for obj_name, obj in objects.items():
        print(f"{obj_name}: {format(obj.data.__sizeof__(), ',')} Bytes")
        
    
    print("\nAfter manual cleanup:")
    print("Remaining objects:", list(objects.keys()))
    
    # Remove the rest of the objects
    del objects["object_0"]
    del objects["object_2"]
    del objects["object_4"]
    
    # Force garbage collection despite it being disabled
    gc.collect()
    
    print("\nAfter manual cleanup two:")
    print("Remaining objects:", list(objects.keys()))
    
    # Re-enable garbage collector before program ends
    gc.enable()
    print("\nGarbage collector re-enabled:", gc.isenabled())

if __name__ == "__main__":
    main()



"""
Sample output incase you dont want to run this program:

Garbage collector enabled: False
Creating objects...
Created object_0
Created object_1
Created object_2
Created object_3
Created object_4

Objects memory usage
object_0: 8,000,040 Bytes
object_1: 8,000,040 Bytes
object_2: 8,000,040 Bytes
object_3: 8,000,040 Bytes
object_4: 8,000,040 Bytes

Before manual cleanup:
Current objects: ['object_0', 'object_1', 'object_2', 'object_3', 'object_4']

Performing manual cleanup...

Remaining objects memory usage:
object_0: 8,000,040 Bytes
object_2: 8,000,040 Bytes
object_4: 8,000,040 Bytes

After manual cleanup:
Remaining objects: ['object_0', 'object_2', 'object_4']

After manual cleanup two:
Remaining objects: []

Garbage collector re-enabled: True

"""