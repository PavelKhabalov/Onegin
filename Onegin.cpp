#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>


struct text_info make_text_info(FILE* text);

struct str* make_strs(char* buffer, size_t nlines);

/*!
\brief returns size of file
\param text adress of file which size returns
\return size of file
*/
size_t filelen(FILE* text);

/*!
\brief read file and fill in buffer and count number of lines, also returns adress to array of struct str
\param text adress of file which function read
\param buffer adress of buffer
\param len size of file
\nlines adress of variable of numbers of lines in file
\return adress to array of struct str that include all lines of file
*/
size_t readf(FILE* text, char* buffer, size_t len);

/*!
\brief sort array of struct str
\param strs array that should be sorted
\param nlines numbers of elements in strs
\param cmp_by_end defins if we need to compare strings by end
\return nothing
*/
void strsort(struct str* strs, int nlines, bool cmp_by_end);

/*!
\brief compare two strings
\param a, b compared strings
\param cmp_by_end defins if we need to compare strings by end
\return 1 if a > b, 0 if a == b, -1 if a < b
*/
int comparestr(struct str a, struct str b, bool cmp_by_end);

/*!
\brief swap two strings
\param a, b adresses of swaped strings
\return nothing
*/
void swapstr(struct str* a, struct str* b);

/*!
\brief output array of strings to file
\param strs array of strings that we output
\param output file that's output
\return nothing
*/
void foutput(struct str* strs, int nlines, FILE* output);

/*!
\brief output string to file
\param s string that we output
\param output file that's output
\return nothing
*/
void putstr(char* s, FILE* output);

void free_info(struct text_info poem);

/*
\param begin_, end_ adresses of begin str and end str
*/
struct str
    {
    char* begin_;
    char* end_;
    };

struct text_info
    {
    char* buffer;
    struct str* strs;
    size_t nlines;
    size_t len;
    };



int main(int argc, char** argv)
    {
    char *input = "input.txt", *output = "output.txt";

    if (argc == 3)
        {
        input  = argv[1];
        output = argv[2];
        }

    FILE* text   = fopen(input , "r");
    FILE* result = fopen(output, "w");

    assert(text   != NULL);
    assert(result != NULL);

    struct text_info poem = make_text_info(text);

    fclose(text);

    strsort(poem.strs, poem.nlines, 0);
    foutput(poem.strs, poem.nlines, result);

    fprintf(result, "\n=====================\n\n");

    strsort(poem.strs, poem.nlines, 1);
    foutput(poem.strs, poem.nlines, result);

    fprintf(result, "\n=====================\n\n");

    fprintf(result, "%s", poem.buffer);

    fclose(result);

    free_info(poem);

    return 0;
    }


//-----------------------------------------------------------------------------

struct text_info make_text_info(FILE* text)
    {
    size_t len = 0, nlines = 0;
    len = filelen(text);

    char* buffer = (char*) calloc(len + 1, sizeof(char));
    nlines = readf(text, buffer, len);

    struct str* strs = make_strs(buffer, nlines);

    struct text_info poem = {buffer, strs, nlines, len};

    return poem;
    }


//-----------------------------------------------------------------------------


size_t filelen(FILE* text)
    {
    assert(text != NULL);

    fseek(text, 0, SEEK_END);
    int len = ftell(text);
    fseek(text, 0, SEEK_SET);

    return len;
    }


//-----------------------------------------------------------------------------

size_t readf(FILE* text, char* buffer, size_t len)
    {
    assert(text   != NULL);
    assert(buffer != NULL);

    size_t nlines = 0;

    size_t size = fread(buffer, sizeof(char), len, text) + 1;
    buffer = (char*) realloc(buffer, size);
    buffer[size] = '\0';

    for (size_t i = 0; i < size; i++)
        {
        if (buffer[i] == '\n')
            {
            nlines = nlines + 1;
            }
        }

    return nlines;
    }


//-----------------------------------------------------------------------------


struct str* make_strs(char* buffer, size_t nlines)
    {
    struct str* strs = (struct str*) calloc(nlines, sizeof(struct str));

    strs -> begin_ = buffer;
    for (size_t i = 0, j = 0; j < nlines; i++)
        {
        if (buffer[i] == '\n')
            {
            (strs + j++) -> end_ = buffer + i;
            if (j < nlines)
                {
                (strs + j) -> begin_ = buffer + i + 1;
                }
            }
        }

    return strs;
    }


//-----------------------------------------------------------------------------

void strsort(struct str* strs, int nlines, bool cmp_by_end)
    {
    nlines--;
    for (; nlines > 0; nlines--)
        {
        for (int i = 0; i < nlines; i++)
            {
            if (comparestr(*(strs + i), *(strs + i + 1), cmp_by_end) == 1)
                {
                swapstr(strs + i, strs + i + 1);
                }
            }
        }
    }


//-----------------------------------------------------------------------------

int comparestr(struct str a, struct str b, bool cmp_by_end)
    {
    int len_a = a.end_ - a.begin_ + 1, len_b = b.end_ - b.begin_ + 1;
    for (int i = 0, j = 0;;)
        {

        if (i >= len_a)
            {
            if (i == j)
                {
                return 0;
                }
            return -1;
            }
        if (j >= len_b)
            {
            return 1;
            }


        char x = a.begin_[i + (len_a - 2 * i - 1) * cmp_by_end];
        char y = b.begin_[j + (len_b - 2 * j - 1) * cmp_by_end];
        if (!isalpha(x))
            {
            i++;
            }
        else if (!isalpha(y))
            {
            j++;
            }
        else
            {
            x = isupper(x)? tolower (x) : x;
            y = isupper(y)? tolower (y) : y;

            if (x > y)
                {
                return 1;
                }
            else if (x < y)
                {
                return -1;
                }
            else
                {
                i++;
                j++;
                }
            }
        }
    }


//-----------------------------------------------------------------------------

void swapstr(struct str* a, struct str* b)
    {
    struct str templ = *a;
    *a = *b;
    *b = templ;
    }


//-----------------------------------------------------------------------------

void foutput(struct str* strs, int nlines, FILE* output)
    {
    for (int i = 0; i < nlines; i++)
        {
        if (!((strs + i) -> begin_ == (strs + i) -> end_))
            {
            putstr((strs + i) -> begin_, output);
            }
        }
    }


//-----------------------------------------------------------------------------

void putstr(char* s, FILE* output)
    {
    for (int i = 0; s[i] != '\0'; i++)
        {
        putc(s[i], output);
        if (s[i] == '\n')
            {
            break;
            }
        }
    }


//-----------------------------------------------------------------------------

void free_info(struct text_info poem)
    {
    free(poem.buffer);
    free(poem.strs);
    }
