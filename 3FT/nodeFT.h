/*--------------------------------------------------------------------*/
/* nodeFT.h                                                           */
/* Author: Annika Yeh and William Oh                                  */
/*--------------------------------------------------------------------*/

#ifndef NODE_INCLUDED
#define NODE_INCLUDED

#include <stddef.h>
#include "a4def.h"
#include "path.h"


/* 
  A Node_T is a node in a File Tree of either file or directory
  Directories contains separate dynamic arrays for file children and 
  directory children.
  Files contains content and its length.
 */
typedef struct node *Node_T;

/*
  Creates a new node in the Directory Tree, with path oPPath, parent 
  oNParent, and boolean isFile (TRUE if node is a file, FALSE if node is
  a directory). Returns an int SUCCESS status and sets *poNResult
  to be the new node if successful. Otherwise, sets *poNResult to NULL
  and returns status:
  * MEMORY_ERROR if memory could not be allocated to complete request
  * CONFLICTING_PATH if oNParent's path is not an ancestor of oPPath
  * NO_SUCH_PATH if oPPath is of depth 0
                 or oNParent's path is not oPPath's direct parent
                 or oNParent is NULL but oPPath is not of depth 1
  * ALREADY_IN_TREE if oNParent already has a child with this path
*/
int NodeFT_new(Path_T oPPath, Node_T oNParent, boolean isFile, 
Node_T *poNResult);

/*
  Destroys and frees all memory allocated for the subtree rooted at
  oNNode, i.e., deletes this node and all its descendents. Returns the
  number of nodes deleted.
*/
size_t NodeFT_free(Node_T oNNode);

/* Returns the path object representing oNNode's absolute path. */
Path_T NodeFT_getPath(Node_T oNNode);

/*
  Returns TRUE if oNParent has a directory child with path oPPath. 
  Returns FALSE if it does not.

  If oNParent has such a child, stores in *pulChildID the child's
  identifier (as used in NodeFT_getChild). If oNParent does not have
  such a child, stores in *pulChildID the identifier that such a
  child _would_ have if inserted.
*/
boolean NodeFT_hasDirChild(Node_T oNParent, Path_T oPPath,
                         size_t *pulChildID);

/*
  Returns TRUE if oNParent has a file child with path oPPath. 
  Returns FALSE if it does not.

  If oNParent has such a child, stores in *pulChildID the child's
  identifier (as used in NodeFT_getChild). If oNParent does not have
  such a child, stores in *pulChildID the identifier that such a
  child _would_ have if inserted.
*/
boolean NodeFT_hasFileChild(Node_T oNParent, Path_T oPPath,
                         size_t *pulChildID);


/* Returns the number of children that oNParent has. */
size_t NodeFT_getNumDirChildren(Node_T oNParent);

/* Returns the number of children that oNParent has. */
size_t NodeFT_getNumFileChildren(Node_T oNParent);

/*
  Returns an int SUCCESS status and sets *poNResult to be the directory 
  child node of oNParent with identifier ulChildID, if one exists.
  Otherwise, sets *poNResult to NULL and returns status:
  * NO_SUCH_PATH if ulChildID is not a valid child for oNParent
*/
int NodeFT_getDirChild(Node_T oNParent, size_t ulChildID,
                  Node_T *poNResult);

/*
  Returns an int SUCCESS status and sets *poNResult to be the file 
  child node of oNParent with identifier ulChildID, if one exists.
  Otherwise, sets *poNResult to NULL and returns status:
  * NO_SUCH_PATH if ulChildID is not a valid child for oNParent
*/
int NodeFT_getFileChild(Node_T oNParent, size_t ulChildID,
                  Node_T *poNResult);

/* 
  Adds a directory child oNChild to the parent node oNParent. Returns an
  int SUCCESS status if successful, otherwise returns status:
  * MEMORY_ERROR if memory could not be allocated to complete request
  * ALREADY_IN_TREE if oNParent already has a child with this path
*/
int NodeFT_addDirChild(Node_T oNParent, Node_T oNChild);

/* 
  Adds a file child oNChild to the parent node oNParent. Returns an
  int SUCCESS status if successful, otherwise returns status:
  * MEMORY_ERROR if memory could not be allocated to complete request
*/
int NodeFT_addFileChild(Node_T oNParent, Node_T oNChild);

/* 
  Returns an int SUCCESS status and sets *poNResult to be the contents 
  of the file node oNNode, if one exists.
  Otherwise, sets *poNResult to NULL and returns status:
  * NO_SUCH_PATH if oNNode is null 
  * WRONG_TYPE if oNNode is not a file
*/
int NodeFT_getContents(Node_T oNNode, Node_T *poNResult);

/* 
  Returns an int SUCCESS status and sets *poNResult to be the length 
  of the content of the file node oNNode, if one exists.
  Otherwise, sets *poNResult to NULL and returns status:
  * NO_SUCH_PATH if oNNode is null 
  * WRONG_TYPE if oNNode is not a file
*/
int NodeFT_getContentLength(Node_T oNNode, Node_T *poNResult);

/*
  Sets the content of the file node oNNode to the contents at pvContents
  of length ulLength. Returns an int SUCCESS status if successful. 
  Otherwise, returns status:
  * MEMORY_ERROR if memory could not be allocated to complete request
  * NO_SUCH_PATH if oNNode is NULL
  * WRONG_TYPE if oNNode is not a file
*/
int NodeFT_setContents(Node_T oNNode, void *pvContents, size_t ulLength);

/* 
  Returns TRUE if node oNNode is a file. 
  Returns FALSE if it is a directory.
*/
boolean NodeFT_isFile(Node_T oNNode);

/*
  Returns a the parent node of oNNode.
  Returns NULL if oNNode is the root and thus has no parent.
*/
Node_T NodeFT_getParent(Node_T oNNode);

/*
  Compares oNFirst and oNSecond lexicographically based on their paths.
  Returns <0, 0, or >0 if onFirst is "less than", "equal to", or
  "greater than" oNSecond, respectively.
*/
int NodeFT_compare(Node_T oNFirst, Node_T oNSecond);

/*
  Returns a string representation for oNNode, or NULL if
  there is an allocation error.

  Allocates memory for the returned string, which is then owned by
  the caller!
*/
char *NodeFT_toString(Node_T oNNode);

#endif
