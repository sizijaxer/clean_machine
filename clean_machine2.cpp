#include <iostream>
#include <string>
#include "vector.hpp"
#include "stack.hpp"
#include "queue.hpp"
//#include <vector>
//#include <queue>
//#include <stack>
#include <windows.h>
using namespace std;
using namespace jt;
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
int flag = 0;///強制退出遞迴
vector<point> path_order;
stack<point> back_path_order;
vector<point>real_back_path_order;
vector<point>real_back_path_order_tmp;
stack<point> new_initial_path_order;
vector<point> real_initial_path_order;
void show_map();
bool isdirty();
bool can_move(int, int);
void DFS(int, int);
int find_back_path_by_BFS(int, int, int);
machine find_new_intial_by_BFS();
bool dirty_around(point );
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
    if(!isdirty()){
        cout<<"1\n";
        cout<<charge.row<<" "<<charge.col<<"\n";
        exit(1);
    }
    //show_map();
    while(isdirty()){
        ///找移動方法
        ///上右下左
        //cout<<"while.. ";

        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++){
                if(map[i][j].type!='1')map[i][j].canvisited = true;
            }
        }

        DFS(robot.i,robot.j);
        //cout<<"here";
        //show_map();
        flag = 0;

        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++){
                if(map[i][j].type!='1')map[i][j].canvisited = true;
            }
        }
        ///若停止點不再原點
        bool cnt = true;
        if(robot.i!=charge.row && robot.j!=charge.col){
            vector<point>::iterator i;
            for(i = real_back_path_order_tmp.begin();i != real_back_path_order_tmp.end();i++){
                if(!cnt){
                    path_order.push_back(*i);
                    bt_remain--;
                }
                cnt = false;
                if(map[(*i).row][(*i).col].isclean==false)map[(*i).row][(*i).col].type = '@';
                map[(*i).row][(*i).col].isclean=true;
            }
        }
        robot.i = charge.row;
        robot.j = charge.col;
        //cout<<"["<<bt_remain<<"]\n";
        bt_remain = battery;
        if(!isdirty())break;
        ///uisng BFS to find the new initial point
        robot = find_new_intial_by_BFS();
    }
    ///let robot go home
    bool cnt = true;
    if(robot.i!=charge.row && robot.j!=charge.col){
        vector<point>::iterator i;
        for(i = real_back_path_order_tmp.begin();i != real_back_path_order_tmp.end();i++){
            if(!cnt){
                path_order.push_back(*i);
                bt_remain--;
            }
            cnt = false;
            if(map[(*i).row][(*i).col].isclean==false)map[(*i).row][(*i).col].type = '@';
            map[(*i).row][(*i).col].isclean=true;
        }
    }
    //cout<<"finish!\n";
    //cout<<"{remain="<<bt_remain<<"}\n";
    cout<<path_order.size()-1<<"\n";
    vector<point>::iterator j;
    for(j = path_order.begin();j != path_order.end();j++){
        cout<<(*j).row<<" "<<(*j).col<<"\n";
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
void show_map(){
    //Sleep(300);
    cout<<"\n";
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            cout<<map[i][j].type<<" ";
        }
        cout<<"\n";
    }
}
void DFS(int si,int sj){
    point start;
    start.row = si;
    start.col = sj;
    path_order.push_back(start);
    if(map[si][sj].type=='0')map[si][sj].type = '@';
    map[si][sj].isclean = true;
    //show_map();
    //cout<<"robot at "<<si<<" "<<sj<<" "<<"{"<<bt_remain<<"}\n";
    //判斷能不能會去
    if(!can_move(si,sj))back_to_battery();
    if(flag==1)return;

    if(si-1>=0 && map[si-1][sj].isclean==false){//上
        bt_remain--;
        DFS(si-1,sj);
        if(flag==1)return;

        bt_remain--;
        path_order.push_back(start);
        //show_map();
        //cout<<"robot at"<<si<<" "<<sj<<" "<<"{"<<bt_remain<<"}\n";
        //判斷能不能會去
        if(!can_move(si,sj))back_to_battery();
        if(flag==1)return;
    }
    if(sj+1<cols && map[si][sj+1].isclean==false){
        bt_remain--;
        DFS(si,sj+1);
        if(flag==1)return;

        bt_remain--;
        path_order.push_back(start);
        //show_map();
        //cout<<"robot at"<<si<<" "<<sj<<" "<<"{"<<bt_remain<<"}\n";
        //判斷能不能會去
        if(!can_move(si,sj))back_to_battery();
        if(flag==1)return;
    }
    if(si+1<rows && map[si+1][sj].isclean==false){
        bt_remain--;
        DFS(si+1,sj);
        if(flag==1)return;

        bt_remain--;
        path_order.push_back(start);
        //show_map();
        //cout<<"robot at"<<si<<" "<<sj<<" "<<"{"<<bt_remain<<"}\n";
        //判斷能不能會去
        if(!can_move(si,sj))back_to_battery();
        if(flag==1)return;
    }
    if(sj-1>=0 && map[si][sj-1].isclean==false){
        bt_remain--;
        DFS(si,sj-1);
        if(flag==1)return;

        bt_remain--;
        path_order.push_back(start);
        //show_map();
        //cout<<"robot at"<<si<<" "<<sj<<" "<<"{"<<bt_remain<<"}\n";
        //判斷能不能會去
        if(!can_move(si,sj))back_to_battery();
        if(flag==1)return;
    }
    return;
}
bool can_move(int si,int sj){
    int steps = 0;
    steps = find_back_path_by_BFS(si,sj,steps);
    //cout<<"{"<<steps<<" steps to go back R}\n";
    ///init can visit for BFS
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            if(map[i][j].type!='1')map[i][j].canvisited = true;
        }
    }
    while(!back_path_order.empty()){
                //cout<<"("<<back_path_order.top().row<<" "<<back_path_order.top().col<<") ";
                real_back_path_order.push_back(back_path_order.top());
                back_path_order.pop();
    }
    //cout<<"\n";
    if(bt_remain<=steps+1) return false;
    else{
        real_back_path_order_tmp = real_back_path_order;
        real_back_path_order.clear();
        return true;
    }
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
    //cout<<"("<<si<<" "<<sj<<")";
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
    while(current.row!=start.row || current.col!=start.col){
        back_path_order.push(current);
        steps++;
        int tmp_i = current.row;
        current.row = pred[current.row][current.col].pre_i;
        current.col = pred[tmp_i][current.col].pre_j;
    }
    back_path_order.push(current);
    return steps;
}
void back_to_battery(){
    vector<point>::iterator i;
    for(i = path_order.begin();i != path_order.end();i++){
        //cout<<"("<<(*i).row<<" "<<(*i).col<<") ";
    }
    //cout<<"\n=====\n";
    bool cnt=true;//頭省略
    for(i = real_back_path_order.begin();i != real_back_path_order.end();i++){
        if(!cnt)path_order.push_back(*i);
        cnt = false;
        if(map[(*i).row][(*i).col].isclean==false)map[(*i).row][(*i).col].type = '@';
        map[(*i).row][(*i).col].isclean=true;
        //cout<<"(Y"<<(*i).row<<" "<<(*i).col<<") ";
    }
    //cout<<"\n=====\nall_path:\n";
    for(i = path_order.begin();i != path_order.end();i++){
        //cout<<"("<<(*i).row<<" "<<(*i).col<<") ";
    }
    bt_remain = battery;
    //cout<<"\ncharging.."<<"{remain="<<bt_remain<<"}\n";
    real_back_path_order.clear();
    flag = 1;
}
machine find_new_intial_by_BFS(){
    //cout<<"start to find new init pt:\n robot at "<<robot.i<<" "<<robot.j<<"\n";
    machine new_initial;
    point start;
    start.row = robot.i;
    start.col = robot.j;
    int si = start.row;
    int sj = start.col;
    point target;
    map[si][sj].canvisited = false;
    point pred[rows][cols];
    queue<point>q;
    q.push(start);
    while(!q.empty()){
        //cout<<"finding...";
        si = q.front().row;
        sj = q.front().col;
        ///上右下左
        q.pop();
        if(si-1>=0 && map[si-1][sj].canvisited==true){//cout<<"UP ";
            map[si-1][sj].canvisited=false;
            pred[si-1][sj].pre_i = si;
            pred[si-1][sj].pre_j = sj;
            point next_p;
            next_p.row = si-1;
            next_p.col = sj;
            q.push(next_p);
            //cout<<"here\n";
            if(dirty_around(next_p)){
                target = next_p;
                break;
            }
        }
        if(sj+1<cols && map[si][sj+1].canvisited==true){//cout<<"right ";
            map[si][sj+1].canvisited=false;
            pred[si][sj+1].pre_i = si;
            pred[si][sj+1].pre_j = sj;
            point next_p;
            next_p.row = si;
            next_p.col = sj+1;
            q.push(next_p);
            if(dirty_around(next_p)){
                target = next_p;
                break;
            }
        }
        if(si+1<rows && map[si+1][sj].canvisited==true){//cout<<"down ";
            map[si+1][sj].canvisited=false;
            pred[si+1][sj].pre_i = si;
            pred[si+1][sj].pre_j = sj;
            point next_p;
            next_p.row = si+1;
            next_p.col = sj;
            q.push(next_p);
            if(dirty_around(next_p)){
                target = next_p;
                break;
            }
        }
        if(sj-1>=0 && map[si][sj-1].canvisited==true){//cout<<"left ";
            map[si][sj-1].canvisited=false;
            pred[si][sj-1].pre_i = si;
            pred[si][sj-1].pre_j = sj;
            point next_p;
            next_p.row = si;
            next_p.col = sj-1;
            q.push(next_p);
            if(dirty_around(next_p)){
                target = next_p;
                break;
            }
        }
    }
    new_initial.i = target.row;
    new_initial.j = target.col;
    new_initial.type = '$';
    while(target.row!=robot.i || target.col!=robot.j){
        new_initial_path_order.push(target);
        int tmp_i = target.row;
        target.row = pred[target.row][target.col].pre_i;
        target.col = pred[tmp_i][target.col].pre_j;
    }
    ///add to path_order
    int steps=0;
    bool cnt=true;
    while(!new_initial_path_order.empty()){
        steps++;
        stack<point>det = new_initial_path_order;
        det.pop();
        if(!det.empty()){
            path_order.push_back(new_initial_path_order.top());
        }
        //cnt=false;
        new_initial_path_order.pop();
    }
    bt_remain -= steps;
    return new_initial;
}
bool dirty_around(point next_p){
    int si = next_p.row;
    int sj = next_p.col;
    //cout<<"here2";
    if(si-1>=0 && map[si-1][sj].isclean==false) return true;
    if(sj+1<cols && map[si][sj+1].isclean==false) return true;
    if(si+1<rows && map[si+1][sj].isclean==false) return true;
    if(sj-1>=0 && map[si][sj-1].isclean==false) return true;
    return false;
}
