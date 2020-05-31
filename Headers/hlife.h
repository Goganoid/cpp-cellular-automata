//
// Created by egor0 on 5/12/2020.
//
/// It is unused, but the code is working///
/// I spent a lot of time on this, so I can not delete it///
#ifndef CPP_LIFE_HLIFE_H
#define CPP_LIFE_HLIFE_H
#include <functional>
#include <unordered_map>
#include <sstream>
#include "RLE_Coder.h"
#include <map>


template<typename T,typename X>
X map_pop(std::map<T,X> * map,T val, X else_value){
    auto iter = map->find(val);
    if(iter!=map->end()){
        X ret_value = std::move(iter->second);
        map->erase(iter);
        return ret_value;
    }
    else{
        return else_value;
    }
}


template<typename A,typename B>
class LRU{
public:
//    size_t maxSize = std::pow(2,24);
    size_t maxSize = 100;
    std::unordered_map<A,B> map;
    std::list<A> dq;

    B& operator[](A index){
        return map[index];
    }

    void insert(A key, B value) {

//        if (map.find(p1) == map.end()) {
//            std::cout<<map.size()<<std::endl;
            if (map.size() >= maxSize)  {
                // delete least recently used element
                A last = dq.back();

                // Pops the last elmeent
                dq.pop_back();

                // Erase the last
                map.erase(last);
            }
            map.insert(std::make_pair(key, value));
//        }
//        else{
//            dq.remove(p1);
//        }
        dq.push_front(key);

    }
    bool contains(A p){
        return (map.find(p) != map.end());
    }
    bool contains_else_insert(A key,B value){
        if(map.find(p) != map.end()){
            return true;
        }
        else{
            insert(key,value);
            return false;
        }
    }
};


class Node{
public:
    unsigned int k;
    Node  *a= nullptr;
    Node  *b= nullptr;
    Node  *c= nullptr;
    Node  *d= nullptr;
    bool isNone = true;
    size_t hash;
    unsigned int n;
    Node(){};
    Node(unsigned int k,unsigned int n, size_t hash){
        this->k = k;
        this->n = n;
        this->hash = hash;
    }
    Node(unsigned int k,unsigned int n, size_t hash,
                Node a,
                Node b,
                Node c,
                Node d)
    {
     this->k = k;
     this->n = n;
     this->hash = hash;
     this->a = new Node(a);
     this->b = new Node(b);
     this->c = new Node(c);
     this->d = new Node(d);
     isNone = false;
    }
    std::string info(){
        std::stringstream ss;
        ss<<" Level: "<<k<<", n: "<<n<<", Hash:" << hash;
        return ss.str();
    }

};
#define on  Node(0,1,1)
#define off  Node(0,0,0)


class Tree{
public:
    Node tree;
    LRU<size_t,Node> cache;
    LRU<size_t,Node> gen_cache;
    Tree(){}

    Node join(Node a, Node b, Node c, Node d){
        unsigned int n = a.n+b.n+c.n+d.n;
        size_t nhash =(
                                 a.k + 2 +
                                 + 5131830419411 * a.hash + 3758991985019 * b.hash
                                 + 8973110871315 * c.hash + 4318490180473 * d.hash
                                 + 3422133589621 * a.n    + 3184081875910 * b.n
                                 + 2038932598190 + c.n    + 3825980280180 * d.n
                         );
        if(cache.contains(nhash)){
            return cache.map[nhash];
        }
        else{
            Node result = Node(a.k + 1,n,nhash, a, b, c, d);
            cache.insert(nhash,result);
            return result;
        }
    }

