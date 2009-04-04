#include <math.h>
#include <vector>
using namespace std;
#define max(a,b)  ((a)>(b)?(a):(b))
template<typename E>
class AVL_TMP
{ 
		template <typename E>
		class AVL_NODE
		{
		public:
				AVL_NODE():ln(0),rn(0),depth(0){}
				AVL_NODE( const E& e):data(e),ln(0),rn(0),depth(0){}
				~AVL_NODE(){ if (ln) delete ln; if (rn) delete rn; }

				bool operator < (E& e){  return data < e; }
				bool operator > (E& e){  return data > e; }
				bool operator == (E& e){ return data == e; }
				bool operator != (E& e){ return data != e; }

				E getdata(){return data;}

				E data;
				int depth;
				AVL_NODE<E> *ln,*rn;
		};

public: 
		typedef E dataType;
		typedef AVL_TMP<E> Myt;
		typedef AVL_NODE<E> n;
		typedef n* npos;
		typedef npos iterator;
		enum unbalanceType {LL,RR,LR,RL};
		AVL_TMP():root(0),size(0),depth(-1){}
		~AVL_TMP(){ if(root) delete root; }

		iterator begin(){return root;}
		bool insert(const E& e);
		npos find(const E& e);
		npos findpre(const E& e);
		bool del(dataType);
		bool balance(AVL_TMP<E>::iterator pos){
				if(pos == NULL) throw 0;
				int lh,rh;
				if(pos->ln == NULL ) lh = -1;
				else lh = pos->ln->depth;
				if(pos->rn == NULL ) rh = -1;
				else rh = pos->rn->depth;
				return abs( lh - rh ) < 2 ;
		}
		virtual void frontOrder(){};
		virtual void midOrder(){ };

protected:
		void LLr(AVL_TMP<E>::iterator pos,AVL_TMP<E>::iterator prePos);
		void LRr(AVL_TMP<E>::iterator pos,AVL_TMP<E>::iterator prePos);
		void RRr(AVL_TMP<E>::iterator pos,AVL_TMP<E>::iterator prePos);
		void RLr(AVL_TMP<E>::iterator pos,AVL_TMP<E>::iterator prePos);
		void updateDepth(AVL_TMP<E>::iterator pos);
		bool delAux(E const& e,AVL_TMP<E>::iterator pos = NULL);
		iterator findMax(iterator );
		iterator findMin(iterator );
		bool upTree(int iDepth,iterator itRoot,unsigned long iSize){depth = iDepth;root = itRoot;size = iSize; return true;}
		bool upRoutineDepth(vector<iterator>&);
		bool adjust(iterator a,iterator b,iterator c,iterator prePos = NULL);
		npos root;
		int depth;
		unsigned long size;
};
template<typename E>
bool AVL_TMP<E>::adjust(iterator a,iterator b,iterator c,iterator prePos){
		bool b1,b2;
		b1 = b == a->ln;
		b2 = c == b->ln;
		unbalanceType ub;
		if(b1&&!b2)   ub = LR;
		if(!b1&&b2)   ub = RL;
		if(b1&&b2)    ub = LL;
		if(!b1&&!b2)  ub = RR;
		switch(ub) {
  case  LL :LLr(a,prePos);
		  break;
  case  LR :LRr(a, prePos);
		  break;
  case  RR :RRr(a,prePos);
		  break;
  case  RL :RLr(a,prePos);
		  break;
		}  //end switch
		return true;
}
template<typename E>
bool AVL_TMP<E>::upRoutineDepth(vector<iterator>&routine){
		//该函数主要是将路径节点的深度更新并且使得那些不平衡的节点平衡
		int size = routine.size();
		while (size--) {
				updateDepth(routine[size]);
				if (!balance(routine[size])) {//不平衡得调整
						iterator cur = routine[size],prePos = NULL;
						if(size-1>=0)
								prePos = routine[size-1];
						//检查当前不平衡节点的哪颗子树的高度更高
						bool bl = cur->ln != NULL;
						bool br = cur->rn != NULL;
						if (!bl) {//肯定有右孩子
								if(cur->rn->ln) RLr(cur,prePos);
								else RRr(cur,prePos);
						}
						else{//有左孩子
								if (!br) {//没右孩子
										if (cur->ln->ln) LLr(cur,prePos);
										else LRr(cur,prePos);
								}
								else{ //有右孩子,此时需要检查左右孩子的高度，则右子树高度至少为1
										//因此左子树高度至少为3，则左子树的节点个数肯定大于4
										if (cur->ln->depth > cur->rn->depth) LLr(cur,prePos);
										else RRr(cur,prePos);
								}
						}
				}
		}
		return true;
}
template<typename E>
AVL_TMP<E>::iterator AVL_TMP<E>::findMax(AVL_TMP<E>::iterator pos){//以pos为根的树的最大值的节点
		if (!pos) return NULL;
		iterator p = pos;
		while(p->rn) p = p->rn;
		return p;
}
template<typename E>
AVL_TMP<E>::iterator AVL_TMP<E>::findMin(AVL_TMP<E>::iterator pos){
		iterator p = pos;
		while (p->ln) p = p->ln;
		return p;
}
template<typename E>
void AVL_TMP<E>::updateDepth(AVL_TMP<E>::iterator pos){
		bool b1 = pos->ln == NULL,b2 = pos->rn ==NULL;
		switch(b1) {
 case true:
		 if(b2) pos->depth = 0;
		 else pos->depth = pos->rn->depth+1;
		 break;
 default: //false
		 if(b2)  pos->depth = pos->ln->depth+1;
		 else pos->depth = max(pos->ln->depth , pos->rn->depth )+1;
		}
		if(pos == root) depth = pos->depth;
}
template<typename E>
void AVL_TMP<E>::LLr(AVL_TMP<E>::iterator pos,AVL_TMP<E>::iterator prePos){
		typename AVL_TMP<E>::iterator t, a = pos, b = t = pos->ln ;
		pos->ln = t->rn;
		t->rn = pos;
		if(root == a) root = b;
		if(prePos != NULL)
				if(prePos->ln == a) prePos->ln = b;
				else prePos->rn =  b;
				updateDepth(a);updateDepth(b);
}
template<typename E>
void AVL_TMP<E>::LRr(AVL_TMP<E>::iterator pos,AVL_TMP<E>::iterator prePos){
		AVL_TMP<E>::iterator a = pos,b = pos ->ln, c = b->rn;
		b->rn = c->ln ; a->ln = c->rn;
		c->ln = b;  c->rn =a;
		if(a == root ) root = c ;
		if(prePos != NULL)
				if(prePos->ln == a) prePos->ln = c;
				else prePos->rn =  c;
				updateDepth(a);updateDepth(b);updateDepth(c);
}
template<typename E>
void AVL_TMP<E>::RRr(AVL_TMP<E>::iterator pos,AVL_TMP<E>::iterator prePos ){
		AVL_TMP<E>::iterator a = pos ,t, b = t = pos->rn ;
		pos->rn = t->ln;
		t->ln = pos;
		if(prePos != NULL)
				if(prePos->ln == a) prePos->ln = b;
				else prePos->rn =  b;
				if(root == a) root = b;
				updateDepth(a);updateDepth(b);
}
template<typename E>
void AVL_TMP<E>::RLr(AVL_TMP<E>::iterator pos,AVL_TMP<E>::iterator prePos){
		AVL_TMP<E>::iterator a = pos, b = pos->rn , c = b->ln;
		a->rn = c->ln ;  b->ln = c->rn;
		c->ln = a; c->rn = b;
		if(prePos != NULL)
				if(prePos->ln == a) prePos->ln = c;
				else prePos->rn =  c;
				if( a == root) root = c;
				updateDepth(a);updateDepth(b);updateDepth(c);
}
template<typename E>
bool AVL_TMP<E>::insert(const E& e){
		if(root == NULL) {root = new AVL_NODE<E>(e); size++; depth = root->depth;return true;}
		bool bUpdateDepth = false;
		vector<AVL_TMP<E>::iterator> routin;
		typename AVL_TMP<E>::iterator p = root,pos,prePos;
		for (int i = 0 ; i < size ;i++ ) {
				routin.push_back(p);
				if(p->data > e){
						if ( p->ln == NULL ) {
								p->ln = pos = new AVL_NODE<E>(e);
								bUpdateDepth = p->rn == NULL;
								break;
						}
						else { p = p->ln ; continue;}
				}
				if(p->data  < e){
						if (p->rn == NULL) {
								p->rn = pos = new AVL_NODE<E>(e) ;
								bUpdateDepth = p->ln == NULL;
								break;
						}
						else {  p = p->rn ; continue;}
				}
				return false;   //already exists
		}  //insertion finished
		size++;
		if(size <= 2 ) {
				updateDepth(root);
				return true;
		}
		if(!bUpdateDepth) return true;   //balance

		bool unAdjusted = true;
		// check for balance and adjust depth
		for (i = routin.size()-1; i  >=0 ; i-- ) {
				if(!balance(routin.at(i)))
						if(unAdjusted) {  //  unbalance! get unbalance type
								if(i-1 >= 0) prePos = routin.at(i-1);
								else prePos = NULL;
								AVL_TMP<E>::iterator a = routin.at(i) , b = routin.at(i+1) , c;
								if(i+2 >= routin.size() ) c = pos;
								else c = routin.at(i+2);
								bool b1,b2;
								b1 = b == a->ln;
								b2 = c == b->ln;
								unbalanceType ub;
								if(b1&&!b2)   ub = LR;
								if(!b1&&b2)   ub = RL;
								if(b1&&b2)    ub = LL;
								if(!b1&&!b2)  ub = RR;

								switch(ub) {
	 case  LL :LLr(routin.at(i),prePos);
			 break;
	 case  LR :LRr(routin.at(i),prePos);
			 break;
	 case  RR :RRr(routin.at(i),prePos);
			 break;
	 case  RL :RLr(routin.at(i),prePos);
			 break;
								}  //end switch
								unAdjusted = false;
						}  //end if
						updateDepth(routin.at(i));  //update the depth of the node in the routin
						depth = root->depth;
		}//end for
		return true;
};
template<typename E>
AVL_TMP<E>::npos AVL_TMP<E>::find(const E& e){//search for position
		npos p=root;
		while (p&&p->data!=e)
				if(e>p->data) p=p->rn;
				else p= p->ln;
				return p;
}
template<typename E>
AVL_TMP<E>::npos AVL_TMP<E>::findpre(const E& e){//search for parent node position
		npos p,pre;
		p=pre=root;
		while (p&&p->data!=e) {
				pre = p;
				if (e>p->data) p=p->rn;
				else p = p->ln;
		}
		if(p) if(p->data==e) return NULL;//already existed
		return pre;
}
template<typename E>
bool AVL_TMP<E>::delAux(E const& e,AVL_TMP<E>::iterator pos){
		// 1.递归删除节点，直到删除的是叶子节点 
		// 2. 删除叶子节点,更新树的数据成员
		// 3. 更新路径上的节点深度并且检查平衡因子 
		static vector<iterator> routine;
		iterator p = pos;
		bool bUpdate = false;
		if(!pos){//第一次调用
				p = root;
				while (p&&e!=p->data) {//找到节点，并且将寻找路径存入表中
						routine.push_back(p);
						if(p->data > e) p = p->ln;
						else p = p->rn;
				}
				if(p == NULL){ //没找到
						routine.clear(); 
						return false;
				}
				else pos = p;
		}
		if (pos->ln||pos->rn) {//不是叶子节点，则该节点有孩子节点，可能是一个或者两个
				routine.push_back(pos);//还得往下删除
				if (pos->ln&&!pos->rn){ //情况一: 只有有左孩子
						//找到左子树中的最大值的位置
						iterator max = pos->ln;
						while (max->rn) { routine.push_back(max); max = max->rn;}
						bUpdate = false;
						//伪删除
						pos->data = max->data;
						delAux(max->data,max);
				}
				else if (!pos->ln&&pos->rn) { //情况二：只有右孩子
						//找到右子树中的最小值
						iterator min = pos->rn;
						while (min->ln) { routine.push_back(min); min = min->ln;}
						bUpdate = false;
						//伪删除
						pos->data = min->data;
						delAux(min->data,min);
				}
				else //情况三：有俩个孩子
				{
						//找到左子树中的最大值
						iterator max = pos->ln;
						while (max->rn) { routine.push_back(max); max = max->rn;}
						bUpdate = false;
						//伪删除
						pos->data = max->data;
						delAux(max->data,max);
				}
		}
		else
		{//是叶子节点
				//有三种情况，是其父节点的左子树且没有兄弟，是其父节点的右子树且没有兄弟，有兄弟
				//取得其父节点
				iterator parent = NULL;
				if (routine.size()) //有父节点
						parent = routine[routine.size()-1];
				else{//即该节点是根节点,无根节点
						delete root;
						routine.clear();
						upTree(-1,NULL,0);
						return true;
				}  //完成根节点的删除
				//有父节点
				if (pos == parent->ln&&!parent->rn) {//情况一：是父节点的左孩子且没兄弟
						//删除节点
						parent->ln = NULL;
						delete pos;
						//需要更新路径上的节点的深度
						bUpdate = true;
						upRoutineDepth(routine);
						upTree(root->depth,root,size-1);
						routine.clear();
						//改写父节点的孩子指针
				}//完成情况一叶子节点的删除
				else{
						if (pos == parent->rn && !parent->ln ) { //情况二：是父节点的右孩子且没兄弟
								parent->rn = NULL;
								delete pos; 
								bUpdate = true;
								upRoutineDepth(routine);
								upTree(root->depth,root,size-1);
								routine.clear();
						}//完成情况二叶子节点的删除
						else{//情况三：有兄弟
								//只需要将节点删除，并清理路径表就可以了
								if (pos == parent->ln) parent->ln = NULL;
								else parent->rn = NULL;
								delete pos;
								routine.clear();
						}//完成情况三的叶子节点删除
				}
		}
		return true;
}

template<typename E>
bool AVL_TMP<E>::del(dataType e){
		return delAux(e);
}