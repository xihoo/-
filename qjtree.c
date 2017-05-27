#include <stdio.h>
#include <stdlib.h>

#define BLACK 0
#define RED 1
double Max(double a,double b,double c){
    if(a > b)
        return a > c ? a : c;
    else
        return b > c ? b : c;
}
typedef struct interval{
    double low;
    double high;
}interval;
typedef struct lesson{
    int number;
    char *name;
    interval inter;
}lesson;
typedef struct node{
    struct node *left;
    struct node *right;
    struct node *p;
    int color;
    double key;
    interval inter;
    double max;
    lesson *less;

}node;
static node *nil={NULL};
static node *root;
int overlap(interval a,interval b){
    if(a.high<b.low){
        return 0;
    }
    if(a.low>b.high){
        return 0;
    }
    return 1;
}

void left_rotate(node *x){
    //printf("hello");
    node *y=x->right;
    x->right=y->left;
    if(y->left!=nil){
        y->left->p=x;
    }
    y->p=x->p;
    if(x->p==nil){
        root=y;
    }
    else if(x==x->p->left)
        x->p->left=y;
    else
        x->p->right=y;   
    y->left=x;
    x->p=y;
    y->max=x->max;
    //printf("hello");
    //x->max=Max(x->max, x->left->max, x->right->max);
    if(x->left==nil){
        if(x->right==nil){
            x->max = Max(x->inter.high, -1, -1);
        }
        else
            x->max=Max(x->inter.high, -1, x->right->max);
    }
    else if(x->right==nil){
        if(x->left==nil){
            x->max = Max(x->inter.high, -1, -1);
        }
        else
            x->max = Max(x->inter.high, x->left->max, -1);
    }
           
    else 
        x->max = Max(x->inter.high, x->left->max, x->right->max);

    //printf("hello");
}
void right_rotate(node *x){
    node *y = x->left;
    x->left = y->right;
    if(y->right != nil)
        y->right->p = x;
    y->p = x->p;
    if(x->p == nil)
        root = y;
    else if(x == x->p->right)
        x->p->right = y;
    else 
        x->p->left = y;
    y->right = x;
    x->p = y;
    y->max = x->max;
    if(x->left==nil){
        if(x->right==nil){
            x->max = Max(x->inter.high, -1, -1);
        }
        else
            x->max=Max(x->inter.high, -1, x->right->max);
    }
    else if(x->right==nil){
        if(x->left==nil){
            x->max = Max(x->inter.high, -1, -1);
        }
        else
            x->max = Max(x->inter.high, x->left->max, -1);
    }
           
    else 
        x->max = Max(x->inter.high, x->left->max, x->right->max);

}
void insertfixup(node *z){
    node *y;
    if(z->p==nil){
        root->color = BLACK;
        return ;
                
    }
    while(z->p->color==RED){
        if(z->p == z->p->p->left)
        {   
            y = z->p->p->right;
            //printf("hello");
            if(y!=nil)
            {   //printf("hello");
                if(y->color==RED){
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
                if(z->p==nil)
                    break;}
                //printf("hello");}
             
                else if(z == z->p->right)
                {   //printf("hello");
                    z = z->p;
                    left_rotate(z);
                    //printf("hello");
                }
                else{
                z->p->color = BLACK;
                printf("hello");
                z->p->p->color = RED;
                right_rotate(z->p->p);}}
            else {
                if(z == z->p->right)
                {   //printf("hello");
                    z = z->p;
                    left_rotate(z);
                    //printf("hello");
                }
                else{
                z->p->color = BLACK;
                //printf("hello");
                z->p->p->color = RED;
                right_rotate(z->p->p);}
            }
            
        }
        else if(z->p == z->p->p->right)
        {
            y = z->p->p->left;
            if(y!=nil)
            {
                if(y->color == RED)
                {
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
                if(z->p==nil)
                    break;
                }
                else if(z == z->p->left)
                {
                    z = z->p;
                    right_rotate(z);
                }
                else{
                z->p->color = BLACK;
                z->p->p->color = RED;
                left_rotate(z->p->p);}}
            else{
                if(z == z->p->left)
                {
                    z = z->p;
                    right_rotate(z);
                }
                else{
                z->p->color = BLACK;
                z->p->p->color = RED;
                left_rotate(z->p->p);}

            }
            
        }
    }
    root->color = BLACK;
}
void insert(node *z){
    node *y = nil, *x = root;
    //找到应该插入的位置，与二叉查找树的插入相同
    while(x != nil)
    {
        x->max = Max(x->max, z->max,-1);//这句语句用于对节点附加信息的维护
        y = x;
        if(z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    z->p = y;
    if(y == nil){
        root = z;
        //printf("hello");
    }
    else if(z->key < y->key)
        y->left = z;
    else
        y->right = z;
    z->left = nil;
    z->right = nil;
    //将新插入的结点转为红色
    z->color = RED;
    //printf("hello");
    //从新插入的结点开始，向上调整
    insertfixup(z);
}
node *Min(node *x){
    while(x->left != nil)  
        x = x->left;  
    return x; 
}
node *successor(node *x){
    if(x->right != nil)     
        return Min(x->right);     
    node *y = x->p;  
    while(y != nil && x == y->right)  
    {  
        x = y;  
        y = y->p;  
    }  
    return y; 
}
void deletefixup(node *x){
    node *w;
    while(x != root && x->color == BLACK)
    {
        if(x == x->p->left)
        {
            w = x->p->right;
            if(w->color == RED)
            {
                w->color = BLACK;
                x->p->color = RED;
                left_rotate(x->p);
                w = x->p->right;
            }
            if(w->left->color == BLACK && w->right->color == BLACK)
            {
                w->color = RED;
                x = x->p;
            }
            else
            {
                if(w->right->color == BLACK)
                {
                    w->left->color = BLACK;
                    w->color = RED;
                    right_rotate(w);
                    w = x->p->right;
                }
                w->color =x->p->color;
                x->p->color = BLACK;
                w->right->color = BLACK;
                left_rotate(x->p);
                x = root;
            }
        }
        else if(x == x->p->right)
        {
           
            w = x->p->left;
            if(w->color == RED)
            {
                
                w->color = BLACK;
                x->p->color = RED;       
                right_rotate(x->p);
                w = x->p->left;
            }         
            if(w->right->color == BLACK && w->left->color == BLACK)
            {
        
                w->color = RED;              
                x = x->p;
            }
            else
            {
                if(w->left->color == BLACK)
                {
                    w->right->color = BLACK;
                    w->color = RED;
                    left_rotate(w);
                    w = x->p->left;
                }
              
                w->color =x->p->color;
                x->p->color = BLACK;
                w->left->color = BLACK;
                right_rotate(x->p);
                x = root;
            }
        }
    }
    x->color = BLACK;
}
node *delete(node *z){
    node *x, *y, *p;
    if(z->left == nil || z->right == nil)
    {   //printf("hello");
        y = z;
        p = y->p;
        if(y->left!=nil)
        {
            p->max=Max(p->inter.high,y->left->max,-1);
        }
        else
        {
            if(y->right==nil)
            p->max=Max(p->inter.high,-1,-1);
            else
            p->max=Max(p->inter.high,y->right->max,-1);
        }
        p = p->p;
        if(p!=nil){
        while(p->max == y->max)
        {
            p->max = Max(p->inter.high, p->left->max, p->right->max);
            p = p->p;
        }}
    }
    else {y = successor(z);}
         //printf("%lf",y->key);}
    p = y->p;
    if(y->left!=nil)
    {
        p->max=Max(p->inter.high,y->left->max,-1);
    }
    else
    {
        if(y->right==nil)
        p->max=Max(p->inter.high,-1,-1);
        else
        p->max=Max(p->inter.high,y->right->max,-1);
    }
    p = p->p;
    if(p!=nil){
    while(p->max == y->max)
    {
        if(p->left==nil){
        if(p->right==nil){
            p->max = Max(p->inter.high, -1, -1);
        }
        else
            p->max=Max(p->inter.high, -1, p->right->max);
    }
    else if(p->right==nil){
        if(p->left==nil){
            p->max = Max(p->inter.high, -1, -1);
        }
        else
            p->max = Max(p->inter.high, p->left->max, -1);
    }
           
    else 
        p->max = Max(p->inter.high, p->left->max, p->right->max);
    p = p->p;
    }}
    //printf("hello");
    if(y->left != nil)
        x = y->left;
    else {
    if(y->right!=nil){
    x = y->right;
    x->p = y->p;}
    else
    x=nil;
    }
    //printf("helloo");
    if(y->p == nil)
        root = x;
    else if(y == y->p->left)
        y->p->left = x;
    else
        y->p->right = x;
    //printf("helloo");
    if(y != z)
    {
        z->inter = y->inter;
        z->key = y->key;
        z->max = y->max;
        z->less=y->less;
        p = z->p;
        if(p!=nil){
        while(p->max < z->max)
        {
            p->max = z->max;
            p = p->p;
            if(p==nil)
                break;
        }}
    }
    //printf("helloo");
    if(y->color == BLACK&&x!=nil){
        //printf("\nhhhhh\n");
        deletefixup(x);}
    return y;
}
void search(node *root,interval i){
    node *x=root;
    node *y;
    if(overlap(x->inter,i)){
        printf("%d\n",x->less->number);
    }
    if(x->left != nil && x->left->max >= i.low) { 
        search(x->left, i);    
        }
    if(x->right != nil && x->key <= i.high)  
        search(x->right, i); 
}
void print(node *z){
    if(z==nil)
        return;
    print(z->left);
    printf("\ncourse number:%d",z->less->number);
    printf("   course name:%s",z->less->name);
    if(z->color==1)
    printf("   RED   ");
    else
    printf("    BLACK  ");
    printf("time:[%lf , %lf]",z->inter.low,z->inter.high);
    printf("   max:%lf\n",z->max);
    print(z->right);
}



int main(){
    //nil->color=BLACK;
    root=nil;
    node a;
    interval in={25,30};
    lesson les={20,"math",in};
    a.inter=in;
    a.less=&les;
    a.key=25;
    a.max=30;
    //printf("%lf",a.inter.low);
    insert(&a);
    //printf("%lf",root->key);
    node b;
    interval in1={8,9};
    lesson les1={21,"english",in1};
    b.inter=in1;
    b.less=&les1;
    b.key=8;
    b.max=9;
    insert(&b);
    //printf("%lf",root->key);
    node c;
    interval in2={16,21};
    lesson les2={22,"biology",in2};
    c.inter=in2;
    c.less=&les2;
    c.key=16;
    c.max=21;
    insert(&c);
    //printf("%lf",root->key);
    node d;
    interval in3={5,8};
    lesson les3={23,"chemistry",in3};
    d.inter=in3;
    d.less=&les3;
    d.key=5;
    d.max=8;
    insert(&d);
    //printf("\n%d",root->left->left->color);
    node e;
    interval in4={26,27};
    lesson les4={24,"chinese",in4};
    e.inter=in4;
    e.less=&les4;
    e.key=26;
    e.max=27;
    insert(&e);
    //printf("\n%d",root->right->color);
    node f;
    interval in5={17,19};
    lesson les5={25,"history",in5};
    f.inter=in5;
    f.less=&les5;
    f.key=17;
    f.max=19;
    insert(&f);
    //printf("%lf",root->right->left->key);
    node g;
    interval in6={19,20};
    lesson les6={26,"physics",in6};
    g.inter=in6;
    g.less=&les6;
    g.key=19;
    g.max=20;
    insert(&g);
    //printf("%lf",root->right->left->right->key);
    node i;
    interval in8={15,23};
    lesson les8={28,"geography",in8};
    i.inter=in8;
    i.less=&les8;
    i.key=15;
    i.max=23;
    insert(&i);
    //printf("%lf",root->left->key);
    node h;
    interval in7={6,10};
    lesson les7={27,"astronomy",in7};
    h.inter=in7;
    h.less=&les7;
    h.key=6;
    h.max=10;
    insert(&h);
    //printf("%lf",root->left->left->right->key);
    node j;
    interval in9={1,3};
    lesson les9={29,"computer",in9};
    j.inter=in9;
    j.less=&les9;
    j.key=1;
    j.max=3;
    insert(&j);
    //printf("\n%d",root->left->left->left->color);
    print(root);
    interval sear={7,11};
    search(root,sear);
    printf("-----------------------------------------------------------------------------");
    delete(&b);
    print(root);
    search(root,sear);
    




    
    //delete(&b);
    //if(root->left->left==nil)
   // printf("hello!");
    //else
    //printf("%lf",root->left->key);
    //printf("%d",root->color);

    
    
    
    
    
    
    
    
    
    
    /*int a;
    struct interval b={1.0,2.0};
    struct interval c={3.0,5.0};
    a=overlap(b,c);
    printf("%d",a);*/
   
    
    /*double a=1.0,b=3.0,c=20.0;
    double d=max(a,b,c);
    printf("%lf",d);*/
}