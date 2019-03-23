#include <ctime>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>

#include <string>
#include "json/json.h"
#include <iostream>
#include <fstream>
typedef long long ll ;
#define rep(i, a, b) for (int i = a ; i <= b ; ++ i)
using namespace std ;

/* vrbs */
// colors : 0 -> gold   1-5 -> green, white, blue, black, red.
const string color_name[6]={"gold","green","white","blue","black","red"};
int change_col_to_num(string str){
    if (str[0] == 'g' && str[1] == 'o') return 0;
    if (str[0] == 'g' && str[1] == 'r') return 1;
    if (str[0] == 'w') return 2;
    if (str[0] == 'b' && str[2] == 'u') return 3;
    if (str[0] == 'b' && str[2] == 'a') return 4;
    if (str[0] == 'r') return 5;
}//0-gold;  1-black;   2-blue;    3-green;     4-red;    5-white;



/* Table */
    int Round ;
    string PlayerName ;
    struct GEM {
        int color ;
        int count ;
        GEM() {}
        GEM(int _color, int _count) {
            color = _color ;
            count = _count ;
        }
        void out(int ty = 0){
            cout<<color_name[color]<<" : "<<count;
            if (ty) cout<<endl;
        }
    } ;
    vector <GEM> Gem ;
    struct CARD {
        int level ;
        int score ;
        int color ;
        vector <GEM> cost ;
        void out(){
            cout<<"CARD : {"<<endl;
            cout<<"\tlevel : "<<level<<endl;
            cout<<"\tscore : "<<score<<endl;
            cout<<"\tcolor : "<<color_name[color]<<endl;
            cout<<"\tcost : [";

            vector<GEM>::iterator gg;
            for (gg = cost.begin(); gg != cost.end(); gg++){
                cout<<"   ";
                (*gg).out();
            }

            cout<<"\t]\n}"<<endl;
        }
    } ;
    vector <CARD> Card ;
    struct NOBLE {
        int score ;
        vector <GEM> req ;

        void out(){
            cout<<"NOBLE : {"<<endl;
            cout<<"\tscore : "<<score<<endl;
            cout<<"\treq : [";

            vector<GEM>::iterator gg;
            for (gg = req.begin(); gg != req.end(); gg++){
                cout<<"   ";
                (*gg).out();
            }

            cout<<"\t]\n}"<<endl;
        }
    } ;
    vector <NOBLE> Noble ;

/* Player */

    struct PLAYER {
        string name ;
        int score ;
        vector <GEM> gem ;
        vector <CARD> hand ; // purchased cards
        vector <CARD> res ; // reserved cards
        vector <NOBLE> nob ;
        void out(){
            cout<<"PLAYER : {"<<endl;
            cout<<"\tname = "<<name<<endl;
            cout<<"\tscore = "<<score<<endl;

            cout<<"\tGEM : [";
            vector<GEM>::iterator gg;
            for (gg = gem.begin(); gg != gem.end(); gg++){
                cout<<"   ";
                (*gg).out();
            }
            cout<<"]"<<endl;
            cout<<"purchased cards : "<<endl;
            vector<CARD>::iterator cc;
            for (cc = hand.begin(); cc != hand.end(); cc++){
                cout<<"   ";
                (*cc).out();
            }
            cout<<"reserved cards : "<<endl;
            for (cc = res.begin(); cc != res.end(); cc++){
                cout<<"   ";
                (*cc).out();
            }
            cout<<"nobles : "<<endl;
            vector<NOBLE>::iterator nn;
            for (nn = nob.begin(); nn != nob.end(); nn++){
                cout<<"   ";
                (*nn).out();
            }
            cout<<"}"<<endl;
        }
    } ;
    vector <PLAYER> Player ;

/* Game */
    int MyID ;
    int CntGem[3][15], CntCol[3][15] ;
    struct OPERATE {
        int type ;
        // 0 : GetGem_Diff
        vector <int> GetGemDiff ;
        // 1 : GetGem_Same
        int GetGemSame ;
        // 2 : Reserve_Card
        CARD ReserveCard ;
        // 3 : Reserve_Card_From_The_Top ;
        int ReserveCardFromTheTop ;
        // 4 : PurchaseCard ;
        CARD PurchaseCard ;
        // 5 : PurchaseReservedCard
        CARD PurchaseReservedCard ;
        // 6 : DesignateNoble
        NOBLE DesignateNoble ;
        int val ;
        bool friend operator < (OPERATE a, OPERATE b) {
            return a.val < b.val ;
        }
    } ;
    vector <OPERATE> ans ;
    OPERATE candidate ;

