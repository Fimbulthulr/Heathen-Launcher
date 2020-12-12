#ifndef GRAPH_H
#define GRAPH_H

/*
 * =====================================================================================
 *
 *       Filename:  graph.h
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

#include <inttypes.h>
#include <stddef.h>

struct graph
{
	size_t n_vertex;
	int64_t *vertices;
	size_t *n_edges;
	size_t **adjacency_list;
};


int
lexicographically_minimal_topological_sort
    (size_t n,
    size_t *L,
    struct graph *g);
#endif /* GRAPH_H */
