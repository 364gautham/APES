#ifndef __dll_H_
#define __dll_H_

#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
		
struct node{
	struct node *prev;
	struct node *next;
};

struct info{
	uint32_t data;
	struct node element;
};

typedef enum state
{
	success,
	error
}dll_state;

dll_state te_insert_begin(struct info* head,uint32_t data){
	struct info * temp = (struct info*)malloc(sizeof(struct info));
	//malloc(sizeof(struct node));	
	temp->data = data;
	if(head == NULL){
		head = temp;
		head->element.prev = NULL;
		head->element.next = NULL;
		return success;	
	}
	
	head->element.prev = (struct node*)temp;
	temp->element.prev = NULL;
	temp->element.next = (struct node*)head;		
	head = temp;
	return success;
}
struct info* insert_begin(struct info* head,uint32_t data){
	struct info * temp = (struct info*)malloc(sizeof(struct info));
	//malloc(sizeof(struct node));	
	temp->data = data;
	if(head == NULL){
		head = temp;
		head->element.prev = NULL;
		head->element.next = NULL;
		return head;	
	}
	
	head->element.prev = (struct node*)temp;
	temp->element.prev = NULL;
	temp->element.next = (struct node*)head;		
	head = temp;
	return head;
}

dll_state te_insert_end(struct info* head,uint32_t data){
	struct info * temp = (struct info*)malloc(sizeof(struct info));
	struct info * temp1 = head;	
	temp->data = data;
	if(head == NULL){
		head = temp;
		head->element.prev = NULL;
		head->element.next = NULL;
		return success;	
	}
	while(temp1->element.next!=NULL){
	temp1= (struct info*)temp1->element.next;
	}
	
	temp1->element.next = (struct node*)temp;
	temp->element.next = NULL;
	
	temp->element.prev= (struct node*)temp1 ;		
	//head = temp;
	return success;
}

struct info* insert_end(struct info* head,uint32_t data){
	struct info * temp = (struct info*)malloc(sizeof(struct info));
	struct info * temp1 = head;		
	temp->data = data;
	if(head == NULL){
		head = temp;
		head->element.prev = NULL;
		head->element.next = NULL;
		return head;	
	}
	while(temp1->element.next!=NULL){
	temp1= (struct info*)temp1->element.next;
	}
	
	temp1->element.next = (struct node*)temp;
	temp->element.next = NULL;
	
	temp->element.prev= (struct node*)temp1 ;		
	//head = temp;
	return head;
}

dll_state te_insert_at_pos(struct info* head,uint32_t data,uint32_t pos){
	struct info * temp = (struct info*)malloc(sizeof(struct info));
	struct info * temp1 = head;
	struct info * temp2;	
	//malloc(sizeof(struct node));		
	if(head == NULL && pos > 0)
	return success;
	if(pos == 0){	
	insert_begin(head,data);
	return success;
	}
	if(pos==size(head)){
	insert_end(head,data);
	return success;
	}
	temp->data = data;
	if(head == NULL){
		head = temp;
		head->element.prev = NULL;
		head->element.next = NULL;
		return success;	
	}
	
	for(int i =1;i<pos-1;i++){
		//if(pos == 1)
		
		temp1= (struct info*)temp1->element.next;	
	}
	
	if(pos == 1){
	temp->element.prev = (struct node*)temp1;
	temp->element.next = temp1->element.next ;
	temp1->element.next = (struct node*)temp;
	return success;
	}
	temp->element.prev= temp1->element.next ;
	temp1 = (struct info*)temp1->element.next;	
	temp->element.next = temp1->element.next;	
	temp2 = (struct info*)temp1->element.next;
	temp2->element.prev = (struct node*)temp;	
	
	temp1->element.next = (struct node*) temp;
		
		
	return success;
}


int32_t size(struct info* head){
	struct info * temp=head;
	int32_t i =0;
	if(head==NULL)
	return 0;
	while((temp->element.next) != NULL){
	temp = (struct info*)temp->element.next;
	i++;
	}
	return i+1;
}



dll_state te_del_begin(struct info* head){
	struct info * temp =head;	
	if(head == NULL){
		return success;	
	}	
	free(head);	
	temp->element.prev =NULL;	
	head=(struct info*)temp->element.next;
	return success;
}



dll_state te_del_end(struct info* head){
	struct info * temp =head;
	struct info * temp1;	
	if(head == NULL){
		return success;	
	}
	while(temp->element.next!=NULL){
	temp= (struct info*)temp->element.next;
	}	
	temp1 = (struct info*)temp->element.prev;
	temp1->element.next = NULL;
	free(temp);	
	return success;
}

