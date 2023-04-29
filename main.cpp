#include "weighting_saps.h"
#include <sys/times.h>
#include <unistd.h>

long long tabu_tenure;

struct tms start, stop;
const int cutoff_time=100000;


void select_vertex_and_set_random(int &selected_vertex, int &selected_set)
{
	int best_vertex_A, best_vertex_B;
	int best_score_A, best_score_B;
	int i, v;
	int v_score_A, v_score_B;
	bool permit_A, permit_B;
	int pos;
	int v1, v2;
	int v1_score_A, v2_score_A, v1_score_B, v2_score_B;
	
	if(set_A_count<constraint_b_value) permit_A = true;
	else permit_A = false;
	
	if(set_B_count<constraint_b_value) permit_B = true;
	else permit_B = false;
	
	
	if(permit_A && permit_B) selected_set = rand()%2+1;
	else if(permit_A && !permit_B) selected_set = 1;
	else if(!permit_A && permit_B) selected_set = 2; 
	else if(!permit_A && !permit_B) selected_set = -1;
	
	pos = rand()%set_C_count;
	v1 = set_C[pos];
	pos = rand()%set_C_count;
	v2 = set_C[pos];
	if(selected_set==1)
	{
		v1_score_A = compute_weighted_score_A(v1);
		v2_score_A = compute_weighted_score_A(v2);
		if(v1_score_A>v2_score_A) selected_vertex = v1;
		else if(v1_score_A<v2_score_A) selected_vertex = v2;
		else
		{
			if(rand()%2==0) selected_vertex = v1;
			else selected_vertex = v2;
		}
	}
	else if(selected_set==2)
	{
		v1_score_B = compute_weighted_score_B(v1);
		v2_score_B = compute_weighted_score_B(v2);
		if(v1_score_B>v2_score_B) selected_vertex = v1;
		else if(v1_score_B<v2_score_B) selected_vertex = v2;
		else
		{
			if(rand()%2==0) selected_vertex = v1;
			else selected_vertex = v2;
		}
	}
	else selected_vertex = -1;
	
	return;
}

void select_vertex_and_set_age(int &selected_vertex, int &selected_set)
{
	int best_vertex_A, best_vertex_B;
	int best_score_A, best_score_B;
	int i, v;
	int v_score_A, v_score_B;
	bool permit_A, permit_B;
	int pos;
	
	if(set_A_count<constraint_b_value) permit_A = true;
	else permit_A = false;
	
	if(set_B_count<constraint_b_value) permit_B = true;
	else permit_B = false;
	
	
	if(permit_A)
	{
		best_vertex_A = set_C[0];
		
		for(i=1; i<set_C_count; i++)
		{
			v = set_C[i];
			if(time_stamp_A[v]<time_stamp_A[best_vertex_A])
				best_vertex_A = v;
		}
	}
	
	if(permit_B)
	{
		best_vertex_B = set_C[0];
		
		for(i=1; i<set_C_count; i++)
		{
			v = set_C[i];
			if(time_stamp_B[v]<time_stamp_B[best_vertex_B])
				best_vertex_B = v;
		}
	}
	
	if(permit_A && permit_B)
	{
		if(time_stamp_A[best_vertex_A]<time_stamp_B[best_vertex_B])
		{
			selected_vertex = best_vertex_A;
			selected_set = 1;
		}
		else
		{
			selected_vertex = best_vertex_B;
			selected_set = 2;
		}
	}
	else if(permit_A && !permit_B)
	{
		selected_vertex = best_vertex_A;
		selected_set = 1;
	}
	else if(!permit_A && permit_B)
	{
		selected_vertex = best_vertex_B;
		selected_set = 2;
	}
	else
	{
		selected_vertex = -1;
		selected_set = -1;
	}
}

