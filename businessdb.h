#pragma once

#define NUM_BUCKETS 10

typedef struct Customer{
	char *email;
	char *name;
	int shoeSize;
	char *favFood;
	struct Customer *next;
} Customer;

typedef struct Hashtable{
	size_t numBuckets;
	Customer* buckets[NUM_BUCKETS];
} Hashtable;

unsigned long hash(char *str);

Customer* find_customer(char *email, Hashtable* hashtable);

void add_customer(char *email, char *name, int shoeSize, char *favFood, Hashtable* hashtable);

void delete_customer(char *email, Hashtable *hashtable);

void print_customer_info(Customer* customer);

void list_customers(Hashtable *hashtable);

void quit(Hashtable *hashtable);
