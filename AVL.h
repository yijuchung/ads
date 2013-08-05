#include <cstddef>
#include <stack>
#include <iostream>
using namespace std;

#define subheight(n) ((n) == NULL ? -1 : (n)->iSubHeight)
#define max(a,b) ((a) > (b) ? (a) : (b))

class avNode{
public:
	int iKey;
	int iVal;
	int iSubHeight;

	//avNode* avParent;
	avNode* avChilds[2];
	// 0 left, 1 right

	avNode( int i, int v ){
		iKey = i;
		iVal = v;
		iSubHeight = 0;
		avChilds[0] = NULL;
		avChilds[1] = NULL;
	};
};

class avTree{
public:
	avNode* avRoot;
	
	avTree(){
		avRoot = NULL;
	}

	avTree(int i, int v){
		avRoot = new avNode(i, v);
	}

	void insert( int i, int v );
	bool find( int i );
	void release( avNode *pRoot );

	// dex 0 RR, 1 LL
	avNode *rotate( avNode *pRoot, int dex );
	avNode *rotateTwice( avNode *pRoot, int dex );

	void printInorder( avNode *pRoot );
	void printPostorder( avNode *pRoot );
};

void avTree::insert( int i, int v ){
	if( avRoot == NULL ){
		// add new node
		//cout << "create new tree root:" << i << endl;
		avRoot = new avNode(i,v);
		return ;
	}

	//int dex = avRoot->iKey < i;
	//avRoot->avChilds[dex] = avNode::insert(avRoot->avChilds[dex], i);

	std::stack<avNode*> stPath;
	std::stack<int> stIndex;
	//stPath.push(avRoot);

	avNode* it = avRoot;
	int dex;

	//cout << "find insert position" << endl;
	while( it != NULL ){
		stPath.push(it);
		//cout << "push parent:" << it->iKey << endl;
		dex = it->iKey < i;
		//cout << i << " compare to " << it->iKey << " go to " << dex << endl;
		stIndex.push(dex);

		it = it->avChilds[dex];
	}

	it = stPath.top();

	it->avChilds[dex] = new avNode(i,v);
	if( it->iSubHeight == 0)
		it->iSubHeight = 1;

	//cout << i << " insert to " << it->iKey << "'s " << dex << endl;
	//cout << "update parent:" << it->iKey << "'s subheight to " << it->iSubHeight << endl;

	stPath.pop();
	stIndex.pop();

	//if( stPath.empty())
	//	return avRoot;

	bool bDone = false;
	while( !bDone ){
		
		//cout << "check parent:" << it->iKey << " from " << dex << endl;

		int lh = 0,rh = 0,maxh;
		lh = subheight(it->avChilds[dex]);
		rh = subheight(it->avChilds[!dex]);

		//cout << "(lh,rh):(" << lh << "," << rh << ")" << endl;

		if( lh == rh )
			bDone = true;
		else if( lh - rh > 1 ){
		//if( lh - rh > 1 ){
			//cout << "rotate needed" << endl;
			
			avNode *nA = it->avChilds[dex]->avChilds[dex];
			avNode *nB = it->avChilds[dex]->avChilds[!dex];

			//cout << "before rotate" << endl;
			//avNode::printInorder(avRoot);
			if( subheight(nA) >= subheight(nB) )
				it = rotate( it, !dex );
			else
				it = rotateTwice(it, !dex );
			
			// parent not root of tree

			//cout << "remain parent:" << stPath.size() << endl;
			if( stPath.size()  > 0 )
				stPath.top()->avChilds[stIndex.top()] = it;
			else
				avRoot = it;

			//cout << "after rotate" << endl;
			//avNode::printInorder(avRoot);

			bDone = true;
		}

		// update balance
		lh = subheight(it->avChilds[dex]);
		rh = subheight(it->avChilds[!dex]);
		maxh = max(lh,rh);

		it->iSubHeight = maxh+1;
		//cout << "update " << it->iKey << "'s height to " << it->iSubHeight << endl;

		if( stPath.empty() )
			break;

		it = stPath.top();
		stPath.pop();
		dex = stIndex.top();
		stIndex.pop();
	}
}

bool avTree::find( int i ){

	avNode* it = avRoot;
	while( it != NULL ){
		if( it->iKey == i )
			return true;
		else
			it = it->avChilds[it->iKey < i];
	}

	return false;
}

avNode *avTree::rotate( avNode *pRoot,int dex ){
	
	avNode *avTemp = pRoot->avChilds[!dex];
	
	pRoot->avChilds[!dex] = avTemp->avChilds[dex];
	avTemp->avChilds[dex] = pRoot;

	int iRh = subheight( pRoot->avChilds[0] );
	int iLh = subheight( pRoot->avChilds[1] );
	int iTh = subheight( avTemp->avChilds[!dex] );

	//cout << iRh << "," << iLh << "," << iTh << endl;

	pRoot->iSubHeight = max( iRh, iLh ) + 1;
	//cout << "update " << avRoot->iKey << " height to " << avRoot->iSubHeight << endl;
	avTemp->iSubHeight = max( iTh, pRoot->iSubHeight ) + 1;
	//cout << "update " << avTemp->iKey << " height to " << avTemp->iSubHeight << endl;
	
	return avTemp;
}

avNode *avTree::rotateTwice( avNode *pRoot, int dex ){
	
	pRoot->avChilds[!dex] = rotate( pRoot->avChilds[!dex], !dex );
	 
	return rotate( pRoot, dex );
}


void avTree::printInorder( avNode *pRoot ){
	if( pRoot->avChilds[0] != NULL )
		printInorder(pRoot->avChilds[0]);

	//cout << "(" << pRoot->iKey << "," << pRoot->iSubHeight << ")" << endl;
	cout << pRoot->iVal << " ";

	if( pRoot->avChilds[1] != NULL )
		printInorder(pRoot->avChilds[1]);
}

void avTree::printPostorder( avNode *pRoot ){
	if( pRoot->avChilds[0] != NULL )
		printPostorder(pRoot->avChilds[0]);

	if( pRoot->avChilds[1] != NULL )
		printPostorder(pRoot->avChilds[1]);

	cout << pRoot->iVal << " ";
}

void avTree::release( avNode *pRoot ){
	if( pRoot->avChilds[0] != NULL )
		release(pRoot->avChilds[0]);

	if( pRoot->avChilds[1] != NULL )
		release(pRoot->avChilds[1]);

	if( pRoot != NULL )
		delete pRoot;
}