/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name:
 * Email:
 */

#include <stdlib.h>
#include <assert.h>
#include "dynarray.h"
#include "pq.h"

/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 */
struct node{
	void* val;
	int pri;
};

struct pq{
   struct dynarray* da;
};


/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() {
   struct pq* p = malloc(sizeof(struct pq));
   p->da = dynarray_create();
   return p;
}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue.  That is the responsibility of the caller.
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
	assert(pq);
	assert(pq->da);
	dynarray_free(pq->da);
	free(pq);
	
}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
   if(dynarray_size(pq->da) == 0){
      return 1;
   }
   else{
      return 0;
   }
}


/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, LOWER priority
 * values are assigned to elements with HIGHER priority.  In other words, the
 * element in the priority queue with the LOWEST priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, LOWER priority values
 *     should correspond to elements with HIGHER priority.  In other words,
 *     the element in the priority queue with the LOWEST priority value should
 *     be the FIRST one returned.
 */
void pq_insert(struct pq* pq, void* value, int priority) {
	struct node* n = malloc(sizeof(struct node));
	n->val = value;
	n->pri = priority;
	dynarray_insert(pq->da, -1, n);
	int c = dynarray_size(pq->da) - 1;
	int p = (c-1)/2;
	while(c > 0){
		struct node* child = dynarray_get(pq->da, c);
		struct node* parent = dynarray_get(pq->da, p);
		if(child->pri < parent->pri){
			struct node* temp = parent;
		   	dynarray_set(pq->da, p, child);
			dynarray_set(pq->da, c, temp);
		}
		c = p;
		p = (c-1)/2;
	}
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_first(struct pq* pq) {
  assert(pq->da);
  struct node* n = dynarray_get(pq->da, 0);
  return n->val;
}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */
int pq_first_priority(struct pq* pq) {
   assert(pq->da);
   struct node* n = dynarray_get(pq->da, 0);
   return n->pri;
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_remove_first(struct pq* pq) {
  assert(pq->da);
  struct node* end = dynarray_get(pq->da, -1);
  struct node* front = dynarray_get(pq->da, 0);
  void* num = front->val;
  free(front);
  if(dynarray_size(pq->da) == 1){
  	dynarray_remove(pq->da,-1);
  }
  else{
  dynarray_remove(pq->da, -1);
  dynarray_remove(pq->da, 0);

  dynarray_insert(pq->da, 0, end);
  }
  int p = 0;
  int li = p*2 + 1;
  int idx;
  while(li < dynarray_size(pq->da)){
	int ri = p*2 + 2;
	if(ri < dynarray_size(pq->da)){
	   	struct node* small;
		struct node* pn = dynarray_get(pq->da, p);
		struct node* lc = dynarray_get(pq->da, li);
		struct node* rc = dynarray_get(pq->da, ri);
		if(lc->pri < rc->pri){
			small = lc;
			idx = li;
		}
		else{
			small = rc;
			idx = ri;
		}
		if(pn->pri > small->pri){
			struct node* temp = pn;
			dynarray_set(pq->da, p, small);
			dynarray_set(pq->da, idx, temp);
			p = idx;
			li = p*2 + 1;
		}
		else{
			li = dynarray_size(pq->da);
		}
	}
	else{
		struct node* pn = dynarray_get(pq->da, p);
		struct node* lc = dynarray_get(pq->da, li);
		if(pn->pri > lc->pri){
			struct node* temp = pn;
			dynarray_set(pq->da, p, lc);
			dynarray_set(pq->da, li, temp);
			p = li;
			li = p*2 + 1;
		}
		else{
			li = dynarray_size(pq->da);
		}
	}

  }
  return num;
}
