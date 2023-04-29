#include "basic.h"

void increase_vertex_weight()
{
	int i, v;
	
	for(i=0; i<set_C_count; i++)
	{
		v = set_C[i];
		vertex_weight[v]++;
		
		if(vertex_weight[v] == vertex_cost[v]+1)
		{
			large_weight_vertexes[large_weight_vertexes_count++] = v;
		}
	}
	
	return;
}

void decrease_weight_weight()
{
	int i, v;
	
	for(i=large_weight_vertexes_count-1; i>=0; i--)
	{
		v = large_weight_vertexes[i];
		
		if(vertex_label[v]!=0)
		{
			vertex_weight[v]--;
			if(vertex_weight[v] == vertex_cost[v])
			{
				large_weight_vertexes[i] = large_weight_vertexes[--large_weight_vertexes_count];
			}
		}
	}
	
	return;
}

void update_vertex_weight()
{
	if(rand()%100<90)
	{
		decrease_weight_weight();
	}
	else
	{
		increase_vertex_weight();
	}
}

int compute_weighted_score_A(int v)
{
	int v_weighted_score_A = vertex_weight[v];
	int i, v2;
	int length = my_graph[v].size();
	
	for(i=0; i<length; i++)
	{
		v2 = my_graph[v][i];
		if(vertex_label[v2]==2) v_weighted_score_A -= vertex_weight[v2];
	}
	
	return v_weighted_score_A;
}

int compute_weighted_score_B(int v)
{
	int v_weighted_score_B = vertex_weight[v];
	int i, v2;
	int length = my_graph[v].size();
	
	for(i=0; i<length; i++)
	{
		v2 = my_graph[v][i];
		if(vertex_label[v2]==1) v_weighted_score_B -= vertex_weight[v2];
	}
	
	return v_weighted_score_B;
}

