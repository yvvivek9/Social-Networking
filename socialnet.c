#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include<stdbool.h>
#define MAX 100

typedef struct node 
{
    int id; //ID of user
    int numfren; //number of friends of user
    char name[MAX]; //name of user
    int* friends; //friends of user as an array
    struct node* right;  //user to the right
    struct node* left; //user to the left
} node;

struct node* retUser(char str[MAX])
{
    char name[MAX];
    char ID[MAX];
    char strfriends[MAX];

    //copied ID
    char*token=strtok(str,",");
    strcpy(ID,token);

    //copied Name
    token=strtok(NULL,",");
    strcpy(name,token);

    //copied friends' ID
    token=strtok(NULL,",");
    strcpy(strfriends,token);
    //printf("%s\n",(strfriends));

    //creating user nodes
    int id=atoi(ID);
    struct node *user = malloc(sizeof(struct node));
    user->id=id;
    user->friends=(int*)malloc(MAX * sizeof(int));
    strcpy(user->name,name);


    //adding user's friend's IDs
    token = strtok(strfriends,"|");  
    int i=0;
    while( token != NULL ) 
    {

        int temp=atoi(token);
        user->friends[i]=temp;
        i++;
        token = strtok(NULL,"|");
    }
    user->numfren=i;
    if(i==0){user->friends[i]=-1;}
    return user;
}

//search for user with id=key
struct node* search(int key, struct node *users)
{
    node* searched = NULL;
    if(users != NULL){
        if(users->id == key)
            return users;
        searched = search(key, users->left);
        if(searched != NULL)
            return searched;
        searched = search(key, users->right);
        if(searched != NULL)
            return searched;
    }
    return searched;
}

bool checkarray(int* array, int key)  
{
    for(int i = 0; i < 100; i++){
        if(array[i] == key)
            return true;
    }
    return false;
}

//see document for explanattion
struct node* refineUser(struct node*user, struct node *users)
{
    int* frnds = (int*)malloc(MAX*sizeof(int));
    int num = 0;
    node* searched;
    while(search(user->id, users) != NULL)
        user->id++;
    for(int i = 0; i < user->numfren; i++){
        searched = search(user->friends[i], users);
        if(searched != NULL){
            frnds[num++] = user->friends[i];
            if(searched->friends[0] == -1){
                searched->friends[0] = user->id;
                searched->numfren = 1;
            }
            else if(checkarray(searched->friends, user->friends[i]) == false)
                searched->friends[searched->numfren++] = user->id;
        }
    }
    if(num == 0){
        user->friends = frnds;
        user->friends[0] = -1;
        user->numfren = 0;
    }
    else{
        user->friends = frnds;
        user->numfren = num;
    }
    return user;
}

//insert user with id
struct node* insertUser(struct node*root,int id,struct node*user)
{
    node* temp;
    node* returr = root;
    user->left = NULL;
    user->right = NULL;
    if(root == NULL)
        return user;
    else{
        while(root != NULL){
            temp = root;
            if(root->id < user->id)
                root = root->right;
            else
                root = root->left;
        }
        if(temp->id < user->id)
            temp->right = user;
        else
            temp->left = user;
        return returr;
    }
    
}

//prints friends list
void friends(int id, struct node *users) 
{
    node* searched = search(id, users);
    if(searched->numfren == 0)
        printf("%d", searched->friends[0]);
    else{
        for(int i = 0; i < searched->numfren; i++)
            printf("%d", searched->friends[i]);
    }
}

//find child node with minimum value (inorder successor) - helper to delete node
struct node *minValueNode(struct node *node)
{
    struct node* current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}

//deletes itself from its friend's nodes
struct node *deleteFriends(int key, struct node*users)
{
    if(users != NULL){
        int* frnds = (int*)malloc(MAX*sizeof(int));
        int num = 0;
        if(checkarray(users->friends, key) == true){
            for(int i = 0; i < users->numfren; i++){
                if(users->friends[i] != key)
                    frnds[num++] = users->friends[i];
            }
            if(num == 0){
                users->friends[0] = -1;
                users->numfren = 0;
            }
            else{
                users->friends = frnds;
                users->numfren = num;
            }
        }
        deleteFriends(key, users->left);
        deleteFriends(key, users->right);
    }
}

// Deleting a node
struct node *deleteNode(struct node *root, int key)
{
    if (root == NULL)
        return root;
  
    if (key < root->id)
        root->left = deleteNode(root->left, key);
  
    else if (key > root->id)
        root->right = deleteNode(root->right, key);
  
    else {
        if (root->left == NULL) {
            struct node* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            struct node* temp = root->left;
            free(root);
            return temp;
        }
  
        struct node* temp = minValueNode(root->right);
        root->id = temp->id;
        strcpy(root->name, temp->name);
        root->friends = temp->friends;
        root->numfren = temp->numfren;

        root->right = deleteNode(root->right, temp->id);
    }
    return root;
}

//Print USER's IDs in ascending order
void printInOrder(node* myusers) 
{
    if(myusers != NULL){
        printInOrder(myusers->left);
        printf("%d", myusers->id);
        printInOrder(myusers->right);
    }
}


int main(int argc, char **argv)
{
    node *users=NULL;   
    while(1)
    {

        int opt, id;
        fflush(stdin);
        scanf("%d",&opt);
        char str[MAX];
        switch (opt)
        {
            case 1:
      
                scanf("%s",str);
                struct node*tbins=retUser(str);
                tbins=refineUser(tbins, users);
                users=insertUser(users,tbins->id,tbins);
                break;

            case 2:
           
                scanf("%d", &id);
                deleteFriends(id, users);
                users=deleteNode(users, id);
                break;

            case 3:
        
                scanf("%d", &id);
                node* result=search(id,users);
                if(result==NULL) 
                    printf("USER NOT IN TREE.\n");
                else{
                    printf("%d\n",result->id);
                }
                break;

            case 4:
                scanf("%d", &id);
                friends(id, users);
                break;

            case 5:
                printInOrder(users);
                break;

            case 6:
                exit(0);
                break;

            default: 
                printf("Wrong input! \n");
                break;
        }

    }
    return 0;
}