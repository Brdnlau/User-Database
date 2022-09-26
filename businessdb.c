#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "businessdb.h"


// Thank you Dan Bernstein.
unsigned long hash(char *str) {
  unsigned long hash = 5381;
  int c;

  while (*str != '\0') {
    c = *str;
    hash = ((hash << 5) + hash) + (unsigned char)c; /* hash * 33 + c */
    str++;
  }
  return hash;
}

Customer* find_customer(char *email, Hashtable* hashtable) {
	size_t bucket = hash(email) % hashtable->numBuckets;
	Customer* customer = hashtable->buckets[bucket];
	while(customer != NULL) {
		if (strcmp(customer->email, email) == 0) {
			return customer;
		}
		customer = customer->next;
	}
	return false;

}

void add_customer(char *email, char *name, int shoeSize, char *favFood, Hashtable* hashtable) {
	size_t bucket = hash(email) % hashtable->numBuckets;
	Customer *newCustomer;
	if (!find_customer(email, hashtable)){
		newCustomer = malloc(sizeof(Customer));
		newCustomer->email = strdup(email);
		newCustomer->name = strdup(name);
		newCustomer->shoeSize = shoeSize;
		newCustomer->favFood = strdup(favFood);
		newCustomer->next = hashtable->buckets[bucket];
		hashtable->buckets[bucket] = newCustomer;
		printf("Customer %s has been added to bucket %lu.\n", email, bucket);
	}
	else{
		newCustomer = find_customer(email, hashtable);
		free(newCustomer->name);
		newCustomer->name = strdup(name);
		newCustomer->shoeSize = shoeSize;
		free(newCustomer->favFood);
		newCustomer->favFood = strdup(favFood);
		printf("Customer %s has been edited in bucket %lu.\n", email, bucket);
	}
}

void delete_customer(char *email, Hashtable *hashtable){
	if (!find_customer(email,hashtable)){
		printf("Customer not found.\n");
		return;
	}
	size_t bucket = hash(email) % hashtable->numBuckets;
	Customer *previous = hashtable->buckets[bucket];
	Customer *delete = NULL;
	while(previous != NULL){
		if (strcmp(previous->email, email) == 0){
			delete = previous;
			hashtable->buckets[bucket] = hashtable->buckets[bucket]->next;
			break;
		}
		if (strcmp(previous->next->email, email) == 0){
			delete = previous->next;
			previous->next = delete->next;
			break;
		}
		previous = previous->next;
	}
	free(delete->email);
	delete->email = NULL;
	free(delete->name);
	delete->name = NULL;
	free(delete->favFood);
	delete->favFood = NULL;
	free(delete);
	delete = NULL;
	printf("Customer %s has been deleted.\n", email);
}	

void print_customer_info(Customer* customer){
	printf("\nEmail: %s\nName: %s\nShoe Size: %d\nFavorite Food: %s\n---\n", customer->email, customer->name, customer->shoeSize, customer->favFood);
}

void list_customers(Hashtable *hashtable){
	for (size_t i=0; i < hashtable->numBuckets; i++){
		//printf("Listing bucket %zu\n", i);
		Customer* customer = hashtable->buckets[i];
		while(customer != NULL){
			print_customer_info(customer);
			customer = customer->next;
		}
	}
}

void quit(Hashtable *hashtable){
	for (size_t i = 0; i < hashtable->numBuckets; i++){
		Customer *current = hashtable->buckets[i];
		while(current != NULL){
			Customer *delete = current;
			current = current->next;
			free(delete->email);
        		delete->email = NULL;
        		free(delete->name);
        		delete->name = NULL;
        		free(delete->favFood);
        		delete->favFood = NULL;
        		free(delete);
        		delete = NULL;
		}
		hashtable->buckets[i] = NULL;
	}
	free(hashtable);
	hashtable = NULL;
}

int main(void){
	char buff[512];

	// create hashtable
	Hashtable* database = malloc(sizeof(Hashtable));
	database->numBuckets = 10;
	for (size_t i = 0; i < database->numBuckets; i++){database->buckets[i] = NULL;} 

	// temp vars
	char email[320], name[76], favFood[50];
        int shoeSize = 0;

	// read file and store into hashtable
	FILE *infile = fopen("customers.tsv", "r");
	while((fgets(buff, 512, infile) != NULL)){
		sscanf(buff, "%s\t%[^\t]\t%d\t%[^\n]", email, name, &shoeSize, favFood);
		add_customer(email, name, shoeSize, favFood, database);
	}
	fclose(infile);

	// ask for commands and call appropriate functions until quit
	while(true){
		char command[7];
		printf("Enter Command: ");
		scanf("%6s", command);

		if (strncmp(command, "add", 6) == 0){
			printf("Enter email address: ");
			scanf("%s", email);
			printf("Enter name: ");
			scanf(" %75[^\n]s", name);
			printf("Enter shoe size: ");
			scanf("%d", &shoeSize);
			printf("Enter favorite food: ");
			scanf(" %49[^\n]s", favFood);
			add_customer(email,name,shoeSize,favFood,database);
		}
		else if (strncmp(command, "lookup", 6) == 0){
			printf("Enter email address: ");
			scanf("%s", email);
			if (find_customer(email,database)){
				Customer* customer = find_customer(email,database);
				print_customer_info(customer);
			}
			else{
				printf("Customer not found.\n");
			}
			
		}
		else if (strncmp(command, "delete", 6) == 0){
			printf("Enter email address: ");
			scanf("%s", email);
			delete_customer(email, database);
		}
		else if (strncmp(command, "list", 6) == 0){
			list_customers(database);
		}
		else if (strncmp(command, "save", 6) == 0){
			infile = fopen("customers.tsv", "w");
			for (size_t i=0; i < database->numBuckets; i++){
                		Customer* customer = database->buckets[i];
                		while(customer != NULL){
					fprintf(infile, "%s\t%s\t%d\t%s\n", customer->email, customer->name, customer->shoeSize, customer->favFood);
					customer = customer->next;
				}
			}
			fclose(infile);
		}
		else if (strncmp(command, "quit", 6) == 0){
			quit(database);
			return 0;
		}
		else{
			printf("Unknown Command.\n");
		}
	}
}
