/***************************************

  Practice coding for search (with Graph)

  Implementation file

  Taylor Shin, Feb. 09 2017

 ***************************************/

#include <stdlib.h>
#include <assert.h>

#include "search.h"


/* Search Info. constructors */
SearchInfo SearchInfoInit(Graph g)
{
	SearchInfo s;
	int n;

	s = (SearchInfo)malloc(sizeof(search_info));
	assert(s);

	s->graph = g;
	s->reached = 0;

	n = GraphVertexCount(g);

	s->preorder = CreateEmptyArray(n);
	s->time = CreateEmptyArray(n);
	s->parent = CreateEmptyArray(n);
	s->depth = CreateEmptyArray(n);

	return s;
}

/* Destroy search info */
int SearchInfoDestroy(SearchInfo s)
{
	free(s->preorder);
	free(s->time);
	free(s->parent);
	free(s->depth);
	free(s);
}

/* Search functions */
int SearchDFS(SearchInfo results, int root)
{
	return Search(results, root, 0);
}
int SearchBFS(SearchInfo results, int root)
{
	return Search(results, root, 1);
}


/* Utilities */
static int* CreateEmptyArray(int n)
{
	int *a;
	int i;

	a = (int*)malloc(sizeof(int)*n);
	assert(a);

	for (i=0; i<n; ++i) a[i] = SEARCH_INFO_NULL;

	return a;
}

/* push some stuff into queue */
static int Push(Graph g, int u, int v, void* data)
{
	Queue q;
	q = *(Queue*)data;

	assert(q.top < GraphEdgeCount(g)+1);

	q.e[q.top].u = u;
	q.e[q.top].v = v;
	q.top++;

	return 0;
}

/* The search function */
static int Search(SearchInfo r, int root, int use_queue)
{
	Queue q;
	edge cur;

	/* Make queue */
	q.e = NULL; q.bottom = 0; q.top = 0;
	q.e = (Edge)malloc(sizeof(edge)*(GraphEdgeCount(r->graph)+1));
	//q.e = (Edge)calloc((GraphEdgeCount(r->graph)+1), sizeof(edge));
	assert(q.e);

	/* Push stuff into queue */
	Push(r->graph, root, root, &q);

	while (q.bottom < q.top) {
		//cur = (Edge)malloc(sizeof(edge));
		//assert(cur);
		if (use_queue) {
			cur = (q.e[q.bottom++]);
		}
		else {
			cur = (q.e[--q.top]);
		}

		if (r->parent[cur.v] != SEARCH_INFO_NULL) continue;

		assert(r->reached < GraphVertexCount(r->graph));

		r->parent[cur.v] = cur.u;
		r->time[cur.v] = r->reached;
		r->preorder[r->reached++] = cur.v;

		if (cur.u == cur.v)
			r->depth[cur.v] = 0;
		else
			r->depth[cur.v] = r->depth[cur.u] + 1;

		GraphForeach(r->graph, cur.v, Push, &q);
		//free(cur);
	}

	free(q.e);

	return 0;
}
