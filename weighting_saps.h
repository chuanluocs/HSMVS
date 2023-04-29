#include "basic.h"

int avg_vertex_weight_threshold=100;
double p_scale = 0.5;

void smooth_vertex_weight()
{
	int v, tmp_weight;
	int avg_vertex_weight = total_vertex_weight/num_vertexs;
	total_vertex_weight = 0;
	for(v=0; v<num_vertexs; v++)
	{
		if(vertex_label[v]!=0)
		{
			tmp_weight = vertex_weight[v]*p_scale+avg_vertex_weight*(1.0-p_scale);
			if(tmp_weight<vertex_cost[v]) tmp_weight = vertex_cost[v];
			vertex_weight[v] = tmp_weight;
		}
		total_vertex_weight += vertex_weight[v];
	}
}

void update_vertex_weight()
{
	int i, v;
	
	for(i=0; i<set_C_count; i++)
	{
		v = set_C[i];
		vertex_weight[v]++;
	}
	
	total_vertex_weight += set_C_count;
	int avg_vertex_weight = total_vertex_weight/num_vertexs;
	if(avg_vertex_weight>avg_vertex_weight_threshold)
	{
		smooth_vertex_weight();
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
