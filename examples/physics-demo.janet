# Adapted from the C library examples https://github.com/raysan5/physac/blob/master/examples/physics_demo.c
# Requires Jaylib (Raylib janet bindings)
(use jaylib)
(use ../build/janet-physac)

(def SCREEN_WIDTH 800)
(def SCREEN_HEIGHT 450)

(def RNG (math/rng))

(var floor nil)
(var circle nil)

(defn create-floor []
  # Create floor rectangle physics body
  # Disable floor to convert it to static (no dynamics, but collisions)
  (set floor (create-physics-body-rectangle
              [(/ SCREEN_WIDTH 2) SCREEN_HEIGHT] 500 100 10))
  (put floor :enabled false))

(defn create-circle []
  # Create obstacle circle physics body, like floor it is disabled
  # Disabled to convert it to static (no dynamics, but collisions)
  (var circle (create-physics-body-circle
               [(/ SCREEN_WIDTH 2) (/ SCREEN_HEIGHT 2)] 45 10))
  (put circle :enabled false))

(defn init []
  (init-window SCREEN_WIDTH SCREEN_HEIGHT "physac in janet")
  (set-target-fps 60)
  (init-physics)
  (create-floor)
  (create-circle))

(defn update []
  (update-physics)

  (when (key-pressed? :r)
    (reset-physics)
    (create-floor)
    (create-circle))

  (when (mouse-button-pressed? :left)
    (create-physics-body-polygon
     (get-mouse-position)
     (+ 20 (math/rng-int RNG 60))
     (+ 3 (math/rng-int RNG 5))
     10))

  (when (mouse-button-pressed? :right)
    (create-physics-body-circle
     (get-mouse-position)
     (+ 10 (math/rng-int RNG 35))
     10))

  (for i 0 (get-physics-bodies-count)
    (if-let [body (get-physics-body i)
             below-screen (> (get-in body [:position 1])
                             (* SCREEN_HEIGHT 2))]
      (destroy-physics-body body))))

(defn draw []
  (begin-drawing)
  (clear-background [0 0 0])
  (draw-fps (- SCREEN_WIDTH 90) (- SCREEN_HEIGHT 30))

  (for i 0 (get-physics-bodies-count)
    (if-let [body (get-physics-body i)
             body_exists? (not (nil? body))
             vertex-count (get-physics-shape-vertices-count i)]

      (for j 0 vertex-count
        # Get physics bodies shape vertices to draw lines
        # Note: GetPhysicsShapeVertex() already calculates rotation transformations
        (let [vertex-a (get-physics-shape-vertex body j)
              jj (mod (+ j 1) vertex-count)
              vertex-b (get-physics-shape-vertex body jj)]
          (draw-line-v vertex-a vertex-b :green)))))

  (draw-text "Left mouse button to create a polygon" 10 10 10 :white)
  (draw-text "Right mouse button to create a circle" 10 25 10 :white)
  (draw-text "Press 'R' to reset example" 10 40 10 :white)
  (end-drawing))

(init)
(while (not (window-should-close))
  (update)
  (draw))

(close-physics)
(close-window)
