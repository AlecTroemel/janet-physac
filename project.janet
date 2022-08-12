(declare-project
  :name "janet-physac"
  :description "Janet bindings to Physac"
  :url ""
  :author "Alec Troemel <alectroemel@hotmail.com>")

(rule "build/janetphysac.so" ["CMakeLists.txt"]
      (do
        (os/mkdir "build")
        (os/cd "build")
        (os/execute ["cmake" ".."] :p)
        (assert
         (zero?
          (os/execute ["make"] :p)))))

(add-dep "build" "build/janetui.so")
