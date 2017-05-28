
#include <stdio.h>
#include <tidy/tidy.h>
#include <tidy/buffio.h>
#include <curl/curl.h>
#include <fstream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> /* the header where mmap is defined */
#include <fcntl.h>



using namespace ::std;

char c;
int count;
FILE *ptr_to_file;
void append(char* str, char ch);
char* search_for_string="";
void deleteFromFile(char* deleteString);
int i;
string filename = "outputfile.txt";
int compare_char(char* search_for_string, char* c, int count){
    for(i =0; search_for_string[i] != '\0'; i++){
        //if(search_for_string[i] == c){{
            
        //}
    }
    return count;
}
void deleteFromFile(char* deleteString){
    char *buffer;
    
    char* search_for_string = deleteString;
    buffer = (char *)malloc(10000 * sizeof(char));
      if(!buffer)
    {
        printf("Memory allocation error!\n");
        exit(1);
    }else printf("Memory allocation successful.\n");
    
    
    ptr_to_file = fopen("outputfile.txt", "r");
    while (!feof(ptr_to_file)){
        for(i =0; search_for_string[i] != '\0'; i++){
        
            if(search_for_string[i] == c){
                putchar(c);
                //printf("%c", c);
                c = fgetc(ptr_to_file);
                
                if(feof(ptr_to_file)){
                    printf("end of file");
                    break;
                }

            } else{
                i=-1;
                
                append(buffer, c);
                //putchar(c);
                c = fgetc(ptr_to_file);
                
                if(feof(ptr_to_file)){
                    printf("end of file");
                    break;
                }

            }
        }
    }
    fclose(ptr_to_file);
    ptr_to_file = fopen("outputfile.txt", "w");
    fprintf(ptr_to_file, "%s", buffer);
    fclose(ptr_to_file);
}

void append(char* str, char ch)
{
    
    int len = strlen(str);
    str[len] = ch;
    str[len+1] = '\0';
}


/* curl write callback, to fill tidy's input buffer...  */
uint write_cb(char *in, uint size, uint nmemb, TidyBuffer *out)
{
    uint r;
    r = size * nmemb;
    tidyBufAppend(out, in, r);
    return r;
}

/* Traverse the document tree */
void dumpNode(TidyDoc doc, TidyNode tnod, int indent)
{
    TidyNode child;
    for(child = tidyGetChild(tnod); child; child = tidyGetNext(child) ) {
        ctmbstr name = tidyNodeGetName(child);
        if(name) {
            /* if it has a name, then it's an HTML tag ...*/
            TidyAttr attr;
            if (strcmp(name, "h4")==0 ){
                TidyBuffer buf;
                tidyBufInit(&buf);
                tidyNodeGetText(doc, child, &buf);
                printf("%*.*s", indent, indent, buf.bp?(char *)buf.bp:"");
                std::ofstream outfile;
                outfile.open("outputfile.txt", std::ios_base::app);
                outfile << buf.bp;
                tidyBufFree(&buf);
            //printf("%*.*s%s", indent, indent, "<", name);
            /* walk the attribute list */
            for(attr=tidyAttrFirst(child); attr; attr=tidyAttrNext(attr) ) {
            }
            //printf(">\n");
            }
            if (strcmp(name, "p")==0 ){
                
                //printf("%*.*s%s", indent, indent, "<", name);
                /* walk the attribute list */
                for(attr=tidyAttrFirst(child); attr; attr=tidyAttrNext(attr) ) {
                    //printf("%s\n", tidyAttrName(attr));
                    //tidyAttrValue(attr)?printf("%s", tidyAttrValue(attr)):printf(" ");
                    if (strcmp(tidyAttrValue(attr), "price btn btn-danger") == 0){
                    //printf("%s%s%s", "11111111111111111",tidyAttrValue(attr),"11111111111111");
                
                    TidyBuffer buf;
                    tidyBufInit(&buf);
                    tidyNodeGetText(doc, child, &buf);
                    printf("%*.*s\n", indent, indent, buf.bp?(char *)buf.bp:"");
                        std::ofstream outfile;
    
                        outfile.open("outputfile.txt", std::ios_base::app);
                        outfile << buf.bp;

                    tidyBufFree(&buf);
                    }
                }
                //printf(">\n");
            }
            
        }
        else {
             /*if it doesn't have a name, then it's probably text, cdata, etc...*/
            /*TidyBuffer buf;
            tidyBufInit(&buf);
            tidyNodeGetText(doc, child, &buf);
            printf("%*.*s\n", indent, indent, buf.bp?(char *)buf.bp:"");
            tidyBufFree(&buf);
        */}
        dumpNode(doc, child, indent + 4);  /*recursive */
    }
}

int main(int argc, char **argv)
{
    system("ls");
    if( remove( "outputfile.txt" ) != 0 ){
        perror( "Error deleting file" );
        
    }else{
        puts( "File successfully deleted" );
    }
    
    /*pFile = fopen("adc.txt", "r");

    while (!feof(pFile)){
        fgets(line, 150, pFile);
        strcpy(ptr, line);
        ptr += strlen(line);
        //printf("%s%s", ptr, "\n");
        //printf("%s", buffer);
    }
        
    fclose(pFile);*/
    
    

    CURL *curl;
    char curl_errbuf[CURL_ERROR_SIZE];
    TidyDoc tdoc;
    TidyBuffer docbuf = {0};
    TidyBuffer tidy_errbuf = {0};
    int err;
        curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.superliquor.co.nz/");
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_errbuf);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
        
        tdoc = tidyCreate();
        tidyOptSetBool(tdoc, TidyForceOutput, yes); /* try harder */
        tidyOptSetInt(tdoc, TidyWrapLen, 4096);
        tidySetErrorBuffer(tdoc, &tidy_errbuf);
        tidyBufInit(&docbuf);
        
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &docbuf);
        err=curl_easy_perform(curl);
        if(!err) {
            err = tidyParseBuffer(tdoc, &docbuf); /* parse the input */
            if(err >= 0) {
                err = tidyCleanAndRepair(tdoc); /* fix any problems */
                if(err >= 0) {
                    err = tidyRunDiagnostics(tdoc); /* load tidy error buffer */
                    if(err >= 0) {
                        dumpNode(tdoc, tidyGetRoot(tdoc), 0); /* walk the tree */
                        //fprintf(stderr, "%s\n", tidy_errbuf.bp); /* show errors */
                    }
                }
            }
        }
        else
            fprintf(stderr, "%s\n", curl_errbuf);
        
        /* clean-up */ 
        curl_easy_cleanup(curl);
        tidyBufFree(&docbuf);
        tidyBufFree(&tidy_errbuf);
        tidyRelease(tdoc);
    
    
     // deleteFromFile("h");
    //deleteFromFile("p class=\"price btn btn-danger\">");
    //deleteFromFile("<span>");
    //deleteFromFile("</span>");
   // deleteFromFile("<p class=\"price btn btn-danger\"");
   // deleteFromFile("<p>");
   // deleteFromFile("</p>");
   // deleteFromFile("<p>");

        return err;
    
    
    
    return 0;
}