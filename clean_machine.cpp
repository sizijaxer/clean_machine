#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <windows.h>
using namespace std;
struct point{
    int row;
    int col;
    char type;
    bool isclean;
    ///for BFS
    bool canvisited=true;
    int pre_i;
    int pre_j;
    ///
};
struct machine{
    int i;
    int j;
    char type = '$';
};

int rows;
int cols;
point** map;
int battery;
point charge;
int bt_remain;
machine robot;
bool first_time_return = true;
int flag = 0;
vector<point> path_order;
stack<point> back_path_order;
vector<point>real_back_path_order;
int dir[4]={0,1,2,3};///上右下左
void show_map();
bool isdirty();
bool can_move(int, int);
void DFS(int, int);
int find_back_path_by_BFS(int, int, int);
void back_to_battery();
int main(){
    ////初始化
    cin>>rows>>cols;
    cin>>battery;
    bt_remain = battery;
    map = new point*[rows];
    for(int i=0;i<rows;i++){
        map[i] = new point[cols];
    }
    string s;
    for(int i=0;i<rows;i++){
        cin>>s;
        for(int j=0;j<cols;j++){
            if(s[j]=='1'){
                map[i][j].row = i;
                map[i][j].col = j;
                map[i][j].type = '1';
                map[i][j].isclean = true;
                map[i][j].canvisited = false;
            }
            else if(s[j]=='0'){
                map[i][j].row = i;
                map[i][j].col = j;
                map[i][j].type = '0';
                map[i][j].isclean = false;
                map[i][j].canvisited = true;
            }
            else if(s[j]=='R'){
                ///機器初始位置
                map[i][j].row = robot.i = charge.row =i;
                map[i][j].col = robot.j = charge.col =j;
                ////
                map[i][j].type = 'R';
                map[i][j].isclean = true;
                map[i][j].canvisited = true;
            }
        }
    }
    show_map();
    while(isdirty()){
        ///找移動方法
        ///上右下左
        //cout<<"while.. ";
        path_order.push_back(charge);
        if(robot.i-1>=0 && map[robot.i-1][robot.j].isclean==false){
            ///start point
            flag = 0;
            cout<<"DFS up: ";
            bt_remain--;
            int next_i = robot.i-1;
            int next_j = robot.j;
            DFS(next_i,next_j);
            if(flag==0)bt_remain--;
        }
        //cout<<isdirty()<<"\n";
        if(robot.j+1<cols && map[robot.i][robot.j+1].isclean==false){
            flag = 0;
            cout<<"DFS right: ";
            bt_remain--;
            int next_i = robot.i;
            int next_j = robot.j+1;
            DFS(next_i,next_j);
            if(flag==0)bt_remain--;
        }
        if(robot.i+1<rows && map[robot.i+1][robot.j].isclean==false){
            flag = 0;
            cout<<"DFS down: ";
            bt_remain--;
            int next_i = robot.i+1;
            int next_j = robot.j;
            DFS(next_i,next_j);
            if(flag==0)bt_remain--;
        }
        //cout<<"finish0";
        if(robot.j-1>=0 && map[robot.i][robot.j-1].isclean==false){
            flag = 0;
            cout<<"DFS left: ";
            bt_remain--;
            int next_i = robot.i;
            int next_j = robot.j-1;
            DFS(next_i,next_j);
            if(flag==0)bt_remain--;
        }
        show_map();
        if(flag==0)path_order.push_back(charge);
        flag = 0;
        first_time_return = true;
        if(isdirty())break;
        ///找剩下的點
        
    }
    cout<<"finish!\n";
    cout<<"{remain="<<bt_remain<<"}\n";
    vector<point>::iterator i;
    for(i = path_order.begin();i != path_order.end();i++){
        cout<<"("<<(*i).row<<" "<<(*i).col<<") ";
    }
    ///isdirty==false
    ///return to R
}
void show_map(){
    Sleep(300);
    cout<<"\n";
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            cout<<map[i][j].type<<" ";
        }
        cout<<"\n";
    }
}
bool isdirty(){
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            if(map[i][j].isclean==false)return true;
        }
    }
    return false;
}
void DFS(int si,int sj){
    if(si<0 || si>rows || sj<0 || sj>cols){
        cout<<"finish1 ";
        return;
    }
    point p;
    p.row = si;
    p.col = sj;
    map[si][sj].isclean = true;
    map[si][sj].type = '@';
    path_order.push_back(p);
    show_map();
    ///上右下左
    /// can_move 判斷電池是否足夠
    if(map[si-1][sj].isclean==false && flag == 0){
        cout<<"up ";
        int next_i = si-1;
        int next_j = sj;
        ///test
        bt_remain--;
        cout<<"\n{remain="<<bt_remain<<"}\n";
        if(!can_move(next_i,next_j)){
            while(!back_path_order.empty()){
                cout<<"("<<back_path_order.top().row<<" "<<back_path_order.top().col<<") ";
                real_back_path_order.push_back(back_path_order.top());
                back_path_order.pop();
            }
            cout<<"\n";
            back_to_battery();
            flag = 1;///強制結束
            //exit(1);
        }
        else while(!back_path_order.empty())back_path_order.pop();
        if(flag==1)return;
        ///
        DFS(si-1,sj);
        if(flag==1)return;
        ///回去
        if(first_time_return)first_time_return=false;
        else bt_remain--;
        cout<<"\n{remain="<<bt_remain<<"}\n";
        if(!can_move(next_i,next_j)){
            while(!back_path_order.empty()){
                cout<<"("<<back_path_order.top().row<<" "<<back_path_order.top().col<<") ";
                real_back_path_order.push_back(back_path_order.top());
                back_path_order.pop();
            }
            cout<<"\n";
            back_to_battery();
            flag = 1;///強制結束
            //exit(1);
        }
        else while(!back_path_order.empty())back_path_order.pop();
        if(flag==1)return;

        path_order.push_back(p);
    }
    if(map[si][sj+1].isclean==false && flag == 0){
        cout<<"right ";
        int next_i = si;
        int next_j = sj+1;
        bt_remain--;
        cout<<"\n{remain="<<bt_remain<<"}\n";
        if(!can_move(next_i,next_j)){
            while(!back_path_order.empty()){
                cout<<"("<<back_path_order.top().row<<" "<<back_path_order.top().col<<") ";
                real_back_path_order.push_back(back_path_order.top());
                back_path_order.pop();
            }
            cout<<"\n";
            back_to_battery();
            flag = 1;///強制結束
            //exit(1);
        }
        else while(!back_path_order.empty())back_path_order.pop();
        if(flag==1)return;

        DFS(si,sj+1);
        if(flag==1)return;

        if(first_time_return)first_time_return=false;
        else bt_remain--;
        cout<<"\n{remain="<<bt_remain<<"}\n";
        if(!can_move(next_i,next_j)){
            while(!back_path_order.empty()){
                cout<<"("<<back_path_order.top().row<<" "<<back_path_order.top().col<<") ";
                real_back_path_order.push_back(back_path_order.top());
                back_path_order.pop();
            }
            cout<<"\n";
            back_to_battery();
            flag = 1;///強制結束
            //exit(1);
        }
        else while(!back_path_order.empty())back_path_order.pop();
        if(flag==1)return;

        path_order.push_back(p);
    }
    if(map[si+1][sj].isclean==false && flag == 0){///
        cout<<"down ";
        bt_remain--;
        cout<<"\n{remain="<<bt_remain<<"}\n";
        int next_i = si+1;
        int next_j = sj;
        if(!can_move(next_i,next_j)){
            while(!back_path_order.empty()){
                cout<<"("<<back_path_order.top().row<<" "<<back_path_order.top().col<<") ";
                real_back_path_order.push_back(back_path_order.top());
                back_path_order.pop();
            }
            cout<<"\n";
            back_to_battery();
            flag = 1;///強制結束
            //exit(1);
        }
        else while(!back_path_order.empty())back_path_order.pop();
        if(flag==1)return;

        DFS(si+1,sj);
        if(flag==1)return;

        if(first_time_return)first_time_return=false;
        else bt_remain--;
        cout<<"\n{remain="<<bt_remain<<"}\n";
        if(!can_move(next_i,next_j)){
            while(!back_path_order.empty()){
                cout<<"("<<back_path_order.top().row<<" "<<back_path_order.top().col<<") ";
                real_back_path_order.push_back(back_path_order.top());
                back_path_order.pop();
            }
            cout<<"\n";
            back_to_battery();
            flag = 1;///強制結束
            //exit(1);
        }
        else while(!back_path_order.empty())back_path_order.pop();///已建好路程
        if(flag==1)return;

        path_order.push_back(p);
    }
    if(map[si][sj-1].isclean==false && flag == 0){
        cout<<"left ";
        bt_remain--;
        cout<<"\n{remain="<<bt_remain<<"}\n";
        int next_i = si;
        int next_j = sj-1;
        if(!can_move(next_i,next_j)){
            while(!back_path_order.empty()){
                cout<<"("<<back_path_order.top().row<<" "<<back_path_order.top().col<<") ";
                real_back_path_order.push_back(back_path_order.top());
                back_path_order.pop();
            }
            cout<<"\n";
            back_to_battery();
            flag = 1;///強制結束
            //exit(1);
        }
        else while(!back_path_order.empty())back_path_order.pop();
        if(flag==1)return;

        DFS(si,sj-1);
        if(flag==1)return;

        if(first_time_return)first_time_return=false;
        else bt_remain--;
        cout<<"\n{remain="<<bt_remain<<"}\n";
        if(!can_move(next_i,next_j)){
            while(!back_path_order.empty()){
                cout<<"("<<back_path_order.top().row<<" "<<back_path_order.top().col<<") ";
                real_back_path_order.push_back(back_path_order.top());
                back_path_order.pop();
            }
            cout<<"\n";
            back_to_battery();
            flag = 1;///強制結束
            //exit(1);
        }
        else while(!back_path_order.empty())back_path_order.pop();
        if(flag==1)return;

        path_order.push_back(p);
    }
    //cout<<"finish2";
    return;  
}
bool can_move(int si,int sj){
    int steps = 0;
    steps = find_back_path_by_BFS(si,sj,steps);
    cout<<"{"<<steps<<" steps to go back R}";
    ///init can visit for BFS
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            if(map[i][j].type!='1')map[i][j].canvisited = true;
        }
    }
    if(bt_remain<=steps) return false;
    else return true;
}
int find_back_path_by_BFS(int si,int sj,int steps){
    ///上右下左
    point start;
    start.row = si;
    start.col = sj;
    map[si][sj].canvisited=false;
    point **pred;
    pred = new point*[rows];
    for(int i=0;i<rows;i++){
        pred[i] = new point[cols];
    }
    cout<<"("<<si<<" "<<sj<<")";
    queue<point>q;
    q.push(start);
    while(!q.empty()){
        si = q.front().row;
        sj = q.front().col;
        q.pop();
        if(si-1>=0 && map[si-1][sj].canvisited==true){
            //cout<<"up to"<<si-1<<" "<<sj<<" ";
            map[si-1][sj].canvisited=false;
            point next_p;
            next_p.row = si-1;
            next_p.col = sj;
            pred[si-1][sj].pre_i = si;
            pred[si-1][sj].pre_j = sj;
            if(next_p.row==charge.row && next_p.col==charge.col)break;
            q.push(next_p);
        }
        if(sj+1<cols && map[si][sj+1].canvisited==true){
            //cout<<"right to"<<si<<" "<<sj+1<<" ";
            map[si][sj+1].canvisited=false;
            point next_p;
            next_p.row = si;
            next_p.col = sj+1;
            pred[si][sj+1].pre_i = si;
            pred[si][sj+1].pre_j = sj;
            if(next_p.row==charge.row && next_p.col==charge.col)break;
            q.push(next_p);
        }
        if(si+1<rows && map[si+1][sj].canvisited==true){
            //cout<<" ("<<si+1<<" "<<sj<<")";
            //cout<<"down to"<<si+1<<" "<<sj<<" ";
            map[si+1][sj].canvisited=false;
            point next_p;
            next_p.row = si+1;
            next_p.col = sj;
            pred[si+1][sj].pre_i = si;
            pred[si+1][sj].pre_j = sj;
            //cout<<"("<<si+1<<" "<<sj<<")"<<"<-"<<"("<<pred[si+1][sj].pre_i<<" "<<pred[si+1][sj].pre_j<<")";
            if(next_p.row==charge.row && next_p.col==charge.col){
                break;
            }
            q.push(next_p);
        }
        if(sj-1>=0 && map[si][sj-1].canvisited==true){
            //cout<<"right to"<<si<<" "<<sj-1<<" ";
            map[si][sj-1].canvisited=false;
            point next_p;
            next_p.row = si;
            next_p.col = sj-1;
            pred[si][sj-1].pre_i = si;
            pred[si][sj-1].pre_j = sj;
            if(next_p.row==charge.row && next_p.col==charge.col)break;
            q.push(next_p);
        }
    }
    ///count the steps of backing to charge
    point current;
    current.row = charge.row;
    current.col = charge.col;
    /*int current_i = charge.row;
    int current_j = charge.col;*/
    //for(int i=6;i>4;i--){
        //cout<<pred[4][4].pre_i<<" "<<pred[4][4].pre_j<<" ";
    //}
    while(current.row!=start.row || current.col!=start.col){
        back_path_order.push(current);
        steps++;
        int tmp_i = current.row;
        current.row = pred[current.row][current.col].pre_i;
        current.col = pred[tmp_i][current.col].pre_j;
    }
    back_path_order.push(current);
    cout<<"\n";
    return steps;
}
void back_to_battery(){
    vector<point>::iterator i;
    for(i = path_order.begin();i != path_order.end();i++){
        cout<<"("<<(*i).row<<" "<<(*i).col<<") ";
    }
    cout<<"\n=====\n";
    bool cnt=true;//頭省略
    for(i = real_back_path_order.begin();i != real_back_path_order.end();i++){
        if(!cnt)path_order.push_back(*i);
        cnt = false;
        if(map[(*i).row][(*i).col].isclean==false)map[(*i).row][(*i).col].type = '@';
        map[(*i).row][(*i).col].isclean=true;
        cout<<"(Y"<<(*i).row<<" "<<(*i).col<<") ";
    }
    cout<<"\n=====\nall_path:";
    for(i = path_order.begin();i != path_order.end();i++){
        cout<<"("<<(*i).row<<" "<<(*i).col<<") ";
    }
    bt_remain = battery;
    cout<<"charging.."<<"{remain="<<bt_remain<<"}\n";
    real_back_path_order.clear();
}
