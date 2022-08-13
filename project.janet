(declare-project
  :name "janet-physac"
  :description "Janet bindings to Physac"
  :url ""
  :author "Alec Troemel <alectroemel@hotmail.com>")

(declare-native
  :name "janet-physac"
  :cflags ["-Iphysac/src"]
  :source ["main.c"]
  :headers ["types.h" "physac/src/physac.h"])
