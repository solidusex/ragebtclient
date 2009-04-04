#include "AVLTree.h"


typedef struct __avl_node_tag avl_node_t;

typedef struct __avl_node_tag
{
		int				bf;
		int				data;
		avl_node_t		*parent;
		avl_node_t		*child[2];
}avl_node_t;


typedef enum {left = 0, right = 1} child_idx_t;

typedef child_idx_t	rotate_dir_t;


#define avl_max(a,b) ((a) > (b) ? (a) : (b))

#define avl_min(a,b) ((a) < (b) ? (a) : (b))



inline child_idx_t __get_opposite(child_idx_t idx) {return (child_idx_t)(1 - (int)idx);}

inline int __get_node_side(const avl_node_t *node)
{
		if(node->parent == NULL)return 0;

		if(node == node->parent->child[left]) return -1;
		
		assert(node == node->parent->child[right]);
		return 1;
}

/*
向左转
				A                                     B
               / \                                   / \
              /   \                                 /   \
             a     B           ==>                 A     c
                  / \                             / \
                 /   \                           /   \
                b     c                         a     b

NewBal(A) = OldBal(A) - 1 - max(OldBal(B), 0)
NewBal(B) = OldBal(B) - 1 + min(NewBal(A), 0)



向右转
				A                                     B
               / \                                   / \
              /   \                                 /   \
             B     a           ==>                 c     A
            / \                                         / \
           /   \                                       /   \
          c     b                                     b     a


NewBal(A) = OldBal(A) + 1 - min(OldBal(B), 0)
NewBal(B) = OldBal(B) + 1 + max(NewBal(A), 0)

*/


#define left_heavy		(-1)
#define balanced		(0)
#define right_heavy		(+1)

inline void __rotate_node(avl_node_t *node, rotate_dir_t dir, avl_node_t **proot)
{
		assert(node != NULL &&  proot != NULL && *proot != NULL);

		avl_node_t *chd = node->child[__get_opposite(dir)];
		assert(chd != NULL);
		
		node->child[__get_opposite(dir)] = chd->child[dir];

		if(chd->child[dir] != NULL) chd->child[dir]->parent = node;
		chd->child[dir] = node;
		chd->parent = node->parent;
		
		if(node == *proot)
		{
				*proot = chd;
		}else if(node == node->parent->child[left])
		{
				node->parent->child[left] = chd;
		}else
		{
				node->parent->child[right] = chd;
		}

		node->parent = chd;
		
		if(dir == left)
		{
				node->bf = node->bf - 1 - avl_max(chd->bf, 0);
				chd->bf  = chd->bf - 1 + avl_min(node->bf, 0);
		}else
		{
				node->bf = node->bf + 1 - avl_min(chd->bf, 0);
				chd->bf =  chd->bf + 1 + avl_max(node->bf, 0);
		}
}





inline bool __rotate_once(avl_node_t *node, rotate_dir_t dir, avl_node_t **proot)
{
		assert(node != NULL &&  proot != NULL && *proot != NULL);
		
		bool changed_high = (node->child[__get_opposite(dir)]->bf != 0);
		
		__rotate_node(node, dir, proot);
		
		return changed_high;
}


inline bool __rotate_twice(avl_node_t *node, rotate_dir_t dir, avl_node_t **proot)
{
		__rotate_once(node->child[__get_opposite(dir)], __get_opposite(dir), proot);

		__rotate_once(node, dir, proot);
		return true;
}


inline bool __rebalance_node(avl_node_t *node, avl_node_t **proot)
{
		assert(node != NULL && proot != NULL && *proot != NULL);

		bool changed_high = false;
		
		if(node->bf < left_heavy)
		{
				if(node->child[left]->bf == right_heavy)
				{
						changed_high = __rotate_twice(node, right, proot);
				}else
				{
						changed_high = __rotate_once(node, right, proot);
				}
		}else if(node->bf > right_heavy)
		{
				if(node->child[right]->bf == left_heavy)
				{
						changed_high = __rotate_twice(node, left, proot);
				}else
				{
						changed_high = __rotate_once(node, left, proot);
				}
		}
		
		return changed_high;
}

typedef enum {__erase = -1, __insert = 1}cmd_t;

inline void __uni_fixup(avl_node_t *node, avl_node_t **proot, int side, cmd_t cmd)/*node为被删除子结点或增加子结点的节点*/
{
		avl_node_t *p;
		assert(proot != NULL);
		
		while(node != NULL)
		{
				node->bf += (cmd * side);
				
				p = node->parent;
				side = __get_node_side(node);
				
				if(cmd == __insert)
				{
						if(node->bf == 0) return;
						if(__rebalance_node(node, proot)) return;
				}else
				{
						if(node->bf != 0)
						{
								if(!__rebalance_node(node, proot)) return;
						}
				}

				node = p;
		}

}


