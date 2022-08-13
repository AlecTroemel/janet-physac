#include <janet/janet.h>
#include "physac.h"

int physicsbody_get(void *p, Janet key, Janet *out);
void physicsbody_put(void *p, Janet key, Janet value);

static const JanetAbstractType AT_PhysicsBody = {
  "physac/physics-body",
  NULL,
  NULL,
  physicsbody_get,
  physicsbody_put,
  JANET_ATEND_PUT
};

int physicsbody_get(void *p, Janet key, Janet *out) {
  PhysicsBody body = (PhysicsBody)p;

  if (!janet_checktype(key, JANET_KEYWORD)) {
    janet_panic("expected keyword");
  }

  const uint8_t *kw = janet_unwrap_keyword(key);

  if (!janet_cstrcmp(kw, "id")) {
    *out = janet_wrap_integer(body->id);
    return 1;
  }

  return 0;
}

void physicsbody_put(void *p, Janet key, Janet value) {
  PhysicsBody body = (PhysicsBody)p;

  if (!janet_checktype(key, JANET_KEYWORD)) {
    janet_panic("expected keyword");
  }

  const uint8_t *kw = janet_unwrap_keyword(key);

  if (!janet_cstrcmp(kw, "use-gravity")) {
    if (!janet_checktype(value, JANET_BOOLEAN)) {
      janet_panic("expected booleanvalue");
    }

    body->useGravity = janet_unwrap_boolean(value);
  }
}

static PhysicsBody *physac_getphysicsbody(const Janet *argv, int32_t n) {
  return ((PhysicsBody *)janet_getabstract(argv, n, &AT_PhysicsBody));
}

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
  janet_fixarity(argc, 3);

  Vector2 pos = physac_getvec2(argv, 0);
  float radius = janet_getnumber(argv, 1);
  float density = janet_getnumber(argv, 2);
  PhysicsBody body = CreatePhysicsBodyCircle(pos, radius, density);

  return janet_wrap_abstract(body);
}

JANET_FN(cfun_create_physics_body_rectangle,
         "($name/create-physics-body-rectangle)",
         "Creates a new rectangle physics body with generic parameters.") {
  janet_fixarity(argc, 4);

  Vector2 pos = physac_getvec2(argv, 0);
  float width = janet_getnumber(argv, 1);
  float height = janet_getnumber(argv, 2);
  float density = janet_getnumber(argv, 3);
  PhysicsBody body = CreatePhysicsBodyRectangle(pos, width, height, density);

  return janet_wrap_abstract(body);
}

JANET_FN(cfun_physics_add_force,
         "($name/physics-add-force)",
         "Adds a force to a physics body") {
  janet_fixarity(argc, 2);
  PhysicsBody body = *physac_getphysicsbody(argv, 0);
  Vector2 force = physac_getvec2(argv, 1);
  PhysicsAddForce(body, force);
  return janet_wrap_nil();
}

JANET_FN(cfun_physics_add_torque,
         "($name/physics-add-torque)",
         "Adds a angular force to a physics body.") {
  janet_fixarity(argc, 2);
  PhysicsBody body = *physac_getphysicsbody(argv, 0);
  float amount = janet_getnumber(argv, 1);
  PhysicsAddTorque(body, amount);
  return janet_wrap_nil();
}


JANET_FN(cfun_get_physics_bodies_count,
         "($name/get-physics-bodies-count)",
         "Returns the current amount of created physics bodies.") {
  janet_fixarity(argc, 0);
  (void) argv;
  int count = GetPhysicsBodiesCount();
  return janet_wrap_integer(count);
}

JANET_FN(cfun_get_physics_body,
         "($name/get-physics-body)",
         "Returns a physics body of the bodies pool at a specific index") {
  janet_fixarity(argc, 1);
  (void) argv;
  int index = janet_getinteger(argv, 0);
  PhysicsBody body = GetPhysicsBody(index);
  return janet_wrap_abstract(body);
}


JANET_FN(cfun_destroy_physics_body,
         "($name/destroy-physics-body)",
         "Unitializes and destroy a physics body") {
  janet_fixarity(argc, 1);
  PhysicsBody body = *physac_getphysicsbody(argv, 0);
  DestroyPhysicsBody(body);
  return janet_wrap_nil();
}

JANET_FN(cfun_close_physics,
         "($name/close-physics)",
         "Unitializes physics pointers and closes physics loop thread") {
  janet_fixarity(argc, 0);
  (void) argv;
  ClosePhysics();
  return janet_wrap_nil();
}

JANET_MODULE_ENTRY(JanetTable *env) {
  JanetRegExt cfuns[] = {
    JANET_REG("init-physics", cfun_init_physics),
    JANET_REG("set-physics-gravity", cfun_set_physics_gravity),
    JANET_REG("create-physics-body-circle", cfun_create_physics_body_circle),
    JANET_REG("create-physics-body-rectangle", cfun_create_physics_body_rectangle),
    JANET_REG("physics-add-force", cfun_physics_add_force),
    JANET_REG("physics-add-torque", cfun_physics_add_torque),
    JANET_REG("get-physics-bodies-count", cfun_get_physics_bodies_count),
    JANET_REG("get-physics-body", cfun_get_physics_body),
    JANET_REG("destroy-physics-body", cfun_destroy_physics_body),
    JANET_REG("close-physics", cfun_close_physics),
    JANET_REG_END
  };
  janet_cfuns_ext(env, "janet-physac", cfuns);
}
