#include <check.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <configLoader.h>



START_TEST(config_basic_test){
  struct config c;
  config_loader_init(&c, 0, 0);

  load_config(&c, "test.cfg");

  ck_assert_str_eq(get_config(&c, "test"), "Hello World");
  ck_assert_str_eq(get_config(&c, "value"), "1");
  ck_assert_str_eq(get_config(&c, "value2"), "3");
  ck_assert_str_eq(get_config(&c, "other test"), "lots of spaces");
}
END_TEST

Suite * list_suite(void){

  Suite *s;
  TCase *tc_core;

  s = suite_create("configLoader");


  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, config_basic_test);
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