dll_state te_del_pos(struct info* head,uint32_t pos){
	struct info * temp = head;
	struct info * temp1;
	struct info * temp2;		
	if(head == NULL)
	return success;
	if(pos == 1){	
	te_del_begin(head);
	return success;
	}
	if(pos==size(head)){
	te_del_end(head);
	return success;
	}
	while(pos>1){
		pos--;
		temp=(struct info*)temp->element.next;
	}
	temp2 = temp;	
	temp1 = (struct info*)temp->element.prev;
	temp =(struct info*) temp->element.next;
	temp->element.prev = (struct node*)temp1;	

	temp=(struct info*)temp2->element.prev; 
	temp->element.next = temp2->element.next;
	free(temp2);
	return success;
}

struct info* del_begin(struct info* head){
	struct info * temp =head;
	struct info * temp1;	
	temp1=(struct info*)temp->element.next;	
	if(head == NULL){
		return head;	
	}	
	temp1->element.prev =NULL;	
	head=temp1;
	free(temp);
	return head;
}
struct info* del_end(struct info* head){
	struct info * temp =head;
	struct info * temp1;	
	if(head == NULL){
		return head;	
	}
	while(temp->element.next!=NULL){
	temp= (struct info*)temp->element.next;
	}	
	temp1 = (struct info*)temp->element.prev;
	temp1->element.next = NULL;
	free(temp);	
	return head;
}

struct info * destroy_all(struct info* head){
	struct info * temp = head;
	while(temp->element.next !=NULL){
		temp =(struct info*) temp->element.next;
		free(temp);
	}
	head = 	NULL;
	return head;
}
struct info* insert_at_pos(struct info* head,uint32_t data,uint32_t pos){
	struct info * temp = (struct info*)malloc(sizeof(struct info));
	struct info * temp1 = head;
	struct info * temp2;	
	//malloc(sizeof(struct node));		
	if(head == NULL && pos > 0)
	return head;
	if(pos == 0){	
	head = insert_begin(head,data);
	return head;
	}
	if(pos==size(head)){
	head = insert_end(head,data);
	return head;
	}
	temp->data = data;
	if(head == NULL){
		head = temp;
		head->element.prev = NULL;
		head->element.next = NULL;
		return head;	
	}
	
	for(int i =1;i<pos-1;i++){
		//if(pos == 1)
		
		temp1= (struct info*)temp1->element.next;	
	}
	
	if(pos == 1){
	temp->element.prev = (struct node*)temp1;
	temp->element.next = temp1->element.next ;
	temp1->element.next = (struct node*)temp;
	return head;
	}
	temp->element.prev= temp1->element.next ;
	temp1 = (struct info*)temp1->element.next;	
	temp->element.next = temp1->element.next;	
	temp2 = (struct info*)temp1->element.next;
	temp2->element.prev = (struct node*)temp;	
	
	temp1->element.next = (struct node*) temp;
		
		
	
	return head;
}

struct info* del_pos(struct info* head,uint32_t pos){
	struct info * temp = head;
	struct info * temp1;
	struct info * temp2;		
	if(head == NULL)
	return head;
	if(pos == 1){	
	head = del_begin(head);
	return head;
	}
	if(pos==size(head)){
	head = del_end(head);
	return head;
	}
	while(pos>1){
		pos--;
		temp=(struct info*)temp->element.next;
	}
	temp2 = temp;	
	temp1 = (struct info*)temp->element.prev;
	temp =(struct info*) temp->element.next;
	temp->element.prev = (struct node*)temp1;	/* next node prev linked to previous */

	temp=(struct info*)temp2->element.prev; 
	temp->element.next = temp2->element.next;
	free(temp2);
	return head;
}		

void test_insert_begin()
{
        struct info* head;
        head = NULL;
        int32_t x;
        dll_state status;
        x = 5;
        head = insert_begin(head,x);
	if(head->data == x && head->element.prev == NULL)
	status = success;        
	else
	status = error;
	assert_int_equal(status,success);
}
void test_insert_begin2()
{
	struct info* head;
	head = NULL;
	int32_t x;
	for(int i=0;i<4;i++)
	{
		
		head = insert_begin(head,(i+3));		
	}        
	
        dll_state status;
        x = 5;
        head = insert_begin(head,x);
        if(head->data == x && head->element.prev == NULL)
	status = success;        
	else
	status = error;
	assert_int_equal(status,success);
}
void test_insert_end()
{
        struct info* head;
        head = NULL;
        int32_t x;
        dll_state status;
        x = 5;
        status = te_insert_end(head,x);
        assert_int_equal(status,success);
}
void test_insert_end2()
{
	struct info* head;
	head = NULL;
	int32_t x;
	for(int i=0;i<4;i++)
	{
		head = insert_begin(head,(i+3));		
	}        
	
        dll_state status;
        x = 5;
        head = insert_end(head,x);
	while(head->element.next!=NULL){
	head= (struct info*)head->element.next;
	}
	if(head->data == x && head->element.next == NULL)
	status = success;
	else
	status = error;
        assert_int_equal(status,success);
}

