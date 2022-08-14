#include <janet/janet.h>

#define PHYSAC_IMPLEMENTATION
#include <physac.h>

#include "types.h"

JANET_FN(cfun_init_physics,
         "(physac/init-physics)",
         "Initializes physics values, pointers and creates physics loop thread.") {
  janet_fixarity(argc, 0);
  (void) argv;
  InitPhysics();
  return janet_wrap_nil();
}

JANET_FN(cfun_update_physics,
         "(physac/update-physics)",
         "Update physics system.") {
  janet_fixarity(argc, 0);
  (void) argv;
  UpdatePhysics();
  return janet_wrap_nil();
}

JANET_FN(cfun_set_physics_timestep,
         "(physac/set-physics-timestep)",
         "Sets physics fixed time step in milliseconds. 1.666666 by default.") {
  janet_fixarity(argc, 1);
  (void) argv;
  double delta = janet_getnumber(argv, 0);
  SetPhysicsTimeStep(delta);
  return janet_wrap_nil();
}

JANET_FN(cfun_set_physics_gravity,
         "(physac/set-physics-gravity x y)",
         "Sets physics global gravity force.") {
  janet_fixarity(argc, 2);

  float x = janet_getnumber(argv, 0);
  float y = janet_getnumber(argv, 1);
  SetPhysicsGravity(x, y);

  return janet_wrap_nil();
}

JANET_FN(cfun_create_physics_body_circle,
         "(physac/create-physics-body-circle pos radius density)",
         "Creates a new circle physics body with generic parameters.") {
  janet_fixarity(argc, 3);

  Vector2 pos = physac_getvec2(argv, 0);
  float radius = janet_getnumber(argv, 1);
  float density = janet_getnumber(argv, 2);
  PhysicsBody body = janet_abstract(&AT_PhysicsBody, sizeof(PhysicsBodyData));
  *body = *CreatePhysicsBodyCircle(pos, radius, density);

  return janet_wrap_abstract(body);
}

JANET_FN(cfun_create_physics_body_rectangle,
         "(physac/create-physics-body-rectangle pos width height density)",
         "Creates a new rectangle physics body with generic parameters.") {
  janet_fixarity(argc, 4);

  Vector2 pos = physac_getvec2(argv, 0);
  float width = janet_getnumber(argv, 1);
  float height = janet_getnumber(argv, 2);
  float density = janet_getnumber(argv, 3);

  PhysicsBody body = janet_abstract(&AT_PhysicsBody, sizeof(PhysicsBodyData));
  *body = *CreatePhysicsBodyRectangle(pos, width, height, density);

  return janet_wrap_abstract(body);
}

JANET_FN(cfun_create_physics_body_polygon,
         "(physac/create-physics-body-polygon pos radius sides density)",
         "Creates a new polygon physics body with generic parameters.") {
  janet_fixarity(argc, 4);

  Vector2 pos = physac_getvec2(argv, 0);
  float radius = janet_getnumber(argv, 1);
  int sides = janet_getinteger(argv, 2);
  float density = janet_getnumber(argv, 3);
  PhysicsBody body = janet_abstract(&AT_PhysicsBody, sizeof(PhysicsBodyData));
  *body = *CreatePhysicsBodyPolygon(pos, radius, sides, density);

  return janet_wrap_abstract(body);
}

JANET_FN(cfun_physics_add_force,
         "(physac/physics-add-force force)",
         "Adds a force to a physics body") {
  janet_fixarity(argc, 2);
  PhysicsBody body = *physac_getphysicsbody(argv, 0);
  Vector2 force = physac_getvec2(argv, 1);
  PhysicsAddForce(body, force);
  return janet_wrap_nil();
}

JANET_FN(cfun_physics_add_torque,
         "(physac/physics-add-torque amount)",
         "Adds a angular force to a physics body.") {
  janet_fixarity(argc, 2);
  PhysicsBody body = *physac_getphysicsbody(argv, 0);
  float amount = janet_getnumber(argv, 1);
  PhysicsAddTorque(body, amount);
  return janet_wrap_nil();
}


JANET_FN(cfun_get_physics_bodies_count,
         "(physac/get-physics-bodies-count)",
         "Returns the current amount of created physics bodies.") {
  janet_fixarity(argc, 0);
  (void) argv;
  int count = GetPhysicsBodiesCount();
  return janet_wrap_integer(count);
}

