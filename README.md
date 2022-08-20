### After possibly the shortest turnaround in the history of a git repo. Im archiving these bindings in favor of [chipmunk 2d bindings](https://github.com/slembcke/Chipmunk2D). Turns out Physac was mainly built as an example and not intended for real use.

# janet-physac

[Janet](https://janet-lang.org) bindings to [Physac](https://github.com/raysan5/physac). The bindings
are faithful to the original C API, especially where it makes sense, but function names
have been "lispified" (kebab case, question marks instead of the word "Is", etc.).

Here are some deviations I've made from the core C api.

- GetPhysicsShapeType converts resulting integer to keywords `:circle` and `:polygon` respectively.
- Although the  `get-physics-shape-type` and `get-physics-shape-vertices-count` functions are implemented, you can also get this info right off the PhysicsBody through its getter.
   ```janet
   (get-physics-shape-type (my-physics-body :id))
   (get-physics-shape-vertices-count (my-physics-body :id))

   (get my-physics-body :shape-type)
   (get my-physics-body :vertices-count)
   ```
- Similary, `set-physics-body-rotation` is implemenented but you can alternatively use a `put` on the physics body
   ```janet
   (set-physics-body-rotation my-physics-body radians)
   (put my-physics-body :rotation radians)
   ```
