#include<bits/stdc++.h>
using namespace std;

void input() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    #ifndef ONLINE_JUDGE
    freopen("Input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
    #endif 
}

void printTable(vector<vector<pair<int,int>>> &costs, vector<int> &supply, vector<int> &demand, int n, int m) {
    cout<<"\t\t";
    for(int i=0; i<m; i++) {
        cout<<"Warehouse "<<i+1<<"\t";
    }
    cout<<"Supply"<<endl;

    for(int i=0; i<n; i++) {
        cout<<"Factory "<<i+1<<"\t";
        for(int j=0; j<m; j++) {
            if(costs[i][j].second == -1) {
                cout<<costs[i][j].first<<"\t\t";
            }
            else if(costs[i][j].second == 0) {
                cout<<costs[i][j].first<<"[~]"<<"\t\t";
            }
            else {
                cout<<costs[i][j].first<<"["<<costs[i][j].second<<"]"<<"\t\t";
            }
        }
        cout<<supply[i]<<endl;
    }

    int sum = 0;
    cout<<"Demand\t\t";
    for(int i=0; i<m; i++) {
        cout<<demand[i]<<"\t\t";
        sum += demand[i];
    }
    cout<<sum<<endl;
    cout<<endl;
}

void balance(vector<vector<pair<int,int>>> &costs, int &n, int &m, vector<int> &supply, vector<int> &demand, int totSupply, int totDemand) {
    if(totDemand == totSupply) {
        return;
    }
    else if(totSupply > totDemand) {
        m++;
        for(int i=0; i<n; i++) {
            costs[i].push_back({0,-1});
        }
        demand.push_back(totSupply-totDemand);
    }
    else {
        n++;
        vector<pair<int,int>> v;
        for(int i=0; i<m; i++) {
            v.push_back({0,-1});
        }
        costs.push_back(v);
        supply.push_back(totDemand-totSupply);
    }
}

vector<pair<int,int>> checkLoop(vector<vector<pair<int,int>>> &costs, int n, int m, int i, int j, string prevMove, pair<int,int> destination, vector<vector<bool>> &visited) {
    if(destination.first == i && destination.second == j) {
        return {destination};
    }
    if(i >= n || j >= m || i < 0 || j < 0 || visited[i][j]) {
        return {};
    }

    vector<vector<bool>> visited1 = visited;
    visited[i][j] = true;
    vector<pair<int,int>> loop;
    if(costs[i][j].second == -1) {
        //Unoccupied cell
        if(prevMove == "left") {
            return checkLoop(costs, n, m, i, j-1, "left", destination, visited);
        }
        if(prevMove == "right") {
            return checkLoop(costs, n, m, i, j+1, "right", destination, visited);
        }
        if(prevMove == "up") {
            return checkLoop(costs, n, m, i-1, j, "up", destination, visited);
        }
        if(prevMove == "down") {
            return checkLoop(costs, n, m, i+1, j, "down", destination, visited);
        }
    }
    else {
        //Occupied cell
        if(prevMove == "left") {
            loop = checkLoop(costs, n, m, i, j-1, "left", destination, visited);
            visited = visited1;
            if(loop.size() > 0) {
                return loop;
            }

            loop.push_back({i, j});
            vector<pair<int,int>> loop2;

            loop2 = checkLoop(costs, n, m, i-1, j, "up", destination, visited);
            visited = visited1;
            loop.insert(loop.end(), loop2.begin(), loop2.end());
            if(loop.size() > 1) {
                return loop;
            }

            loop2 = checkLoop(costs, n, m, i+1, j, "down", destination, visited);
            visited = visited1;
            loop.insert(loop.end(), loop2.begin(), loop2.end());
            if(loop.size() > 1) {
                return loop;
            }
        }
        else if(prevMove == "right") {
            loop = checkLoop(costs, n, m, i, j+1, "right", destination, visited);
            visited = visited1;
            if(loop.size() > 0) {
                return loop;
            }

            loop.push_back({i, j});
            vector<pair<int,int>> loop2;

            loop2 = checkLoop(costs, n, m, i-1, j, "up", destination, visited);
            visited = visited1;
            loop.insert(loop.end(), loop2.begin(), loop2.end());
            if(loop.size() > 1) {
                return loop;
            }

            loop2 = checkLoop(costs, n, m, i+1, j, "down", destination, visited);
            visited = visited1;
            loop.insert(loop.end(), loop2.begin(), loop2.end());
            if(loop.size() > 1) {
                return loop;
            }
        }
        else if(prevMove == "up") {
            loop = checkLoop(costs, n, m, i-1, j, "up", destination, visited);
            visited = visited1;
            if(loop.size() > 0) {
                return loop;
            }

            loop.push_back({i, j});
            vector<pair<int,int>> loop2; 

            loop2 = checkLoop(costs, n, m, i, j-1, "left", destination, visited);
            visited = visited1;
            loop.insert(loop.end(), loop2.begin(), loop2.end());
            if(loop.size() > 1) {
                return loop;
            }

            loop2 = checkLoop(costs, n, m, i, j+1, "right", destination, visited);
            visited = visited1;
            loop.insert(loop.end(), loop2.begin(), loop2.end());
            if(loop.size() > 1) {
                return loop;
            }
        }
        else if(prevMove == "down") {
            loop = checkLoop(costs, n, m, i+1, j, "down", destination, visited);
            visited = visited1;
            if(loop.size() > 0) {
                return loop;
            }

            loop.push_back({i, j});
            vector<pair<int,int>> loop2; 

            loop2 = checkLoop(costs, n, m, i, j+1, "right", destination, visited);
            visited = visited1;
            loop.insert(loop.end(), loop2.begin(), loop2.end());
            if(loop.size() > 1) {
                return loop;
            }

            loop2 = checkLoop(costs, n, m, i, j-1, "left", destination, visited);
            visited = visited1;
            loop.insert(loop.end(), loop2.begin(), loop2.end());
            if(loop.size() > 1) {
                return loop;
            }
        }
    }
    return {};
}

