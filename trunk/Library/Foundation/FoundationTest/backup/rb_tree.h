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

		while(node != *proot && (node == NULL || node->color == RB_BLACK))/*��node��Ϊroot����nodeΪ˫�ڽڵ�ʱѭ������*/
		{
				if(node == parent->left)/*ֻҪnode != root,��node->parent ��Ȼ��ΪNULL*/
				{
						sibling = parent->right; 

						if(sibling->color == RB_RED)/*��Ϊnode��˫�ڽڵ㣬sibling->color �ֵ���RB_RED,����node���κ�һ��ֶ�ӽڵ㶼��ΪҶ�ڵ�*/
						{
								sibling->color = RB_BLACK;
								parent->color = RB_RED;
								RotateToLeft(parent, proot);
								sibling = parent->right;
						}

						if((sibling->left == NULL || sibling->left->color == RB_BLACK) && (sibling->right == NULL || sibling->right->color == RB_BLACK))
						{
								/*(1):ִ�е�����siblingһ����ΪRB_RED, ����sibling->left && sibling->right ��Ϊ�ڽڵ㣬����ֱ�ӽ�root��sibling�¸���subtree�ĺڽڵ��ȥһ���Ϳ�����*/
								sibling->color = RB_RED;
								/*�����ƶ�һ�㣬��ʱparentΪ˫�ڽڵ㣨��Ϊ��sibling��·���¼�����һ���ڽڵ㣬����sibling��node�µĺ�ɫ�ڵ���ȣ������������root��ʼ�����������ڽڵ�����*/
								node = parent;
								parent = parent->parent;
						}else
						{
								if(sibling->right == NULL || sibling->right->color == RB_BLACK)/*�����node��Զֶ��Ϊ�ڽڵ㣬���ֶ��һ���Ǻ�ɫ�ģ����򲻻�ִ�е���*/
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
							/*	if(sibling->right != NULL)*/sibling->right->color = RB_BLACK;/*�ұߴ����ڲ��ڵ㣬��Ⱦ�ڣ�������Ǻ�*/
								RotateToLeft(parent, proot);/*��ˣ���parentȾ�ڣ�������node���ڵ�˫��·���ϣ���ʱˬ�ڽڵ�ɶ���*/
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
								if(sibling->left == NULL || sibling->left->color == RB_BLACK)/*ͬ���˴�sibling->right һ��ΪRB_RED*/
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
/*����ڵ�Ϊ��ڵ㣬���ڵ�ҲΪ��ڵ㣬������ڵ�͸��ڵ�Ⱦ�ڣ�үү�ڵ�Ⱦ�죨��Ϊ�常�͸��׶�Ϊ�죬����үү�ض�Ϊ�ڣ���
��үү��ʼ�ĺ�ڽڵ��������4��֮��node������үү�ڵ㣬���¿�ʼѭ����ֱ��root��ĳ�����ڵ�Ϊ��*/
						if(uncle != NULL && uncle->color == RB_RED)
						{
								uncle->color = RB_BLACK;
								node->parent->color = RB_BLACK;
								node->parent->parent->color = RB_RED;
								node = node->parent->parent;
						}else
						{
/*����常�ڵ�Ϊ�ڣ��������ڵ��ڸ��ڵ��Ҳ࣬�򽫸��ڵ�����ת����ʱ�½ڵ��Ϊ���ڵ㣩��֮��үү�ڵ�(��ǰ�ڵ��үү��Ⱦ�죨��Ϊ��ǰ�ڵ�͸��ڵ㶼Ϊ�죬����ҲҶ�ڵ�һ��Ϊ��)
�����ڵ�Ⱦ�ڣ�����ǰ�ڵ��үү�ڵ�����ת����ǰ·���µĺ�ڵ㱻�ָ�������·��һ������ʱ��·���Ϻ�ɫ�ڵ��������5����Ϊ�ڽڵ㲢û��������ֻ������ת����һ����ڵ㣩
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

