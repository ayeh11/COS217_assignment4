/*--------------------------------------------------------------------*/
/* checkerDT.c                                                        */
/* Author: Annika Yeh and William Oh                                  */
/*--------------------------------------------------------------------*/

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "checkerDT.h"
#include "dynarray.h"
#include "path.h"



/* see checkerDT.h for specification */
boolean CheckerDT_Node_isValid(Node_T oNNode) {
   Node_T oNParent;
   Path_T oPNPath;
   Path_T oPPPath;

   /* Sample check: a NULL pointer is not a valid node */
   if(oNNode == NULL) {
      fprintf(stderr, "A node is a NULL pointer\n");
      return FALSE;
   }

   /* Sample check: parent's path must be the longest possible
      proper prefix of the node's path */
   oNParent = Node_getParent(oNNode);
   if(oNParent != NULL) {
      oPNPath = Node_getPath(oNNode);
      oPPPath = Node_getPath(oNParent);

      if(Path_getSharedPrefixDepth(oPNPath, oPPPath) !=
         Path_getDepth(oPNPath) - 1) {
         fprintf(stderr, "P-C nodes don't have P-C paths: (%s) (%s)\n",
                 Path_getPathname(oPPPath), Path_getPathname(oPNPath));
         return FALSE;
      }
   }

   return TRUE;
}

/*
   Performs a pre-order traversal of the tree rooted at oNNode.
   Returns FALSE if a broken invariant is found and
   returns TRUE otherwise.

   You may want to change this function's return type or
   parameter list to facilitate constructing your checks.
   If you do, you should update this function comment.
*/
static boolean CheckerDT_treeCheck(Node_T oNNode, DynArray_T oSeenNodes) {
   size_t ulIndex;
   size_t numChildren;
   size_t i;
   Node_T oNParent;

   if(oNNode!= NULL) {

      /* Sample check on each node: node must be valid */
      /* If not, pass that failure back up immediately */
      if(!CheckerDT_Node_isValid(oNNode))
         return FALSE;

      /* Check for duplicate path */
      for(i = 0; i < DynArray_getLength(oSeenNodes); i++) {
         if(Node_compare(oNNode, DynArray_get(oSeenNodes, i)) == 0) {
            fprintf(stderr, "Duplicate path found: %s\n", Path_getPathname(Node_getPath(oNNode)));
            return FALSE;
         }
      }
      /* Check for lexicographic order */
      oNParent = Node_getParent(oNNode);
      if (oNParent != NULL) {
         numChildren = Node_getNumChildren(oNParent);
         for(i = 0; i < numChildren - 1; i++) {
            Node_T oNChild1 = NULL;
            Node_T oNChild2 = NULL;
            /* Get adjacent children */
            if(Node_getChild(oNParent, i, &oNChild1) != SUCCESS ||
               Node_getChild(oNParent, i + 1, &oNChild2) != SUCCESS) {
               fprintf(stderr, "Error retrieving children\n");
               return FALSE;
            }  

            if(Path_comparePath(Node_getPath(oNChild1), Node_getPath(oNChild2)) > 0) {
               fprintf(stderr, "Children not in lexicographic order: %s\n", 
                     Path_getPathname(Node_getPath(oNNode)));
               return FALSE;
            } 
         }
      }

      DynArray_add(oSeenNodes, oNNode);

      /* Recur on every child of oNNode */
      for(ulIndex = 0; ulIndex < Node_getNumChildren(oNNode); ulIndex++)
      {
         Node_T oNChild = NULL;
         int iStatus = Node_getChild(oNNode, ulIndex, &oNChild);

         if(iStatus != SUCCESS) {
            fprintf(stderr, "getNumChildren claims more children than getChild returns\n");
            return FALSE;
         }

         /* if recurring down one subtree results in a failed check
            farther down, passes the failure back up immediately */
         if(!CheckerDT_treeCheck(oNChild, oSeenNodes))
            return FALSE;
      }
   }
   return TRUE;
}

/* see checkerDT.h for specification */
boolean CheckerDT_isValid(boolean bIsInitialized, Node_T oNRoot,
                          size_t ulCount) {

   DynArray_T oSeenNodes;
   boolean bResult;
   size_t actualCount;

   /* Sample check on a top-level data structure invariant:
      if the DT is not initialized, its count should be 0. */
   if(!bIsInitialized)
      if(ulCount != 0) {
         fprintf(stderr, "Not initialized, but count is not 0\n");
         return FALSE;
      }

   /* Checks invariants recursively at each node from the root. */
   oSeenNodes = DynArray_new(0);
   bResult = CheckerDT_treeCheck(oNRoot, oSeenNodes);

   /* Check number of seen nodes matches ulCount */
   actualCount = DynArray_getLength(oSeenNodes);
   if(actualCount != ulCount) {
      fprintf(stderr, "Node count doesn't match\n");
      bResult = FALSE;
   }

   DynArray_free(oSeenNodes);

   return bResult;
}
