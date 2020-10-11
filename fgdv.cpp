/*#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
using namespace std;
int main()
{
	freopen("bfs.in", "r", stdin);       // для чтения из файла a.in
	freopen("bfs.out", "w", stdout);  // для записи в  файл   a.out:

	int N, start, finish;//s-start f-finish
	cin >> N >> start >> finish;
	start--;
	finish--;
	vector<vector<int>> a(N);
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			int t = 0;
			cin >> t;
			if (t==1) a[i].push_back(j);
		}
	queue<int> q;
	q.push(start);
	vector<int>dist(N, INT_MAX);
	dist[start] = 0;
	while (!q.empty())
	{
		int cur = q.front();
		q.pop();
		for (int i = 0; i < a[cur].size(); i++)
		{
			int next = a[cur][i];
			if (dist[cur] + 1 < dist[next])
			{
				q.push(next);
				dist[next] = dist[cur] + 1;
			}
		}
	}
	cout << ((dist[finish]!=INT_MAX)?dist[finish]:0);
}*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;
int work2nd(vector<int> vect, int el)
{
	int ret = 0;
	for (int v = 0; v < vect.size(); v++)
	{
		ret += (vect[v] == el);
	}
	return ret;
}
int main()
{
	freopen("rootdist.in", "r", stdin);       // для чтения из файла a.in
	freopen("rootdist.out", "w", stdout);  // для записи в  файл   a.out
	int n;
	cin >> n;
	vector<vector<int>> a(n);
	for (int i = 1; i < n; i++)
	{
		int t;
		cin >> t;
		a[t-1].push_back(i);
	}
	vector<int> dist(n, INT_MAX);
	queue<int> q;
	int start = 0;
	q.push(start);
	dist[0] = 0;
	while (!q.empty())
	{
		int cur = q.front();
		q.pop();
		for (int i = 0; i < a[cur].size(); i++)
		{
			int next = a[cur][i];
			if (dist[cur] + 1 < dist[next])
			{
				q.push(next);
				dist[next] = dist[cur] + 1;
			}
		}
	}
	int m = *std::max_element(dist.begin(), dist.end());
	cout << m <<endl<<work2nd(dist,m);
}//
