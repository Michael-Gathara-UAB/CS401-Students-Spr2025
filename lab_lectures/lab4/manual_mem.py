import gc
import weakref
import time

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
    
    # Let's see the objects we've created
    print("\nBefore manual cleanup:")
    print("Current objects:", list(objects.keys()))
    
    # Manual cleanup of specific objects
    print("\nPerforming manual cleanup...")
    del objects["object_1"]
    del objects["object_3"]
    
    # Force garbage collection despite it being disabled
    gc.collect()
    
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
