#include <string.h>
#include <stdio.h>
#include "jsmn.h"
#include <stdarg.h>
    

void insert(char *str, char *pch, int pos); 
static int jsoneq(const char *json, jsmntok_t *tok, const char *s);


void json_set(char json[], ...)
{
    va_list valist;
    jsmn_parser parser;
    jsmntok_t t[500];
    int args_num=0;
	char* parent;
    char* key;
    char* value;
    char* tmp;
	
	
    jsmn_init(&parser);
    int item_number = jsmn_parse(&parser, json, strlen(json), t, 500);

    printf("\nitem_number: %d\n", item_number);
    va_start(valist, json);
    while(1){
        value = tmp;
        tmp = va_arg(valist, char*);
        if(strcmp(tmp, "")==0){
            break;
        }args_num++;
        key = value;
    }
	
    char additem[20];
    char newitem[100];
    //strcpy(newitem, ",");
    strcpy(newitem, "\"");
    strcat(newitem, key);
    strcat(newitem, "\"");
    strcat(newitem, ":");
    strcat(newitem, "\"");
    strcat(newitem, value);
    strcat(newitem, "\"");
    //strcat(newitem, "}");
    //strcat(newitem, "\0");
	

	args_num = args_num - 2;
	

	if(0 == args_num){
		if(0 != t[0].size)
			insert(newitem, ",", 0); 
		insert(json, newitem, t[0].end-1); 
	}
	
		int itemIndex = 0;
	int itemStartPosition = 0;
	int itemEndPosition = strlen(json);
	int break_tag = 0;
	
	
	JUMP_HERE:
	itemIndex = 0;
	itemStartPosition = 0;
	itemEndPosition = strlen(json);
	    jsmn_init(&parser);

	item_number = jsmn_parse(&parser, json, strlen(json), t, 500);
	//break_tag = 0;
    va_start(valist, json);
	for(int p=0; p<args_num; p++){
		break_tag = 0;
		parent = va_arg(valist, char*);
		
		for(int i=itemIndex; i<item_number; i++){
			itemIndex = i;
			if(t[i].start <= itemStartPosition){
				printf("start index %d\n", itemIndex);
			}else
			if(t[i].start >= itemEndPosition){
				printf("End index %d\n", itemIndex);
			}else
			if(jsoneq(json, &t[i], parent) == 0){  //into
				printf("\nfound parent-%s\n", parent);
				itemStartPosition = t[i+1].start;
				itemEndPosition = t[i+1].end;
				if(p == args_num-1){
					if(0 != t[i+1].size)
						insert(newitem, ",", 0); 
					insert(json, newitem, t[i+1].end-1); 
						printf("\n%s\n", json);

					return;
				}
				//if(break_tag)return;
				break_tag = 1;
				break;
			}else{
				itemStartPosition = t[i+1].end;
			}
		}


		if(!break_tag){  //first parent add
			printf("out index %d\n", t[itemIndex-1].end-t[itemIndex-1].start);
			
			
			//printf("- User: %.*s\n", 2,json + t[itemIndex].end-2);
			
							//printf("\n%s\n", json);

			if(json[t[itemIndex].end-2] == '{' || json[t[itemIndex].end-2] == '[')
				sprintf(additem, "\"%s\":{}", parent);
			else
				sprintf(additem, ",\"%s\":{}", parent);
			printf("%s\n", additem);
			
			insert(json, additem, itemEndPosition-1); 
				printf("\n%s\n", json);
				goto JUMP_HERE;
		}
	
		
	}

	
	
    va_end(valist);
	printf("\n%s\n", json);

}




int main()
{
	
    char json[500] = "{\"a\":{\"a\":\"aa\", \"b\":{\"c\":\"ccc\"}, \"c\":[\"c1\", \"c2\"]}, \"b\":[22, 33], \"c\":{\"d\":\"ddd\"}}";
    

	char j[200] = "{}";
	
    //json_set(json, "a", "b", "k", "vvvvvvvvvvvv", "");
    json_set(j, "a", "e", "k", "vvvvvvvvvvvv", "");
    //json_set(json, "k", "vvvvvvvvvvvv", "");
    //json_set(json, "m", "n", "k", "vvvvvvvvvvvv", "");

    //if(t.start!=0 && t.end!=0)
        //printf("eeee%.*s\n", t.end-t.start, json + t.start);


//    printf("%s",(char*)jsmn_get(json, "c", "d", "")); 
    return 0;

}


void insert(char *str, char *pch, int pos) 
{
    int len = strlen(str);
    int nlen = strlen(pch);
    for (int i = len - 1; i >= pos; --i) {
        *(str + i + nlen) = *(str + i);
    }
    for (int n = 0; n < nlen;n++)
        *(str + pos + n) = *pch++;
    *(str + len + nlen) = 0;
}


static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}


