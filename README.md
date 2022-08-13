# janet-physac

[Janet](https://janet-lang.org) bindings to [Physac](https://github.com/raysan5/physac). The bindings
are faithful to the original C API, especially where it makes sense, but function names
have been "lispified" (kebab case, question marks instead of the word "Is", etc.).

Here are some deviations I've made from the core C api.

- GetPhysicsShapeType converts resulting integer to keywords `:circle` and `:polygon` respectively.
- Although the  `GetPhysicsShapeType` and `GetPhysicsShapeVerticesCount` functions are implemented, you can also get this info right off the PhysicsBody through its getter.
   ```janet
   (get my-physics-body :shape-type)
   (get my-physics-body :vertices-count)
   ```
