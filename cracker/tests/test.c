

#include <CUnit/CUnit.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h> 
#include <stdbool.h>
#include "cracker.c" 


Liste *list1 = initialisation();
Liste2 *list2 = initialisation2(); 
int setup(void)  
{ 
	return 0; 
}


int teardown(void) 
{ 
	return 0; 
}

void test_consonne(void){
	CU_ASSERT_EQUAL(consonne("arnaud"), 3);
	CU_ASSERT_EQUAL(consonne("oooooo"), 0);
	CU_ASSERT_EQUAL(consonne("llllllllll"), 10);
	CU_ASSERT_EQUAL(consonne("s"), 1);
	CU_ASSERT_EQUAL(consonne("seidhfjlmdritrdj"), 13);

}

void test_voyelle(void){
	CU_ASSERT_EQUAL(voyelle("arnaud"), 3);
	CU_ASSERT_EQUAL(voyelle("ooooo"), 5);
	CU_ASSERT_EQUAL(voyelle("llllll"), 0);
	CU_ASSERT_EQUAL(voyelle("aaaaaaaaaazzzzzz"), 10);
	CU_ASSERT_EQUAL(voyelle("a"), 1);
}
void test_initialisation1(void){
CU_ASSERT_PTR_NULL(list1->premier->hash);
CU_ASSERT_PTR_NULL(list1->premier->suivant);
}

void test_initialisation2(void){
CU_ASSERT_PTR_NULL(list2->premier->trad);
CU_ASSERT_PTR_NULL(list2->premier->suivant);
}

void test_insertion1(void){
insertion(list1,"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")
CU_ASSERT_STRING_EQUAL(list->premier->hash, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
}

void test_insertion2(void){
insertion2(list2, "papa");
CU_ASSERT_STRING_EQUAL(list2->premier->trad, "papa");
CU_ASSERT_PTR_NOT_NULL(list2->premier->suivant);
}

void test_suppression1(void){
supression(list1);
CU_ASSERT_PTR_NULL(list1->premier->hash);

}

void test_suppression2(void){
suppression2(list2);
CU_ASSERT_PTR_NULL(list1->premier->hash);
CU_ASSERT_PTR_NULL(list1->premier->suivant);
}


int main()
{
	CU_pSuite pSuite = NULL;

	/* initialize the CUnit test registry */
	if(CUE_SUCCESS != CU_initialize_registry())
	return CU_get_error();

	/* add a suite to the registry */
	pSuite = CU_add_suite("Suite de test pour cracker",setup,teardown);
	if(NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	/* NOTE - ORDER IS IMPORTANT - first fct added = first to be run */
	if(NULL == CU_add_test(pSuite, "test_consonne", test_consonne()) || 
	NULL == CU_add_test(pSuite, "test_voyelle", test_voyelle()) ||
	NULL == CU_add_test(pSuite, "test_initialisation1", test_initialisation1()) ||
	NULL == CU_add_test(pSuite, "test_initialisation2", test_initialisation2()) ||
	NULL == CU_add_test(pSuite, "test_insertion1", test_insertion1()) ||
	NULL == CU_add_test(pSuite, "test_insertion2", test_insertion2()) ||
	NULL == CU_add_test(pSuite, "test_suppression1", test_suppression1()) ||
	NULL == CU_add_test(pSuite, "test_suppression2", test_suppression2()))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* Run all tests using the CUnit Basic interface */
	//U_basic_set_mode(CU_BRM_SILENT);
	CU_basic_run_tests();
	CU_basic_show_failures(CU_get_failure_list());
	CU_cleanup_registry();
	return CU_get_error();
}