vector<pair<int,int>> makeLoop(vector<vector<pair<int,int>>> &costs, int n, int m, int row, int col) {
    vector<vector<bool>> downVisited(n, vector<bool>(m, false));
    vector<pair<int,int>> downLoop = checkLoop(costs, n, m, row+1, col, "down", {row,col}, downVisited);

    vector<vector<bool>> upVisited(n, vector<bool>(m, false));
    vector<pair<int,int>> upLoop = checkLoop(costs, n, m, row-1, col, "up", {row,col}, upVisited);

    vector<vector<bool>> rightVisited(n, vector<bool>(m, false));
    vector<pair<int,int>> rightLoop = checkLoop(costs, n, m, row, col+1, "right", {row,col}, rightVisited);

    vector<vector<bool>> leftVisited(n, vector<bool>(m, false));
    vector<pair<int,int>> leftLoop = checkLoop(costs, n, m, row, col-1, "left", {row,col}, leftVisited);

    vector<pair<int,int>> loop;
    if(downLoop.size() > 0) {
        loop = downLoop;
    }
    else if(leftLoop.size() > 0) {
        loop = leftLoop;
    }
    else if(rightLoop.size() > 0) {
        loop = rightLoop;
    }
    else if(upLoop.size() > 0) {
        loop = upLoop;
    }
    return loop;
}

void resolveDegeneracy(vector<vector<pair<int,int>>> &costs, int n, int m, int reqAllocations) {
    vector<vector<int>> independentCells;
    for(int row=0; row<n; row++) {
        for(int col=0; col<m; col++) {
            if(costs[row][col].second == -1) {
                vector<pair<int,int>> loop = makeLoop(costs, n, m, row, col);

                if(loop.size() == 0) {
                    independentCells.push_back({costs[row][col].first,row,col});
                }
            }
        }
    }

    sort(independentCells.begin(), independentCells.end());
    costs[independentCells[0][1]][independentCells[0][2]].second = 0;
    reqAllocations--;

    if(reqAllocations > 0) {
        resolveDegeneracy(costs, n, m, reqAllocations);
    }
}

int NorthWestCorner(vector<vector<pair<int,int>>> &costs, vector<int> supply, vector<int> demand, int n, int m) {
    vector<int> supply1 = supply;
    vector<int> demand1 = demand;
    int row = 0, col = 0;
    while((row < n) && (col < m)) {
        int allocation = min(supply[row], demand[col]);
        costs[row][col].second = allocation;

        supply[row] -= allocation;
        demand[col] -= allocation;

        if(supply[row] == 0) {
            row++;
        }
        if(demand[col] == 0) {
            col++;
        }
    }

    int initialCost = 0;
    for(row=0; row<n; row++) {
        for(col=0; col<m; col++) {
            if(costs[row][col].second > 0) {
                initialCost += costs[row][col].first * costs[row][col].second;
            }
        }
    }
    cout<<endl;

    cout<<"Initial Allocations: "<<endl;
    printTable(costs, supply1, demand1, n, m);

    int noOfAllocation = 0;
    for(int row=0; row<n; row++) {
        for(int col=0; col<m; col++) {
            if(costs[row][col].second >= 0) {
                noOfAllocation++;
            }
        }
    }

    if(noOfAllocation < n+m-1) {
        resolveDegeneracy(costs, n, m, n+m-1-noOfAllocation);
        cout<<"\nDegeneracy resolved: "<<endl;
        printTable(costs, supply1, demand1, n, m);
    }

    return initialCost;
}

vector<int> calUV(vector<vector<pair<int,int>>> &costs, int n, int m) {
    vector<int> u(n, INT_MIN);
    vector<int> v(m, INT_MIN);
    u[0] = 0;
    int noOfU = 1, noOfV = 0;
    while(noOfU != n || noOfV != m) {
        for(int row=0; row<n; row++) {
            for(int col=0; col<m; col++) {
                if(costs[row][col].second >= 0) {
                    if((u[row] == INT_MIN && v[col] == INT_MIN) || (u[row] != INT_MIN && v[col] != INT_MIN)) {
                        continue;
                    }
                    else if(u[row] == INT_MIN) {
                        u[row] = costs[row][col].first - v[col];
                        noOfU++;
                    }
                    else if(v[col] == INT_MIN) {
                        v[col] = costs[row][col].first - u[row];
                        noOfV++;
                    }
                }
            }
        }
    }
    u.insert(u.end(), v.begin(), v.end());
    return u;
}

