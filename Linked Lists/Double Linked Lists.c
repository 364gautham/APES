/*
* This code implements Doubly linked list data structure
* using two structures defined as below
* Author : Gautham K A
* Date : 4th Feb 2018
*/



#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
		
struct node{
	struct node *prev;
	struct node *next;
};

struct info{
	uint32_t data;
	struct node element;
};

/*
*Del Begin functions deletes node at the beginning of the list
* @ input : Head pointer
* return : Head Pointer
*/
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

/*
*Del end function deletes node at the end of the list
* @ input : Head pointer
* return : Head Pointer
*/
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

/*
*Del at position  function deletes node at the particular postion of the list
* with head pointer position being 1.
* @ input : Head pointer
* return : Head Pointer
*/

struct info* del_pos(struct info* head,uint32_t pos){
	struct info * temp1=head;
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
	while(pos && temp1->next != NULL){
		temp1 =temp1->next;
		pos--;
	}
	temp2=temp1->prev;
	temp2->next=temp1->next;
	temp2=temp1->next;
	temp2->prev=temp1->prev;
	
	free(temp1);
	return head;
}

/*
*Insert Begin function inserts node at the beginning of the list
* @ input : Head pointer , data to add
* return : Head Pointer
*/
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


/*
*Insert end function inserts node at the end of the list
* @ input : Head pointer , data to add
* return : Head Pointer
*/
struct info* insert_end(struct info* head,uint32_t data){
	struct info * temp = (struct info*)malloc(sizeof(struct info));
	struct info * temp1 = head;	
	//malloc(sizeof(struct node));	
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


/*
*Insert at  postion  function inserts node at the respective position from the list
* Head at position 1 : Given Position 1  inserts node after 1st node
* Position zero inserts at beginning
* @ input : Head pointer, data and position to add at
* return : Head Pointer
*/
struct info* insert_at_pos(struct info* head,uint32_t data,uint32_t pos){
	
	//malloc(sizeof(struct node));		
	if(head == NULL && pos > 0)
	return head;
	if(head == NULL){
		if(pos==0){
			struct info * temp = (struct info*)malloc(sizeof(struct info));
			temp->prev=NULL;
			temp->next=NULL:
			return temp;
		}
		else
			return head;
	}
	
	struct info * temp = (struct info*)malloc(sizeof(struct info));
	struct info * temp1 = head;
	struct info * temp2;
	
	while((pos-1) && temp1->next != NULL){
		temp1 =temp1->next;
		pos--;
	}

	/*If given position exists, insert the node and return head node pointer*/
	if(temp1->next!= NULL)
	{
		temp->data=data;
		temp->next = temp1->next;
		temp1->next->prev = temp;
		temp1->next = temp;
		temp->prev = temp1;
		return head;
	}
	else
	{
		/*If position doesnt exist, return NULL*/
		printf("Given position doesn't exist\n");
		return NULL;
	}

}
/*
* functions to verify traversal and proper data storage
*/
void Print(struct info* head){
	struct info *temp = head;
	printf("List isss:\n");
	while(temp!=NULL)
	{
		printf("%d",temp->data);
		temp = (struct info*)temp->element.next;
	}
}
void Print_rev(struct info* head){
	struct info * temp = head;
	if(temp ==NULL)
	return;
	while(temp->element.next != NULL)
	{
		temp = (struct info*)temp->element.next;
	}

	printf("Revserse\n");
	while(temp != NULL)
	{
		printf("%d",temp->data);
		temp = (struct info*)temp->element.prev;
	}
}



/*
*Destroy all function removes all nodes of the list by freeing memory
* @ input : Head pointer
* return : Head Pointer
*/
struct info * destroy_all(struct info* head){
	struct info * temp = head;
	while(temp->element.next !=NULL){
		temp =(struct info*) temp->element.next;
		free(temp);
	}
	//free(temp);
	return head;
}

/*
*Size function gives number of links in the list
* @ input : Head pointer
* return : integer
*/
int32_t size(struct info* head){
	struct info * temp=head;
	int32_t i =0;
	if(head == NULL)
	return 0;
	while((temp->element.next) != NULL){
	temp = (struct info*)temp->element.next;
	i++;
	}
	return i+1;
}
/*
* Main function is used for initialising head pointer and 
* checking the working of function using print fucntions if required
*/
int main(){
	struct info* head;
	head = NULL;
	int32_t x,y,num;
	for(int i=0;i<4;i++)
	{
		printf("enter data\n");
		scanf("%d",&x);
		head = insert_end(head,x);
		Print(head);
			
	}
	
		//printf("enter pos\n");
		//scanf("%d",&y);
		head = del_begin(head);
		//printf("enter data\n");
		//scanf("%d",&x);		
		//printf("enter pos\n");
		//scanf("%d",&y);		
		//head = insert_at_pos(head,x,y);
		//destroy_all(head);
		//num=size(head);		
		//printf("\n%d\n",num);		
		Print(head);
	
}
	
	
		