    Node get_zero(int k){
        if(k==0){
            return off;
        }
        else{
            return join(get_zero(k - 1), get_zero(k - 1),
                         get_zero(k - 1), get_zero(k - 1));
        }
    }
    Node centre(Node m){
        Node z = get_zero(m.k - 1);
        Node a = join(z, z, z, *(m.a));
        Node b = join(z, z, *(m.b), z);
        Node c = join(z, *(m.c), z, z);
        Node d = join(*(m.d), z, z, z);
        return join(a,b,c,d);
    }
    Node life(Node* a, Node* b, Node* c, Node* d,Node* E, Node* f,Node* g, Node* h, Node* i){
        int outer = 0;
        Node* nodes[8]{a,b,c,d,f,g,h,i};
        for(auto node: nodes ){
            outer+=node->n;
        }

        if((outer==2 && E->n)||outer==3){
            return on;
        }
        else{
            return off;
        }
    }
    Node life4x4(Node& m){
        Node ad = life(m.a->a, m.a->b, m.b->a, m.a->c, m.a->d, m.b->c, m.c->a, m.c->b, m.d->a);
        Node bc = life(m.a->b, m.b->a, m.b->b, m.a->d, m.b->c, m.b->d, m.c->b, m.d->a, m.d->b);
        Node cb = life(m.a->c, m.a->d, m.b->c, m.c->a, m.c->b, m.d->a, m.c->c, m.c->d, m.d->c);
        Node da = life(m.a->d, m.b->c, m.b->d, m.c->b, m.d->a, m.d->b, m.c->d, m.d->c, m.d->d);
        return join(ad,bc,cb,da);
    }
    Node next_gen(Node  m){
        if (m.n==0){
            std::cout<<"Zero exit"<<std::endl;
            return *(m.a);
        }
        else if(m.k == 2){
            std::cout<<"4x4 exit"<<std::endl;
            return life4x4(m);
        }
        else {

            Node c1 = next_gen(join(*(m.a->a), *(m.a->b), *(m.a->c), *(m.a->d)));
            Node c2 = next_gen(join(*(m.a->b), *(m.b->a), *(m.a->d), *(m.b->c)));
            Node c3 = next_gen(join(*(m.b->a), *(m.b->b), *(m.b->c), *(m.b->d)));
            Node c4 = next_gen(join(*(m.a->c), *(m.a->d), *(m.c->a), *(m.c->b)));
            Node c5 = next_gen(join(*(m.a->d), *(m.b->c), *(m.c->b), *(m.d->a)));
            Node c6 = next_gen(join(*(m.b->c), *(m.b->d), *(m.d->a), *(m.d->b)));
            Node c7 = next_gen(join(*(m.c->a), *(m.c->b), *(m.c->c), *(m.c->d)));
            Node c8 = next_gen(join(*(m.c->b), *(m.d->a), *(m.c->d), *(m.d->c)));
            Node c9 = next_gen(join(*(m.d->a), *(m.d->b), *(m.d->c), *(m.d->d)));

            return join(
                    (join(*(c1.d), *(c2.c), *(c4.b), *(c5.a))),
                    (join(*(c2.d), *(c3.c), *(c5.b), *(c6.a))),
                    (join(*(c4.d), *(c5.c), *(c7.b), *(c8.a))),
                    (join(*(c5.d), *(c6.c), *(c8.b), *(c9.a)))
            );
        }
    }
    Node successor(Node  m, int j=-999){
        if (m.n==0){
            return *(m.a);
        }
        else if(m.k == 2){
            return life4x4(m);
        }
        else {
            if(j==-999){
                j = m.k - 2;
            }
            else{
                j = std::min((int)m.k-2,j);
            }

            Node c1 = successor(join(*(m.a->a), *(m.a->b), *(m.a->c), *(m.a->d)),j);
            Node c2 = successor(join(*(m.a->b), *(m.b->a), *(m.a->d), *(m.b->c)),j);
            Node c3 = successor(join(*(m.b->a), *(m.b->b), *(m.b->c), *(m.b->d)),j);
            Node c4 = successor(join(*(m.a->c), *(m.a->d), *(m.c->a), *(m.c->b)),j);
            Node c5 = successor(join(*(m.a->d), *(m.b->c), *(m.c->b), *(m.d->a)),j);
            Node c6 = successor(join(*(m.b->c), *(m.b->d), *(m.d->a), *(m.d->b)),j);
            Node c7 = successor(join(*(m.c->a), *(m.c->b), *(m.c->c), *(m.c->d)),j);
            Node c8 = successor(join(*(m.c->b), *(m.d->a), *(m.c->d), *(m.d->c)),j);
            Node c9 = successor(join(*(m.d->a), *(m.d->b), *(m.d->c), *(m.d->d)),j);

            if(j<m.k-2){
                return join(
                        (join(*(c1.d), *(c2.c), *(c4.b), *(c5.a))),
                        (join(*(c2.d), *(c3.c), *(c5.b), *(c6.a))),
                        (join(*(c4.d), *(c5.c), *(c7.b), *(c8.a))),
                        (join(*(c5.d), *(c6.c), *(c8.b), *(c9.a)))
                );
            }
            else{
            return join(
                    successor(join(c1, c2, c4, c5),j),
                    successor(join(c2, c3, c5, c6),j),
                    successor(join(c4, c5, c7, c8),j),
                    successor(join(c5, c6, c8, c9),j)
            );
            }

        }
    }
    std::vector<Point> expand (Node  node, int x=0, int y=0, int level=0){
        if(node.n==0){
            return std::vector<Point>();
        }
        if(node.k==level){
            Point t{x>>level, y>>level};
            return std::vector<Point>{t};
        }
        else{
            int size = (int) std::pow(2,node.k);
            int offset = size  >> 1;

            auto expandA = expand(*(node.a), x, y, level);

            auto expandB = expand(*(node.b), x+offset, y, level);
            auto expandC = expand(*(node.c), x, y+offset, level);
            auto expandD = expand(*(node.d), x+offset, y+offset, level);


            std::vector<Point> temp;
            temp.insert(temp.begin(),expandA.begin(),expandA.end());
            temp.insert(temp.begin(),expandB.begin(),expandB.end());
            temp.insert(temp.begin(),expandC.begin(),expandC.end());
            temp.insert(temp.begin(),expandD.begin(),expandD.end());
            return temp;
        }
    }
    Node construct(std::vector<Point> alive_cells){
        std::map<Point,Node> pts;
        for(Point point: alive_cells){
            pts[point] = on;
        }
        // TODO add min
        int k=0;
        while(pts.size()!=1){
            std::map<Point,Node> next_level;
            Node z = get_zero(k);
            while(pts.size()>0){
                Point coord = pts.begin()->first;
                coord.x = coord.x - (coord.x & 1);
                coord.y = coord.y - (coord.y & 1);

                Node a = map_pop(&pts,coord,z);
                if(a.hash==4318490180475){
                    std::cout<<"df";
                }
                Node b = map_pop(&pts,{coord.x+1,coord.y},z);
                if(b.hash==4318490180475){
                    std::cout<<"df";
                }
                Node c = map_pop(&pts,{coord.x,coord.y+1},z);
                if(c.hash==4318490180475){
                    std::cout<<"df";
                }
                Node d = map_pop(&pts,{coord.x+1,coord.y+1},z);
                if(d.hash==4318490180475){
                    std::cout<<"df";
                }
                Node test = join(a,b,c,d);
                next_level[{coord.x>>1,coord.y>>1}] = test;

            }
            pts = next_level;
            k+=1;

        }
        return pts.begin()->second;
//        tree = pts.begin()->second;

    }
    Node advance(Node node,unsigned int n){
        if(n==0){
            return node;
        }
        std::vector<int> bits;
        while(n>0){
            bits.push_back(n&1);
            n = n>>1;
            node = centre(node);
        }
        int size = bits.size();
        for(int i=0;i<size;i++){
            int j = size - i - 1;
            if(bits[j]){
                node = successor(node,j);
            }
        }
        return node;
    }

};

#endif //CPP_LIFE_HLIFE_H