void select_vertex_and_set_descent(int &selected_vertex, int &selected_set)
{
	int best_vertex_A, best_vertex_B;
	int best_score_A, best_score_B;
	int i, v;
	int v_score_A, v_score_B;
	bool permit_A, permit_B;
	int pos;
	
	if(set_A_count<constraint_b_value) permit_A = true;
	else permit_A = false;
	
	if(set_B_count<constraint_b_value) permit_B = true;
	else permit_B = false;
	
		
	if(permit_A)
	{
		best_vertex_A = set_C[0];
		best_score_A = compute_weighted_score_A(best_vertex_A);
		
		for(i=1; i<set_C_count; i++)
		{
			v = set_C[i];
			v_score_A = compute_weighted_score_A(v);
			if(v_score_A>best_score_A || (v_score_A==best_score_A && time_stamp_A[v]<time_stamp_A[best_vertex_A]))
			{
				best_score_A = v_score_A;
				best_vertex_A = v;
			}
		}
		
		if(best_score_A<=0) permit_A = false;
	}
	
	if(permit_B)
	{
		best_vertex_B = set_C[0];
		best_score_B = compute_weighted_score_B(best_vertex_B);
		
		for(i=1; i<set_C_count; i++)
		{
			v = set_C[i];
			v_score_B = compute_weighted_score_B(v);
			if(v_score_B>best_score_B || (v_score_B==best_score_B && time_stamp_B[v]<time_stamp_B[best_vertex_B]))
			{
				best_score_B = v_score_B;
				best_vertex_B = v;
			}
		}
		
		if(best_score_B<=0) permit_B = false;
	}
	
	
	if(permit_A && permit_B)
	{
		if(best_score_A>best_score_B)
		{
			selected_vertex = best_vertex_A;
			selected_set = 1;
		}
		else if(best_score_A<best_score_B)
		{
			selected_vertex = best_vertex_B;
			selected_set = 2;
		}
		else
		{
			if(time_stamp_A[best_vertex_A]<time_stamp_B[best_vertex_B])
			{
				selected_vertex = best_vertex_A;
				selected_set = 1;
			}
			else
			{
				selected_vertex = best_vertex_B;
				selected_set = 2;
			}
		}
	}
	else if(permit_A && !permit_B)
	{
		selected_vertex = best_vertex_A;
		selected_set = 1;
	}
	else if(!permit_A && permit_B)
	{
		selected_vertex = best_vertex_B;
		selected_set = 2;
	}
	else
	{
		selected_vertex = -1;
		selected_set = -1;
	}
	
	
	return;	
}

void select_vertex_and_set_tabu(int &selected_vertex, int &selected_set)
{
	int best_vertex_A, best_vertex_B;
	int best_score_A, best_score_B;
	int i, v;
	int v_score_A, v_score_B;
	bool permit_A, permit_B;
	int pos;
	
	if(set_A_count<constraint_b_value) permit_A = true;
	else permit_A = false;
	
	if(set_B_count<constraint_b_value) permit_B = true;
	else permit_B = false;
	
	if(permit_A)
	{
		best_vertex_A = -1;
		
		for(i=0; i<set_C_count; i++)
		{
			v = set_C[i];
			if(step-time_stamp_A[v]>tabu_tenure)
			{
				best_vertex_A = v;
				best_score_A = compute_weighted_score_A(v);
				break;
			}
		}
	
		for(i++; i<set_C_count; i++)
		{
			v = set_C[i];
			if(step-time_stamp_A[v]<=tabu_tenure)
				continue;
			
			v_score_A = compute_weighted_score_A(v);
			if(v_score_A>best_score_A || (v_score_A==best_score_A && time_stamp_A[v]<time_stamp_A[best_vertex_A]))
			{
				best_score_A = v_score_A;
				best_vertex_A = v;
			}
		}
		
		if(best_vertex_A==-1) permit_A = false;
	}
	
	if(permit_B)
	{
		best_vertex_B = -1;
		
		for(i=0; i<set_C_count; i++)
		{
			v = set_C[i];
			if(step-time_stamp_B[v]>tabu_tenure)
			{
				best_vertex_B = v;
				best_score_B = compute_weighted_score_B(v);
				break;
			}
		}
		
		for(i++; i<set_C_count; i++)
		{
			v = set_C[i];
			if(step-time_stamp_B[v]<=tabu_tenure)
				continue;
			
			v_score_B = compute_weighted_score_B(v);
			if(v_score_B>best_score_B || (v_score_B==best_score_B && time_stamp_B[v]<time_stamp_B[best_vertex_B]))
			{
				best_score_B = v_score_B;
				best_vertex_B = v;
			}
		}
		
		if(best_vertex_B==-1) permit_B = false;
	}
	
	if(permit_A && permit_B)
	{
		if(best_score_A>best_score_B)
		{
			selected_vertex = best_vertex_A;
			selected_set = 1;
		}
		else if(best_score_A<best_score_B)
		{
			selected_vertex = best_vertex_B;
			selected_set = 2;
		}
		else
		{
			if(time_stamp_A[best_vertex_A]<time_stamp_B[best_vertex_B])
			{
				selected_vertex = best_vertex_A;
				selected_set = 1;
			}
			else
			{
				selected_vertex = best_vertex_B;
				selected_set = 2;
			}
		}
	}
	else if(permit_A && !permit_B)
	{
		selected_vertex = best_vertex_A;
		selected_set = 1;
	}
	else if(!permit_A && permit_B)
	{
		selected_vertex = best_vertex_B;
		selected_set = 2;
	}
	else if(!permit_A && !permit_B)
	{
		selected_vertex = -1;
		selected_set = -1;
	}
	return;	
}