/* -------------------------------------------------------------------------- */

CARD ReadCard() {
        CARD o ;
        int  _, _x, _y, x , y ;
        scanf("%d%d",  &x, &y) ;
        o.level = x ;
        o.score = y ;
        scanf("%d", &_)  ;
        rep(j, 1, _) {
            scanf("%d%d", &_x, &_y) ;
            o.cost.push_back(GEM(_x, _y)) ;
        }
        return o ;
}

NOBLE ReadNoble() {
        NOBLE o ;
        int  _,  _x, _y,x  ;
        scanf("%d", &x) ;
        o.score = x ;
        scanf("%d",  &_) ;
        rep(j,1 , _) {
            scanf("%d%d", &_x, &_y) ;
            o.req.push_back(GEM(_x, _y)) ;
        }
        return o ;
}
//_____________by VGel begin
void Read() {
    Json::Reader reader;
    Json::Value root;

    ifstream in("pstd.json", ios::binary);
    if( !in.is_open() ){
        cout << "Error opening file\n";
        return;
    }
    if(!reader.parse(in,root)){
        cout << "parse error\n" << endl;
        in.close();
        return;
    }

    Round = root["round"].asInt();
    PlayerName = root["playerName"].asString();

    for (unsigned int i = 0, sz = root["table"]["gems"].size(); i < sz; i++){
        string str;
        int x, y;
        str = root["table"]["gems"][i]["color"].asString();
        x = change_col_to_num(str);
        y = root["table"]["gems"][i]["count"].asInt();

        Gem.push_back(GEM(x,y));
    }
    //out_begin_table_gem
    vector <GEM>::iterator gg;
    for (gg = Gem.begin(); gg != Gem.end(); gg++){
        (*gg).out(1);
    }
    //out_end_table_gem

    for (unsigned int i = 0, sz = root["table"]["cards"].size(); i < sz; i++){
        CARD C;

        C.level = root["table"]["cards"][i]["level"].asInt();
        C.score = root["table"]["cards"][i]["score"].asInt();

        string str;
        str = root["table"]["cards"][i]["color"].asString();
        C.color = change_col_to_num(str);

        for (unsigned int j = 0, szz = root["table"]["cards"][i]["costs"].size(); j < szz; j++){
            string stt;
            int x, y;
            stt = root["table"]["cards"][i]["costs"][j]["color"].asString();
            x = change_col_to_num(stt);
            y = root["table"]["cards"][i]["costs"][j]["count"].asInt();

            C.cost.push_back(GEM(x, y));
        }

        Card.push_back(C);
    }
    //out_begin_table_cards
    vector <CARD>::iterator cc;
    for (cc = Card.begin(); cc != Card.end(); cc++){
        (*cc).out();
    }
    //out_end_table_cards

    for (unsigned int i = 0, sz = root["table"]["nobles"].size(); i < sz; i++){
        NOBLE N;

        N.score = root["table"]["nobles"][i]["score"].asInt();
        for (unsigned int j = 0, szz = root["table"]["nobles"][i]["requirements"].size(); j < szz; j++){
            string stt;
            int x, y;
            stt = root["table"]["nobles"][i]["requirements"][j]["color"].asString();
            x = change_col_to_num(stt);
            y = root["table"]["nobles"][i]["requirements"][j]["count"].asInt();

            N.req.push_back(GEM(x, y));
        }

        Noble.push_back(N);
    }
    //out_begin_table_nobles
    vector <NOBLE>::iterator nn;
    for (nn = Noble.begin(); nn != Noble.end(); nn++){
        (*nn).out();
    }
    //out_end_table_nobles


    for (unsigned int i = 0, sz = root["players"].size(); i < sz; i++){
        PLAYER P;
        P.name = root["players"][i]["name"].asString();
        P.score = root["players"][i]["score"].asInt();
        for (unsigned int j = 0, szz = root["players"][i]["gems"].size(); j < szz; j++){
            string stt;
            int x, y;
            stt = root["players"][i]["gems"][j]["color"].asString();
            x = change_col_to_num(stt);
            y = root["players"][i]["gems"][j]["count"].asInt();

            P.gem.push_back(GEM(x, y));
        }

        for (unsigned int j = 0, szz = root["players"][i]["purchased_cards"].size(); j < szz; j++){
            CARD C;

            C.level = root["players"][i]["purchased_cards"][j]["level"].asInt();
            C.score = root["players"][i]["purchased_cards"][j]["score"].asInt();

            string str;
            str = root["players"][i]["purchased_cards"][j]["color"].asString();
            C.color = change_col_to_num(str);

            for (unsigned int k = 0, szk = root["players"][i]["purchased_cards"][j]["costs"].size(); k < szk; k++){
                string stt;
                int x, y;
                stt = root["players"][i]["purchased_cards"][j]["costs"][k]["color"].asString();
                x = change_col_to_num(stt);
                y = root["players"][i]["purchased_cards"][j]["costs"][k]["count"].asInt();

                C.cost.push_back(GEM(x, y));
            }

            P.hand.push_back(C);
        }

        for (unsigned int j = 0, szz = root["players"][i]["reserved_cards"].size(); j < szz; j++){
            CARD C;

            C.level = root["players"][i]["reserved_cards"][j]["level"].asInt();
            C.score = root["players"][i]["reserved_cards"][j]["score"].asInt();

            string str;
            str = root["players"][i]["reserved_cards"][j]["color"].asString();
            C.color = change_col_to_num(str);

            for (unsigned int k = 0, szk = root["players"][i]["reserved_cards"][j]["costs"].size(); k < szk; k++){
                string stt;
                int x, y;
                stt = root["players"][i]["reserved_cards"][j]["costs"][k]["color"].asString();
                x = change_col_to_num(stt);
                y = root["players"][i]["reserved_cards"][j]["costs"][k]["count"].asInt();

                C.cost.push_back(GEM(x, y));
            }

            P.res.push_back(C);
        }

        for (unsigned int j = 0, szz = root["players"][i]["nobles"].size(); j < szz; j++){
            NOBLE N;

            N.score = root["players"][i]["nobles"][j]["score"].asInt();
            for (unsigned int k = 0, szk = root["players"][i]["nobles"][j]["requirements"].size(); k < szk; k++){
                string stt;
                int x, y;
                stt = root["players"][i]["nobles"][j]["requirements"][k]["color"].asString();
                x = change_col_to_num(stt);
                y = root["players"][i]["nobles"][j]["requirements"][k]["count"].asInt();

                N.req.push_back(GEM(x, y));
            }

            P.nob.push_back(N);
        }

        Player.push_back(P);
    }
    //out_begin_players
    vector <PLAYER>::iterator pp;
    for (pp = Player.begin(); pp != Player.end(); pp++){
        (*pp).out();
    }
    //out_end_players
    in.close();
}
//_____________by VGel end
void Read_mjy() {

    scanf("%d", &Round) ;
    cin >> PlayerName ;
    int tmp, x, y, _, _x,  _y ;
    scanf("%d", &tmp) ;
    rep(i, 1, tmp) {
        scanf("%d%d", &x, &y) ;
        GEM o ;
        o.color = x ;
        o.count = y ;
        Gem.push_back(o) ;
    }
    scanf("%d", &tmp) ;
    rep(i, 1, tmp)  {
        scanf("%d%d", &x, &y) ;
        CARD o ;
        o.level = x ;
        o.score = y ;
        scanf("%d", &_)  ;
        rep(j, 1, _) {
            scanf("%d%d", &_x, &_y) ;
            o.cost.push_back(GEM(_x, _y)) ;
        }
        Card.push_back(o) ;
    }
    scanf("%d", &tmp) ;
    rep(i, 1, tmp) {
        scanf("%d", &x) ;
        NOBLE o ;
        o.score = x ;
        scanf("%d",  &_) ;
        rep(j,1 , _) {
            scanf("%d%d", &_x, &_y) ;
            o.req.push_back(GEM(_x, _y)) ;
        }
        Noble.push_back(o) ;
    }

    rep(i, 0, 2) {
        cin >> Player[i].name ;
        cin >> Player[i].score ;
        scanf("%d", &tmp) ;
        rep(j, 1, tmp) {
            scanf("%d%d", &x, &y) ;
            Player[i].gem.push_back(GEM(x, y)) ;
        }
        scanf("%d", &tmp) ;
        rep(j, 1, tmp)  Player[i].hand.push_back(ReadCard()) ;
        scanf("%d", &tmp) ;
        rep(j, 1, tmp) Player[i].res.push_back(ReadCard()) ;
        scanf("%d", &tmp) ;
        rep(j,1, tmp) Player[i].nob.push_back(ReadNoble()) ;
    }
}

