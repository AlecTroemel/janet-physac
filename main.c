#include <janet/janet.h>
#include "physac.h"

static float idx_getfloat(JanetView idx, int index) {
  if (index >= idx.len) {
    janet_panicf("index %d outside of range [0, %d)", idx.len);
  }
  if (!janet_checktype(idx.items[index], JANET_NUMBER)) {
    janet_panicf("expected number, got %v", idx.items[index]);
  }
  return (float) janet_unwrap_number(idx.items[index]);
}

static Vector2 physac_getvec2(const Janet *argv, int32_t n) {
  JanetView idx = janet_getindexed(argv, n);
  return (Vector2) {
    idx_getfloat(idx, 0),
    idx_getfloat(idx, 1)
  };
}

JANET_FN(cfun_init_physics,
         "($name/init-physics)",
         "Initializes physics values, pointers and creates physics loop thread.") {
  janet_fixarity(argc, 0);
  (void) argv;
  InitPhysics();
  return janet_wrap_nil();
}

JANET_FN(cfun_set_physics_gravity,
         "($name/set-physics-gravity)",
         "Sets physics global gravity force.") {
  janet_fixarity(argc, 2);

  float x = janet_getnumber(argv, 0);
  float y = janet_getnumber(argv, 1);
  SetPhysicsGravity(x, y);

  return janet_wrap_nil();
}

JANET_FN(cfun_create_physics_body_circle,
         "($name/create-physics-body-circle)",
         "Creates a new circle physics body with generic parameters.") {
  janet_fixarity(argc, 2);

  Vector2 pos = physac_getvec2(argv, 0);
  float radius = janet_getnumber(argv, 1);
  float density = janet_getnumber(argv, 2);
  PhysicsBody body = CreatePhysicsBodyCircle(pos, radius, density);

  return janet_wrap_abstract(body);
}

JANET_MODULE_ENTRY(JanetTable *env) {
  JanetRegExt cfuns[] = {
    JANET_REG("init-physics", cfun_init_physics),
    JANET_REG("set-physics-gravity", cfun_set_physics_gravity),
    JANET_REG("create-physics-body-circle", cfun_create_physics_body_circle),
    JANET_REG_END
  };
  janet_cfuns_ext(env, "janet-physac", cfuns);
}
