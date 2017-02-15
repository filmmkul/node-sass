#include <nan.h>
#include "factory.h"
#include "value.h"
#include "number.h"
#include "string.h"
#include "color.h"
#include "boolean.h"
#include "list.h"
#include "map.h"
#include "null.h"
#include "error.h"
#include "..\common.h"

namespace SassTypes
{
  SassTypes::Value* Factory::create(napi_env env, Sass_Value* v) {
    switch (sass_value_get_tag(v)) {
    case SASS_NUMBER:
      return new Number(env, v);

    case SASS_STRING:
      return new String(v);

    case SASS_COLOR:
      return new Color(v);

    case SASS_BOOLEAN:
      return &Boolean::get_singleton(sass_boolean_get_value(v));

    case SASS_LIST:
      return new List(v);

    case SASS_MAP:
      return new Map(v);

    case SASS_NULL:
      return &Null::get_singleton();

    case SASS_ERROR:
      return new Error(v);

    default:
      const char *msg = "Unknown type encountered.";
      CHECK_NAPI_RESULT(napi_throw_type_error(env, msg));
      return new Error(sass_make_error(msg));
    }
  }

  void Factory::initExports(napi_env env, napi_value target) {
    napi_handle_scope scope;
    CHECK_NAPI_RESULT(napi_open_handle_scope(env, &scope));

    napi_value types;
    CHECK_NAPI_RESULT(napi_create_object(env, &types));
    
    napi_propertyname nameNumber;
    CHECK_NAPI_RESULT(napi_property_name(env, "Number", &nameNumber));
    napi_propertyname nameString;
    CHECK_NAPI_RESULT(napi_property_name(env, "String", &nameString));
    napi_propertyname nameColor;
    CHECK_NAPI_RESULT(napi_property_name(env, "Color", &nameColor));
    napi_propertyname nameBoolean;
    CHECK_NAPI_RESULT(napi_property_name(env, "Boolean", &nameBoolean));
    napi_propertyname nameList;
    CHECK_NAPI_RESULT(napi_property_name(env, "List", &nameList));
    napi_propertyname nameMap;
    CHECK_NAPI_RESULT(napi_property_name(env, "Map", &nameMap));
    napi_propertyname nameNull;
    CHECK_NAPI_RESULT(napi_property_name(env, "Null", &nameNull));
    napi_propertyname nameError;
    CHECK_NAPI_RESULT(napi_property_name(env, "Error", &nameError));
    napi_propertyname nameTypes;
    CHECK_NAPI_RESULT(napi_property_name(env, "types", &nameTypes));

    CHECK_NAPI_RESULT(napi_set_property(env, types, nameNumber, Number::get_constructor(env)));
    CHECK_NAPI_RESULT(napi_set_property(env, types, nameString, String::get_constructor(env)));
    CHECK_NAPI_RESULT(napi_set_property(env, types, nameColor, Color::get_constructor(env)));
    CHECK_NAPI_RESULT(napi_set_property(env, types, nameBoolean, Boolean::get_constructor(env)));
    CHECK_NAPI_RESULT(napi_set_property(env, types, nameList, List::get_constructor(env)));
    CHECK_NAPI_RESULT(napi_set_property(env, types, nameMap, Map::get_constructor(env)));
    CHECK_NAPI_RESULT(napi_set_property(env, types, nameNull, Null::get_constructor(env)));
    CHECK_NAPI_RESULT(napi_set_property(env, types, nameError, Error::get_constructor(env)));

    CHECK_NAPI_RESULT(napi_set_property(env, target, nameTypes, types));
  }

  Value* Factory::unwrap(napi_env env, napi_value obj) {
    void* wrapped;
    CHECK_NAPI_RESULT(napi_unwrap(env, obj, &wrapped));
    return static_cast<Value*>(wrapped);
  }
}