void local_search_random(int flips_num)
{
	int selected_vertex;
	int selected_set;
	int tmp_set, tmp_v;
	int i, j;
	
	for(i=0; i<flips_num; i++)
	{
		if(set_A_count>0 && set_A_count<=constraint_b_value && set_B_count>0 && set_B_count<=constraint_b_value && solution_quality<best_solution_quality)
		{
			cout<<"step="<<step<<"; random; ";
			best_solution_quality = solution_quality;
			for(j=0; j<num_vertexs; j++) best_solution[j] = vertex_label[j];
			print_solution();
			if(best_solution_quality==0) return;
		}
		
		select_vertex_and_set_random(selected_vertex, selected_set);
		
		step++;
		
		if(selected_set==1)
		{
			from_set_C_to_set_A(selected_vertex);
			time_stamp_A[selected_vertex] = step-1;
		}
		else if(selected_set==2)
		{
			from_set_C_to_set_B(selected_vertex);
			time_stamp_B[selected_vertex] = step-1;
		}
		else if(selected_set==-1)
		{
			if(set_A_count>0 && set_B_count>0)
			{
				tmp_set = rand()%2+1;
				if(tmp_set==1)
				{
					tmp_v = set_A[rand()%set_A_count];
					from_set_A_to_set_C(tmp_v);
				}
				else
				{
					tmp_v = set_B[rand()%set_B_count];
					from_set_B_to_set_C(tmp_v);
				}
			}
			else if(set_A_count>0 && set_B_count==0)
			{
				tmp_v = set_A[rand()%set_A_count];
				from_set_A_to_set_C(tmp_v);
			}
			else if(set_A_count==0 && set_B_count>0)
			{
				tmp_v = set_B[rand()%set_B_count];
				from_set_B_to_set_C(tmp_v);
			}
			else if(set_A_count==0 && set_B_count==0)
			{
				tmp_v = set_C[rand()%set_C_count];
				from_set_C_to_set_A(tmp_v);
			}
		}
	}
}

