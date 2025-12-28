#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

//地图节点坐标表示
struct Node{
    int x, y;
    Node(int x = 0, int y = 0) : x(x), y(y) {}//构造函数
    bool operator == (const Node& other) const {//比较坐标，判断是否回到起点
        return x == other.x && y == other.y;
    }
};

//图的遍历BFS搜索状态
struct BFSsearch{
    int x, y;
    int step;
    BFSsearch(int x, int y, int step) : x(x), y(y), step(step) {}
};

class SmartWarehouseAGV{
    private:
    vector<string> map;//存储地图
    Node start, end, target;
    bool foundST, foundTE;
    int M, N, K;
    vector<vector<bool>> visited; //路径访问
    vector<vector<Node>> parent; //路径回溯
    vector<Node> pathST; //S -> T
    vector<Node> pathTE; //T -> E

    public:
    SmartWarehouseAGV() : foundST(false), foundTE(false) {}

    //地图数据读取
    void readMap(){
        cin >> M >> N >> K;
        map.resize(M);
        for(int i = 0; i < M; i++){
            cin >> map[i];
            for(int j = 0; j < N; j++){
                if(map[i][j] == 'S') start = Node(i, j);
                else if(map[i][j] == 'E') end = Node(i, j);
                else if(map[i][j] == 'T') target = Node(i, j);
            }
        }
    }

    //BFS搜索 从起点startPoint到终点endPoint 的最短路径
    vector<Node> BFS(Node startPoint, Node endPoint) {
        //初始化visited，parent
        visited.assign(M, vector<bool>(N, false));
        parent.assign(M, vector<Node>(N, Node(-1, -1)));

        queue<BFSsearch> q;
        
        //起点入队
        q.push(BFSsearch(startPoint.x, startPoint.y, 0));
        visited[startPoint.x][startPoint.y] = true;

        int dx[4] = {-1, 1, 0, 0}; //左，右
        int dy[4] = {0, 0, -1, 1}; //上，下

        //队列不为空，进入循环
        while(!q.empty()){
            BFSsearch cur = q.front();
            q.pop();

            int x = cur.x;
            int y = cur.y;

            //到达终点后回溯路径
            if(x == endPoint.x && y == endPoint.y){
                vector<Node> path;
                Node p = endPoint;

                while(!(p == startPoint)){
                    path.push_back(p);
                    p = parent[p.x][p.y];
                }
                path.push_back(startPoint);
                reverse(path.begin(), path.end());
                return path;
            }
            //四个方向上的判断
            for(int i = 0; i < 4; i++){
                int nx = x + dx[i];
                int ny = y + dy[i];

                //1.边界判断
                if(nx < 0 || nx >= M || ny < 0 || ny >= N) continue;
                //2.遇到障碍物/已访问
                if(map[nx][ny] == '1' || visited[nx][ny]) continue;

                visited[nx][ny] = true;
                parent[nx][ny] = Node(x, y);
                q.push(BFSsearch(nx, ny, cur.step + 1));
            }
        }
        //搜索失败
        return vector<Node>();
    }

    //主要实现函数
    void solve(){
        readMap();

        //S -> T
        vector<Node> pathST = BFS(start, target);
        if(pathST.empty()){
            cout << "无解" << endl;
            return;
        }
        //T -> E
        vector<Node> pathTE = BFS(target, end);
        if(pathTE.empty()){
            cout << "无解" << endl;
            return;
        }

        //电量统计
        int stepST = pathST.size() - 1;
        int stepTE = pathTE.size() - 1;

        int totalStep = stepST + stepTE;

        if(totalStep > K){
            cout << "电量不足" << endl;
            return;
        }

        //输出路径坐标
        for(size_t i = 0; i < pathST.size(); i++){
            cout << "(" << pathST[i].x << ", " << pathST[i].y << ")";
            cout << " -> ";
        }
        for(size_t i = 0; i < pathTE.size(); i++){
            cout << "(" << pathTE[i].x << ", " << pathTE[i].y << ")";
            if((i+1) < pathTE.size()){
                cout << " -> ";
            }
        }
        cout << endl;

        cout << "总步数：" << totalStep << endl;
        cout << "状态：任务完成" << endl << endl;

        //可视化路径
        for(size_t i = 0; i < pathST.size(); i++){
            Node p = pathST[i];
            if(map[p.x][p.y] == '0'){
                map[p.x][p.y] = '.';
            }
        }
        for(size_t i = 0; i < pathTE.size(); i++){
            Node p = pathTE[i];
            if(map[p.x][p.y] == '0'){
                map[p.x][p.y] = '*';
            }
        }
        cout << "可视化路径：" << endl;
        for(int i = 0; i < M; i++){
            cout << map[i] << endl;
        }
    }
};

int main(){
    SmartWarehouseAGV agv;
    agv.solve();
    return 0;
}