JANET_FN(cfun_get_physics_body,
         "(physac/get-physics-body index)",
         "Returns a physics body of the bodies pool at a specific index") {
  janet_fixarity(argc, 1);
  (void) argv;
  int index = janet_getinteger(argv, 0);
  PhysicsBody body = GetPhysicsBody(index);
  return janet_wrap_abstract(body);
}

JANET_FN(cfun_get_physics_shape_type,
         "(physac/get-physics-shape-type index)",
         "Returns the physics body shape type (:circle or :polygon)") {
  janet_fixarity(argc, 1);
  (void) argv;

  int index = janet_getinteger(argv, 0);
  int shape_type = GetPhysicsShapeType(index);

  return physac_wrap_shapetype(shape_type);
}

JANET_FN(cfun_get_physics_shape_vertices_count,
         "(physac/get-physics-shape-vertices-count index)",
         "Returns the amount of vertices of a physics body shape") {
  janet_fixarity(argc, 1);
  (void) argv;

  int index = janet_getinteger(argv, 0);
  int vertices_count = GetPhysicsShapeVerticesCount(index);

  return janet_wrap_integer(vertices_count);
}

JANET_FN(cfun_get_physics_shape_vertex,
         "(physac/get-physics-shape-vertex body vertex)",
         "Returns transformed position of a body shape (body position + vertex transformed position).") {
  janet_fixarity(argc, 2);
  (void) argv;

  PhysicsBody body = *physac_getphysicsbody(argv, 0);
  int vertex = janet_getinteger(argv, 1);
  Vector2 position = GetPhysicsShapeVertex(body, vertex);
  return physac_wrap_vec2(position);
}

JANET_FN(cfun_set_physics_body_rotation,
         "(physac/set-physics-body-rotation radians)",
         "Sets physics body shape transform based on radians parameter") {
  janet_fixarity(argc, 2);
  (void) argv;
  PhysicsBody body = *physac_getphysicsbody(argv, 0);
  float radians = janet_getnumber(argv, 1);
  SetPhysicsBodyRotation(body, radians);
  return janet_wrap_nil();
}

JANET_FN(cfun_destroy_physics_body,
         "(physac/destroy-physics-body body)",
         "Unitializes and destroy a physics body") {
  janet_fixarity(argc, 1);
  PhysicsBody body = *physac_getphysicsbody(argv, 0);
  DestroyPhysicsBody(body);
  return janet_wrap_nil();
}

JANET_FN(cfun_close_physics,
         "(physac/close-physics)",
         "Unitializes physics pointers and closes physics loop thread") {
  janet_fixarity(argc, 0);
  (void) argv;
  ClosePhysics();
  return janet_wrap_nil();
}

JANET_MODULE_ENTRY(JanetTable *env) {
  JanetRegExt cfuns[] = {
    JANET_REG("init-physics", cfun_init_physics),
    JANET_REG("update-physics", cfun_update_physics),
    JANET_REG("set-physics-timestep", cfun_set_physics_timestep),
    JANET_REG("set-physics-gravity", cfun_set_physics_gravity),
    JANET_REG("create-physics-body-circle", cfun_create_physics_body_circle),
    JANET_REG("create-physics-body-rectangle", cfun_create_physics_body_rectangle),
    JANET_REG("create-physics-body-polygon", cfun_create_physics_body_polygon),
    JANET_REG("physics-add-force", cfun_physics_add_force),
    JANET_REG("physics-add-torque", cfun_physics_add_torque),
    JANET_REG("get-physics-bodies-count", cfun_get_physics_bodies_count),
    JANET_REG("get-physics-body", cfun_get_physics_body),
    JANET_REG("get-physics-shape-type", cfun_get_physics_shape_type),
    JANET_REG("get-physics-shape-vertices-count", cfun_get_physics_shape_vertices_count),
    JANET_REG("get-physics-shape-vertex", cfun_get_physics_shape_vertex),
    JANET_REG("set-physics-body-rotation", cfun_set_physics_body_rotation),
    JANET_REG("destroy-physics-body", cfun_destroy_physics_body),
    JANET_REG("close-physics", cfun_close_physics),
    JANET_REG_END
  };
  janet_cfuns_ext(env, "janet-physac", cfuns);
}
