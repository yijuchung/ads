#include <cstddef>
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
using namespace std;

//template <int iOrder>
class BNode{
public:
	vector<int> vValue;
	vector<int> vReal;
	int iCurrent;
	int iOrder;

	//BNode<iOrder> *pbParent;
	BNode *pbParent;
	//vector<BNode<iOrder>*> vbChilds;
	vector<BNode *> vbChilds;

	BNode(int iO) : iOrder(iO),vValue(iO),vReal(iO),vbChilds(iO+1){
		iCurrent = 0;
		pbParent = NULL;
		for( int i = 0 ; i < iOrder+1 ; i++ ) vbChilds[i] = NULL ;
	}

	BNode( int iO,int iVal, int iReal ) : iOrder(iO),vValue(iO),vReal(iO),vbChilds(iO+1){
		iCurrent = 1;
		vValue[0] = iVal;
		vReal[0] = iReal;
		pbParent = NULL;
		for( int i = 0 ; i < iOrder+1 ; i++ ) vbChilds[i] = NULL ;
	}

	BNode( int iO,vector<int> &v, vector<int> &r ) : iOrder(iO),vValue(iO),vReal(iO),vbChilds(iO+1){
		pbParent = NULL;
		iCurrent = v.size();
		for( int i = 0 ; i < v.size() ; i++ ){
			vValue[i] = v[i];
		}

		for( int i = 0 ; i < v.size() ; i++ ){
			vReal[i] = r[i];
		}

		for( int i = 0 ; i < iOrder+1 ; i++ ){
			vbChilds[i] = NULL;
		}
	}

	bool isLeaf(){
		if( vbChilds[0] == NULL )
			return true;
		else
			return false;
	};
};

//template<int iOrder, int iMid = (iOrder-1)/2>
class BTree{
public:
	//BNode<iOrder> *bRoot;
	BNode *bRoot;
	int iOrder;
	int iMid;
	BTree( int i){
		iOrder = i;
		iMid = (i-1)/2;
		bRoot = NULL;
	}

	void insert( int iVal, int iReal );
	bool search( int iVal );
	//void spiltBT( BNode<iOrder> *pCur );
	void spiltBT( BNode *pCur );
	//void spiltBNode( BNode<iOrder> *&pCur, BNode<iOrder> *&pCur2, int &iMiddle );
	void spiltBNode( BNode *&pCur, BNode *&pCur2, int &iMiddle, int &iMiddleReal );
	void printBTree();
	void printSortedBTree( BNode *pCur );
	void release();
};

//template<int iOrder, int iMid>
void BTree::insert( int iVal, int iReal ){
	//cout << "input : " << iVal << endl;
	//printBTree();

	if( bRoot == NULL )
	{
		//cout << "create new node as root" << endl;
		bRoot = new BNode(iOrder, iVal, iReal );

		return;
	}else{
		BNode *pCur = bRoot;
		bool bFound = false;
		int iPos = 0;
		
		while( !bFound ){
		int iCur = pCur->iCurrent;

			if( iVal > pCur->vValue[iCur-1] ){
				if( pCur->vbChilds[iCur] == NULL ){
					//cout << "reach leaf insert:" << (iCur-1) << endl;
					bFound = true;
					iPos = iCur;
					break;
				}else{
					pCur = pCur->vbChilds[iCur];
				}
			}else{
				for( int i = 0; i < iCur ; i++ ){
					if( iVal == pCur->vValue[i] ){
						// same value input, discard
						return;
					}else if( iVal < pCur->vValue[i] ){
						if( pCur->isLeaf() ){
							//cout << "reach leaf insert:" << i << endl;
							bFound = true;
							iPos = i;
							break;
						}else{
							pCur = pCur->vbChilds[i];
							break;
						}
					}
				}
			}
		}

		// move > val to 1 place after
		for ( int i = pCur->iCurrent ; i > iPos ; i-- ){
			pCur->vValue[i] = pCur->vValue[i-1] ;
			pCur->vReal[i] = pCur->vReal[i-1] ;
		}
        
		pCur->vValue[iPos] = iVal;
		pCur->vReal[iPos] = iReal;
		(pCur->iCurrent)++;

		spiltBT( pCur );
	}
}

