

Documentation for Unit testing regading Implementation:

Insert at Begin : For Success - I checked if data that i passed is stored and node previous pointer to be null 	
			Error if this operation fails
		I have checked for two conditins when head is null and head is pointing to a list that conatins elements.

Insert at End : For success : Returned head pointer is traversed till NULL and also checked for data that is linked to that node and
				if both is verified we can say that operation is successful.

Insert at Position : For Success : For this since we verified for beginning and last position i only implemenetd test case to insert at a 						position in between nodes.
				I checked for data i have sent after traversing head node for number of positions equal to positon value that 					i sent and hence it is verified for inserting at right position with proper links.


Delete at Begin :For Success : For this i created a list with known data values.  Since i am deleting node at beginning . I checked for data 					value with head pointer which is equal to second value in the list created earlier and hence verified.

Delete at End : For Success : For this i created preknown elements list . after deleting a node and returning head pointer , i made head to 				traverse till it sees null and checking data for reuqired list element results in success operation.

Delete at Postion :  For success : Same as insert at pos - i checked only for middle link .
				Preknown list of nodes and data elements is made. For eg : For list of 4 elements (Data : 3,4,5,6)
				i deleted node at 3 rd position and after returning i traversed two times and checked for data value of 				6 in this case and is verified as success if it is equal.

Size : Sucess : I made a known number of elements . I compared the return value to this  and hence it is verified.

Destroy all : Head pointer is checked for Null value.
