// Implements a dictionary's functionality

#include <stdbool.h>
#include "dictionary.h"

#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

//represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// number of hash buckets
const unsigned int hash_buckets = 2000;
// create hash table
node* words[hash_buckets];

//initialize variables and functions
bool loaded = false;
node* insert(node** head, const char *value);
node* find(node* head[hash_buckets], const char *value);
unsigned int hash(const char *value);
void clearNode(node* head[hash_buckets]);

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //open a file
    char str[LENGTH +1];
    FILE *fp;
    fp = fopen(dictionary,"r");
    if (!fp){
        return false;
    }

    //save each line in our hashtable
    const char r = '\n';
    char *token = NULL;
    node * tmp;

    while (fscanf(fp, "%s", str) != EOF){
        (void) insert(words, str);
    }

    //close
    fclose(fp);
    loaded = true;
    return true;
}

// Checks dictionary for word from text. Returns true if word is in dictionary else false
bool check(const char *searchTerm)
{
    //search dict hastable looking for match
    node* searchResponse = find(words, searchTerm);
    if(searchResponse != NULL){
        return true;
    }
    return false;
}


// counts and returns the number of words in dictionary. If dictionary is not loaded, returns 0
unsigned int size(void)
{
    if (!loaded){
        return 0;
    }

    int count = 0;
    for (int i = 0; i < hash_buckets; i++)
    {
        node *tmp = words[i];

        while (tmp != NULL)
        {
            count ++;
            tmp = tmp->next;
        }
    }
    return count;
}

// Unloads dictionary from memory
bool unload(void)
{
    //clear dictionary words hashtable
    clearNode(words);
    return true;
}


//insert a new item to our hashtable
node* insert(node** head, const char *value){
    //allocate space for node
    node *n = malloc(sizeof(node));
    if (n == NULL)
    {
        printf("Can't malloc new node");
        exit(1);
    }

    //copy the string in
    strcpy(n -> word, value);

    //get hash value
    unsigned int hash_value = hash(value);

    //set next to point to location on hashtable(hash_value)
    n -> next = words[hash_value];

    //redirect hashtable(hash_value) to our new node
    words[hash_value] = n;

    return words[hash_value];
}

//hashing function (as found here http://www.cse.yorku.ca/~oz/hash.html)
unsigned int hash(const char *value){
    unsigned int hash_value = 0;
    int c;
    while ((c = *value ++)){
        c = tolower(c); //ensure character is lowercase to eliminate case sesativity from spelling
        hash_value = c + (hash_value << 6) + (hash_value << 16) - hash_value; //apply hash(i) = hash(i-1) * 65599 + str[i]
    }
    //return hash_value mod hash_buckets
    return hash_value % hash_buckets;
}

//find a value in a our hash table
node* find(node* head[hash_buckets], const char *value){
    //get hash value
    unsigned int hash_value = hash(value);

    node *currentNode = head[hash_value];
    while (currentNode != NULL)
    {
        if(strcasecmp(currentNode -> word, value) == 0)
        {
            break;
        }
        currentNode = currentNode->next;
    }
    return currentNode;
}

//clear memory of hashtable/nodes
void clearNode(node* head[hash_buckets]){
    for (int i = 0, n = hash_buckets; i < n; i++)
    {
        node *tmp = head[i];

        while (tmp != NULL)
        {
            node *tmp_lead = tmp->next;
            free(tmp);
            tmp = tmp_lead;
        }
    }
}