#include <bits/stdc++.h>
#include <omp.h>

using namespace std;


class Graph {
private:
	int v;
	vector<vector<int>> adj;

public:
	Graph(int v) {
		this->v = v;
		adj.resize(v);
	}

	void addEdge(int src, int dest) {
		adj[src].push_back(dest);
		adj[dest].push_back(src);
	}

	void bfs(int src) {
		queue<int> q;
		vector<bool> visited(v,false);

		q.push(src);

		visited[src] = true;

		while(!q.empty()) {
			int node = q.front();
			q.pop();

			cout<<node<<" ";

			#pragma omp parallel for
			for(int i=0;i<adj[node].size();i++) {
				int adj_node = adj[node][i];
				if(!visited[adj_node]) {
					visited[adj_node] = true;
					q.push(adj_node);
				}
			}
		}
	}


	void dfsUtil(int src, vector<bool> &visited) {
		visited[src] = true;
		cout<<src<<" ";

		#pragma omp parallel for
		for(int i=0;i<adj[src].size();i++) {
			int adj_node = adj[src][i];
			if(!visited[adj_node]) {
				dfsUtil(adj_node,visited);
			}
		}
	}

	void dfs(int src) {
		vector<bool> visited(v,false);
		dfsUtil(src,visited);
	}
};

int main() {
	Graph g(4);

	int e;

	cout<<"Enter no. of edges: ";
	cin>>e;
	cout<<"\n\nEnter edges:\n";

	for(int i=0;i<e;i++) {
		int src,dest;
		cin>>src>>dest;
		g.addEdge(src,dest);
	}

	cout<<endl;

	int src;

	cout<<"Enter source vertex for dfs: ";
	cin>>src;

	g.dfs(src);
}