inline void __erase_fixup(avl_node_t *node, int side, avl_node_t **proot)/*node为被删除的节点的父节点*/
{
		avl_node_t *p;
		assert(proot != NULL);

		while(node != NULL)
		{
				node->bf -= side;
				p = node->parent;
				side = __get_node_side(node);

				if(node->bf == 0)
				{
						node = p;
						continue;
				}else
				{
						if(!__rebalance_node(node, proot)) return;
						node = p;
				}
		}
}


inline void __insert_fixup(avl_node_t *node, avl_node_t **proot)
{
		assert(node != NULL && proot != NULL && *proot != NULL);
		
		while(node->parent != NULL)
		{
				node->parent->bf += __get_node_side(node);

				if(node->parent->bf == 0) return;
				
				
				if(__rebalance_node(node->parent, proot))return;/*插入过程中，只要树高改变，则一定回复到平衡状态(插入阶段只要做翻转一定改变树高*/
				
				
				assert(node->parent != NULL);
				node = node->parent;
		}
		
		__rebalance_node(*proot, proot);
		
}



inline avl_node_t* __unlink_node(avl_node_t *node, avl_node_t **proot)
{
		assert(node != NULL && proot != NULL && *proot != NULL);
		
		avl_node_t *rm_node, *parent, *chd;
		int side;
		
		rm_node = node;
		if(node->child[left] == NULL)
		{
				chd = node->child[right];
		}else
		{
				if(node->child[right] == NULL)
				{
						chd = node->child[left];
				}else
				{
						rm_node = node->child[right];
						while(rm_node->child[left] != NULL) rm_node = rm_node->child[left];
						chd = rm_node->child[right];
				}
		}

		if(rm_node != node)
		{
				node->child[left]->parent = rm_node;
				rm_node->child[left] = node->child[left];

				if(rm_node != node->child[right])
				{
						parent = rm_node->parent;
						if(chd != NULL) chd->parent = parent;
						parent->child[left] = chd;
						rm_node->child[right] = node->child[right];
						node->child[right]->parent = rm_node;
						side = -1;
				}else
				{
						parent = rm_node;
						side = 1;
				}

				if(node == *proot)
				{
						*proot = rm_node;
				}else if(node == node->parent->child[left])
				{
						node->parent->child[left] = rm_node;
				}else
				{
						node->parent->child[right] = rm_node;
				}

				rm_node->parent = node->parent;

				{ int bf = rm_node->bf; rm_node->bf = node->bf; node->bf = bf;}

		}else
		{
				side = __get_node_side(node);
				parent = rm_node->parent;
				if(chd != NULL) chd->parent = parent;
				
				if(node == *proot)
				{
						*proot = chd;
				}else if(node == node->parent->child[left])
				{
						node->parent->child[left] = chd;
				}else
				{
						node->parent->child[right] = chd;
				}
		}

		//__erase_fixup(parent, side, proot);
		__uni_fixup(parent, proot, side, __erase);
		return node;
}




inline avl_node_t* __create_node(int data, avl_node_t *l, avl_node_t *r, avl_node_t *p)
{
		avl_node_t *res = new avl_node_t;
		res->data = data;
		res->child[left] = l;
		res->child[right] = r;
		res->parent = p;
		res->bf = 0;
		return res;
}


inline int __get_heigh(avl_node_t *root)
{
		if(root == NULL) return 0;

		return 1 + avl_max(__get_heigh(root->child[left]), __get_heigh(root->child[right]));
}

inline avl_node_t* __get_successor(avl_node_t *node)
{
		if(node == NULL)return NULL;

		if(node->child[right] != NULL)
		{
				node = node->child[right];

				while(node->child[left] != NULL) node = node->child[left];
				return node;
		}else
		{
				while(node->parent != NULL && node == node->parent->child[right]) node = node->parent;

				return node->parent;

		}
}

typedef struct __avl_tree_tag
{
		avl_node_t		*root;
		size_t			count;

}avl_tree_t;

bool avl_verify(avl_tree_t *self)
{
		avl_node_t *curr = self->root;

		if(curr == NULL) return (self->count == 0);

		while(curr->child[left] != NULL) curr = curr->child[left];
		
		int lh,rh;
		
		while(curr != NULL)
		{
				lh = __get_heigh(curr->child[left]);
				rh = __get_heigh(curr->child[right]);

				if(curr->bf != (rh - lh)) return false;
				printf("bf == %d rh == %d lh == %d\n", curr->bf, rh,lh);
				curr = __get_successor(curr);
		}
				
		return true;

}