void local_search_age(int flips_num)
{
	int selected_vertex;
	int selected_set;
	int tmp_set, tmp_v;
	int i, j;
	
	for(i=0; i<flips_num; i++)
	{
		if(set_A_count>0 && set_A_count<=constraint_b_value && set_B_count>0 && set_B_count<=constraint_b_value && solution_quality<best_solution_quality)
		{
			cout<<"step="<<step<<"; age; ";
			best_solution_quality = solution_quality;
			for(j=0; j<num_vertexs; j++) best_solution[j] = vertex_label[j];
			print_solution();
			if(best_solution_quality==0) return;
		}
		
		select_vertex_and_set_age(selected_vertex, selected_set);
		
		step++;
		
		if(selected_set==1)
		{
			from_set_C_to_set_A(selected_vertex);
			time_stamp_A[selected_vertex] = step-1;
		}
		else if(selected_set==2)
		{
			from_set_C_to_set_B(selected_vertex);
			time_stamp_B[selected_vertex] = step-1;
		}
		else if(selected_set==-1)
		{
			if(set_A_count>0 && set_B_count>0)
			{
				tmp_set = rand()%2+1;
				if(tmp_set==1)
				{
					tmp_v = set_A[rand()%set_A_count];
					from_set_A_to_set_C(tmp_v);
				}
				else
				{
					tmp_v = set_B[rand()%set_B_count];
					from_set_B_to_set_C(tmp_v);
				}
			}
			else if(set_A_count>0 && set_B_count==0)
			{
				tmp_v = set_A[rand()%set_A_count];
				from_set_A_to_set_C(tmp_v);
			}
			else if(set_A_count==0 && set_B_count>0)
			{
				tmp_v = set_B[rand()%set_B_count];
				from_set_B_to_set_C(tmp_v);
			}
			else if(set_A_count==0 && set_B_count==0)
			{
				tmp_v = set_C[rand()%set_C_count];
				from_set_C_to_set_A(tmp_v);
			}
		}
	}
}


void local_search_descent()
{
	int selected_vertex;
	int selected_set;
	int tmp_set, tmp_v;
	int i, j;
	
	while(true)
	{
		if(set_A_count>0 && set_A_count<=constraint_b_value && set_B_count>0 && set_B_count<=constraint_b_value && solution_quality<best_solution_quality)
		{
			cout<<"step="<<step<<"; descent; ";
			best_solution_quality = solution_quality;
			for(j=0; j<num_vertexs; j++) best_solution[j] = vertex_label[j];
			print_solution();
			if(best_solution_quality==0) return;
		}
		
		select_vertex_and_set_descent(selected_vertex, selected_set);
		
		step++;
		
		if(selected_set==1)
		{
			from_set_C_to_set_A(selected_vertex);
			time_stamp_A[selected_vertex] = step-1;
		}
		else if(selected_set==2)
		{
			from_set_C_to_set_B(selected_vertex);
			time_stamp_B[selected_vertex] = step-1;
		}
		else if(selected_set==-1)
		{
			break;
		}
	}
	return;
}

void local_search_tabu(int flips_num)
{
	int selected_vertex;
	int selected_set;
	int tmp_set, tmp_v;
	int i, j;
	
	for(i=0; i<flips_num; i++)
	{
		if(set_A_count>0 && set_A_count<=constraint_b_value && set_B_count>0 && set_B_count<=constraint_b_value && solution_quality<best_solution_quality)
		{
			cout<<"step="<<step<<"; tabu; ";
			best_solution_quality = solution_quality;
			for(j=0; j<num_vertexs; j++) best_solution[j] = vertex_label[j];
			print_solution();
			if(best_solution_quality==0) return;
		}
		
		select_vertex_and_set_tabu(selected_vertex, selected_set);
		
		step++;
		
		if(selected_set==1)
		{
			from_set_C_to_set_A(selected_vertex);
			time_stamp_A[selected_vertex] = step-1;
		}
		else if(selected_set==2)
		{
			from_set_C_to_set_B(selected_vertex);
			time_stamp_B[selected_vertex] = step-1;
		}
		else if(selected_set==-1)
		{
			if(set_A_count>0 && set_B_count>0)
			{
				tmp_set = rand()%2+1;
				if(tmp_set==1)
				{
					tmp_v = set_A[rand()%set_A_count];
					from_set_A_to_set_C(tmp_v);
				}
				else
				{
					tmp_v = set_B[rand()%set_B_count];
					from_set_B_to_set_C(tmp_v);
				}
			}
			else if(set_A_count>0 && set_B_count==0)
			{
				tmp_v = set_A[rand()%set_A_count];
				from_set_A_to_set_C(tmp_v);
			}
			else if(set_A_count==0 && set_B_count>0)
			{
				tmp_v = set_B[rand()%set_B_count];
				from_set_B_to_set_C(tmp_v);
			}
			else if(set_A_count==0 && set_B_count==0)
			{
				tmp_v = set_C[rand()%set_C_count];
				from_set_C_to_set_A(tmp_v);
			}
		}
	}
	return;
}

int cand_count=20;

