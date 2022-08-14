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

static Janet physac_wrap_vec2(Vector2 x) {
  Janet *tup = janet_tuple_begin(2);
  tup[0] = janet_wrap_number(x.x);
  tup[1] = janet_wrap_number(x.y);
  return janet_wrap_tuple(janet_tuple_end(tup));
}

static Vector2 physac_unwrap_vec2(const Janet val) {
  const Janet *tup = janet_unwrap_tuple(val);
  float x = janet_unwrap_number(tup[0]);
  float y = janet_unwrap_number(tup[1]);
  return (Vector2) { x, y };
}

static Janet physac_wrap_shapetype(int shape_type) {
  if (shape_type == PHYSICS_CIRCLE) {
    return janet_ckeywordv("circle");
  }

  if (shape_type == PHYSICS_POLYGON) {
    return janet_ckeywordv("polygon");
  }

  janet_panic("expected shape type");
}

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

  if (!janet_cstrcmp(kw, "enabled")) {
    *out = janet_wrap_boolean(body->enabled);
    return 1;
  }

  if (!janet_cstrcmp(kw, "position")) {
    *out = physac_wrap_vec2(body->position);
    return 1;
  }

  if (!janet_cstrcmp(kw, "velocity")) {
    *out = physac_wrap_vec2(body->position);
    return 1;
  }

  if (!janet_cstrcmp(kw, "force")) {
    *out = physac_wrap_vec2(body->position);
    return 1;
  }

  if (!janet_cstrcmp(kw, "angular-velocity")) {
    *out = janet_wrap_number(body->angularVelocity);
    return 1;
  }

  if (!janet_cstrcmp(kw, "torque")) {
    *out = janet_wrap_number(body->torque);
    return 1;
  }

  if (!janet_cstrcmp(kw, "orient")) {
    *out = janet_wrap_number(body->orient);
    return 1;
  }

  if (!janet_cstrcmp(kw, "static-friction")) {
    *out = janet_wrap_number(body->staticFriction);
    return 1;
  }

  if (!janet_cstrcmp(kw, "dynamic-friction")) {
    *out = janet_wrap_number(body->dynamicFriction);
    return 1;
  }

  if (!janet_cstrcmp(kw, "restitution")) {
    *out = janet_wrap_number(body->restitution);
    return 1;
  }

  if (!janet_cstrcmp(kw, "use-gravity")) {
    *out = janet_wrap_boolean(body->useGravity);
    return 1;
  }

  if (!janet_cstrcmp(kw, "grounded?")) {
    *out = janet_wrap_boolean(body->isGrounded);
    return 1;
  }

  if (!janet_cstrcmp(kw, "freeze-orient")) {
    *out = janet_wrap_boolean(body->freezeOrient);
    return 1;
  }

  if (!janet_cstrcmp(kw, "shape-type")) {
    int index = body->id;
    int shape_type = GetPhysicsShapeType(index);
    *out = physac_wrap_shapetype(shape_type);
    return 1;
  }

  if (!janet_cstrcmp(kw, "vertices-count")) {
    int index = body->id;
    int vertices_count = GetPhysicsShapeVerticesCount(index);
    *out = janet_wrap_integer(vertices_count);
    return 1;
  }

  return 0;
}

void ensure_vec2(Janet value) {
  if (!janet_checktype(value, JANET_TUPLE)) {
    janet_panic("expected tuple value");
  }
}

void ensure_boolean(Janet value) {
  if (!janet_checktype(value, JANET_BOOLEAN)) {
    janet_panic("expected boolean value");
  }
}

void ensure_number(Janet value) {
  if (!janet_checktype(value, JANET_NUMBER)) {
    janet_panic("expected number value");
  }
}

void physicsbody_put(void *p, Janet key, Janet value) {
  PhysicsBody body = (PhysicsBody)p;

  if (!janet_checktype(key, JANET_KEYWORD)) {
    janet_panic("expected keyword");
  }

  const uint8_t *kw = janet_unwrap_keyword(key);

  if (!janet_cstrcmp(kw, "enabled")) {
    ensure_boolean(value);
    body->enabled = janet_unwrap_boolean(value);
  }

  if (!janet_cstrcmp(kw, "position")) {
    ensure_vec2(value);
    body->position = physac_unwrap_vec2(value);
  }

  if (!janet_cstrcmp(kw, "velocity")) {
    ensure_vec2(value);
    body->velocity = physac_unwrap_vec2(value);
  }

  if (!janet_cstrcmp(kw, "force")) {
    ensure_vec2(value);
    body->force = physac_unwrap_vec2(value);
  }

  if (!janet_cstrcmp(kw, "angular-velocity")) {
    ensure_number(value);
    body->angularVelocity = janet_unwrap_number(value);
  }

  if (!janet_cstrcmp(kw, "torque")) {
    ensure_number(value);
    body->torque = janet_unwrap_number(value);
  }

  if (!janet_cstrcmp(kw, "orient")) {
    ensure_number(value);
    body->orient = janet_unwrap_number(value);
  }

  if (!janet_cstrcmp(kw, "static-friction")) {
    ensure_number(value);
    body->staticFriction = janet_unwrap_number(value);
  }

  if (!janet_cstrcmp(kw, "dynamic-friction")) {
    ensure_number(value);
    body->dynamicFriction = janet_unwrap_number(value);
  }

  if (!janet_cstrcmp(kw, "restitution")) {
    ensure_number(value);
    body->restitution = janet_unwrap_number(value);
  }

  if (!janet_cstrcmp(kw, "use-gravity")) {
    ensure_boolean(value);
    body->useGravity = janet_unwrap_boolean(value);
  }

  if (!janet_cstrcmp(kw, "rotation")) {
    ensure_number(value);
    float radians = janet_unwrap_number(value);
    SetPhysicsBodyRotation(body, radians);
  }
}

static PhysicsBody *physac_getphysicsbody(const Janet *argv, int32_t n) {
  return ((PhysicsBody *)janet_getabstract(argv, n, &AT_PhysicsBody));
}