void Operate() {
    // output vector <OPERATE> ans
    Json::Value root;

    OPERATE opt = ans[0];
    // 0 : GetGem_Diff
    if (opt.type == 0){
        vector <int> ::iterator it;
        for (it = opt.GetGemDiff.begin(); it != opt.GetGemDiff.end(); it++){
            root["get_different_color_gems"].append(color_name[*it]);
        }
    }
    // 1 : GetGem_Same
    else if (opt.type == 1){
        root["get_two_same_color_gems"] = Json::Value(color_name[opt.GetGemSame]);
    }
    // 2 : Reserve_Card
    else if (opt.type == 2){
        Json::Value res;
        Json::Value cd;
        cd["color"] = Json::Value(color_name[opt.ReserveCard.color]);

        vector <GEM>::iterator gg;
        for (gg = opt.ReserveCard.cost.begin(); gg != opt.ReserveCard.cost.end(); gg++){
            Json::Value gem;
            gem["color"] = Json::Value(color_name[(*gg).color]);
            gem["count"] = Json::Value((*gg).count);

            cd["costs"].append(Json::Value(gem));
        }

        cd["level"] = Json::Value(opt.ReserveCard.level);
        cd["score"] = Json::Value(opt.ReserveCard.score);


        res["card"] = Json::Value(cd);
        root["reserve_card"] = Json::Value(res);
    }
    // 3 : Reserve_Card_From_The_Top ;
    else if (opt.type == 3){
        Json::Value res;
        res["level"] = Json::Value(opt.ReserveCardFromTheTop);
        root["reserve_card"] = Json::Value(res);
    }
    // 4 : PurchaseCard ;
    else if (opt.type == 4){
        Json::Value pur;
        pur["color"] = Json::Value(color_name[opt.PurchaseCard.color]);
        vector <GEM>::iterator gg;
        for (gg = opt.PurchaseCard.cost.begin(); gg != opt.PurchaseCard.cost.end(); gg++){
            Json::Value gem;
            gem["color"] = Json::Value(color_name[(*gg).color]);
            gem["count"] = Json::Value((*gg).count);

            pur["costs"].append(Json::Value(gem));
        }
        pur["level"] = Json::Value(opt.PurchaseCard.level);
        pur["score"] = Json::Value(opt.PurchaseCard.score);
        root["purchase_card"] = Json::Value(pur);
    }
    // 5 : PurchaseReservedCard
    else if (opt.type == 5){
        opt.PurchaseReservedCard.out();

        Json::Value pur;
        pur["color"] = Json::Value(color_name[opt.PurchaseReservedCard.color]);
        vector <GEM>::iterator gg;
        for (gg = opt.PurchaseReservedCard.cost.begin(); gg != opt.PurchaseReservedCard.cost.end(); gg++){
            Json::Value gem;
            gem["color"] = Json::Value(color_name[(*gg).color]);
            gem["count"] = Json::Value((*gg).count);

            pur["costs"].append(Json::Value(gem));
        }
        pur["level"] = Json::Value(opt.PurchaseReservedCard.level);
        pur["score"] = Json::Value(opt.PurchaseReservedCard.score);
        root["purchase_reserved_card"] = Json::Value(pur);
    }

    if (ans.size() > 1){
        opt = ans[1];

        opt.DesignateNoble.out();
        Json::Value nob;
        vector <GEM>::iterator gg;
        for (gg = opt.DesignateNoble.req.begin(); gg != opt.DesignateNoble.req.end(); gg++){
            Json::Value gem;
            gem["color"] = Json::Value(color_name[(*gg).color]);
            gem["count"] = Json::Value((*gg).count);

            nob["requirements"].append(Json::Value(gem));
        }
        nob["score"] = Json::Value(opt.DesignateNoble.score);
        root["noble"] = Json::Value(nob);
    }

	Json::StyledWriter sw;
	cout << sw.write(root) << endl;
}


