#include <string.h>
#include <stdio.h>
#include "jsmn.h"
#include <stdarg.h>
    

void insert(char *str, char *pch, int pos); 
static int jsoneq(const char *json, jsmntok_t *tok, const char *s);


char* json_set(char* json, ...)
{
    va_list valist;
    jsmn_parser parser;
    jsmntok_t t[50];
    jsmntok_t token;
    int args_num=0;

    static char backJson[500] = {'\0'};


    strcpy(backJson, json);

    memset(&token, 0, sizeof(token));


    jsmn_init(&parser);
    int item_number = jsmn_parse(&parser, json, strlen(json), t, 50);

    
    printf("\n%d\n\n", item_number);

    va_start(valist, json);
    char* key;
    char* value;
    char* tmp;
    while(1){
        value = tmp;
        tmp = va_arg(valist, char*);
        if(strcmp(tmp, "")==0){
            break;
        }args_num++;
        key = value;
    }

    printf("\nkey:%s, val:%s, tmp:%s\n", key, value, tmp);

    char newitem[50];
    strcpy(newitem, ",");

    strcat(newitem, "\"");
    strcat(newitem, key);
    strcat(newitem, "\"");
    strcat(newitem, ":");
    strcat(newitem, "\"");
    strcat(newitem, value);
    strcat(newitem, "\"");
    //strcat(newitem, "}");
    //strcat(newitem, "\0");


    args_num = args_num - 2;

    if(args_num == 0){
        insert(backJson, newitem, t[0].end-1); 
        printf("\n%s\n", backJson);
        va_end(valist);
        return backJson;
    }

    va_start(valist, json);
    printf("\nargs_num:%d\n", args_num);

    int itemIndex=0;
    int itemPosition=0;
    for(int n=0; n<args_num; n++){
        char* a = va_arg(valist, char*);
        for(int i=itemIndex; i<item_number; i++){
            printf("\nitem: %d--%d", i, n);
            if(t[i].start < itemPosition){
                printf("\n%d--%d\n", i, n);
                continue;
            }


            if(t[i+1].type == JSMN_OBJECT){
                printf(" find object\n");
                if(jsoneq(backJson, &t[i], a) == 0){
                    if(n == args_num - 1){ //the last layer

                        insert(backJson, newitem, t[i+1].end-1); 
                        va_end(valist);

                        printf("\n%s\n", backJson);
                        return backJson;
                    }
                    
                    itemPosition = t[i+1].start;
                    itemIndex = i;

                    printf(" \nnot this\n");
                    break;

                }else{
                    itemPosition = t[i+1].end;
                    
                    printf("\nitemPosition:%d\n", itemPosition);
                itemIndex = i;
                continue;
                }
            }else
            if(t[i+1].type == JSMN_ARRAY){
            }else{

                printf("%s\n", a);
                printf("%.*s\n", t[i].end-t[i].start, json + t[i].start);
                if(jsoneq(backJson, &t[i], a) == 0){
                    printf("dddddddddd");

                    
                    //insert(backJson, a, t[i+1].end); 

                    
                    printf("\n%s\n", backJson);
                    //printf("%.*s\n", t[i+1].end-t[i+1].start, json + t[i+1].start);
                    va_end(valist);
                }
            }


        }


    }


    va_end(valist);
    return backJson;
}




int main()
{
    char* json = "{\"a\":{\"a\":\"aa\", \"b\":{\"c\":\"ccc\"}, \"c\":[\"c1\", \"c2\"]}, \"b\":[22, 33], \"c\":{\"d\":\"ddd\"}}";
    
    //printf("%s", json);


    printf("\n%s\n", json_set(json, "a", "k", "v", ""));

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