void test_insert_pos0()
{
        struct info* head;
        head = NULL;
        int32_t x,y;
        dll_state status;
        x = 5;
	y=0;
        status = te_insert_at_pos(head,x,y);
        assert_int_equal(status,success);
}
void test_insert_posn()
{
	struct info* head;
	head = NULL;
	int32_t x,y;
	for(int i=0;i<4;i++)
	{
		
		head = insert_begin(head,(i+3));		
	}        
	
        dll_state status;
        x = 25;
	y=2;
        head = insert_at_pos(head,x,y);
	for(int i=0;i<2;i++)
	{
		head = (struct info*)head->element.next;
	}
	if(head->data == 25)
	status = success;
	else
	status = error;
	
        assert_int_equal(status,success);
}

void test_insert_pos_last()
{
	struct info* head;
	head = NULL;
	int32_t x,y;
	for(int i=0;i<4;i++)
	{
		
		head = insert_begin(head,(i+3));		
	}        
	
        dll_state status;
        x = 5;
	y=4;
        status = te_insert_at_pos(head,x,y);
        assert_int_equal(status,success);
}

void test_del_begin()
{
        struct info* head;
        head = NULL;
        dll_state status;
        status = te_del_begin(head);
        assert_int_equal(status,success);
}
void test_del_begin2()
{
	struct info* head;
	head = NULL;
	uint32_t x;
	x =7;
	for(int i=0;i<4;i++)
	{
		head = insert_end(head,x);
		x = x+1;		
	}        
        dll_state status;
        head = del_begin(head);
	if(head->data == 8)
	status = success;
	else
	status = error;
	
        assert_int_equal(status,success);
}
void test_del_end()
{
        struct info* head;
        head = NULL;
        dll_state status;
        status = te_del_end(head);
        assert_int_equal(status,success);
}
void test_del_end2()
{
	struct info* head;
	head = NULL;
	uint32_t x;
	dll_state status;
	x = 7;
	for(int i=0;i<4;i++)
	{		
		head = insert_end(head,x);
		x = x +1;		
	}        
        
	
        head = del_end(head);
	while(head->element.next!=NULL){
	head= (struct info*)head->element.next;
	}
	if(head->data == 9)
	status = success;
	else
	status = error;
        assert_int_equal(status,success);
}

void test_del_pos0()
{
        struct info* head;
        head = NULL;
        int32_t x;
        dll_state status;
	x=0;
        status = te_del_pos(head,x);
        assert_int_equal(status,success);
}
void test_del_posn()
{
	struct info* head;
	head = NULL;
	int32_t x;
	for(int i=0;i<4;i++)
	{
		head = insert_begin(head,(i+3));		
	}        
        dll_state status;
        x = 3;
	head = del_pos(head,x);
	for(int i=0;i<2;i++)
	{
		head = (struct info*)head->element.next;
	}
	if(head->data == 3)  // list is 6,5,4,3 deleting at 3rd positon i.e 4 and checking at pos 3 for value
	status = success;
	else
	status = error;
     
        assert_int_equal(status,success);
}

void test_del_pos_last()
{
	struct info* head;
	head = NULL;
	int32_t x;
	for(int i=0;i<4;i++)
	{
		head = insert_begin(head,(i+3));		
	}        
        dll_state status;
        x = 4;
        status = te_del_pos(head,x);
        assert_int_equal(status,success);
}

void test_size()
{
        struct info* head;
        head = NULL;
        dll_state status;
        if(size(head)==0)
	status = success;
	else
	status=error;
        assert_int_equal(status,success);
}
void test_size1()
{
	struct info* head;
	head = NULL;
	for(int i=0;i<4;i++)
	{
		head = insert_begin(head,(i+3));		
	}        
        dll_state status;
        if(size(head)==4)
        status = success;
	else
	status=error;
        assert_int_equal(status,success);
}


void test_destroy_all()
{
	struct info* head;
	head = NULL;
	for(int i=0;i<4;i++)
	{	
		head = insert_begin(head,(i+3));		
	}        
	
        dll_state status;
	head  = destroy_all(head);
	 if( head == NULL)
	status = success;
	else
	status=error;
        assert_int_equal(status,success);
}
#endif

		