/* -------------------------------------------------------------------------- */
// 操作遍历及操作判定

int mygems() {
    rep(i, 0, 2) if (Player[i].name == PlayerName) {
        return Player[i].gem.size() ;
    }
    return 0 ;
}

int CANBUY(string name, CARD card) {
    // >= 0 : 需要花费额外的多少个宝石，金宝石算3个
    // < 0 : 距离买它还需要多少个宝石，金宝石算3个
    int cst = 0 ;
    rep(i, 0, 2) if (Player[i].name == name) {
        CARD res = card ;
        rep(j, 0, (int) Player[i].hand.size() - 1) {
            rep(k, 0, (int) res.cost.size() - 1) if (res.cost[k].color == Player[i].hand[j].color)
                -- res.cost[k].count ;
        }
        rep(j, 0, (int) Player[i].gem.size() - 1) {
            rep(k, 0, (int) res.cost.size() - 1) if (res.cost[k].color == Player[i].gem[j].color) {
                cst += min(res.cost[k].count, Player[i].gem[j].count) ;
                res.cost[k].count -= min(res.cost[k].count, Player[i].gem[j].count) ;
            }
        }
        int ex = 0 ;
        rep(j, 0, (int) Player[i].res.size() - 1) if (res.cost[j].count > 0) ex += res.cost[j].count ;
        if (ex <= Player[i].gem[0].count) {
            return cst + ex * 3 ;
        }
        cst = (ex - Player[i].gem[0].count) * 3 ;
        rep(j, 1, 5) cst += res.cost[j].count ;
        return - cst ;
    }
}

