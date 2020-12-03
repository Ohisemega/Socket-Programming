#ifndef QUERY_H
#define QUERY_H

/*
 http_query:
 Performs an HTTP 1.0 query to a given host and page and port number.
 host is a hostname and page is a path on the remote server.
 On any error (for example an invalid path or host name)
 NULL is returned and any resources associated with the query
 are cleaned up. An error may be printed on stderr.
 The string returned is the raw http response with content,
 and the user is responsible for freeing the memory 
 arguments:
 host - hostname e.g. www.canterbury.ac.nz
 page - e.g.  /index.html
 */

inline int checker (int func, char * sentence)
{
  if (func == -1)
  {
  puts(sentence);
  exit(EXIT_FAILURE);
  }
}

inline void error(char* message)
{
    puts(message);
    exit(EXIT_FAILURE);
}

typedef struct Buffer{
    char *data;
    size_t length;
}Buffer;

Buffer* httpQuery(char* host, char *page, int port);

/*
 * http Content:
 Separate the content from the header of an http request.
 NOTE: returned string is an offset into the response, so
 should not be freed by the user. Do not copy the data.
 return NULL on failure (i.e. the buffer passed in is not a HTTP response)
 */

char* httpGetRequest(struct Buffer* response);

Buffer *httpURL(const char *URL);
void (*func)();
/*
function to free buffer memory
*/
inline static void freeBuffer(struct Buffer* buffer)
    {
       free(buffer->data);
       free(buffer);
    }
#endif