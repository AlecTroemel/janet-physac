(declare-project
  :name "janet-physac"
  :description "Janet bindings to Physac"
  :url ""
  :author "Alec Troemel <alectroemel@hotmail.com>")

(declare-native
  :name "janet-physac"
  :cflags ["-Iphysac/src"]
  :source ["src/main.c"]
  :headers ["src/types.h" "physac/src/physac.h"])
