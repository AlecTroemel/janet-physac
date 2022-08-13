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
