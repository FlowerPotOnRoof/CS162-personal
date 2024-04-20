/*

Copyright © 2019 University of California, Berkeley

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

word_count provides lists of words and associated count

Functional methods take the head of a list as first arg.
Mutators take a reference to a list as first arg.
*/

#include "word_count.h"

/* Basic utilities */

char *new_string(char *str) {
  char *new_str = (char *) malloc(strlen(str) + 1);
  if (new_str == NULL) {
    return NULL;
  }
  return strcpy(new_str, str);
}

WordCount *new_wc(char *word, int count, WordCount* next){
  char func_name[] = "new_wc";
  WordCount *wc = (WordCount*)malloc(sizeof(WordCount));
  if(!wc){
      fprintf(
          stderr,
          "Error from %s:Failed to malloc!\n",
          func_name
      );
      return NULL;
  }
  wc->word = word;
  wc->count = count;
  wc->next = next;
  return wc;
}

int extract_word(FILE *infile, char *word, size_t str_len){
  /* Extract a word from the given file
     Returns 0 if failed
     otherwise 1
  */
  if(!infile || !word) return 0;
  char func_name[] = "extract_word";

  if(ferror(infile)){
    fprintf(
      stderr,
      "Error from %s: an error (%d) exists in the given file stream\n",
      func_name,
      ferror(infile)
    );
    return 0;
  }  

  char c;
  size_t idx = 0;
  // move to first alpha
  while((c = tolower(fgetc(infile))) != EOF && !isalpha(c)); 
  if(c==EOF) return 0;
  word[idx] = c;
  // count number of letters and write char to word
  while((c = tolower(fgetc(infile))) != EOF && isalpha(c)){
    idx++;
    if(idx==str_len) return 0;
    word[idx] = c;
  }
  word[idx+1] = '\0'; 
  return 1;
}

int init_words(WordCount **wclist) {
  /* Initialize word count.
     make *wclist be a dummy node
     Returns 0 if no errors are encountered
     in the body of this function; 1 otherwise.
  */
  char func_name[] = "init_words";

  *wclist = (WordCount*)malloc(sizeof(WordCount));
  if(!(*wclist)){
    fprintf(
      stderr,
      "Error from %s: Failed to malloc!\n",
      func_name
    );
  }
  (*wclist)->count = -1;
  (*wclist)->word = NULL;
  (*wclist)->next = NULL;
  return 0;
}

ssize_t len_words(WordCount *wchead) {
  /* Return -1 if any errors are
     encountered in the body of
     this function.
  */
    size_t len = 0;
    if(!wchead) return -1;
    do{
      len++;
      wchead = wchead->next;
    }while(wchead); 
    return len;
}

WordCount *find_word(WordCount *wchead, char *word) {
  /* Return count for word, if it exists */
  if(!word || !wchead) return NULL;

  WordCount *wc = NULL;
  wchead = wchead->next;
  while(wchead){
    if(strcmp(wchead->word, word)==0) return wchead;
    wchead = wchead->next;
  }
  return wc;
}

int add_word(WordCount **wclist, char *word) {
  /* If word is present in word_counts list, increment the count.
     Otherwise insert with count 1.
     Returns 0 if no errors are encountered in the body of this function; 1 otherwise.
  */
  if(!wclist || !word) return 1; 

  WordCount *p = find_word((*wclist), word);
  if(p) p->count++;
  else{
    (*wclist)->next = new_wc(new_string(word),1,(*wclist)->next); 
  }
  return 0;
}

void fprint_words(WordCount *wchead, FILE *ofile) {
  /* print word counts to a file */
  WordCount *wc;
  for (wc = wchead->next; wc; wc = wc->next) {
    fprintf(ofile, "%i\t%s\n", wc->count, wc->word);
  }
}

void clean(WordCount *wchead){
  if(!wchead) return;

  WordCount* s = wchead;
  WordCount* f = wchead->next;
  while(true){
    free(s);
    s = f;
    if(!f) return;
    f = f->next;
  }
}