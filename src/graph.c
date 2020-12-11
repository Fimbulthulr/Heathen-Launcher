/*
 * =====================================================================================
 *
 *       Filename:  graph.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/12/20 08:59:29
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Freyja Walberg (), gx231@stud.uni-heidelberg.de
 *   Organization:  Universität Heidelberg
 *
 * =====================================================================================
 */
#define _GNU_SOURCE
#include <stdlib.h>

#include "graph.h"

static int
vertex_compare
	(const void *a,
	 const void *b,
		   void *v);

int
lexicographically_minimal_topological_sort
	(size_t n,
	 size_t *L,
	 struct graph *g)
{
	if(n < g->n_vertex)
	{
		return 1;
	}
	size_t *in = calloc(sizeof(*in), g->n_vertex);
	size_t S_start = 0;
	size_t S_end = 0;
	for(size_t i = 0; i < g->n_vertex; ++i)
	{
		size_t *edge_list = g->adjacency_list[i];
		for(size_t j = 0; j < g->n_edges[i]; ++j)
		{
			in[edge_list[j]] += 1;
		}
	}
	for(size_t i = 0; i < g->n_vertex; ++i)
	{
		if(in[i] == 0)
		{
			L[S_end] = i;
			S_end += 1;
		}
	}

	while(S_start < S_end)
	{
		qsort_r(L+S_start, S_end - S_start, sizeof(*L), *vertex_compare, g->vertices);
		size_t curr_idx = L[S_start];

		size_t *edge_list = g->adjacency_list[curr_idx];
		for(size_t i = 0; i < g->n_edges[curr_idx]; ++i)
		{
			size_t neighbour = edge_list[i];
			in[neighbour] -= 1;
			if(in[neighbour] == 0)
			{
				L[S_end] = neighbour;
				S_end += 1;
			}
		}

		S_start += 1;
	}
	free(in);
	int ret = 0;
	if(S_end < g->n_vertex)
	{
		ret = -1;
	}

	return ret;
}


static int
vertex_compare
	(const void *A,
	 const void *B,
	       void *V)
{
	const int64_t *v = V;
	size_t a = * (const size_t *)A;
	size_t b = * (const size_t *)B;
	int ret;
	if( v[a] == v[b] )
	{
		if(a > b)
		{
			ret =  1;
		}
		else if(a < b)
		{
			ret =  0;
		}
		else
		{
			ret = -1;
		}
	}
	else
	{
		if(v[a] > v[b])
		{
			ret =  1;
		}
		else
		{
			ret = -1;
		}
	}
	return ret;
		
}
