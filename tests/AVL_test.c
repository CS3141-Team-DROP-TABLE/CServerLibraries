#include <check.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <AVL.h>


int comp(void *a, void *b, size_t cmp_sz){
  return memcmp(a,b,cmp_sz);
}

void del(void *a, void *b){
  free(a);
}

START_TEST(avl_basic_test){
  struct tree t;
  avl_init(&t);

  char *str1= "azzz";
  char *str2= "bzzz";
  char *str3= "czzz";
  char *str4= "dzzz";
  char *str5= "bxzz";
  char *str6= "cxzz";
  char *str7= "cxxz";

  cmp_func cmp = &comp;
  all_func d = &del;

  ck_assert_int_eq(avl_insert(&t, str1, str1, comp, 4), 1);
  ck_assert_int_eq(avl_verify_tree(&t, comp, 4) ,0);
  ck_assert_str_eq(avl_search(&t, str1, comp, 4), str1);

  ck_assert_int_eq(avl_insert(&t, str1, str1, comp, 4), 0);

  
  ck_assert_int_eq(1, avl_insert(&t, str3, str3, comp, 4));
  ck_assert_str_eq(avl_search(&t, str3, comp, 4), str3);
  ck_assert_int_eq(avl_verify_tree(&t, comp, 4) ,0);
  ck_assert_int_eq(0, avl_insert(&t, str3, str3, comp, 4));
  
  
  ck_assert_int_eq(1, avl_insert(&t, str6, str6, comp, 4));
  ck_assert_str_eq(avl_search(&t, str6, comp, 4), str6);
  ck_assert_int_eq(avl_verify_tree(&t, comp, 4) ,0);
  ck_assert_int_eq(0, avl_insert(&t, str6, str6, comp, 4));
  
  ck_assert_int_eq(1, avl_insert(&t, str4, str4, comp, 4));
  ck_assert_str_eq(avl_search(&t, str4, comp, 4), str4);
  ck_assert_int_eq(avl_verify_tree(&t, comp, 4) ,0);
  ck_assert_int_eq(0, avl_insert(&t, str4, str4, comp, 4));
  
  ck_assert_int_eq(1, avl_insert(&t, str5, str5, comp, 4));
  ck_assert_str_eq(avl_search(&t, str5, comp, 4), str5);
  ck_assert_int_eq(avl_verify_tree(&t, comp, 4) ,0);
  ck_assert_int_eq(0, avl_insert(&t, str5, str5, comp, 4));

  ck_assert_int_eq(1, avl_insert(&t, str2, str2, comp, 4));
  ck_assert_str_eq(avl_search(&t, str2, comp, 4), str2);
  ck_assert_int_eq(avl_verify_tree(&t, comp, 4) ,0);
  ck_assert_int_eq(0, avl_insert(&t, str2, str2, comp, 4));

  ck_assert_int_eq(1, avl_insert(&t, str7, str7, comp, 4));
 
  ck_assert_int_eq(avl_verify_tree(&t, comp, 4) ,0);


  
  avl_remove(&t, str1, comp, 4);
  ck_assert_int_eq(avl_verify_tree(&t, comp, 4) ,0);
  ck_assert_ptr_eq(avl_search(&t, str1, comp, 4), NULL);
  
  avl_remove(&t, str3, comp, 4);
  ck_assert_int_eq(avl_verify_tree(&t, comp, 4) ,0);
  ck_assert_ptr_eq(avl_search(&t, str3, comp, 4), NULL);
  
  avl_remove(&t, str6, comp, 4);
  ck_assert_int_eq(avl_verify_tree(&t, comp, 4) ,0);
  ck_assert_ptr_eq(avl_search(&t, str6, comp, 4), NULL);
  
  avl_remove(&t, str4, comp, 4);
  ck_assert_int_eq(avl_verify_tree(&t, comp, 4) ,0);
  ck_assert_ptr_eq(avl_search(&t, str4, comp, 4), NULL);
  
  avl_remove(&t, str5, comp, 4);
  ck_assert_int_eq(avl_verify_tree(&t, comp, 4) ,0);
  ck_assert_ptr_eq(avl_search(&t, str5, comp, 4), NULL);
  
  
  avl_remove(&t, str2, comp, 4);
  ck_assert_int_eq(avl_verify_tree(&t, comp, 4) ,0);
  ck_assert_ptr_eq(avl_search(&t, str2, comp, 4), NULL);
  
  
  avl_remove(&t, str7, comp, 4);
  ck_assert_int_eq(avl_verify_tree(&t, comp, 4) ,0);
  ck_assert_ptr_eq(avl_search(&t, str7, comp, 4), NULL);
  


  ck_assert_int_eq(avl_insert(&t, str1, str1, comp, 4), 1);
  ck_assert_int_eq(avl_verify_tree(&t, comp, 4) ,0);
  ck_assert_str_eq(avl_search(&t, str1, comp, 4), str1);

  ck_assert_int_eq(avl_insert(&t, str1, str1, comp, 4), 0);

  
  ck_assert_int_eq(1, avl_insert(&t, str3, str3, comp, 4));
  ck_assert_str_eq(avl_search(&t, str3, comp, 4), str3);
  ck_assert_int_eq(avl_verify_tree(&t, comp, 4) ,0);
  ck_assert_int_eq(0, avl_insert(&t, str3, str3, comp, 4));
  
  
  ck_assert_int_eq(1, avl_insert(&t, str6, str6, comp, 4));
  ck_assert_str_eq(avl_search(&t, str6, comp, 4), str6);
  ck_assert_int_eq(avl_verify_tree(&t, comp, 4) ,0);
  ck_assert_int_eq(0, avl_insert(&t, str6, str6, comp, 4));
  
  ck_assert_int_eq(1, avl_insert(&t, str4, str4, comp, 4));
  ck_assert_str_eq(avl_search(&t, str4, comp, 4), str4);
  ck_assert_int_eq(avl_verify_tree(&t, comp, 4) ,0);
  ck_assert_int_eq(0, avl_insert(&t, str4, str4, comp, 4));
  
  ck_assert_int_eq(1, avl_insert(&t, str5, str5, comp, 4));
  ck_assert_str_eq(avl_search(&t, str5, comp, 4), str5);
  ck_assert_int_eq(avl_verify_tree(&t, comp, 4) ,0);
  ck_assert_int_eq(0, avl_insert(&t, str5, str5, comp, 4));

  ck_assert_int_eq(1, avl_insert(&t, str2, str2, comp, 4));
  ck_assert_str_eq(avl_search(&t, str2, comp, 4), str2);
  ck_assert_int_eq(avl_verify_tree(&t, comp, 4) ,0);
  ck_assert_int_eq(0, avl_insert(&t, str2, str2, comp, 4));

  ck_assert_int_eq(1, avl_insert(&t, str7, str7, comp, 4));
 
  ck_assert_int_eq(avl_verify_tree(&t, comp, 4) ,0);

  avl_clear_tree(&t, d);

  

}
END_TEST

Suite * list_suite(void){

  Suite *s;
  TCase *tc_core;

  s = suite_create("AVL");


  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, avl_basic_test);
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
