#include <janet/janet.h>
#include "physac.h"

JANET_FN(cfun_hello_native,
         "($name/hello-native)",
         "Evaluate to \"Hello!\". but implementedd in C.") {
  janet_fixarity(argc, 0);
  (void) argv;
  return janet_cstringv("Hello!");
}


JANET_MODULE_ENTRY(JanetTable *env) {
  JanetRegExt cfuns[] = {
    JANET_REG("hello-native", cfun_hello_native),
    JANET_REG_END
  };
  janet_cfuns_ext(env, "janet-physac", cfuns);
}
