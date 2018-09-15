#include <string.h>
#include <stdio.h>
#include "jsmn.h"
#include <stdarg.h>
    

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

char* jsmn_get(char* json, ...)
{
    va_list valist;
    jsmn_parser parser;
    jsmntok_t t[50];
    jsmntok_t token;
    int args_num=0;
    static char r[50];



    memset(&token, 0, sizeof(token));


    jsmn_init(&parser);
    int item_number = jsmn_parse(&parser, json, strlen(json), t, 50);

    
    printf("\n%d\n\n", item_number);

    va_start(valist, json);
    while(1){
        if(strcmp(va_arg(valist, char*), "")==0){
            break;
        }args_num++;
    }

    va_start(valist, json);
    printf("\nargs_num:%d\n", args_num);

    int itemIndex=1;
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
                    if(jsoneq(json, &t[i], a) == 0){
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
                }
                if(jsoneq(json, &t[i], a) == 0){
                    printf("dddddddddd");
                    strncpy(r, json + t[i+1].start, t[i+1].end-t[i+1].start);
                    //printf("%.*s\n", t[i+1].end-t[i+1].start, json + t[i+1].start);
                    printf("\n%s\n", r);
                    va_end(valist);
                    return r;
                    //return t[i+1];
                }


        }


        printf("%s\n", a);
    }


    va_end(valist);
}




int main()
{
    char* json = "{\"a\":{\"a\":\"aa\", \"b\":{\"c\":\"ccc\", \"k\":20, \"t\":true}, \"c\":[\"c1\", \"c2\"]}, \"b\":[22, 33], \"c\":{\"d\":\"ddd\"}}";

    char * t = jsmn_get(json, "a", "b", "t", "");


    //if(t.start!=0 && t.end!=0)
        //printf("eeee%.*s\n", t.end-t.start, json + t.start);


//    printf("%s",(char*)jsmn_get(json, "c", "d", "")); 
    return 0;

}