bool CheckNoble(string name, NOBLE noble) {
    rep(i, 0, 2) if (Player[i].name == name) {
        NOBLE res = noble ;
        rep(j, 0, (int) Player[i].hand.size() - 1) {
            rep(k, 0, (int) res.req.size() - 1) if (res.req[k].color == Player[i].hand[j].color)
                -- res.req[k].count ;
        }
        int ex = 0 ;
        rep(j, 1, 5) if (res.req[j].count > 0) ex += res.req[j].count ;
        return ex == 0 ;
    }
}

int CANNOBLE(string name, NOBLE noble) {
    rep(i, 0, 2) if (Player[i].name == name) {
        NOBLE res = noble ;
        rep(j, 0, (int) Player[i].hand.size() - 1) {
            rep(k, 0, (int) res.req.size() - 1) if (res.req[k].color == Player[i].hand[j].color)
                -- res.req[k].count ;
        }
        int ex = 0 ;
        rep(j, 1, 5) if (res.req[j].count > 0) ex += res.req[j].count ;
        return ex ;
    }
}

bool CANRESERVE() {
    rep(i, 0, 2) if (PlayerName == Player[i].name) {
        return Player[i].res.size() < 3 ;
    }
}

int CountTheDiffCard(string name, CARD card, int co) {
    rep(i, 0, 2) if (Player[i].name == name) {
        CARD res = card ;
        int sum = 0 ;
        rep(k, 0, (int) res.cost.size() - 1) if (res.cost[k].color == co) sum = res.cost[k].count ;
        rep(j, 0, (int) Player[i].hand.size() - 1) {
            if (Player[i].hand[j].color != co) continue ;
            sum -- ;
        }
        rep(j, 0, (int) Player[i].gem.size() - 1) {
            if (Player[i].gem[j].color != co) continue ;
            sum -= Player[i].gem[j].count ;
        }
        sum -= Player[i].gem[0].count ;
        return max(sum, 0) ;
    }
}

int CountTheDiffNoble(string name, NOBLE Noble, int co) {
    rep(i, 0, 2) if (Player[i].name == name) {
        NOBLE res = Noble ;
        int sum = 0 ;
        rep(k, 0, (int) res.req.size() - 1) if (res.req[k].color == co) sum = res.req[k].count ;
        rep(j, 0, (int) Player[i].hand.size() - 1) {
            if (Player[i].hand[j].color != co) continue ;
            sum -- ;
        }
        return max(sum, 0) ;
    }
}

void TryGetGemDiff(int w, vector <int> val) { // val [0~30]
    if (10 - mygems() >= 1) {
        rep(i, 0, 5) if (Gem[i].count && val[Gem[i].color]) {
            OPERATE o ;
            o.type = 0 ;
            o.val = val[Gem[i].color] ;
            o.val *= w ;
            o.GetGemDiff.push_back(i) ;
            if (candidate < o) candidate = o ;
        }
    }
    if (10 - mygems() >= 2) {
        rep(i, 0, 5) rep(j, i + 1, 5) if (Gem[i].count && Gem[j].count && val[Gem[i].color] && val[Gem[j].color]) {
            OPERATE o ;
            o.type = 0 ;
            o.val = val[Gem[i].color] + val[Gem[j].color] ;
            o.val *= w ;
            o.GetGemDiff.push_back(i) ;
            o.GetGemDiff.push_back(j) ;
            if (candidate < o) candidate = o ;

        }
    }
    if (10 - mygems() >= 3) {
        rep(i, 0, 5) rep(j, i + 1, 5) rep(k, j + 1, 5) if (Gem[i].count && Gem[j].count && val[Gem[i].color] && val[Gem[j].color] && Gem[k].count && val[Gem[k].color]) {
            OPERATE o ;
            o.type = 0 ;
            o.val = val[Gem[i].color] + val[Gem[j].color] + val[Gem[k].color] ;
            o.val *= w ;
            o.GetGemDiff.push_back(i) ;
            o.GetGemDiff.push_back(j) ;
            o.GetGemDiff.push_back(k) ;
            if (candidate < o) candidate = o ;

        }
    }
}