//template<int iOrder, int iMid>
void BTree::spiltBNode( BNode* &pCur, BNode* &pCur2, int &iMiddle, int &iMiddleReal ){
	iMiddle = pCur->vValue[iMid];
	iMiddleReal = pCur->vReal[iMid];
	//cout << iMid << endl;
	vector<int> vt( pCur->vValue.begin()+iMid+1,pCur->vValue.begin()+2*iMid+1);
	vector<int> vrt( pCur->vReal.begin()+iMid+1,pCur->vReal.begin()+2*iMid+1);
	pCur2 = new BNode(iOrder, vt, vrt );

	pCur2->pbParent = pCur->pbParent;

	for( int i = 0 ; i < iMid+1 ; i++ ){
		pCur2->vbChilds[i] = pCur->vbChilds[iMid+1+i];
		pCur->vbChilds[iMid+1+i] = NULL;

		// update the child's parent to new parent
		if( pCur2->vbChilds[i] != NULL )
			pCur2->vbChilds[i]->pbParent = pCur2;
	}
	//if(iMid == 0)
	//	pCur->iCurrent = 1;
	//else
		pCur->iCurrent = iMid;
}

//template<int iOrder, int iMid>
void BTree::spiltBT ( BNode *pCur ){
	
	while( pCur->iCurrent >= iOrder )
	{
		// over order
		BNode *pCur2 = NULL;
		int iMiddle = 0;
		int iMiddleReal = 0;

		spiltBNode( pCur,pCur2, iMiddle,iMiddleReal );

		if( pCur->pbParent == NULL ){
			// i'm root
			//BNode<iOrder> *bPar = new BNode<iOrder>( iMiddle );
			BNode *bPar = new BNode(iOrder, iMiddle,iMiddleReal );
			bPar->vbChilds[0] = pCur;
			bPar->vbChilds[1] = pCur2;
			pCur->pbParent = bPar;
			pCur2->pbParent = bPar;
			bRoot = bPar;
		}else{
			//BNode<iOrder> *bPar = pCur->pbParent;
			BNode *bPar = pCur->pbParent;
			int iParSize = bPar->iCurrent;

			if( iMiddle > bPar->vValue[iParSize-1] ){
				bPar->vValue[iParSize] = iMiddle;
				bPar->vReal[iParSize] = iMiddleReal;
				bPar->vbChilds[iParSize+1] = pCur2;
				(bPar->iCurrent)++;
			}else{
				for( int i = 0 ; i<iParSize ; i++ ){
					if( iMiddle < bPar->vValue[i] ){
						for ( int j = iParSize ; j > i ; j-- ){
							bPar->vValue[j] = bPar->vValue[j-1] ;
							bPar->vReal[j] = bPar->vReal[j-1] ;
						}
        
						bPar->vValue[i] = iMiddle;
						bPar->vReal[i] = iMiddleReal;
						(bPar->iCurrent)++;

						for ( int j = iParSize+1 ; j > i+1 ; j-- ) bPar->vbChilds[j] = bPar->vbChilds[j-1] ;
                        
						bPar->vbChilds[i+1] = pCur2;

						break;
					}
				}
			}

			// keep spliting nodes upwards
			pCur = bPar;
			
		}
	}
}

//template<int iOrder, int iMid>
bool BTree::search( int iVal ){
	//BNode<iOrder> *pbCur = bRoot;
	BNode *pbCur = bRoot;

	while( pbCur != NULL ){

		int iCur = pbCur->iCurrent;

		if( iVal > pbCur->vValue[iCur-1] ){
			pbCur = pbCur->vbChilds[iCur];
		}else{

			for( int i = 0 ; i<iCur ; i++ ){
				if( iVal == pbCur->vValue[i] )
					return true;
				else if( iVal < pbCur->vValue[i] ){
					pbCur = pbCur->vbChilds[i];
					break;
				}
			}
		}

	}

	return false;
}

//template<int iOrder, int iMid>
void BTree::printBTree(){
	//cout << "------------------" << endl;
	if( bRoot == NULL )
		return;
	queue<BNode *> qNode;
	qNode.push(bRoot);

	while( !qNode.empty() ){
		BNode *bCur = qNode.front();
		qNode.pop();

		for( int i = 0 ; i<bCur->iCurrent ; i++)
			cout << bCur->vReal[i] << " ";
		
		for( int i = 0 ; i<bCur->iCurrent+1 ; i++){
			if( bCur->vbChilds[i] != NULL)
				qNode.push( bCur->vbChilds[i] );
		}
	}
}

void BTree::printSortedBTree( BNode *pCur ){
	for( int i=0 ;i< pCur->iCurrent+1 ; i++){
		if( pCur->vbChilds[i] )
			printSortedBTree( pCur->vbChilds[i] );
		if( i < pCur->iCurrent )
			cout << pCur->vReal[i] << " ";
	}
}

void BTree::release(){

	if( bRoot == NULL )
		return;
	stack<BNode *> qNode;
	qNode.push(bRoot);

	while( !qNode.empty() ){
		BNode *bCur = qNode.top();
		qNode.pop();

		for( int i = 0 ; i<bCur->iCurrent+1 ; i++){
			if( bCur->vbChilds[i] != NULL)
				qNode.push( bCur->vbChilds[i] );
		}

		delete bCur;
	}

}