vector<vector<int>> calD(vector<vector<pair<int,int>>> &costs, vector<int> &u, vector<int> &v, int n, int m) {
    vector<vector<int>> d;
    for(int row=0; row<n; row++) {
        for(int col=0; col<m; col++) {
            if(costs[row][col].second == -1) {
                int dij = costs[row][col].first - (u[row] + v[col]);
                if(dij < 0) {
                    d.push_back({dij, costs[row][col].first, row, col});
                }
            }
        }
    }
    return d;
}

int MODI(vector<vector<pair<int,int>>> &costs, vector<int> &supply, vector<int> &demand, int n, int m) {
    vector<int> uv = calUV(costs, n, m);
    vector<int> u = {uv.begin(), uv.begin()+n};
    vector<int> v = {uv.begin()+n, uv.end()};

    vector<vector<int>> d = calD(costs, u, v, n, m);
    sort(d.begin(), d.end());

    while(d.size() > 0) {
        vector<pair<int,int>> loop = makeLoop(costs, n, m, d[0][2], d[0][3]);

        int minAllocation = INT_MAX;
        for(int i=0; i<loop.size(); i+=2) {
            minAllocation = min(minAllocation, costs[loop[i].first][loop[i].second].second);
        }

        for(int i=0; i<loop.size(); i++) {
            if(i % 2 == 1) {
                if(costs[loop[i].first][loop[i].second].second == -1) {
                    costs[loop[i].first][loop[i].second].second = minAllocation;
                }
                else {
                    costs[loop[i].first][loop[i].second].second += minAllocation;
                }
            }
            else {
                if(costs[loop[i].first][loop[i].second].second == 0) {
                    costs[loop[i].first][loop[i].second].second = -1;
                }
                else {
                    costs[loop[i].first][loop[i].second].second -= minAllocation;
                    if(costs[loop[i].first][loop[i].second].second == 0) {
                        costs[loop[i].first][loop[i].second].second = -1;
                    }
                }
            }
        }

        int noOfAllocation = 0;
        for(int row=0; row<n; row++) {
            for(int col=0; col<m; col++) {
                if(costs[row][col].second >= 0) {
                    noOfAllocation++;
                }
            }
        }

        if(noOfAllocation < n+m-1) {
            resolveDegeneracy(costs, n, m, n+m-1-noOfAllocation);
        }

        uv = calUV(costs, n, m);
        u = {uv.begin(), uv.begin()+n};
        v = {uv.begin()+n, uv.end()};
        d = calD(costs, u, v, n, m);
        sort(d.begin(), d.end());
    }

    int finalCost = 0;
    for(int row=0; row<n; row++) {
        for(int col=0; col<m; col++) {
            if(costs[row][col].second != -1) {
                finalCost += costs[row][col].first * costs[row][col].second;
            }
        }
    }

    return finalCost;
}

int main() {
    // input();

    //Taking input:
    cout<<"\nEnter the number of factories: "<<endl;
    int n;
    cin>>n;

    cout<<"\nEnter the number of warehouses: "<<endl;
    int m;
    cin>>m;

    cout<<"\nEnter the cost of transportations: "<<endl;
    vector<vector<pair<int,int>>> costs(n, vector<pair<int,int>>(m, {0, -1}));
    for(int i=0; i<n; i++) {
        for(int j=0; j<m; j++) {
            cin>>costs[i][j].first;
        }
    }

    cout<<"\nEnter the supply of factories: "<<endl;
    vector<int> supply(n);
    for(auto &i:supply) {
        cin>>i;
    }

    cout<<"\nEnter the demand of warehouses: "<<endl;
    vector<int> demand(m);
    for(auto &i:demand) {
        cin>>i;
    }
    
    //Printing the initial table
    cout<<"\nInitial Table: "<<endl;
    printTable(costs, supply, demand, n, m);

    //Check if problem is balanced
    int totSupply = 0, totDemand = 0;
    for(int i=0; i<n; i++) {
        totSupply += supply[i];
    }
    for(int i=0; i<m; i++) {
        totDemand += demand[i];
    }

    if(totSupply != totDemand) {
        cout<<"\nBalanced Problem: "<<endl;
        balance(costs, n, m, supply, demand, totSupply, totDemand);
        printTable(costs, supply, demand, n, m);
    }

    // Calculating initial cost
    int initialCost = NorthWestCorner(costs, supply, demand, n, m);
    cout<<"Initial Cost:\t"<<initialCost<<endl<<endl;

    //Modified distribution Method
    int finalCost = MODI(costs, supply, demand, n, m);
    cout<<"\nFinal Table: "<<endl;
    printTable(costs, supply, demand, n, m);
    cout<<"Final cost:\t"<<finalCost<<endl;

    return 0;
}