void TryGetGemSame(int w, vector <int> val) { // val [0~30]
    if (10 - mygems() >= 2) {
        rep(i, 0, 5) if (Gem[i].count > 1 && val[Gem[i].color]) {
            OPERATE o ;
            o.type = 1 ;
            o.val = val[Gem[i].color] * 2 ;
            o.val *= w ;
            o.GetGemDiff.push_back(i) ;
            if (candidate < o) candidate = o ;
        }
    }
}

void ReserveCard(int wscore, vector <int> wcolor) {
    if (!CANRESERVE()) return ;
    rep(i, 0, (int) Card.size() - 1) {
        OPERATE o ;
        o.type = 2 ;
        o.val = Card[i].score * wscore + wcolor[Card[i].color] ;
        o.PurchaseCard = Card[i] ;
        o.val /= (CANBUY(PlayerName, Card[i]) + 1) * (CANBUY(PlayerName, Card[i]) + 1) ;
        if (candidate < o) candidate = o ;
    }
}

void ReserveCardFromTheTop(int w) {
    OPERATE o ;
    o.type = 3 ;
    o.val = w ;
    o.ReserveCardFromTheTop = 2 ;
    if (candidate < o) candidate = o ;
}

void PurchaseCard(int wscore, vector <int> wcolor) { // wscore [100 ~ 500]   wcolor [50~100]
    rep(i, 0, (int) Card.size() - 1) {
        int tmp = CANBUY(PlayerName, Card[i]) ;
        if (tmp >= 0) {
            OPERATE o ;
            o.type = 4 ;
            o.val = Card[i].score * wscore + wcolor[Card[i].color] ;
            o.val /= tmp ;
            o.PurchaseCard = Card[i] ;
            if (candidate < o) candidate = o ;
        }
    }
}

void PurchaseReservedCard(int wscore, vector <int> wcolor) {
    rep(id, 0, 2) if (Player[id].name == PlayerName) {
        rep(i, 0, (int) Player[id].res.size() - 1) {
            CARD now = Player[id].res[i] ;
            int tmp = CANBUY(PlayerName, now) ;
            if (tmp >= 0) {
                OPERATE o ;
                o.type = 5 ;
                o.val = now.score * wscore + wcolor[now.color] ;
                o.val /= tmp ;
                o.PurchaseReservedCard = now ;
                if (candidate < o) candidate = o ;
            }
        }
    }
}

void DesignateNoble() {
    rep(i, 0, (int) Noble.size() - 1) {
        if (CheckNoble(PlayerName, Noble[i])) {
            OPERATE o ;
            o.type = 6 ;
            o.DesignateNoble = Noble[i] ;
            ans.push_back(o) ;
            return ;
        }
    }
}

/* -------------------------------------------------------------------------- */
// 发展经济时期策略


void Develop() {
    // Evaluate Gems
    vector <int> ValGem ;
    int mx = 0 ;
    rep(i, 1, 5) mx = max(mx, CntGem[MyID][i]) ;
    mx ++ ;
    int base = 1 ;
    rep(i, 1, 5) base = max(base, mx - CntGem[MyID][i]) ;
    ValGem.push_back(0) ;
    rep(i, 1, 5) {
        ValGem.push_back((int) (30.0 * (mx - CntGem[MyID][i]) / base) ) ;
    }
    TryGetGemDiff(30, ValGem) ;
    TryGetGemSame(30, ValGem) ;
    // Evaluate color
    vector <int> ValCol ;
    mx = 0 ;
    rep(i, 1, 5) mx = max(mx, CntCol[MyID][i]) ;
    mx ++ ;
    base = 1 ;
    rep(i, 1, 5) base = max(base, mx - CntCol[MyID][i]) ;
    ValCol.push_back(0) ;
    rep(i, 1, 5) {
        ValCol.push_back((int) (100.0 * (mx - CntCol[MyID][i]) / base) ) ;
    }
    ReserveCard(5, ValCol) ;
    ReserveCardFromTheTop(5) ;
    PurchaseCard(100, ValCol) ;
    PurchaseReservedCard(150, ValCol) ;
    ans.push_back(candidate) ;
    DesignateNoble() ;
}

