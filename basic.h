#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int constraint_b_value;
int num_vertexs;
int num_edges;
int total_vertex_cost;

long long step;
long long maximum_step = 4000000000000000000ll;

vector<int> *my_graph;
int *vertex_cost;
int *vertex_label;
int *vertex_pos;
long long *time_stamp_A;
long long *time_stamp_B;
int *vertex_weight;
int total_vertex_weight;

int *large_weight_vertexes;
int large_weight_vertexes_count;

int *set_A;
int set_A_count;
int *set_B;
int set_B_count;

int *set_C;
int set_C_count;

int *best_solution;
int best_solution_quality;
int solution_quality;

void malloc_my_graph();
void free_my_graph();
void print_graph();
void print_solution();
void print_solution_in_detail();
void print_current_solution_in_detail();
void print_set_A();
void print_set_B();
void print_set_C();
bool is_neighbor(int v1, int v2);
bool is_any_neighbor_in_set_A(int v);
bool is_any_neighbor_in_set_B(int v);
int compute_score_A(int v);
int compute_score_B(int v);
void deal_instance(char *filename);
void initial_solution();
void select_vertex_and_set(int &selected_vertex, int &selected_set);
void from_set_C_to_set_A(int v);
void from_set_C_to_set_B(int v);
void from_set_A_to_set_C(int v);
void from_set_B_to_set_C(int v);
void local_search();
bool verify_solution();


void malloc_my_graph()
{
	int malloc_length = num_vertexs+10;
	my_graph = new vector<int> [malloc_length];
	vertex_cost = new int [malloc_length];
	vertex_label = new int [malloc_length];
	vertex_pos = new int [malloc_length];
	time_stamp_A = new long long [malloc_length];
	time_stamp_B = new long long [malloc_length];
	vertex_weight = new int [malloc_length];
	set_A = new int [malloc_length];
	set_A_count = 0;
	set_B = new int [malloc_length];
	set_B_count = 0;
	set_C = new int [malloc_length];
	set_C_count = 0;
	solution_quality = 0;
	best_solution = new int [malloc_length];
	
	total_vertex_cost = 0;
	total_vertex_weight = 0;
	
	large_weight_vertexes = new int [malloc_length];
	large_weight_vertexes_count = 0;
	return;
}

void free_my_graph()
{
	delete []my_graph;
	delete []vertex_cost;
	delete []vertex_label;
	delete []vertex_pos;
	delete []vertex_weight;
	delete []time_stamp_A;
	delete []time_stamp_B;
	delete []set_A;
	delete []set_B;
	delete []set_C;
	delete []best_solution;
	
	delete []large_weight_vertexes;
	return;
}

void print_graph()
{
	int i, j, length;
	for(i=0; i<num_vertexs; i++)
	{
		cout<<i<<": ";
		length = my_graph[i].size();
		for(j=0; j<length; j++)
		{
			cout<<my_graph[i][j]<<" ";
		}
		cout<<endl;
	}
}

void print_solution()
{
	cout<<"|A|="<<set_A_count<<"; |B|="<<set_B_count<<"; |C|="<<set_C_count<<"; best="<<best_solution_quality<<endl;
}

void print_solution_in_detail()
{
	cout<<"|A|="<<set_A_count<<"; |B|="<<set_B_count<<"; |C|="<<set_C_count<<"; best="<<best_solution_quality<<endl;

	int i;
	for(i=0; i<num_vertexs; i++)
	{
		cout<<i<<" ";
		if(vertex_label[i]==0) cout<<"C";
		else if(vertex_label[i]==1) cout<<"A";
		else if(vertex_label[i]==2) cout<<"B";
		cout<<endl;
	}
}