void select_vertex_and_set(int &selected_vertex, int &selected_set)
{
	int best_vertex_A, best_vertex_B;
	int best_score_A, best_score_B;
	int i, v;
	int v_score_A, v_score_B;
	bool permit_A, permit_B;
	int pos;
	int v1, v2;
	int v1_score_A, v2_score_A, v1_score_B, v2_score_B;
	
	if(set_A_count<constraint_b_value) permit_A = true;
	else permit_A = false;
	
	if(set_B_count<constraint_b_value) permit_B = true;
	else permit_B = false;
	
	
	if(rand()%100<5)
	{
		if(permit_A && permit_B) selected_set = rand()%2+1;
		else if(permit_A && !permit_B) selected_set = 1;
		else if(!permit_A && permit_B) selected_set = 2; 
		else if(!permit_A && !permit_B) selected_set = -1;
	
		pos = rand()%set_C_count;
		v1 = set_C[pos];
		pos = rand()%set_C_count;
		v2 = set_C[pos];
		if(selected_set==1)
		{
			v1_score_A = compute_weighted_score_A(v1);
			v2_score_A = compute_weighted_score_A(v2);
			if(v1_score_A>v2_score_A) selected_vertex = v1;
			else if(v1_score_A<v2_score_A) selected_vertex = v2;
			else
			{
				if(rand()%2==0) selected_vertex = v1;
				else selected_vertex = v2;
			}
		}
		else if(selected_set==2)
		{
			v1_score_B = compute_weighted_score_B(v1);
			v2_score_B = compute_weighted_score_B(v2);
			if(v1_score_B>v2_score_B) selected_vertex = v1;
			else if(v1_score_B<v2_score_B) selected_vertex = v2;
			else
			{
				if(rand()%2==0) selected_vertex = v1;
				else selected_vertex = v2;
			}
		}
		else selected_vertex = -1;
		return;
	}
	
	if(permit_A)
	{
		best_vertex_A = -1;
		
		for(i=0; i<cand_count; i++)
		{
			v1 = set_C[rand()%set_C_count];
			if(step-time_stamp_A[v1]>tabu_tenure)
			{
				v1_score_A = compute_weighted_score_A(v1);
				best_vertex_A = v1;
				best_score_A = v1_score_A;
				break;
			}
		}
		
		for(i++; i<cand_count; i++)
		{
			v1 = set_C[rand()%set_C_count];
			if(step-time_stamp_A[v1]<=tabu_tenure) continue;
			
			v1_score_A = compute_weighted_score_A(v1);
			if(v1_score_A>best_score_A || (v1_score_A==best_score_A && time_stamp_A[v1]<time_stamp_A[best_vertex_A]))
			{
				best_vertex_A = v1;
				best_score_A = v1_score_A;
			}
		}
		
		if(best_vertex_A==-1) permit_A = false;
	}
	
	if(permit_B)
	{
		best_vertex_B = -1;
		
		for(i=0; i<cand_count; i++)
		{
			v1 = set_C[rand()%set_C_count];
			if(step-time_stamp_B[v1]>tabu_tenure)
			{
				v1_score_B = compute_weighted_score_B(v1);
				best_vertex_B = v1;
				best_score_B = v1_score_B;
				break;
			}
		}
		
		for(i++; i<cand_count; i++)
		{
			v1 = set_C[rand()%set_C_count];
			if(step-time_stamp_B[v1]<=tabu_tenure) continue;
			
			v1_score_B = compute_weighted_score_B(v1);
			if(v1_score_B>best_score_B || (v1_score_B==best_score_B && time_stamp_B[v1]<time_stamp_B[best_vertex_B]))
			{
				best_vertex_B = v1;
				best_score_B = v1_score_B;
			}
		}
		
		if(best_vertex_B==-1) permit_B = false;
	}
	
	if(permit_A && permit_B)
	{
		if(best_score_A>best_score_B)
		{
			selected_vertex = best_vertex_A;
			selected_set = 1;
		}
		else if(best_score_A<best_score_B)
		{
			selected_vertex = best_vertex_B;
			selected_set = 2;
		}
		else
		{
			if(time_stamp_A[best_vertex_A]<time_stamp_B[best_vertex_B])
			{
				selected_vertex = best_vertex_A;
				selected_set = 1;
			}
			else
			{
				selected_vertex = best_vertex_B;
				selected_set = 2;
			}
		}
	}
	else if(permit_A && !permit_B)
	{
		selected_vertex = best_vertex_A;
		selected_set = 1;
	}
	else if(!permit_A && permit_B)
	{
		selected_vertex = best_vertex_B;
		selected_set = 2;
	}
	else if(!permit_A && !permit_B)
	{
		selected_vertex = -1;
		selected_set = -1;
	}
	return;
}


