#include <check.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <stringMap.h>



START_TEST(stringMap_basic_test){
  struct str_map s;

  str_map_init(&s, NULL, -1);

  ck_assert_int_eq(str_map_insert_str(&s, "ABC", "Hello"), 1);
  ck_assert_int_eq(str_map_insert_str(&s, "ABC", "don't matter"), 0);
  ck_assert_str_eq(str_map_search(&s, "ABC"), "Hello");
  ck_assert_str_eq(str_map_remove(&s, "ABC"), "Hello");
  ck_assert_ptr_eq(str_map_remove(&s, "ABC"), NULL);
}
END_TEST

Suite * list_suite(void){

  Suite *s;
  TCase *tc_core;

  s = suite_create("stringMap");


  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, stringMap_basic_test);
  suite_add_tcase(s, tc_core);

  return s;

}

int main(){

  int number_failed;
  Suite *s;
  SRunner *sr;
  
  s = list_suite();
  sr = srunner_create(s);
  
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
  
}