avl_node_t* avl_insert_equal(avl_tree_t *self, int data)
{
		if(self->root == NULL)
		{
				self->root = __create_node(data, NULL,NULL,NULL);
				self->count++;
				return self->root;
		}

		avl_node_t *curr = self->root;
		avl_node_t *parent = NULL;


		while(curr != NULL)
		{
				parent = curr;
				if(curr->data <= data)
				{
						curr = curr->child[right];
				}else
				{
						curr = curr->child[left];
				}
		}

		avl_node_t *new_node = __create_node(data, NULL,NULL, parent);
		
		parent->child[parent->data > data ? left : right] = new_node;
		
		//__insert_fixup(new_node, &self->root);
		__uni_fixup(new_node->parent, &self->root, __get_node_side(new_node), __insert);
		
		self->count++;
		return new_node;
}


avl_node_t* avl_find_node(avl_tree_t *self, int data)
{
		avl_node_t *result = NULL;
		avl_node_t *curr = self->root;

		while(curr != NULL)
		{
				if(curr->data < data)
				{
						curr = curr->child[right];
				}else
				{
						if(curr->data == data)
						{
								result = curr;
						}
						curr = curr->child[left];
				}
		}

		return result;
}

bool avl_erase_node(avl_tree_t *self, int data)
{
		bool res = false;
		avl_node_t *tmp = avl_find_node(self, data);
		
		if(tmp != NULL)
		{
				tmp = __unlink_node(tmp, &self->root);
				self->count--;
				delete tmp;
				res = true;
		}
		
		return res;
}


void __print_tree(size_t level, avl_node_t *node)
{
		size_t i;
		if(node == NULL)return;
		__print_tree(level + 1, node->child[right]);
		for(i = 0; i < level; ++i) printf(" - ");
		printf("%d:%d\n", node->data, node->bf);
		__print_tree(level + 1, node->child[left]);
}



void print_tree(avl_tree_t *self)
{
		__print_tree(0, self->root);
}



avl_tree_t* avl_tree_create()
{
		avl_tree_t* res = new avl_tree_t;

		res->root = NULL;
		res->count = 0;

		return res;

}




























#if(0)
typedef struct __avl_node_tag avl_node;

typedef struct __avl_node_tag
{
		avl_node		*parent;
		avl_node		*left;
		avl_node		*right;
		
		int				data;
		int				bf;
		
}avl_node;

#define avl_max(a,b) ((a) > (b) ? (a) : (b))

#define avl_min(a,b) ((a) < (b) ? (a) : (b))


/*
向左转

NewBal(A) = OldBal(A) - 1 - max(OldBal(B), 0)
NewBal(B) = OldBal(B) - 1 + min(NewBal(A), 0)
*/

void __rotate_to_left(avl_node *node, avl_node **proot)
{
		assert(node != NULL && node->right != NULL && proot != NULL);
		avl_node *r = node->right;

		node->right = r->left;
		if(r->left != NULL) r->left->parent = node;
		r->left = node;
		r->parent = node->parent;

		if(*proot == node)
		{
				*proot = r;
		}else if(node == node->parent->left)
		{
				node->parent->left = r;
		}else
		{
				node->parent->right = r;
		}

		node->parent = r;
		
		r->bf = r->bf - 1 - avl_max(r->bf, 0);
		r->bf = r->bf - 1 + avl_min(r->bf, 0);
}


/*
      NewBal(A) = OldBal(A) + 1 - min(OldBal(B), 0)
      NewBal(B) = OldBal(B) + 1 + max(NewBal(A), 0)

*/
void __rotate_to_right(avl_node *node, avl_node **proot)
{
		assert(node != NULL && node->left != NULL && proot != NULL);
		
		avl_node *l = node->left;

		node->left = l->right;
		if(l->right != NULL) l->right->parent = node;
		l->parent = node->parent;

		if(*proot == node)
		{
				*proot = l;
		}else if(node == node->parent->left)
		{
				node->parent->left = l;
		}else
		{
				node->parent->right = l;
		}

		node->parent = l;

		r->bf = r->bf + 1 - avl_min(l->bf, 0);
		l->bf = l->bf + 1 + avl_max(r->bf, 0);
}

typedef enum {left_r = 0, right_r = 1} avl_dir;

inline avl_dir opposite_dir(avl_dir dir)
{
		return avl_dir(1 - (int)dir);
}

void __single_rotate(avl_node *node, avl_node **proot, avl_dir dir)
{
		if(dir == left_r)
				__rotate_to_left(node, proot);
		else
				__rotate_to_right(node, proot);

}

void __double_rotate(avl_node *node, avl_node **proot, avl_dir dir)
{
		avl_dir opp = opposite_dir(dir);

		__single_rotate((dir == left_r ? node->right : node->left), proot, opp);
		__single_rotate(node, proot, dir);
}

#endif


