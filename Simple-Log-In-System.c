/*************************************************************************
        Copyright © 2021 Konstantinidis Konstantinos
	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at
        http://www.apache.org/licenses/LICENSE-2.0
	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*************************************************************************/
#include <stdio.h>
#include <ctype.h>

#define HMax 10
#define VMax 100
#define EndOfList -1

typedef struct{
	char RecKey[8];
	char password[6];
	int Link;
} ListElm;

typedef struct{
	int HashTable[HMax];
    int Size;
	int SubListPtr;
    int StackPtr;
	ListElm List[VMax];
} HashListType;

typedef enum{
    FALSE, TRUE
} boolean;

void CreateHashList(HashListType *HList);
int HashKey(char Key[]);
boolean FullHashList(HashListType HList);
void SearchSynonymList(HashListType HList,char KeyArg[],int *Loc,int *Pred);
void SearchHashList(HashListType HList,char KeyArg[],int *Loc,int *Pred);
void AddRec(HashListType *HList,ListElm InRec);
int findAverage(char s[]);
void BuildHashList(HashListType *Hlist);

main(){
    char choice;
    HashListType Hlist;
    ListElm Item;
    int Loc, Pred;

    BuildHashList(&Hlist);
    do{
        printf("New entry Y/N (Y=Yes, N=No)\n");
        scanf(" %c", &choice);
    }
    while(choice!='N' && choice!='Y');

    if(choice == 'Y'){
        do{
            printf("USERNAME: ");
            scanf("%s", Item.RecKey);
            getchar();
            printf("\n");

            printf("PASSWORD: ");
            scanf("%s", Item.password);
            getchar();
            printf("\n");

            SearchHashList(Hlist, Item.RecKey, &Loc, &Pred);
            if(Loc != -1){
                if(strcmp(Hlist.List[Loc].password, Item.password)==0)
                    printf("You have logged in to the system\n");
                else
                    printf("Access is forbidden: Wrong password\n");
            }
            else
                printf("Access is forbidden: Wrong user ID\n");

            do{
                printf("New entry Y/N (Y=Yes, N=No)\n");
                scanf(" %c", &choice);
            }
            while(choice!='N' && choice!='Y');
        }while(choice != 'N');
    }

    system("PAUSE");
}

//A method to get the HashKey
int HashKey(char Key[]){
    int average;

    average = findAverage(Key);
	return average % 10; //The list will have 10 positions
}

int findAverage(char s[]){
    int length;
    length = strlen(s);
    return ((s[0] + s[length - 1])/2); //return = (ASCII code of 1st char + ASCII code of last char) / 2
}

//A method to create a Hash List
void CreateHashList(HashListType *HList){
	int index;

	HList->Size=0;
	HList->StackPtr=0;
   	index=0;
	while (index<HMax){
		HList->HashTable[index]=EndOfList;
		index=index+1;
	}

   	index=0;
	while(index < VMax-1){
		HList->List[index].Link = index + 1;
		strcpy(HList->List[index].password, "0");
		strcpy(HList->List[index].RecKey, "0");
		index=index+1;
	}
	HList->List[index].Link=EndOfList;
}

//A method to see if the list is full
boolean FullHashList(HashListType HList){
	return(HList.Size==VMax);
}

//A method to search for synonyms in the hash list
void SearchSynonymList(HashListType HList, char KeyArg[8], int *Loc, int *Pred){
	int Next;
	Next=HList.SubListPtr;
	*Loc=-1;
	*Pred=-1;
	while(Next!=EndOfList){
		if(strcmp(HList.List[Next].RecKey,KeyArg) == 0){
			*Loc=Next;
            Next=EndOfList;
		}
		else{
			*Pred=Next;
			Next=HList.List[Next].Link;
		}
	}
}

//A method to search inside the hash list
void SearchHashList(HashListType HList,char KeyArg[],int *Loc,int *Pred){
	int HVal;
	HVal=HashKey(KeyArg);
    if(HList.HashTable[HVal]==EndOfList){
		*Pred=-1;
		*Loc=-1;
	}
	else{
		HList.SubListPtr=HList.HashTable[HVal];
		SearchSynonymList(HList,KeyArg,Loc,Pred);
	}
}

//A method to add a new record in the list
void AddRec(HashListType *HList,ListElm InRec){
	int Loc, Pred, New, HVal;

	if(!(FullHashList(*HList))){
		Loc=-1;
		Pred=-1;
		SearchHashList(*HList,InRec.RecKey,&Loc,&Pred);
		if(Loc==-1){
			HList->Size=HList->Size +1;
			New=HList->StackPtr;
			HList->StackPtr=HList->List[New].Link;
			HList->List[New]=InRec;
			if (Pred==-1){
			    	HVal=HashKey(InRec.RecKey);
              			HList->HashTable[HVal]=New;
				HList->List[New].Link=EndOfList;
			}
			else{
				HList->List[New].Link=HList->List[Pred].Link;
				HList->List[Pred].Link=New;
			}
		}
		else
			printf("There is already someone with the same key \n");
	}
	else
		printf("Full list...");
}

//A method to create the hash list, read the file and insert the records in the list
void BuildHashList(HashListType *Hlist){
    ListElm Item;
    FILE *fp;
    int nscan;

    CreateHashList(&(*Hlist)); //Create the Hash list

    fp = fopen("I5f6.txt", "r"); //Read the file
    if(fp == NULL)
        printf("Problem opening file");
    else{
        while(TRUE){
            nscan = fscanf(fp, "%s %s", Item.RecKey, Item.password);
            if (nscan == EOF)
                break;
            if (nscan != 2){
                printf("Improper file format\n");
                break;
            }
            else
                AddRec(Hlist, Item); //Add each recording in the list
        }
    }
}
