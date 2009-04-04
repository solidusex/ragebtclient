#pragma once

#include "Foundation.h"

namespace {

typedef enum{RB_RED = 0x00, RB_BLACK = 0x01}RB_COLOR;

struct Node
{
public:
		int				item;
		Node			*left;
		Node			*right;
		Node			*parent;
		RB_COLOR		color;

public:
		Node(int i, Node *p = NULL, Node *l = NULL, Node *r = NULL, RB_COLOR c = RB_RED)
				: item(i)
				, left(l)
				, right(r)
				, parent(p)
				, color(c)
		{


		}

		~Node()
		{

		}
};

size_t count_nodes(Node *node)
{
		if(node == NULL) return 0;

		return count_nodes(node->left) + count_nodes(node->right) + 1;
}

Node* GetSelectNode(Node *root, size_t n)
{
		if(root == NULL) return NULL;
		printf("root->item == %d n == %d\n", root->item, n);
		size_t lc = count_nodes(root->left);
		
		printf("lc == %d\n", lc);
		if(lc == n) return root;

		if(n < lc) return GetSelectNode(root->left, n);

		return GetSelectNode(root->right, n - lc - 1);

}

size_t GetRank(const Node *root, const Node *node)
{
		assert(node != NULL && root != NULL);

		size_t count = count_nodes((Node*)(node->left));
		printf("count == %d\n", count);
		while(node != root)
		{
				if(node == node->parent->right)
				{
						count = count + count_nodes(node->parent->left) + 1;
				}
				node = node->parent;
		}

		return count;
}

Node* GetMaxNode(Node *root)
{
		if(root == NULL)return NULL;
		
		while(root->right != NULL) root = root->right;
		return root;
}

Node* GetMinNode(Node *root)
{
		if(root == NULL)return NULL;
		
		while(root->left != NULL)root = root->left;
		return root;
}

Node* GetSuccessor(Node *root)
{
		if(root == NULL)return NULL;

		if(root->right != NULL)
		{
				return GetMinNode(root->right);
		}else
		{
				while(root->parent != NULL && root == root->parent->right)
				{
						root = root->parent;
				}
				return root->parent;
		}
}

Node* GetPreDecessor(Node *root)
{
		if(root == NULL)return NULL;

		if(root->left != NULL)
		{
				return GetMaxNode(root->left);
		}else
		{
				while(root->parent != NULL && root == root->parent->left)root = root->parent;
				return root->parent;
		}
}

void RotateToLeft(Node *node, Node **proot)
{
		assert(proot != NULL && *proot != NULL && node != NULL && node->right != NULL);

		Node *r = node->right;
		node->right = r->left;
		if(r->left != NULL)r->left->parent = node;
		r->parent = node->parent;
		
		
		if(node == *proot)
		{
				*proot = r;
		}else if(node == node->parent->left)
		{
				node->parent->left = r;
		}else
		{
				node->parent->right = r;
		}
		r->left = node;
		node->parent = r;
}

void RotateToRight(Node *node, Node **proot)
{
		assert(proot != NULL && *proot != NULL && node != NULL && node->left != NULL);

		Node *l = node->left;
		node->left = l->right;
		if(l->right != NULL) l->right->parent = node;
		l->parent = node->parent;

		if(node == *proot)
		{
				*proot = l;
		}else if(node == node->parent->left)
		{
				node->parent->left = l;
		}else
		{
				node->parent->right = l;
		}
		l->right = node;
		node->parent = l;
}


void erase_fix_up(Node **proot, Node *node, Node *parent)
{
		Node *sibling = NULL;
		assert(proot != NULL);
		assert(*proot != NULL);
		
		if(node == *proot) 
		{
				printf("xxxxxxxxxxxxxxxxxxxx\n");
				assert(parent == NULL);
		}

		while(node != *proot && (node == NULL || node->color == RB_BLACK))/*当node不为root或者node为双黑节点时循环修正*/
		{
				if(node == parent->left)/*只要node != root,则node->parent 必然不为NULL*/
				{
						sibling = parent->right; 

						if(sibling->color == RB_RED)/*因为node是双黑节点，sibling->color 又等于RB_RED,所以node的任何一个侄子节点都不为叶节点*/
						{
								sibling->color = RB_BLACK;
								parent->color = RB_RED;
								RotateToLeft(parent, proot);
								sibling = parent->right;
						}

						if((sibling->left == NULL || sibling->left->color == RB_BLACK) && (sibling->right == NULL || sibling->right->color == RB_BLACK))
						{
								/*(1):执行到这里sibling一定不为RB_RED, 而后sibling->left && sibling->right 都为黑节点，所以直接将root到sibling下各个subtree的黑节点减去一个就可以了*/
								sibling->color = RB_RED;
								/*向上移动一层，此时parent为双黑节点（因为在sibling的路径下减少了一个黑节点，所以sibling和node下的黑色节点相等，但并不代表从root开始的所有子树黑节点均相等*/
								node = parent;
								parent = parent->parent;
						}else
						{
								if(sibling->right == NULL || sibling->right->color == RB_BLACK)/*如果此node的远侄子为黑节点，则近侄子一定是红色的，否则不会执行到此*/
								{
										sibling->left->color = RB_BLACK;
										sibling->color = RB_RED;
										RotateToRight(sibling, proot);
										sibling = parent->right;
								}
								assert(sibling->color == RB_BLACK);

								sibling->color = parent->color;
								parent->color = RB_BLACK;
								assert(sibling->right->color == RB_RED);
							/*	if(sibling->right != NULL)*/sibling->right->color = RB_BLACK;/*右边存在内部节点，则染黑，否则就是黑*/
								RotateToLeft(parent, proot);/*如此，将parent染黑，补给了node所在的双黑路径上，此时爽黑节点可丢弃*/
								break;
						}
				}else
				{
						sibling = parent->left;

						if(sibling->color == RB_RED)
						{
								sibling->color = RB_BLACK;
								parent->color = RB_RED;
								RotateToRight(parent, proot);
								sibling = parent->left;
						}
						assert(sibling != NULL && sibling->color == RB_BLACK);

						if((sibling->left == NULL || sibling->left->color == RB_BLACK) && (sibling->right == NULL || sibling->right->color == RB_BLACK))
						{
								sibling->color = RB_RED;
								node = parent;
								parent = parent->parent;
						}else
						{
								if(sibling->left == NULL || sibling->left->color == RB_BLACK)/*同理，此处sibling->right 一定为RB_RED*/
								{
										sibling->right->color = RB_BLACK;
										sibling->color = RB_RED;
										RotateToLeft(sibling, proot);
										sibling = parent->left;
								}

								assert(sibling->color == RB_BLACK);

								sibling->color = parent->color;
								parent->color = RB_BLACK;
								assert(sibling->left->color == RB_RED);
								/*if(sibling->left != NULL)*/sibling->left->color = RB_BLACK;
								RotateToRight(parent, proot);
								break;
						}
				}
		}
		if(node != NULL) node->color = RB_BLACK;
}



void insert_fix_up(Node **proot, Node *node)
{
		Node *uncle = NULL;
		assert(proot != NULL && *proot != NULL && node != NULL && node->color == RB_RED);

		while(node != *proot && node->parent->color == RB_RED)
		{
				if(node->parent == node->parent->parent->left)
				{
						uncle = node->parent->parent->right;
/*叔叔节点为红节点，父节点也为红节点，则将叔叔节点和父节点染黑，爷爷节点染红（因为叔父和父亲都为红，所以爷爷必定为黑）则
从爷爷开始的红黑节点符合性质4，之后node上升到爷爷节点，重新开始循环，直到root或某个父节点为黑*/
						if(uncle != NULL && uncle->color == RB_RED)
						{
								uncle->color = RB_BLACK;
								node->parent->color = RB_BLACK;
								node->parent->parent->color = RB_RED;
								node = node->parent->parent;
						}else
						{
/*如果叔父节点为黑，这里，如果节点在父节点右侧，则将父节点向左转（此时新节点变为父节点），之后将爷爷节点(当前节点的爷爷）染红（因为当前节点和父节点都为红，所以也叶节点一定为黑)
，父节点染黑，将当前节点的爷爷节点向右转，则当前路径下的红节点被分给了右面路径一个，此时此路径上黑色节点符合性质5（因为黑节点并没多起来，只是向右转移了一个红节点）
*/
								if(node == node->parent->right)
								{
										node = node->parent;
										RotateToLeft(node, proot);
								}
								
								node->parent->color = RB_BLACK;
								node->parent->parent->color = RB_RED;
								RotateToRight(node->parent->parent, proot);
						}
				}else
				{
						uncle = node->parent->parent->left;

						if(uncle != NULL && uncle->color == RB_RED)
						{
								uncle->color = RB_BLACK;
								node->parent->color = RB_BLACK;
								node->parent->parent->color = RB_RED;
								node = node->parent->parent;
						}else
						{
								if(node == node->parent->left)
								{
										node = node->parent;
										RotateToRight(node, proot);
								}

								node->parent->color = RB_BLACK;
								node->parent->parent->color = RB_RED;
								RotateToLeft(node->parent->parent, proot);
						}
				}
		}
		(*proot)->color = RB_BLACK;
}



Node* FindNode(Node *root, int key)
{
		while(root != NULL)
		{
				if(key < root->item)
				{
						root = root->left;
				}else if(key > root->item)
				{
						root = root->right;
				}else
				{
						return root;
				}
		}

		return NULL;
}






Node* UnLinkNode(Node **proot, Node *node)
{
		Node *rm_node = NULL;
		Node *rm_chd = NULL;
		Node *parent = NULL;
		assert(proot != NULL);

		if(*proot == NULL || node == NULL) return NULL;

		if(node->left == NULL || node->right == NULL)
		{
				rm_node = node;
		}else
		{
				rm_node = GetSuccessor(node);
		}
		
		assert(rm_node->left == NULL || rm_node->right == NULL);
		parent = rm_node->parent;

		if(rm_node->left != NULL)
		{
				rm_chd = rm_node->left;
		}else
		{
				rm_chd = rm_node->right;
		}
		


		if(rm_chd != NULL)rm_chd->parent = rm_node->parent;
		
		if(rm_node->parent == NULL)
		{
				*proot = rm_chd;
		}else if(rm_node->parent->left == rm_node)
		{
				rm_node->parent->left = rm_chd;
				
		}else
		{
				rm_node->parent->right = rm_chd;
		}

		if(rm_node != node) node->item = rm_node->item;
		
		if(*proot != NULL && rm_node->color == RB_BLACK) erase_fix_up(proot, rm_chd, parent);


		
		return rm_node;
}





void Insert(Node **proot, int key)
{
		assert(proot != NULL);
		
		if(*proot == NULL)
		{
				*proot = new Node(key, NULL, NULL, NULL, RB_BLACK);
				return;
		}

		
		Node *curr = *proot;
		Node *p = NULL;
		while(curr != NULL)
		{
				p = curr;
				if(key < curr->item)
				{
						curr = curr->left;
				}else
				{
						curr = curr->right;
				}
		}

		assert(p != NULL && (p->left == NULL || p->right == NULL));
		Node *new_node = new Node(key, p, NULL, NULL, RB_RED);
		if(key < p->item)
		{
				p->left = new_node;
		}else
		{
				p->right = new_node;
		}

		insert_fix_up(proot, new_node);
		
}

bool Remove(Node **proot, int key)
{
		Node* rm_node = FindNode(*proot, key);

		if(rm_node == NULL)return false;

		rm_node = UnLinkNode(proot, rm_node);

		delete rm_node;
		return true;
}


#if(0)

Node* copy_rb_internal(const Node *root, Node *parent)
{
		if(root == NULL)return NULL;
		Node *new_node =  new Node(root->item, parent, NULL, NULL, root->color);
		new_node->left = copy_rb_internal(root->left, new_node);
		new_node->right = copy_rb_internal(root->right, new_node);
		return new_node;
}
#endif


#if(0)
link_type __copy(link_type x, link_type p) 
{
		// structural copy.  x and p must be non-null.
		link_type top = clone_node(x);
		top->parent = p;

		
		if (x->right)top->right = __copy(right(x), top);
		
		p = top;
		x = left(x);

		while (x != 0) 
		{
				link_type y = clone_node(x);
				p->left = y;
				y->parent = p;
				if (x->right)
						y->right = __copy(right(x), y);
				p = y;
				x = left(x);
		}
		return top;
}
#endif

Node* copy_rb_internal(const Node *root, Node *parent)
{
		if(root == NULL)return NULL;
		Node *new_node =  new Node(root->item, parent, NULL, NULL, root->color);
		
		new_node->right = copy_rb_internal(root->right, new_node);

		parent = new_node;
		root = root->left;

		while(root != NULL)
		{
				parent->left = new Node(root->item, parent, NULL, NULL, root->color);
				parent->left->right = copy_rb_internal(root->right, parent->left);
				root = root->left;
				parent = parent->left;
		}

		return new_node;
}


Node* CopyRBTree(const Node *root)
{
		return copy_rb_internal(root, NULL);
}


#if(0)
Node* CopyRBTree(const Node *root)
{
		Node* new_tree = NULL;
		std::stack<const Node*> ss;
		std::stack<Node**> s_new;

		if(root != NULL)
		{
				ss.push(root);
				s_new.push(&new_tree);
		}
		
		while(!ss.empty())
		{
				const Node *tmp = ss.top();
				ss.pop();

				Node **pnew = s_new.top();
				s_new.pop();
				*pnew = new Node(tmp->item, (s_new.empty() ? NULL : *s_new.top()), NULL, NULL, tmp->color);
				
				if(tmp->right != NULL)
				{
						ss.push(tmp->right);
						s_new.push(&((*pnew)->right));
				}

				if(tmp->left != NULL)
				{
						s_new.push(&((*pnew)->left));
						ss.push(tmp->left);
				}
		}

		return new_tree;
}
#endif




void print_tree(Node *root, size_t level)
{
		if(root == NULL)return;

		print_tree(root->right, level + 1);
		for(size_t i = 0; i < level; ++i)printf(" - ");
		if(root->color == RB_RED)
		{
				printf("r%d\n", root->item);
		}else
		{
				printf("b%d\n", root->item);
		}
		print_tree(root->left, level + 1);
}

void PrintTree(Node *root)
{
		print_tree(root, 0);
}

size_t CountNode(Node *root)
{
		if(root == NULL) return 0;

		return CountNode(root->left) + CountNode(root->right) + 1;
}



#if(0)

int main()
{

		/*
		srand(time(NULL));
		std::vector<int> vi;

		for(int i = 1; i < 30; ++i)
		{
				//vi.push_back(rand() % 10000);
				vi.push_back(i);
		}

		for(int i = 0; i < vi.size(); ++i)
		{
				Insert(&root, vi[i]);
		}

		//std::random_shuffle(vi.begin(), vi.end());

		PrintTree(root);
		printf("-----------------------------------------------------\n");
		cin.get();

		

		
		
		printf("count nodes == %d\n", CountNode(root));
		printf("-------------------------------------\n");
		cin.get();

		for(int i = 0; i < vi.size(); ++i)
		{
				cin.get();
				if(!Remove(&root, vi[i]))
				{
						printf("vi[i] == %d\n", vi[i]);
						assert(false);
				}
				
				PrintTree(root);
				printf("-------------------------------------\n");
		}
*/
		for(size_t i = 0; i < 10; ++i) Insert(&root, i);
		
		PrintTree(root);

		cin.get();

		cout << GetRank(root, GetMaxNode(root)) << endl;
/*
		Node *curr = GetMinNode(root);
		while(curr != NULL)
		{
				printf("curr == %d\n", curr->item);
				curr = GetSuccessor(curr);
		}

		printf("xxxxxxxxxxx == %d\n", GetPreDecessor(FindNode(root, 2))->item);*/

	//	PrintTree(root);
	//	printf("count nodes == %d\n", CountNode(root));

/*		Node *curr = GetMaxNode(root);

		while(curr != NULL)
		{
				printf("%d\n", curr->item);
				curr = GetPreDecessor(curr);
		}*/
		
		


		printf("done\n");
		cin.get();
		return 0;
}
#endif

}