/* -------------------------------------------------------------------------- */
// 抢分时期策略

vector <int> EvaluateColor() {
    vector <int> res ;
    rep(i, 0, 5) res.push_back(0) ;
    rep(i, 0, (int) Card.size() - 1) {
        int tmp = CANBUY(PlayerName, Card[i]) ;
        if (tmp >= 0 || tmp < - 5) continue ;
        rep(j, 1, 5) res[j] += CountTheDiffCard(PlayerName, Card[i], j) * (tmp + 6) * (tmp + 6) ;
    }
    rep(i, 0, (int) Noble.size() - 1) {
        int tmp = CANNOBLE(PlayerName, Noble[i]) ;
        if (tmp >= 0 || tmp < - 5) continue ;
        rep(j, 1, 5) res[j] += CountTheDiffNoble(PlayerName, Noble[i], j) * (tmp + 6) * (tmp + 6) * 3 ;
    }
    int mx = 1 ;
    rep(i, 1, 5) mx = max(mx, res[i]) ;
    rep(i, 1, 5) res[i] = (int) (30.0 * res[i] / mx) ;
    return res ;
}

void Aim() {// Evaluate Gems
    vector <int> ValCol = EvaluateColor() ;
    TryGetGemDiff(10, ValCol) ;
    TryGetGemSame(10, ValCol) ;
    // Evaluate color
    rep(i, 0, 5) ValCol[i] *= 2 ;
    ReserveCard(200, ValCol) ;
    ReserveCardFromTheTop(50) ;
    PurchaseCard(500, ValCol) ;
    PurchaseReservedCard(300, ValCol) ;
    ans.push_back(candidate) ;
    DesignateNoble() ;
}

/* -------------------------------------------------------------------------- */



/* -------------------------------------------------------------------------- */
// 舒适时期策略

bool CheckStatus(string name) {
    // check 这个人在当前局面下能不能赢，改一改可获得这个人在当前局面下能获得多少分
    rep(id, 0, 2) if (Player[id].name == name) {
        if (Player[id].score >= 15) return false ;
        rep(i, 0, (int) Card.size() - 1) {
            if (CANBUY(name, Card[i]) && Player[id].score + Card[i].score >= 15) return true ;
        }
        rep(i, 0, (int) Player[id].res.size() - 1) {
            if (CANBUY(name, Player[id].res[i]) && Player[id].score + Player[id].res[i].score >= 15) return true ;
        }
        rep(i, 0, (int) Noble.size() - 1) {
            if (CheckNoble(name, Noble[i]) && Player[id].score + Noble[i].score >= 15) return true ;
        }
    }
    return false ;
}

void GoAndGetIt() {
    rep(id, 0, 2) if (Player[id].name == PlayerName) {
        rep(i, 0, (int) Card.size() - 1) {
            if (CANBUY(PlayerName, Card[i]) && Player[id].score + Card[i].score >= 15) {
                OPERATE o ;
                o.type = 4 ;
                o.PurchaseCard = Card[i] ;
                ans.push_back(o) ;
                DesignateNoble() ;
                return ;
            }
        }
        rep(i, 0, (int) Player[id].res.size() - 1) {
            if (CANBUY(PlayerName, Player[id].res[i]) && Player[id].score + Player[id].res[i].score >= 15) {
                OPERATE o ;
                o.type = 5 ;
                o.PurchaseReservedCard = Player[id].res[i] ;
                ans.push_back(o) ;
                DesignateNoble() ;
                return ;
            }
        }
        rep(i, 0, (int) Noble.size() - 1) {
            if (CheckNoble(PlayerName, Noble[i]) && Player[id].score + Noble[i].score >= 15) {
                OPERATE o ;
                o.type = 6 ;
                o.DesignateNoble = Noble[i] ;
                ans.push_back(o) ;
                return ;
            }
        }
    }
}

void Pre() {
    rep(id, 0, 2) {
        if (PlayerName == Player[id].name) MyID = id ;
        rep(i, 0, 10) CntGem[id][i] = 0 ;
        rep(i, 0, (int) Player[id].gem.size() - 1) {
            CntGem[id][Player[id].gem[i].color] += Player[id].gem[i].count ;
        }
    }
    rep(id, 0, 2) {
        rep(i, 0, 10) CntCol[id][i] = 0 ;
        rep(i, 0, (int) Player[id].hand.size() - 1) {
            CntCol[id][Player[id].hand[i].color] ++ ;
        }
    }
}