void print_current_solution_in_detail()
{
	cout<<"|A| = "<<set_A_count<<"; |B| = "<<set_B_count<<"; |C| = "<<set_C_count<<"; solution_quality = "<<solution_quality<<endl;

	int i;
	for(i=0; i<num_vertexs; i++)
	{
		cout<<i<<" ";
		if(vertex_label[i]==0) cout<<"C";
		else if(vertex_label[i]==1) cout<<"A";
		else if(vertex_label[i]==2) cout<<"B";
		cout<<endl;
	}
}

void print_set_A()
{
	int i;
	cout<<"set A: ";
	for(i=0; i<num_vertexs; i++)
	{
		if(vertex_label[i]==1) cout<<i<<" ";
	}
	cout<<endl;
}

void print_set_B()
{
	int i;
	cout<<"set B: ";
	for(i=0; i<num_vertexs; i++)
	{
		if(vertex_label[i]==2) cout<<i<<" ";
	}
	cout<<endl;
}


void print_set_C()
{
	int i;
	cout<<"set C: ";
	for(i=0; i<num_vertexs; i++)
	{
		if(vertex_label[i]==0) cout<<i<<" ";
	}
	cout<<endl;
}

bool is_neighbor(int v1, int v2)
{
	int left = 0;
	int right = my_graph[v1].size()-1;
	int middle, tmp_v;
	
	while(left<=right)
	{
		middle = (left+right)/2;
		tmp_v = my_graph[v1][middle];
		if(v2<tmp_v)
		{
			right = middle-1;
		}
		else if(v2>tmp_v)
		{
			left = middle+1;
		}
		else
		{
			return true;
		}
	}
	return false;
}

bool is_any_neighbor_in_set_A(int v)
{
	int i, v2;
	int length = my_graph[v].size();
	for(i=0; i<length; i++)
	{
		v2 = my_graph[v][i];
		if(vertex_label[v2]==1) return true;
	}
	return false;
}

bool is_any_neighbor_in_set_B(int v)
{
	int i, v2;
	int length = my_graph[v].size();
	for(i=0; i<length; i++)
	{
		v2 = my_graph[v][i];
		if(vertex_label[v2]==2) return true;
	}
	return false;
}

int compute_score_A(int v)
{
	int v_score_A = vertex_cost[v];
	int i, v2;
	int length = my_graph[v].size();
	
	for(i=0; i<length; i++)
	{
		v2 = my_graph[v][i];
		if(vertex_label[v2]==2) v_score_A -= vertex_cost[v2];
	}
	
	return v_score_A;
}

int compute_score_B(int v)
{
	int v_score_B = vertex_cost[v];
	int i, v2;
	int length = my_graph[v].size();
	
	for(i=0; i<length; i++)
	{
		v2 = my_graph[v][i];
		if(vertex_label[v2]==1) v_score_B -= vertex_cost[v2];
	}
	
	return v_score_B;
}


void deal_instance(char *filename)
{
	int i, v, v1, v2, tmp_vc, tmp_num_edges;
	string tmp_cmd, line;
	const char* c_line;
	ifstream fin;
	fin.open(filename, ios::in);
	
	getline(fin, line);
	c_line = line.c_str();
	istringstream input_line(line);
	input_line>>constraint_b_value>>num_vertexs>>tmp_num_edges;
	malloc_my_graph();
	
	total_vertex_cost = 0;
	total_vertex_weight = 0;
	for(i=0; i<num_vertexs; i++)
	{
		getline(fin, line);
		c_line = line.c_str();
		istringstream input_line(line);
		input_line>>tmp_cmd>>v>>tmp_vc;
		vertex_cost[v] = tmp_vc;
		total_vertex_cost += tmp_vc;
		time_stamp_A[v]=-1;
		time_stamp_B[v]=-1;
		vertex_weight[v] = vertex_cost[v];
		total_vertex_weight += vertex_weight[v];
	}
	
	num_edges = 0;
	for(i=0; i<tmp_num_edges; i++)
	{
		getline(fin, line);
		c_line = line.c_str();
		istringstream input_line(line);
		input_line>>tmp_cmd>>v1>>v2>>tmp_cmd;
		if(v1==v2) continue;
		if(is_neighbor(v1, v2)) continue;
		my_graph[v1].push_back(v2);
		my_graph[v2].push_back(v1);
		num_edges++;
	}
	
	for(i=0; i<num_vertexs; i++)
	{
		sort(my_graph[i].begin(), my_graph[i].end());
	}
	
	fin.close();
}

