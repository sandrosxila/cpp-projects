#AVL Tree Data Structure

There's given AVL tree implementation file. AVL tree class
method names resemble names of the STL class methods. Here
are example of use of the class.

###Creation of the Class instance

```c++
//pattern: class name <type name> instance name
AVL <long long> object;
```

### Element *insertion* in the AVL tree

_note: duplicates won't be inserted_
```c++
//pattern: instanceName.insert(data)
object.insert(99);
```

###Element *deletion* from the AVL tree

_note: non-existing elements won't be deleted_
```c++
//pattern: instanceName.erase(data)
object.erase(99);
```

###_Printing_ elements

printing elements
```c++
object.print();
or
object.print(0);
or
object.print(false);
```
print elements in ascending order
```c++
object.print(1);
or
object.print(true);
```

###Getting the *size* of the AVL three class instance
```c++
//pattern: instanceName.size()
int sizeOfTree = object.size();
```

###Getting the *root* element of the AVL tree
```c++
//pattern: instanceName.size()
int rootElement = object.top();
```
