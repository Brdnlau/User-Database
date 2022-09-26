# Design Document


## Purpose
*The purpose of this program is to implement a database that keeps track of Customers information using their emails as keys in a hashtable.*


## Structure
*The structure of this program will be as follows:*


### `businessdb.h`
*This header file contains the following:*

#### 1. `Customer`
This struct represents a LinkedList node, containing the information of each Customer, as well as where the next one in the bucket is.


#### 2. `Hashtable`
This struct contains the number of buckets of the hashtable as well as the buckets array.


#### 3. `businessdb.c` Function Signatures


### `businessdb.c`

#### 1. `hash`
This function returns an unsigned long to represent a string input. 

It is borrowed from Dan Bernstein. 


#### 2. `find_customer`
This function finds a target customer and returns false if it cannot be found in the specified hashtable and the a pointer to the Customer node if it is found.

First, the bucket the target shoul be in is quickly determined using the hash function modulo the number of buckets in the target hashtable. A while loop is then used to iterate through the bucket, using strcmp to check if the email of each node matches the email it is
looking for. If it is found, a pointer to the Customer is returned. After the loop, false is returned.  


#### 3. `add_customer`
This function uses the previously defined hash function and adds a new Customer struct to the top of the linked list in the bucket, setting its values to be the ones specified in the parameters. However, if the Customer already exists, it will edit its values instead.

The bucket the target is in is quickly determined first using the hash function modulo the number of buckets in the target hashtable. The `find_customer` function is then used to check if the email already exists. If it doesn't, a Customer struct is allocated memory via 
malloc and its values are assigned the inputed values, allocating memory for each string using strdup. Its next value then points the the first node in the bucket. If it does exist already, a Customer struct pointer is initialized and points to the existing customer given by `find_customer`. The previous strings are freed and new ones are assigned using strdup.


#### 4. `delete_customer`
This function finds and deletes the target Customer from the hashtable, freeing the memory allocated to it and makes the previous node point to the one after the target.

The `find_customer` function is first used in an if statement to check if the customer exists, and if it does not, it quickly returns. The bucket the target is in is then quickly determined using the hash function modulo the number of buckets in the target hash table.
A while loop will be used to iterate through the bucket and find the target, and the previous Customer before the target. Once both are determined, the previous node's next points to the node after the target. The target's string values, which contain previously allocated
memory from strdup, is then freed and set the NULL. The node itself is then freed and set the NULL.


#### 5. `print_customer_info`
This function takes in a Customer struct and prints out its values in a clean, formatted way.


#### 6. `list_customers`
This function iterates through a target hashtable and prints out all of the values from each of the nodes in all of the buckets.

A for loop is used to go through each index of the buckets array, and a while loop will be used to go through the linked list in each bucket. In each linked list node, `print_customer_info` will be called to print each node cleanly.

#### 7. `quit`
This function frees all of the memory used in the program, specifically the hashtable.

A for loop is used to go through each index of the buckets array, and a while loop is used to go through the linked list in each bucket. In each linked list noded, email, name, and favorite food strings are freed and their pointers are set to NULL. 
The node itself is then freed as well and set to NULL. The hashtable itself is then freed and set to NULL as well. 

### 8. `main`
This function manages the program, taking in commands to modify the hashtable and file. it will also read from the file initially to store into the hashtable, and have the ability to write the updated hashtable back into the file.

A buff variable is declared for input. A hashtable is then defined and allocated memory for using malloc. Using a for loop, each bucket is set to NULL. Temporary variables for the fields in Customer is then created. `customers.tsv` is is then opened with read
permissions. A while loop running while fgets() is not NULL reads each line in `customers.tsv`. sscanf is then used to format the buffer and assign them to the temporary variables. `add_customers` is then used to create a new Customer node for each line with the 
specified fields and add them to the hashtable. The file is then closed. Another while loop is used to ask for commands and execute them. A string command is called to store user input to compare. Strcmp is used to check with if statements to check if the user
inputted a valid command. If not, a nice message is printed telling them. For the command add, the user is prompted for the 4 customer fields and they are copied to the temporary variables set at the beginning using scanf. `add_customer` is then called. 
For the command lookup, scanf is again used to copy the email to the temporary email variable. `find_customer` is then called to check if the customer the user is looking for exists, if it does, `print_customer_info` is used to print the customer's data. 
For the command delete, scanf is again used to copy the email to the temporary email variable. `delete_customer` is then called. For the command list, `list_customers` is called. In save, `customers.tsv` is open with write permissions first. A for loop is used to iterate through the buckets and a while loop is used to go through the linked list in each bucket. fprintf is then used to write each line into the file. The file is then closed. For the command quit, the function `quit` is called and zero is returned.
