#include<iostream>
#include<stdlib.h>
#include <fstream>
#include <string.h>
#include <sstream>
using namespace std;
//Declaring the node structure of the Red-Black Tree with 'r' for Red and 'b' for Black
typedef struct tree{
	struct tree* left;
	struct tree* right;
	struct tree* parent;
	int ID;
	int count;
	char color;
}node;
//Doing Left Rotation in search tree that preserves in-order traversal key ordering
void left_rotate(node* &root , node* x , node* &hook){
//Rotating along the right of the given Node x
	node* y=x->right;
	x->right = y->left;

	if(y->left!=hook)
		y->left->parent = x;
	y->parent=x->parent;

	if(x->parent==hook)
	   root = y;
	else if(x==x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;

	y->left = x;
	x->parent = y;
}
//Doing Right Rotation in search tree that preserves in-order traversal key ordering
void right_rotate(node* &root , node* y , node* &hook){
    //Rotating along the left of left of the given node x
	node* x = y->left;
	y->left = x->right;

	if(x->right!=hook)
	   x->right->parent = y;
	x->parent = y->parent;

	if(y->parent==hook)
		root = x;
	else if(y==y->parent->left)
		y->parent->left = x;
	else
	    y->parent->right = x;

	x->right = y;
	y->parent = x;
}
//The parent and “uncle” of z are both red:
//• Color the parent and uncle of z black;
//• Color the grandparent of z red;
//• Repeat on the grandparent of z.
void insert_fixup(node* &root , node* z , node* &hook){
	while(z->parent->color=='r'){

		if(z->parent==z->parent->parent->left){
			node* y=z->parent->parent->right;
			if(y->color=='r'){
				z->parent->color='b';
				y->color='b';
				z->parent->parent->color='r';
				z=z->parent->parent;
			}
			else if(z==z->parent->right){
				z=z->parent;
				left_rotate(root,z,hook);

			    z->parent->color='b';
			    z->parent->parent->color='r';
			    right_rotate(root,z->parent->parent,hook);
	        }
	       else{
	       	    z->parent->color='b';
			    z->parent->parent->color='r';
			    right_rotate(root,z->parent->parent,hook);
		     }
		}
		else{                                   //i.e. (z->p == z->p->p->right) Condition checking is done here
			node* y=z->parent->parent->left;
			if(y->color=='r'){
				z->parent->color='b';
				y->color='b';
				z->parent->parent->color='r';
				z=z->parent->parent;
			 }
			else if(z==z->parent->left){
				z=z->parent;
				right_rotate(root,z,hook);

		    	z->parent->color='b';
			    z->parent->parent->color='r';
			    left_rotate(root,z->parent->parent,hook);
	         }
		   else{
		   	   	z->parent->color='b';
			    z->parent->parent->color='r';
			    left_rotate(root,z->parent->parent,hook);
		     }
		}
	}
	root->color='b';
}
//Note that we start by inserting the new node, z, in the tree just as we would for any other binary tree,
//using the tree_insert function. This new node is labeled red, and possibly destroys the red-black property.
//The main loop moves up the tree, restoring the red-black property.
void insert(node* &root , node* &z , node* &hook){
	node* y=hook;
	node* x=root;
	while(x!=hook){
		y=x;
		if(z->ID < x->ID)
		   x=x->left;
		else
		   x=x->right;
	}
	z->parent=y;

	if(y==hook)
	  root=z;
	else if(z->ID < y->ID)
		y->left=z;
	else
		y->right=z;

	z->left=hook;
	z->right=hook;
	z->color='r';
	insert_fixup(root,z,hook);
}
//Moving the Node v to the left or right of the parents based on the value of the ancestor u
void transplant(node* &root , node* &u , node* &v , node* hook){
	if(u->parent==hook)
	   root=v;
	else if(u==u->parent->left)
	   u->parent->left=v;
	else
	   u->parent->right=v;
	v->parent=u->parent;
}
// Changes the colors and performs rotations to restore the red-black properties.
void del_fixup(node* &root , node* x , node* &hook){
    //Keep iterating until we find that all the leaves are balanced in the number of the black nodes
	while(x!=root && x->color=='b'){
		if(x==x->parent->left){
			node* w=x->parent->right;

			if(w->color=='r'){
				w->color='b';
				x->parent->color='r';
				left_rotate(root,x->parent,hook);
				w=x->parent->right;
			}
			if(w->left->color=='b' && w->right->color=='b'){
				w->color='r';
				x=x->parent;
			}
			else if(w->right->color=='b'){
				w->left->color='b';
				w->color='r';
				right_rotate(root,w,hook);
				w=x->parent->right;

			    w->color=x->parent->color;
			    x->parent->color='b';
			    w->right->color='b';
			    left_rotate(root,x->parent,hook);
			    x=root;
			}
			else{
				w->color=x->parent->color;
			    x->parent->color='b';
			    w->right->color='b';
			    left_rotate(root,x->parent,hook);
			    x=root;
			}
		}

		else{
			node* w=x->parent->left;

			if(w->color=='r'){
				w->color='b';
				x->parent->color='r';
				right_rotate(root,x->parent,hook);
				w=x->parent->left;
			}
			if(w->right->color=='b' && w->left->color=='b'){
				w->color='r';
				x=x->parent;
			}
			else if(w->left->color=='b'){
				w->right->color='b';
				w->color='r';
				left_rotate(root,w,hook);
				w=x->parent->left;

				w->color=x->parent->color;
				x->parent->color='b';
				w->left->color='b';
				right_rotate(root,x->parent,hook);
				x=root;
			}
			else{
				w->color=x->parent->color;
				x->parent->color='b';
				w->left->color='b';
				right_rotate(root,x->parent,hook);
				x=root;
			}
		}
	}
	x->color='b';
}
//Minimum value for a node by traversing to it's leftmost node
node* minValue(node* &root , node* hook){
   node* temp=root;
   	if(temp->left==hook)
   		return temp;
	else
		return minValue(temp->left,hook);
}
//Deletes a given node and compensates for the tree coloring imbalance among the Nodes
void del(node* &root , node* &z , node* &hook){
	node* y=z;
	node* x;
	char y_original_color=y->color;

	if(z->left==hook){
		x=z->right;
		transplant(root,z,z->right,hook);
	}
	else if(z->right==hook){
		x=z->left;
		transplant(root,z,z->left,hook);
	}
	else{
		y=minValue(z->right,hook);
		y_original_color=y->color;
		x=y->right;

		if(y->parent==z){
			x->parent=y;
		}
		else{
			transplant(root,y,y->right,hook);
			y->right=z->right;
			y->right->parent=y;
		}

		transplant(root,z,y,hook);
		y->left=z->left;
		y->left->parent=y;
		y->color=z->color;
	}

	if(y_original_color=='b')
	    del_fixup(root,x,hook);
}
//Doing an Inorder Traversal in the Tree to look for the Node
node* search(node* &root , int x , node* hook){
	node* temp=root;
	while(temp!=hook){
		if(temp->ID == x)
			return temp;
		else if(x < temp->ID)
			temp=temp->left;
		else if(x > temp->ID)
			temp=temp->right;
	}
    node *k=new node;
    k->ID=0;
    k->count=0;
    return k;
}
//Doing an Inorder Traversal of the Tree and returning the corresponding value of the count field
node* count_count(node* &root ,node* hook,int x){
	node* temp=root;
	while(temp!=hook){
		if(temp->ID == x)
			return temp;
		else if(x < temp->ID)
			temp=temp->left;
		else if(x > temp->ID)
			temp=temp->right;
	}
    node *k=new node;
    k->count=0;
    return k;
}
int count_range(node* &root,node* hook,int low,int high){
       // Base case
    if (root==hook) return 0;

    // Special Optional case for improving efficiency
    if (root->ID == high && root->ID == low)
        return root->count;

    // If current node is in range, then include it in count and
    // recur for left and right children of it
    if (root->ID <= high && root->ID >= low)
         return root->count + count_range(root->left,hook, low, high) +
                    count_range(root->right, hook,low, high);

    // If current node is smaller than low, then recur for right
    // child
    else if (root->ID < low)
         return count_range(root->right,hook, low, high);

    // Else recur for left child
    else return count_range(root->left,hook, low, high);
}
//Looking for the node to make the change and then modifying the Count field if found
//with 'c' determining which operation to be applied
node* modify_count(node* &root , int x , node* hook,int fact,char c){
	node* temp=root;
	int del_flag=0;
	while(temp!=hook){
		if(temp->ID == x){
            if(c=='s'){
                temp->count=temp->count-fact;
                if(temp->count<=0){
                    del_flag=1;
                    del(root,temp,hook);
                    break;
                }
            }
            else{
                temp->count=temp->count+fact;
            }
			return temp;
		}
		else if(x < temp->ID){
            temp=temp->left;
		}
		else if(x > temp->ID){
			temp=temp->right;
        }
	}if(del_flag!=1 && c!='s'){
        node* z=new node;
        z->left=hook;
        z->right=hook;
        z->parent=hook;
        z->color='r';
        z->ID=x;
        z->count=fact;
        insert(root,z,hook);
        return z;
	}
	else{
        if(temp->count<=0 || del_flag==1){
            node *x=new node;
            x->ID=0;
            x->count=0;
            return x;
        }
        return temp;
	}
}
//Successor is determined either by the smallest value in the rightmost Node or the common ancestor which is on the right side
node *successor(node* y, node*hook){
    node * a = hook;
    node *z=y->right;
    if(y->right != hook){
        return minValue(z,hook);
    }
    a = y->parent;
    while(a != hook && y == a->right){
        y = a;
        a = a->parent;
    }
    return a;
}
//Searches for the given Node recursively and then calls the successor for that node
node *next(node *root,int id,node*hook){
    node* a=root;
    node* b=hook;
		while(a!=hook){
			b=a;
			if(a->ID==id){
				return successor(a,hook);
			}
			else{
				if(id < a->ID){
					a=a->left;
				}
				else{
					a=a->right;
				}
			}
		}
		if(b->ID > id)
		{
			return b;
		}
		return successor(b,hook);
}
//Returns the rightmost child for a node which is the maximum value in it's child
node *maxValue(node* &root,node *hook)
{
    node* temp = root;
    if(temp->right==hook)
        return temp;
    else{
        return maxValue(temp->right,hook);
    }

}
//Predecessor is determined by the highest value on the leftmost Node or the leftmost common ancestor
node *predecessor(node* y, node*hook){
        node * a = hook;
        node *z=y->left;
		if(y->left != hook){
			return maxValue(z,hook);
		}
		a = y->parent;
		while(a != hook && y == a->left){
			y = a;
			a = a->parent;
		}
		return a;
}
//Searches for the given Node recursively and then calls the successor for that node
node *previous(node *root,int id,node*hook){
    node* a=root;
    node* b=hook;
		while(a!=hook){
			b=a;
			if(a->ID==id){
				return predecessor(a,hook);
			}
			else{
				if(id < a->ID){
					a=a->left;
				}
				else{
					a=a->right;
				}
			}
		}
		if(b->ID < id)
		{
			return b;
		}
		return predecessor(b,hook);
}
int main(int argc, char* argv[]){
    //Reading the input file from the argument of the program, if no argument is given then quit the program
    if (argc <= 1)
        return 0;

    ifstream myfile;
    string f = argv[argc - 1];
    string line;
    string token;
    myfile.open(f.c_str());
    node* hook = new node;
	hook->color='b';

	node* root = hook;
    getline(myfile,line);
    //Creating the RBTree first from the input argument text file
   while(getline(myfile,line)){
        stringstream ss(line);
        getline(ss, token, ' ');
        int id = atoi(token.c_str());
        getline(ss, token, ' ');
        int c=atoi(token.c_str());
        node* z=new node;
        z->left=hook;
        z->right=hook;
        z->parent=hook;
        z->color='r';
        z->ID=id;
        z->count=c;
        insert(root,z,hook);
    }
      string sent;
    //Reading the commands.txt lines until "quit" command is encountered
      while(sent!="quit"){
        sent="";
        getline(cin, sent);
        stringstream ss(sent);
        //Separating the tokens based on the occurences of spaces
        getline(ss, token, ' ');
        //if the command is increase
        if(token=="increase"){
            getline(ss, token, ' ');
            int iden=atoi(token.c_str());
            getline(ss, token, ' ');
            int f=atoi(token.c_str());
            node *op=modify_count(root,iden,hook,f,'a');
            cout<<op->count<<endl;
        }
        //if the command is reduce
        else if(token=="reduce"){
            getline(ss, token, ' ');
            int iden=atoi(token.c_str());
            getline(ss, token, ' ');
            int f=atoi(token.c_str());
            node *op=modify_count(root,iden,hook,f,'s');
            if(op->ID!=0){
                cout<<op->count<<endl;
            }
            else{
                cout<<"0"<<endl;
            }
        }
        //if the command is count
        else if(token=="count"){
            getline(ss, token, ' ');
            int c_m=atoi(token.c_str());
            node* op=new node;
            op=count_count(root,hook,c_m);
            cout<<op->count<<endl;
        }
        //if the command is next
        else if(token=="next"){
            getline(ss,token, ' ');
            int n_m=atoi(token.c_str());
            node*op=next(root,n_m,hook);


            if(op->ID<0 || op==NULL || op==hook){
                 cout<<"0"<<" "<<"0"<<endl;
            }
            else{
                 cout<<op->ID<<" "<<op->count<<endl;
            }

        }
        //if the command is previous
        else if(token=="previous"){
            getline(ss,token, ' ');
            int n_m=atoi(token.c_str());
            node*op=previous(root,n_m,hook);
            if(op->ID<0 || op==NULL || op==hook){
                 cout<<"0"<<" "<<"0"<<endl;
            }
            else{
                 cout<<op->ID<<" "<<op->count<<endl;
            }
        }
        //if the command is inrange
        else if(token=="inrange"){
            getline(ss,token, ' ');
            int fr=atoi(token.c_str());
            getline(ss,token, ' ');
            int to=atoi(token.c_str());
            cout<<count_range(root,hook,fr,to)<<endl;
        }
      }
}
