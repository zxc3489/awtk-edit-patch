﻿/**
 * File:   fscript_array.c
 * Author: AWTK Develop Team
 * Brief:  array functions for fscript
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-03 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#include "tkc/fscript.h"
#include "tkc/object_array.h"
static ret_t func_array_create(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t i = 0;
  object_t* obj = object_array_create();
  value_set_object(result, obj);
  result->free_handle = TRUE;

  for (i = 0; i < args->size; i++) {
    ret_t ret = object_array_push(obj, args->args + i);
    if (ret != RET_OK) {
      break;
    }
  }

  return RET_OK;
}

static value_type_t value_type_from_str(const char* type) {
  if (type != NULL) {
    if (*type == 'i') {
      return VALUE_TYPE_INT32;
    } else if (*type == 'd') {
      return VALUE_TYPE_DOUBLE;
    }
  }

  return VALUE_TYPE_STRING;
}

static ret_t func_array_create_with_str(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  object_t* obj = NULL;
  const char* str = NULL;
  const char* sep = NULL;
  const char* type = NULL;
  FSCRIPT_FUNC_CHECK(args->size >= 2, RET_BAD_PARAMS);

  str = value_str(args->args);
  sep = value_str(args->args + 1);
  type = args->size > 2 ? value_str(args->args + 2) : NULL;

  obj = object_array_create_with_str(str, sep, value_type_from_str(type));
  value_set_object(result, obj);
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);
  result->free_handle = TRUE;

  return RET_OK;
}

static ret_t func_array_dup(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t start = 0;
  uint32_t end = 0;
  object_t* obj = NULL;
  object_t* dup = NULL;
  object_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);

  obj = value_object(args->args);
  arr = OBJECT_ARRAY(obj);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  if (arr->size == 0) {
    dup = object_array_clone(obj);
  } else {
    if (args->size == 1) {
      start = 0;
      end = arr->size;
    } else if (args->size == 2) {
      start = value_uint32(args->args + 1);
      end = arr->size;
    } else {
      start = value_uint32(args->args + 1);
      end = value_uint32(args->args + 2);
    }
    dup = object_array_dup(obj, start, end);
  }
  value_set_object(result, dup);
  return_value_if_fail(dup != NULL, RET_BAD_PARAMS);
  result->free_handle = TRUE;

  return RET_OK;
}

static ret_t func_array_create_repeated(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t i = 0;
  uint32_t n = 0;
  object_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);

  obj = object_array_create();
  value_set_object(result, obj);
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);

  result->free_handle = TRUE;
  n = value_uint32(args->args + 1);
  for (i = 0; i < n; i++) {
    ret_t ret = object_array_push(obj, args->args);
    if (ret != RET_OK) {
      break;
    }
  }

  return RET_OK;
}

static ret_t func_array_push(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t i = 0;
  object_t* obj = NULL;
  object_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size > 1, RET_BAD_PARAMS);
  obj = value_object(args->args);
  arr = OBJECT_ARRAY(obj);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  for (i = 1; i < args->size; i++) {
    ret_t ret = object_array_push(obj, args->args + i);
    if (ret != RET_OK) {
      break;
    }
  }

  value_set_uint32(result, i - 1);

  return RET_OK;
}

static ret_t func_array_pop(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  object_t* obj = NULL;
  object_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = value_object(args->args);
  arr = OBJECT_ARRAY(obj);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  return object_array_pop(obj, result);
}

static ret_t func_array_shift(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  object_t* obj = NULL;
  object_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = value_object(args->args);
  arr = OBJECT_ARRAY(obj);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  return object_array_shift(obj, result);
}

static ret_t func_array_set(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  int32_t index = 0;
  object_t* obj = NULL;
  object_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 3, RET_BAD_PARAMS);
  obj = value_object(args->args);
  arr = OBJECT_ARRAY(obj);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  index = value_int(args->args + 1);
  if (index < 0) {
    index += arr->size;
  }
  return_value_if_fail(index >= 0 && index < arr->size, RET_BAD_PARAMS);

  value_set_bool(result, object_array_set(obj, index, args->args + 2) == RET_OK);

  return RET_OK;
}

static ret_t func_array_get(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  int32_t index = 0;
  object_t* obj = NULL;
  object_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  obj = value_object(args->args);
  arr = OBJECT_ARRAY(obj);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  index = value_int(args->args + 1);
  if (index < 0) {
    index += arr->size;
  }
  return_value_if_fail(index >= 0 && index < arr->size, RET_BAD_PARAMS);

  return object_array_get(obj, index, result);
}

static ret_t func_array_insert(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  int32_t index = 0;
  object_t* obj = NULL;
  object_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 3, RET_BAD_PARAMS);
  obj = value_object(args->args);
  arr = OBJECT_ARRAY(obj);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  index = value_int(args->args + 1);
  if (index < 0) {
    index += arr->size;
  }
  return_value_if_fail(index >= 0, RET_BAD_PARAMS);

  value_set_bool(result, object_array_insert(obj, index, args->args + 2) == RET_OK);

  return RET_OK;
}

static ret_t func_array_remove(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  int32_t index = 0;
  object_t* obj = NULL;
  object_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  obj = value_object(args->args);
  arr = OBJECT_ARRAY(obj);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  index = value_int(args->args + 1);
  if (index < 0) {
    index += arr->size;
  }
  return_value_if_fail(index >= 0 && index < arr->size, RET_BAD_PARAMS);

  value_set_bool(result, object_array_remove(obj, index) == RET_OK);

  return RET_OK;
}

static ret_t func_array_get_and_remove(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  int32_t index = 0;
  object_t* obj = NULL;
  object_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  obj = value_object(args->args);
  arr = OBJECT_ARRAY(obj);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  index = value_int(args->args + 1);
  if (index < 0) {
    index += arr->size;
  }
  return_value_if_fail(index >= 0 && index < arr->size, RET_BAD_PARAMS);

  return object_array_get_and_remove(obj, index, result);
}

static ret_t func_array_index_of(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  object_t* obj = NULL;
  object_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  obj = value_object(args->args);
  arr = OBJECT_ARRAY(obj);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  value_set_int(result, object_array_index_of(obj, args->args + 1));

  return RET_OK;
}

static ret_t func_array_last_index_of(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  object_t* obj = NULL;
  object_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  obj = value_object(args->args);
  arr = OBJECT_ARRAY(obj);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  value_set_int(result, object_array_last_index_of(obj, args->args + 1));

  return RET_OK;
}

static ret_t func_array_clear(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  object_t* obj = NULL;
  object_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = value_object(args->args);
  arr = OBJECT_ARRAY(obj);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  value_set_bool(result, object_array_clear_props(obj) == RET_OK);

  return RET_OK;
}

static ret_t func_array_sort_ex(fscript_t* fscript, fscript_args_t* args, value_t* result,
                                bool_t clone) {
  ret_t ret = RET_OK;
  object_t* obj = NULL;
  bool_t ascending = TRUE;
  bool_t ignore_case = FALSE;
  object_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);
  obj = value_object(args->args);
  if (clone) {
    obj = object_array_clone(obj);
  }
  arr = OBJECT_ARRAY(obj);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  if (args->size > 1) {
    ascending = value_bool(args->args + 1);
  }
  if (args->size > 2) {
    ignore_case = value_bool(args->args + 2);
  }

  if (arr->size > 1) {
    value_t v;
    object_array_get(obj, 0, &v);

    if (v.type == VALUE_TYPE_STRING) {
      ret = object_array_sort_as_str(obj, ascending, ignore_case);
    } else if (v.type == VALUE_TYPE_INT32) {
      ret = object_array_sort_as_int(obj, ascending);
    } else {
      ret = object_array_sort_as_double(obj, ascending);
    }
  }

  if (clone) {
    value_set_object(result, obj);
  } else {
    value_set_bool(result, ret == RET_OK);
  }

  return RET_OK;
}

static ret_t func_array_sort(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  return func_array_sort_ex(fscript, args, result, FALSE);
}

static ret_t func_array_clone_and_sort(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  return func_array_sort_ex(fscript, args, result, TRUE);
}

static ret_t func_array_join(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  str_t str;
  object_t* obj = NULL;
  const char* sep = NULL;
  object_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  obj = value_object(args->args);
  arr = OBJECT_ARRAY(obj);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);
  sep = value_str(args->args + 1);

  str_init(&str, 100);
  if (object_array_join(obj, sep, &str) == RET_OK) {
    value_set_str(result, str.str);
    result->free_handle = TRUE;
  } else {
    str_reset(&str);
  }

  return RET_OK;
}

static ret_t func_array_min(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  return object_array_min(value_object(args->args), result);
}

static ret_t func_array_max(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  return object_array_max(value_object(args->args), result);
}

static ret_t func_array_avg(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  return object_array_avg(value_object(args->args), result);
}

static ret_t func_array_sum(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  return object_array_sum(value_object(args->args), result);
}

ret_t fscript_array_register(void) {
  ENSURE(fscript_register_func("array_create", func_array_create) == RET_OK);
  ENSURE(fscript_register_func("array_dup", func_array_dup) == RET_OK);
  ENSURE(fscript_register_func("array_create_with_str", func_array_create_with_str) == RET_OK);
  ENSURE(fscript_register_func("array_create_repeated", func_array_create_repeated) == RET_OK);
  ENSURE(fscript_register_func("array_push", func_array_push) == RET_OK);
  ENSURE(fscript_register_func("array_pop", func_array_pop) == RET_OK);
  ENSURE(fscript_register_func("array_shift", func_array_shift) == RET_OK);
  ENSURE(fscript_register_func("array_get", func_array_get) == RET_OK);
  ENSURE(fscript_register_func("array_set", func_array_set) == RET_OK);
  ENSURE(fscript_register_func("array_insert", func_array_insert) == RET_OK);
  ENSURE(fscript_register_func("array_remove", func_array_remove) == RET_OK);
  ENSURE(fscript_register_func("array_get_and_remove", func_array_get_and_remove) == RET_OK);
  ENSURE(fscript_register_func("array_index_of", func_array_index_of) == RET_OK);
  ENSURE(fscript_register_func("array_last_index_of", func_array_last_index_of) == RET_OK);
  ENSURE(fscript_register_func("array_clear", func_array_clear) == RET_OK);
  ENSURE(fscript_register_func("array_join", func_array_join) == RET_OK);
  ENSURE(fscript_register_func("array_sort", func_array_sort) == RET_OK);
  ENSURE(fscript_register_func("array_clone_and_sort", func_array_clone_and_sort) == RET_OK);
  ENSURE(fscript_register_func("array_min", func_array_min) == RET_OK);
  ENSURE(fscript_register_func("array_max", func_array_max) == RET_OK);
  ENSURE(fscript_register_func("array_avg", func_array_avg) == RET_OK);
  ENSURE(fscript_register_func("array_sum", func_array_sum) == RET_OK);

  return RET_OK;
}