void initial_solution()
{
	int i, v, rn;
	for(i=0; i<num_vertexs; i++)
	{
		rn = rand()%2;
		if(rn==0)
		{
			if(set_A_count<constraint_b_value)
			{
				set_A[set_A_count] = i;
				vertex_label[i] = 1;
				vertex_pos[i] = set_A_count;
				set_A_count++;
			}
			else if(set_B_count<constraint_b_value)
			{
				set_B[set_B_count] = i;
				vertex_label[i] = 2;
				vertex_pos[i] = set_B_count;
				set_B_count++;
			}
			else
			{
				set_C[set_C_count] = i;
				vertex_label[i] = 0;
				vertex_pos[i] = set_C_count;
				set_C_count++;
				solution_quality += vertex_cost[i];
			}
		}
		else
		{
			if(set_B_count<constraint_b_value)
			{
				set_B[set_B_count] = i;
				vertex_label[i] = 2;
				vertex_pos[i] = set_B_count;
				set_B_count++;
			}
			else if(set_A_count<constraint_b_value)
			{
				set_A[set_A_count] = i;
				vertex_label[i] = 1;
				vertex_pos[i] = set_A_count;
				set_A_count++;
			}
			else
			{
				set_C[set_C_count] = i;
				vertex_label[i] = 0;
				vertex_pos[i] = set_C_count;
				set_C_count++;
				solution_quality += vertex_cost[i];
			}
		}
	}
	
	for(i=set_B_count-1; i>=0; i--)
	{
		v = set_B[i];
		if(is_any_neighbor_in_set_A(v))
		{
			set_B[i] = set_B[--set_B_count];
			vertex_pos[set_B[i]] = i;
			
			set_C[set_C_count] = v;
			vertex_label[v] = 0;
			vertex_pos[v] = set_C_count;
			set_C_count++;
			solution_quality += vertex_cost[v];
		}
	}
}

void from_set_C_to_set_A(int v)
{
	int pos = vertex_pos[v];
	set_C[pos] = set_C[--set_C_count];
	vertex_pos[set_C[pos]] = pos;
	solution_quality -= vertex_cost[v];
	
	set_A[set_A_count] = v;
	vertex_label[v] = 1;
	vertex_pos[v] = set_A_count;
	set_A_count++;
	
	int length = my_graph[v].size();
	int v2, i;
	for(i=0; i<length; i++)
	{
		v2 = my_graph[v][i];
		if(vertex_label[v2]==2)
		{
			pos = vertex_pos[v2];
			set_B[pos] = set_B[--set_B_count];
			vertex_pos[set_B[pos]] = pos;
			
			set_C[set_C_count] = v2;
			vertex_label[v2] = 0;
			vertex_pos[v2] = set_C_count;
			set_C_count++;
			solution_quality += vertex_cost[v2];
		}
	}
}

void from_set_C_to_set_B(int v)
{
	int pos = vertex_pos[v];
	set_C[pos] = set_C[--set_C_count];
	vertex_pos[set_C[pos]] = pos;
	solution_quality -= vertex_cost[v];
	
	set_B[set_B_count] = v;
	vertex_label[v] = 2;
	vertex_pos[v] = set_B_count;
	set_B_count++;
	
	int length = my_graph[v].size();
	int v2, i;
	for(i=0; i<length; i++)
	{
		v2 = my_graph[v][i];
		if(vertex_label[v2]==1)
		{
			pos = vertex_pos[v2];
			set_A[pos] = set_A[--set_A_count];
			vertex_pos[set_A[pos]] = pos;
			
			set_C[set_C_count] = v2;
			vertex_label[v2] = 0;
			vertex_pos[v2] = set_C_count;
			set_C_count++;
			solution_quality += vertex_cost[v2];
		}
	}
}