void Solve() {
    Pre() ;
    candidate.val = - 1 ;
    candidate.type = - 1 ;
    if (Round <= 10) {
        if (rand() & 1) Develop() ;
            else Aim() ;
    } else {
        if (CheckStatus(PlayerName)) {
            GoAndGetIt() ;
        } else {
            /*bool flg = true ;
            rep(i, 0, 2) if (Player[i].name != PlayerName && CheckStatus(Player[i].name)) {
                Block() ;
                flg = false ;
                break ;
            }
            if (flg) {
                Aim() ;
            }*/
            Aim() ;
        }
    }
}

GEM TestRandomGem() {
    GEM o ;
    o.color = rand() % 5 + 1 ;
    o.count = rand() % 5 + 1 ;
    return o ;
}

CARD TestRandomCard() {
    CARD o ;
    o.level = rand() % 3 + 1 ;
    o.score = rand() % 3 ;
    o.color = rand() % 5 + 1 ;
    int _ = random() % 4 + 1 ;
    o.cost.clear() ;
    rep(i, 1, _) o.cost.push_back(TestRandomGem()) ;
    return o ;
}

NOBLE TestRandomNoble() {
    NOBLE o ;
    o.score = rand() % 2 + 3 ;
    int _ = rand() % 3 + 1 ;
    o.req.clear() ;
    rep(i, 1, _) o.req.push_back(TestRandomGem()) ;
    return o ;
}

void TestGetGemDiff() {
    ans.clear() ;
    OPERATE o ;
    o.type = 0 ;
    int a[6] ;
    o.GetGemDiff.clear() ;
    rep(i, 1, 5) a[i] = i ;
    random_shuffle(a + 1, a + 5 + 1) ;
    rep(i, 1, rand() % 3 + 1)
        o.GetGemDiff.push_back(a[i]) ;
    ans.push_back(o) ;
    Operate() ;
}

void TestGetGemSame() {
    ans.clear() ;
    OPERATE o ;
    o.type = 1 ;
    o.GetGemSame = rand() % 5 + 1 ;
    ans.push_back(o) ;
    Operate() ;
}

void TestReserveCard() {
    ans.clear() ;
    OPERATE o ;
    o.type = 2 ;
    o.ReserveCard = TestRandomCard() ;
    ans.push_back(o) ;
    Operate() ;
}

void TestReserveCardFromTheTop() {
    ans.clear() ;
    OPERATE o ;
    o.type = 3 ;
    o.ReserveCardFromTheTop = rand() % 3 + 1 ;
    ans.push_back(o) ;
    Operate() ;
}

void TestPurchaseCard() {
    ans.clear() ;
    OPERATE o ;
    o.type = 4 ;
    o.PurchaseCard = TestRandomCard() ;
    ans.push_back(o) ;
    Operate() ;
}

void TestPurchaseReservedCard() {
    ans.clear() ;
    OPERATE o ;
    o.type = 5 ;
    o.PurchaseReservedCard = TestRandomCard() ;
    ans.push_back(o) ;
    Operate() ;
}

void TestDesignateNoble() {
    OPERATE o ;
    o.type = 6 ;
    o.DesignateNoble = TestRandomNoble() ;
    cout<<endl<<endl<<o.DesignateNoble.req.size()<<endl<<endl;
    ans.push_back(o) ;
    Operate() ;
}

void TestOutput() {
    TestGetGemDiff() ;
    TestGetGemSame() ;
    TestReserveCard() ;
    TestReserveCardFromTheTop() ;
    TestPurchaseCard() ;
    TestPurchaseReservedCard() ;
    TestDesignateNoble() ;
}

void TestOutput1() {
    TestGetGemDiff() ;TestDesignateNoble() ;
    TestGetGemSame() ;TestDesignateNoble() ;
    TestReserveCard() ;TestDesignateNoble() ;
    TestReserveCardFromTheTop() ;TestDesignateNoble() ;
    TestPurchaseCard() ;TestDesignateNoble() ;
    TestPurchaseReservedCard() ; TestDesignateNoble() ;
}

int main(int argc, char *argv[]){
    srand(0) ;
//    Read() ;
//    Solve() ;
    TestOutput1();
    // add to the list, and push in Operate()
//    Operate() ;
    return 0 ;
}