void local_search()
{
	int selected_vertex;
	int selected_set;
	int tmp_set, tmp_v;
	int i, j;
	
	best_solution_quality = total_vertex_cost+1;
	
	if(set_A_count>0 && set_A_count<=constraint_b_value && set_B_count>0 && set_B_count<=constraint_b_value)
	{
		cout<<"step=0; initialization; ";
		best_solution_quality = solution_quality;
		for(j=0; j<num_vertexs; j++) best_solution[j] = vertex_label[j];
		print_solution();
		if(best_solution_quality==0) return;
	}
	
	step=0;
	int loop=0;
	int rn;
	
	while(true)
	{	
		if(set_A_count>0 && set_A_count<=constraint_b_value && set_B_count>0 && set_B_count<=constraint_b_value && solution_quality<best_solution_quality)
		{
			cout<<"step="<<step<<"; cand_count; ";
			best_solution_quality = solution_quality;
			for(j=0; j<num_vertexs; j++) best_solution[j] = vertex_label[j];
			print_solution();
			if(best_solution_quality==0) return;
		}
		
		select_vertex_and_set(selected_vertex, selected_set);
		step++;
		
		if(selected_set==1)
		{
			from_set_C_to_set_A(selected_vertex);
			time_stamp_A[selected_vertex] = step-1;
		}
		else if(selected_set==2)
		{
			from_set_C_to_set_B(selected_vertex);
			time_stamp_B[selected_vertex] = step-1;
		}
		else if(selected_set==-1)
		{
			if(set_A_count>0 && set_B_count>0)
			{
				tmp_set = rand()%2+1;
				if(tmp_set==1)
				{
					tmp_v = set_A[rand()%set_A_count];
					from_set_A_to_set_C(tmp_v);
				}
				else
				{
					tmp_v = set_B[rand()%set_B_count];
					from_set_B_to_set_C(tmp_v);
				}
			}
			else if(set_A_count>0 && set_B_count==0)
			{
				tmp_v = set_A[rand()%set_A_count];
				from_set_A_to_set_C(tmp_v);
			}
			else if(set_A_count==0 && set_B_count>0)
			{
				tmp_v = set_B[rand()%set_B_count];
				from_set_B_to_set_C(tmp_v);
			}
			else if(set_A_count==0 && set_B_count==0)
			{
				tmp_v = set_C[rand()%set_C_count];
				from_set_C_to_set_A(tmp_v);
			}
		}
		
		loop++;
		if(loop==50)
		{
			loop=0;
			times(&stop);
		 	double elap_time = (stop.tms_utime - start.tms_utime +stop.tms_stime - start.tms_stime) / sysconf(_SC_CLK_TCK);
		 	if(elap_time >= cutoff_time) break;
		}
	}
	return;
}



int main(int argc, char **argv)
{
	int seed;
	
	times(&start);
	
	if(argc!=3)
	{
		cout<<"c Usage: "<<argv[0]<<" <instance> <seed>"<<endl;
		return -1;
	}
	sscanf(argv[2], "%d", &seed);
	srand(seed);
	
	deal_instance(argv[1]);
	
	tabu_tenure = 0.1*num_vertexs;
	if(tabu_tenure>500) tabu_tenure = 500;
	
	
	initial_solution();
	
	local_search();
	
	
	if(verify_solution())
	{
		cout<<"c final answer verifying successfully!"<<endl;
		cout<<"c best="<<best_solution_quality<<endl;
	}
	
	free_my_graph();
	return 0;
}