void from_set_A_to_set_C(int v)
{
	int pos = vertex_pos[v];
	set_A[pos] = set_A[--set_A_count];
	vertex_pos[set_A[pos]] = pos;
	
	set_C[set_C_count] = v;
	vertex_label[v] = 0;
	vertex_pos[v] = set_C_count;
	set_C_count++;
	solution_quality += vertex_cost[v];
}

void from_set_B_to_set_C(int v)
{
	int pos = vertex_pos[v];
	set_B[pos] = set_B[--set_B_count];
	vertex_pos[set_B[pos]] = pos;
	
	set_C[set_C_count] = v;
	vertex_label[v] = 0;
	vertex_pos[v] = set_C_count;
	set_C_count++;
	solution_quality += vertex_cost[v];
}

bool verify_solution()
{
	int malloc_length = num_vertexs+10;
	int *tmp_set_A, *tmp_set_B, *tmp_set_C;
	int tmp_set_A_count=0, tmp_set_B_count=0, tmp_set_C_count=0;
	int i, j, v1, v2, length;
	
	tmp_set_A = new int [malloc_length];
	tmp_set_B = new int [malloc_length];
	tmp_set_C = new int [malloc_length];
	
	for(i=0; i<num_vertexs; i++)
	{
		if(best_solution[i]==1)
		{
			tmp_set_A[tmp_set_A_count++] = i;
		}
		else if(best_solution[i]==2)
		{
			tmp_set_B[tmp_set_B_count++] = i;
		}
		else if(best_solution[i]==0)
		{
			tmp_set_C[tmp_set_C_count++] = i;
		}
	}
	
	if(tmp_set_A_count>constraint_b_value) 
	{
		cout<<"c tmp_set_A_count>constraint_b_value"<<endl;
		delete []tmp_set_A;
		delete []tmp_set_B;
		delete []tmp_set_C;
		return false;
	}
	if(tmp_set_A_count==0) 
	{
		cout<<"c tmp_set_A_count==0"<<endl;
		delete []tmp_set_A;
		delete []tmp_set_B;
		delete []tmp_set_C;
		return false;
	}
	if(tmp_set_B_count>constraint_b_value)
	{
		cout<<"c tmp_set_B_count>constraint_b_value"<<endl;
		delete []tmp_set_A;
		delete []tmp_set_B;
		delete []tmp_set_C;
		return false;
	}
	if(tmp_set_B_count==0) 
	{
		cout<<"c tmp_set_B_count==0"<<endl;
		delete []tmp_set_A;
		delete []tmp_set_B;
		delete []tmp_set_C;
		return false;
	}
	
	for(i=0; i<tmp_set_B_count; i++)
	{
		v1 = tmp_set_B[i];
		length = my_graph[v1].size();
		for(j=0; j<length; j++)
		{
			v2 = my_graph[v1][j];
			if(best_solution[v2]==1)
			{
				cout<<"c "<<v2<<" and "<<v1<<" are neighors, but are not in set A and set B, respectively"<<endl;
				delete []tmp_set_A;
				delete []tmp_set_B;
				delete []tmp_set_C;
				return false;
			}
		}
	}
	
	int tmp_solution_quality = 0;
	for(i=tmp_set_C_count-1; i>=0; i--)
	{
		v1 = tmp_set_C[i];
		tmp_solution_quality += vertex_cost[v1];
	}
	
	if(tmp_solution_quality!=best_solution_quality)
	{
		cout<<"c tmp_solution_quality!=best_solution_quality"<<endl;
		delete []tmp_set_A;
		delete []tmp_set_B;
		delete []tmp_set_C;
		return false;
	}
	
	delete []tmp_set_A;
	delete []tmp_set_B;
	delete []tmp_set_C;
	
	return true;
}